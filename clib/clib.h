#ifndef CLIB_H
#define CLIB_H

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

extern char buffer[BUFFER_SIZE];
extern int stop_requested;
extern int client_socket;
// 定义身份证信息结构体
typedef struct{
	char name[100];
	char sex[100];
	char nation[100];
	char birth_day[100];
	char address[100];
	char id_number[100];
	char department[100];
	char expire_start_day[100];
	char expire_end_day[100];
	char reserved[2400];
}ID_DATA;

void dump_data(char *str, unsigned char *text, int len);
int connect_to_server(const char *server_ip, int server_port);
int send_message(int client_socket, const char *message);
void close_connection(int client_socket);
int start_tcp(void);
int qr_read(void);
void ic_read(void);
void id_read(void);

#endif // CLIB_H