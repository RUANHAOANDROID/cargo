#include "clib.h"

char buffer[BUFFER_SIZE];
int stop_requested = 0;
int client_socket = -1;


// QR码相关资源
static int qrfd1 = -1;
static int qrfd2 = -1;
static unsigned char qrTmpBuff[512];

// IC卡相关资源
static uint8_t icKey[] = "\xFF\xFF\xFF\xFF\xFF\xFF";
static uint8_t icData[16];
static uint8_t icSnr[16];
static uint8_t icSnrLen;
static uint8_t icType = 0x01;
static unsigned char icTmpBuffer[5];

// ID卡相关资源
static unsigned char idBuffer[2400] = {0};
static ID_DATA idData;
pthread_spinlock_t lock;


void dump_data(char *str, unsigned char *text, int len) {
    int i;
    printf("%s(%d):", str, len);
    for (i = 0; i < len; i++) {
        printf("%02X ", (unsigned char)text[i]);
    }
    printf("\n");
}

int connect_to_server(const char *server_ip, int server_port) {
    printf("[c] ->connect_to_server \n");
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("[c] ->Error creating socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("[c] ->Error converting server IP address");
        close(client_socket);
        return -1;
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("[c] ->Error connecting to server");
        close(client_socket);
        return -1;
    }

    return client_socket;
}

int send_message(int client_socket, const char *message) {
    printf("[c] ->send message\n");
    int message_length = strlen(message);
    ssize_t bytes_sent = send(client_socket, message, message_length, 0);
    if (bytes_sent == -1) {
        printf("[c] ->send message==-1\n");
        perror("[c] ->Error sending message to server");
        close(client_socket);
        return -1;
    } else if (bytes_sent < message_length) {
        printf("[c] ->send message==-2\n");
        fprintf(stderr, "[c] ->Not all bytes sent to server\n");
        close(client_socket);
        return -1;
    } else if (bytes_sent == 0) {
        printf("[c] ->send message==-3\n");
        fprintf(stderr, "[c] ->Connection closed by server\n");
        close(client_socket);
        return -1;
    }
    printf("[c] ->send ok\n");
    return 0;
}

void close_connection(int client_socket) {
    printf("[c] ->close_connection\n");
    close(client_socket);
    stop_requested = 1;
    client_socket = -1;
}

int start_tcp(void) {
    printf("[c] ->start_tcp\n");
    const char *server_ip = "127.0.0.1";
    const int server_port = 9999;

    client_socket = connect_to_server(server_ip, server_port);
    if (client_socket == -1) {
        fprintf(stderr, "Failed to connect to server\n");
        return 1;
    }
    return 0;
}

void read_data(void) {
    init_devices();
    while (!stop_requested) {
        qr_read();
        ic_read();
        id_read();
        usleep(1000000);
    }
    close_devices();
}
// 初始化所有设备
void init_devices(void) {
    printf("[c] ->Initializing devices\n");
    // 初始化QR码设备
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);
    // 初始化IC卡设备
    PICC_Open(0));
    return 0;
}
// 关闭所有设备
void close_devices(void) {
    printf("[c] ->Closing devices\n");
    QRCode_Close(qrfd1);
    QRCode_Close(qrfd2);
    PICC_Close(0);
}
void qr_read(void) {
    printf("[c] ->start QR read\n");
    int ret = QRCode_RxStr(qrfd1, qrTmpBuff, sizeof(qrTmpBuff), 100);

    if (ret <= 0) {
        ret = QRCode_RxStr(qrfd2, qrTmpBuff, sizeof(qrTmpBuff), 100);
    }

    if (ret > 0) {
        Sys_BeepMs(100);
        memset(buffer, 0, sizeof(buffer));
        buffer[0] = 0x02;

        // 确保字符串以null结尾
        qrTmpBuff[ret] = '\0';
        printf("[c] -> QR len=%d cod=%s\n", ret, qrTmpBuff);

        memcpy(buffer + 1, qrTmpBuff, ret);
        char onlyLenBuffer[ret + 1];
        onlyLenBuffer[0] = 0x02;
        memcpy(onlyLenBuffer + 1, qrTmpBuff, ret);

        send(client_socket, onlyLenBuffer, ret + 1, 0);
        usleep(3000000);
        printf("[c] ->QR scanner sleep 3s\n");
    }
}

