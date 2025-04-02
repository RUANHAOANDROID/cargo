package clib

/*
#cgo LDFLAGS: -L../sdk/lib -lpos -lpng -lfreetype -liconv -lm -lz -lrt
#cgo CFLAGS: -I../sdk/inc
#include "../sdk/inc/Felica.h"
#include "../sdk/inc/mc_led.h"
#include "../sdk/inc/oslib.h"
#include "../sdk/inc/toolslib.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include <arpa/inet.h>
#include <pthread.h>
#define BUFFER_SIZE 512
char buffer[BUFFER_SIZE];
int stop_requested = 0;
int client_socket=-1;
void dump_data(char *str,unsigned char *text,int len)
{
	int i;
    printf("%s(%d):",str,len);
    for(i = 0;i < len;i++)
    {
        printf("%02X ",(unsigned char)text[i]);
    }
    printf("\n");
}

int connect_to_server(const char *server_ip, int server_port) {
  	printf("[c] ->connect_to_server \n");
    // 创建套接字
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("[c] ->Error creating socket");
        return -1;
    }

    // 设置服务器地址信息
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("[c] ->Error converting server IP address");
        close(client_socket);
        return -1;
    }

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("[c] ->Error connecting to server");
        close(client_socket);
        return -1;
    }

    return client_socket;
}

int send_message(int client_socket, const char *message) {
    printf("[c] ->send message\n");
    // 发送消息给服务器
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
    return 0;  // 成功发送消息
}

void close_connection(int client_socket) {
	printf("[c] ->close_connection\n");
	// 关闭套接字
    close(client_socket);
	stop_requested=1;
	client_socket=-1;
}

int start_tcp(void) {
    printf("[c] ->start_tcp\n");
    const char *server_ip = "127.0.0.1";  // 修改为你的服务器IP地址
    const int server_port = 9999;         // 修改为服务器监听的端口号

    client_socket = connect_to_server(server_ip, server_port);
    if (client_socket == -1) {
        fprintf(stderr, "Failed to connect to server\n");
        return 1;
    }
    return 0;
}

int qr_read(){
	printf("[c] ->start qr read\n");
	int qrfd1,qrfd2,ret;
	unsigned char TmpBuff[512];
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);
	uint8_t type = 0x02;
	while(!stop_requested){
		//printf("c ->read QR\n");
		ret = QRCode_RxStr(qrfd1, TmpBuff, 512, 100);
		if(ret <= 0)
			ret = QRCode_RxStr(qrfd2, TmpBuff, 512, 100);
		if(ret > 0)
		{
			Sys_BeepMs(100);
			memset(buffer, 0, sizeof(buffer));
			buffer[0]=type;
			char str[512];
			snprintf(str, sizeof(str), "%s", TmpBuff);
  			memcpy(buffer + 1, TmpBuff, sizeof(TmpBuff));
			printf("[c] -> QR len=%d cod=%s\n",ret,str);
			//printf("\nQRCODE-> : %s\n",str);
			char onlyLenBuffer[ret+1];
			onlyLenBuffer[0]=type;
			memcpy(onlyLenBuffer+1,TmpBuff,ret);
			send(client_socket, onlyLenBuffer, ret+1, 0);
			//send(client_socket, buffer, BUFFER_SIZE, 0);
			usleep(3000000);
			printf("[c] ->QR scanner sleep 2s\n");
		}
		usleep(300000);
	}
}

void ic_read(){
	printf("[c] ->start ic read\n");
    int ret = -1;
    uint8_t key[] = "\xFF\xFF\xFF\xFF\xFF\xFF";
    uint8_t data[16],data_len = 16;
    uint8_t snr[16],snr_len;
    int i;
    PICC_Open(0);
	uint8_t type = 0x01;
	while(!stop_requested){
		//printf("c ->read IC\n");
        if(ret) ret = Mifare_PowerOn(0,snr,&snr_len);
        //printf("\n==========Block[%2d]==========\n",i);
        if(!ret) ret = Mifare_AuthenBlock(i * 4,0,key);
        if(!ret) ret = Mifare_ReadBlock(0 + i * 4,data);
        if(!ret) dump_data("Mifare Read0",data,data_len);
        if(!ret){
			printf("[c] ->ic read len=%d\n",ret);
			Sys_BeepMs(100);
			//dump_data("Mifare Read0",data,data_len);
  	        unsigned char tmpBuffer[5];
			tmpBuffer[0]=type;
			tmpBuffer[1]=data[0];
			tmpBuffer[2]=data[1];
			tmpBuffer[3]=data[2];
			tmpBuffer[4]=data[3];
	        dump_data("[c] ->Send IC Data:\n",tmpBuffer,sizeof(tmpBuffer));
			send(client_socket, tmpBuffer, sizeof(tmpBuffer)+1, 0);
			printf("[c] ->ic read sensor sleep 2s\n");
			usleep(3000000);
		}
		usleep(300000);
	}
}
// 定义身份证信息结构体
typedef struct {
    char name[32];            // 姓名
    char sex[4];              // 性别
    char nation[16];          // 民族
    char birth_day[12];       // 出生日期
    char address[128];        // 住址
    char id_number[20];       // 身份证号码
    char department[64];      // 签发机关
    char expire_start_day[12]; // 有效期起始日期
    char expire_end_day[12];  // 有效期截止日期
    char reserved[256];       // 保留字段
} ID_DATA;
// 全局自旋锁
pthread_spinlock_t lock;
// 锁状态
int lock_status =-1;

int id_read(int dumpInfo){
	if(lock_status == -1){
		printf("init lock");
		pthread_spin_init(&lock, 0);
		lock_status ==1;
	}
	printf("[c] ->start id read\n");
	// 定义缓冲区和变量
    unsigned char idtwo_getbuff[2400] = {0};    // 存储身份证数据的缓冲区
    ushort len = 0;                             // 数据长度
    ushort ret = 0;                             // 函数返回状态
    ID_DATA id_data;                            // 身份证信息结构体
    unsigned long tick;                         // 时间戳，用于性能测量
	// 1. 初始化显示（如果需要显示信息）
    if (dumpInfo) {
        LCD_ClearScreen(0);                     // 清空屏幕
        LCD_ClearAll();                         // 清空所有显示内容
        LCD_Printf(10 + 0, DISP_FONT12, "身份证测试：");    // 显示测试标题
        LCD_Printf(10 + 24, DISP_FONT24 | DISP_CENTER, "请放身份证"); // 提示放置身份证
    }
	// 2. 执行身份证读取操作
    tick = OSTIMER_GetTickCount();              // 记录开始时间
    pthread_spin_lock(&lock);                   // 加锁，保护共享资源

    #if 1
        // 标准身份证读取模式
        ret = IDCARD_AutoRead(&len, idtwo_getbuff);
    #else
        // 带指纹的身份证读取模式
        ret = IDCARD_AutoRead_Fig(&len, idtwo_getbuff);
    #endif
	pthread_spin_unlock(&lock);                 // 解锁
	// 3. 处理读取结果
    if (ret == 0) {
        // 读取成功
        unsigned long duration = OSTIMER_GetTickCount() - tick;  // 计算耗时
        printf("读身份证ok[%ld ms]！！！！！！！！！！！！！！！！\n", duration);
        // Sys_Beep();                          // 可选：成功时发出提示音

        // 解析身份证数据
        #if 1
            parse_id_info((char*)&idtwo_getbuff[7], &id_data);
        #else
            parse_id_info((char*)&idtwo_getbuff[9], &id_data);   // 带指纹模式偏移不同
        #endif

        // 根据dumpInfo选择显示详细程度
        if (dumpInfo) {
            dump_id_info(&id_data);         // 显示详细信息
        } else {
            dump_id_info2(&id_data);        // 显示简略信息
        }
    } else {
        // 读取失败
        printf("读身份证fail,ret=%d\n", ret);
        if (dumpInfo) {
            LCD_Printf(10 + 48, DISP_FONT12, "测试失败 [%d]", ret);  // 显示错误码
        }
    }

    return ret;     // 返回操作结果
}
*/
import "C"
import (
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"sync"
	"time"
)

