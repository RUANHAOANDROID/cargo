#include "clib.h"

char buffer[BUFFER_SIZE];
int stop_requested = 0;
int client_socket = -1;
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
    while (!stop_requested) {
        qr_read();
        id_read();
        id_read();
        usleep(1000000);
    }
}

void qr_read(void) {
    printf("[c] ->start QR read\n");
    int qrfd1, qrfd2, ret;
    unsigned char TmpBuff[512];
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);
    uint8_t type = 0x02;
    ret = QRCode_RxStr(qrfd1, TmpBuff, 512, 100);
    if (ret <= 0)
        ret = QRCode_RxStr(qrfd2, TmpBuff, 512, 100);
    if (ret > 0) {
        Sys_BeepMs(100);
        memset(buffer, 0, sizeof(buffer));
        buffer[0] = type;
        char str[512];
        snprintf(str, sizeof(str), "%s", TmpBuff);
        memcpy(buffer + 1, TmpBuff, sizeof(TmpBuff));
        printf("[c] -> QR len=%d cod=%s\n", ret, str);
        char onlyLenBuffer[ret + 1];
        onlyLenBuffer[0] = type;
        memcpy(onlyLenBuffer + 1, TmpBuff, ret);
        send(client_socket, onlyLenBuffer, ret + 1, 0);
        usleep(3000000);
        printf("[c] ->QR scanner sleep 2s\n");
    }
}

void ic_read(void) {
    printf("[c] ->start IC read\n");
    int ret = -1;
    uint8_t key[] = "\xFF\xFF\xFF\xFF\xFF\xFF";
    uint8_t data[16], data_len = 16;
    uint8_t snr[16], snr_len;
    int i;
    PICC_Open(0);
    uint8_t type = 0x01;
    if (ret) ret = Mifare_PowerOn(0, snr, &snr_len);
    if (!ret) ret = Mifare_AuthenBlock(i * 4, 0, key);
    if (!ret) ret = Mifare_ReadBlock(0 + i * 4, data);
    if (!ret) dump_data("Mifare Read0", data, data_len);
    if (!ret) {
        printf("[c] ->ic read len=%d\n", ret);
        Sys_BeepMs(100);
        unsigned char tmpBuffer[5];
        tmpBuffer[0] = type;
        tmpBuffer[1] = data[0];
        tmpBuffer[2] = data[1];
        tmpBuffer[3] = data[2];
        tmpBuffer[4] = data[3];
        dump_data("[c] ->Send IC Data:\n", tmpBuffer, sizeof(tmpBuffer));
        send(client_socket, tmpBuffer, sizeof(tmpBuffer) + 1, 0);
        printf("[c] ->ic read sensor sleep 2s\n");
        usleep(3000000);
    }
}

void id_read(void) {
    printf("[c] ->start ID read\n");
    unsigned char buffer[2400] = {0};
    ushort len;
    ushort ret;
    char name[100];
    char id_number[100];
    pthread_spin_lock(&lock);
    ret = IDCARD_AutoRead(&len, buffer);
    printf("[c] -> IDCARD_AutoRead returned: %d, len: %d\n", ret, len);
    pthread_spin_unlock(&lock);
    if (ret == 0) {
        printf("[c] ->read id card success\n");
        // 提取并转换 name
        for (int i = 0; i < 99; i++) {
            name[i] = isprint(buffer[i]) ? buffer[i] : '.';  // 只保留可打印字符
        }
        name[99] = '\0';

        // 提取并转换 id_number
        for (int i = 0; i < 99; i++) {
            id_number[i] = isprint(buffer[500 + i]) ? buffer[500 + i] : '.';  // 只保留可打印字符
        }
        id_number[99] = '\0';
        // 打印转换后的字符串
        printf("[INFO] [c] ->ID Card Info: Name: %s\n", name);
        // 直接使用 printf 打印
        printf("[INFO] [c] ->ID Card Info:ID Number: %s\n",id_number);
        usleep(3000000);
    } else {
        printf("[c] ->read id card fail,ret=%d\n",ret);
    }
}

void init_spinlock() {
    pthread_spin_init(&lock, 0);
}

void destroy_spinlock() {
    pthread_spin_destroy(&lock);
}