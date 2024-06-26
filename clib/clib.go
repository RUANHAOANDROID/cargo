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

#define BUFFER_SIZE 1024
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
	stop_requested=1;
	client_socket=-1;
	// 关闭套接字
    close(client_socket);
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
	unsigned char TmpBuff[1024];
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);
	uint8_t type = 0x02;
	while(!stop_requested){
		//printf("c ->read QR\n");
		ret = QRCode_RxStr(qrfd1, TmpBuff, 1024, 100);
		if(ret <= 0)
			ret = QRCode_RxStr(qrfd2, TmpBuff, 1024, 100);
		if(ret > 0)
		{
			Sys_BeepMs(100);
			memset(buffer, 0, sizeof(buffer));
			buffer[0]=type;
			char str[1024];
			snprintf(str, sizeof(str), "%s", TmpBuff);
  			memcpy(buffer + 1, TmpBuff, sizeof(TmpBuff));
			printf("[c] -> QR len=%d cod=%s\n",ret,str);
			//printf("\nQRCODE-> : %s\n",str);
			send(client_socket, buffer, BUFFER_SIZE, 0);
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
        //if(!ret) dump_data("Mifare Read0",data,data_len);
        if(!ret){
			Sys_BeepMs(100);
			//dump_data("Mifare Read0",data,data_len);
			memset(buffer, 0, sizeof(buffer));
			buffer[0]=type;
			buffer[1]=data[0];
			buffer[2]=data[1];
			buffer[3]=data[2];
			buffer[4]=data[3];
			send(client_socket, buffer, BUFFER_SIZE, 0);
			printf("[c] ->ic read sensor sleep 2s\n");
			usleep(3000000);
		}
		usleep(300000);
	}
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

var tmpBuff [1024]C.uchar

func (q *Scanner) Receive() {
	index := C.int(0)
	qrfd0 := C.QRCode_Open(index)
	//index = C.int(1)
	//qrfd1 := C.QRCode_Open(index)
	C.Beep_Init()

	for {
		//value := (*C.char)(unsafe.Pointer(&goSlice[0]))
		ret := C.QRCode_RxStr(qrfd0, &tmpBuff[0], 1024, 100)
		//if ret <= 0 {
		//	fmt.Println("ret <= 0")
		//	ret = C.QRCode_RxStr(qrfd1, &tmpBuff[0], 1024, 100)
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