type ICCarder struct {
	msgChan chan msg.Message
}

func NewICCarder(msgChan chan msg.Message) *ICCarder {
	return &ICCarder{
		msgChan: msgChan,
	}
}

type Scanner struct {
	QRChan chan msg.Message
}

func NewScanner(msgChan chan msg.Message) *Scanner {
	return &Scanner{
		QRChan: msgChan,
	}
}

var tmpBuff [512]C.uchar

func (q *Scanner) Receive() {
	index := C.int(0)
	qrfd0 := C.QRCode_Open(index)
	//index = C.int(1)
	//qrfd1 := C.QRCode_Open(index)
	C.Beep_Init()

	for {
		//value := (*C.char)(unsafe.Pointer(&goSlice[0]))
		ret := C.QRCode_RxStr(qrfd0, &tmpBuff[0], 512, 100)
		//if ret <= 0 {
		//	fmt.Println("ret <= 0")
		//	ret = C.QRCode_RxStr(qrfd1, &tmpBuff[0], 512, 100)
		//}
		if ret > 0 {
			C.Sys_BeepMs(200)
			uChars := tmpBuff[:ret]
			fmt.Print(uChars)
			// Convert the C array to a Go []byte
			byteSlice := make([]byte, len(uChars))
			for i, v := range uChars {
				byteSlice[i] = byte(v)
			}
			hexString := string(byteSlice)
			fmt.Println(hexString)
			q.QRChan <- msg.Message{Type: msg.QRCODE, Content: hexString}
		}
	}
}

// StartC 启动C方法
func StartC(wg sync.WaitGroup) {
	pkg.Log.Println("Start C TCP Client wait 1 Second---")
	time.Sleep(time.Second)
	//defer StopC()
	C.start_tcp()
	wg.Add(1)
	go func() {
		defer wg.Done()
		C.qr_read()
	}()
	wg.Add(1)
	go func() {
		defer wg.Done()
		C.ic_read()
	}()
	wg.Add(1)
	go func() {
		defer wg.Done()
		C.id_read(0)
	}()
	//C.start_tcp()
	//time.Sleep(time.Second)
	//go func() {
	//	defer C.close_tcp_connection()
	//	C.qr_read()
	//	C.ic_read()
	//}()

}
func StopC() {
	pkg.Log.Println("close tcp connection")
	defer C.close_connection(-1)
}
