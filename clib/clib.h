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
#include <stdbool.h>

#define BUFFER_SIZE 512

extern char buffer[BUFFER_SIZE];
extern int stop_requested;
extern int client_socket;

static const int kNameLen = (15 << 1);
static const int kSexLen = (1 << 1);
static const int kNationLen = (2 << 1);
static const int kBirthDayLen = (8 << 1);
static const int kAddressLen = (35 << 1);
static const int kIdNumberLen = (18 << 1);
static const int kDepartmentLen = (15 << 1);
static const int kExpireStartDayLen = (8 << 1);
static const int kExpireEndDayLen = (8 << 1);
static const int kReservedLen = (18 << 1);
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
void init_devices();
void close_devices();
int start_tcp(void);
void qr_read(void);
void ic_read(void);
void id_read(void);
void read_data(int enable_ic, int enable_id);
void parse_id_info(char *rx_buffer, ID_DATA *id_data);
void dump_id_info2(ID_DATA *id_data);

extern bool conv_to_unicode (char *encFrom, char *encTo,char *inbuf, size_t *inlen, char *outbuf, size_t *outlen);

#endif // CLIB_H