void ic_read(void) {
    printf("[c] ->start IC read\n");
    int ret = -1;
    int i = 0; // 假设i为0，原代码中i未初始化

    ret = Mifare_PowerOn(0, icSnr, &icSnrLen);
    if (!ret) ret = Mifare_AuthenBlock(i * 4, 0, icKey);
    if (!ret) ret = Mifare_ReadBlock(0 + i * 4, icData);

    if (!ret) {
        dump_data("Mifare Read0", icData, sizeof(icData));
        printf("[c] ->ic read len=%d\n", ret);
        Sys_BeepMs(100);

        icTmpBuffer[0] = icType;
        icTmpBuffer[1] = icData[0];
        icTmpBuffer[2] = icData[1];
        icTmpBuffer[3] = icData[2];
        icTmpBuffer[4] = icData[3];

        dump_data("[c] ->Send IC Data:\n", icTmpBuffer, sizeof(icTmpBuffer));
        send(client_socket, icTmpBuffer, sizeof(icTmpBuffer), 0);
        printf("[c] ->ic read sensor sleep 3s\n");
        usleep(3000000);
    }
}

void id_read(void) {
    printf("[c] ->start ID read\n");
    ushort len;
    ushort ret;

    pthread_spin_lock(&lock);
    ret = IDCARD_AutoRead(&len, idBuffer);
    printf("[c] -> IDCARD_AutoRead returned: %d, len: %d\n", ret, len);
    pthread_spin_unlock(&lock);

    if (ret == 0) {
        printf("[c] ->read id card success\n");
        parse_id_info((char*)&idBuffer[7], &idData);
        dump_id_info2(&idData);

        unsigned char tmpBuffer[201] = {0};
        tmpBuffer[0] = 0x03; // ID_CARD type
        memcpy(tmpBuffer + 1, idData.name, 100);
        memcpy(tmpBuffer + 101, idData.id_number, 100);

        send(client_socket, tmpBuffer, sizeof(tmpBuffer), 0);
        usleep(3000000);
    } else {
        printf("[c] ->read id card fail,ret=%d\n", ret);
    }
}

void dump_id_info2(ID_DATA *id_data){
    printf("姓名 %s\n",id_data->name);
    printf("性别 %s 民族:%s\n",id_data->sex,id_data->nation);
    printf("出生 %s\n",id_data->birth_day);
    printf("住址 %s\n",id_data->address);
    printf("身份号 %s\n",id_data->id_number);
    printf("签发单位 %s\n",id_data->department);
    printf("有效期限 %s-%s\n",id_data->expire_start_day,	id_data->expire_end_day);
}

static char * unicode_utf8(char **unicode_ptr,size_t inlen, char outbuf[100]){
    char  inbuf[1024];
    size_t retlen,outlen;
    char *unicode = (char*) *unicode_ptr;
    size_t tmp_len = inlen;
    retlen = outlen = 100;
    memcpy(inbuf,unicode,inlen);
    conv_to_unicode("UCS-2LE","UTF-8",inbuf,&inlen,outbuf,&retlen);
    *unicode_ptr += tmp_len - inlen;
    outbuf[outlen - retlen] = 0x00;
    return outbuf;
}
void parse_id_info(char *rx_buffer, ID_DATA *id_data){
    char *ptr = rx_buffer;
    memset(id_data,0,sizeof(ID_DATA));
    unicode_utf8(&ptr,kNameLen        ,id_data->name);
    unicode_utf8(&ptr,kSexLen         ,id_data->sex);
    unicode_utf8(&ptr,kNationLen      ,id_data->nation);
    unicode_utf8(&ptr,kBirthDayLen    ,id_data->birth_day);
    unicode_utf8(&ptr,kAddressLen     ,id_data->address);
    unicode_utf8(&ptr,kIdNumberLen    ,id_data->id_number);
    unicode_utf8(&ptr,kDepartmentLen  ,id_data->department);
    unicode_utf8(&ptr,kExpireEndDayLen,id_data->expire_start_day);
    unicode_utf8(&ptr,kExpireEndDayLen,id_data->expire_end_day);
    unicode_utf8(&ptr,kReservedLen    ,id_data->reserved);
#ifdef IDTWO_PHOTO
    unpackBmp(&rx_buffer[256]);
#endif
}
void init_spinlock() {
    pthread_spin_init(&lock, 0);
}

void destroy_spinlock() {
    pthread_spin_destroy(&lock);
}
