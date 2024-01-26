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

#define BUFFER_SIZE 1024 * 2
char buffer[BUFFER_SIZE];
int stop_requested = 0;
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
    // 创建套接字
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // 设置服务器地址信息
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Error converting server IP address");
        close(client_socket);
        return -1;
    }

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        return -1;
    }

    return client_socket;
}

int send_message(int client_socket, const char *message) {
    printf("send message\n");
    // 发送消息给服务器
    int message_length = strlen(message);
    ssize_t bytes_sent = send(client_socket, message, message_length, 0);
    if (bytes_sent == -1) {
        printf("send message==-1\n");
        perror("Error sending message to server");
        close(client_socket);
        return -1;
    } else if (bytes_sent < message_length) {
        printf("send message==-2\n");
        fprintf(stderr, "Not all bytes sent to server\n");
        close(client_socket);
        return -1;
    } else if (bytes_sent == 0) {
        printf("send message==-3\n");
        fprintf(stderr, "Connection closed by server\n");
        close(client_socket);
        return -1;
    }
    printf("send ok\n");
    return 0;  // 成功发送消息
}

void close_connection(int client_socket) {
    // 关闭套接字
    close(client_socket);
}
void close_tcp_connection() {
	stop_requested=1;
    if (client_socket != -1) {
        close(client_socket);
        client_socket = -1;  // 将 client_socket 重置为无效值
    }
}

int client_socket;
int start_tcp(void) {
    printf("C start\n");
    const char *server_ip = "127.0.0.1";  // 修改为你的服务器IP地址
    const int server_port = 9999;         // 修改为服务器监听的端口号

    client_socket = connect_to_server(server_ip, server_port);
    if (client_socket == -1) {
        fprintf(stderr, "Failed to connect to server\n");
        return 1;
    }
    return 0;
}
int start_card(){
		printf("C send1\n");
	while(!stop_requested){
  		sleep(1); // 等待1秒钟再发送下一条消息
        const char *message = "Hello, Send1!";
        if (send_message(client_socket, message) == -1) {
            printf("send message==--1\n");
            fprintf(stderr, "Failed to send message\n");
            close_connection(client_socket);
            return 1;
        }
	}
}
int start_qr(){
	printf("C start_qr\n");
	int qrfd1,qrfd2,ret;
	unsigned char TmpBuff[1024];
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);
	uint8_t type = 0x02;
	while(!stop_requested){
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
			printf("length: [%d]\n",ret);
			printf("\nQRCODE-> : %s\n",str);
			send(client_socket, buffer, BUFFER_SIZE, 0);
		}
	}
}

void ic_read(void){
    int ret = -1;
    uint8_t key[] = "\xFF\xFF\xFF\xFF\xFF\xFF";
    uint8_t data[16],data_len = 16;
    uint8_t snr[16],snr_len;
    int i;
    PICC_Open(0);
	uint8_t type = 0x01;
	while(!stop_requested){
        if(ret) ret = Mifare_PowerOn(0,snr,&snr_len);
        printf("\n==========Block[%2d]==========\n",i);
        if(!ret) ret = Mifare_AuthenBlock(i * 4,0,key);
        if(!ret) ret = Mifare_ReadBlock(0 + i * 4,data);
        //if(!ret) dump_data("Mifare Read0",data,data_len);
        if(!ret){
			dump_data("Mifare Read0",data,data_len);
			memset(buffer, 0, sizeof(buffer));
			buffer[0]=type;
			buffer[1]=data[0];
			buffer[2]=data[1];
			buffer[3]=data[2];
			buffer[4]=data[3];
			send(client_socket, buffer, BUFFER_SIZE, 0);
		}
		usleep(1000000);
	}
}

//close_connection(client_socket);
*/
import "C"
import (
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"time"
	"unsafe"
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

// Display const
const (
	DISP_FONT12     = C.DISP_FONT12
	DISP_FONT32     = C.DISP_FONT32
	DISP_FONT24     = C.DISP_FONT24
	DISP_GBK        = C.DISP_GBK
	DISP_CLRLINE    = C.DISP_CLRLINE
	DISP_UNDERLINE  = C.DISP_UNDERLINE
	DISP_REVERT     = C.DISP_REVERT
	DISP_ALIGN_MASK = C.DISP_ALIGN_MASK
	DISP_RIGHT      = C.DISP_RIGHT
	DISP_FONT6X8    = C.DISP_FONT6X8
	DISP_BIGFONT    = C.DISP_BIGFONT
)

type Display struct {
}

func (d Display) Init() {
	pkg.Log.Println("display Init")
	C.LCD_Init()
	cLedHeight := C.LCD_GetHeight()
	cLedWidth := C.LCD_GetWidth()
	pkg.Log.Println("display height", cLedHeight)
	pkg.Log.Println("display height", cLedWidth)
}

// ClearScreen 清理屏幕
func (d Display) ClearScreen() {
	cScreen := C.uint32_t(1)
	pkg.Log.Println("display ClearScreen")
	C.LCD_ClearScreen(cScreen)
}

// LCDRow 显示文字
func (d Display) LCDRow(text string, x int16, y int16, mode C.uint) {
	pkg.Log.Println(text, x, y, mode)
	cTitle := C.CString(text)
	defer C.free(unsafe.Pointer(cTitle))
	cX := C.uint16_t(x)
	cY := C.uint16_t(y)
	C.LCD_Display_Row(cTitle, cY, cX, mode)
}

// StartC 启动C方法
func StartC() {
	defer C.close_tcp_connection()
	C.start_tcp()
	time.Sleep(time.Second)
	go C.start_qr()
	go C.ic_read()
}
