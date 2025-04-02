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
int start_tcp(void);
void qr_read(void);
void ic_read(void);
void id_read(void);
void read_data(void);
void parse_id_info(char *rx_buffer, ID_DATA *id_data);
void dump_id_info2(ID_DATA *id_data);
char *sex_group[2] = {"女","男"};
char *nation_group[100] ={
"无",
"汉",
"蒙古",
"回",
"藏",
"维吾尔",
"苗",
"彝",
"壮",
"布依",
"朝鲜",
"满",
"侗",
"瑶",
"白",
"土家",
"哈尼",
"哈萨克",
"傣",
"黎",
"傈僳",
"佤",
"畲",
"高山",
"拉祜",
"水",
"东乡",
"纳西",
"景颇",
"柯尔克孜",
"土",
"达斡尔",
"仫佬",
"羌",
"布朗",
"撒拉",
"毛南",
"仡佬",
"锡伯",
"阿昌",
"普米",
"塔吉克",
"怒",
"乌孜别克",
"俄罗斯",
"鄂温克",
"德昂",
"保安",
"裕固",
"京",
"塔塔尔",
"独龙",
"鄂伦春",
"赫哲",
"门巴",
"珞巴",
"基诺",
"其他",
"革家人",
"穿青人",//59
"其他", //97
"外国血统中国籍人士" //98
};
static short dc_ParseOtherInfo(int flag, char *in_info,char *out_info)
{
  if (flag == 0) {
    if (in_info[0] == '1') {
      strcpy(out_info, "男");
    } else {
      strcpy(out_info, "女");
    }
    return 0;
  } else if (flag == 1) {
    int nation;
    in_info[2] = 0;
    nation = atoi(in_info);

    if(nation == 97) {
      strcpy(out_info, "其他");
    } else if(nation == 98) {
      strcpy(out_info, "外国血统中国籍人士");
    } else if(nation <= 96){
    	strcpy(out_info, nation_group[nation]);
    } else{
      strcpy(out_info, "无");
    }
    return 0;
  }

  return -1;
}
#endif // CLIB_H