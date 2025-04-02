#include <stdlib.h>		
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <ctype.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>  
#include <pthread.h>
#include <sched.h>
#include <assert.h>

#include <time.h>  
#include "oslib.h"
#include "toolslib.h"
#include "mc_led.h"
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>

//err para
#define IDTWO_TRUE        0x00
#define IDTWO_PICCPOLLERR 0x01
#define IDTWO_PICCAPDUERR 0x02
#define IDTWO_OPENIDERR   0x03
#define IDTWO_GETI2CERR   0x04

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

pthread_spinlock_t lock = 0;
pthread_mutex_t mutex; 

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
extern bool conv_to_unicode (char *encFrom, char *encTo,char *inbuf, size_t *inlen, char *outbuf, size_t *outlen);
static char * unicode_utf8(char **unicode_ptr,size_t inlen, char outbuf[100])
{
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
#ifdef IDTWO_PHOTO
#include <dlfcn.h>

void rgbToBgr(char *rgb, int width, int height, char *bgr) {

    for (int j = 0; j < height; ++j) {

        for (int i = 0; i < width * 3; i = i + 3) {

            char temp[3];
            memcpy(temp, rgb + j * width * 3 + i, 3);
            char tp = temp[2];
            temp[2] = temp[0];
            temp[0] = tp;
            memcpy(bgr + j * 2 + j * width * 3 + i, temp, 3);
        }

        memcpy(bgr + (j + 1) * width * 3 + j * 2, "\x00\x00", 2);
    }

}
void rgbToBmp(char *rgb, int xsize, int ysize, char * bmp){
    unsigned char header[54] = {
            0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
            54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0
    };

    long file_size = (long)xsize * (long)ysize * 3 + (long)ysize * 2 + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    long width = xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;

    long height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
    memcpy(bmp, header, 54);
    char bgr[xsize * ysize * 3 + ysize * 2];
    rgbToBgr(rgb, xsize, ysize, bgr);
    memcpy(bmp + 54, bgr, xsize * ysize * 3 + ysize * 2);
}

typedef int (*UNPACK)(char *, char *, int);

int getbmp(char *buf,char *bmp){
    //const char *wltsopath = "libwlt2bmp.so";
    const char *wltsopath = "libwlt_wei.so";
    void *handle = NULL;
    handle = dlopen(wltsopath, RTLD_LAZY);
    if(NULL != handle) {

        printf("dlopen success!\n");
        dump_data("reserved-->",buf, 2400);

        UNPACK unpack = 0;
        unpack = (UNPACK)dlsym(handle, "_Z6unpackPcS_i");
        if(unpack) {
            //printf("unpack result: %d \n", unpack(buf, bmp, 708));
            printf("unpack result: %d \n", unpack(buf, bmp, 752));
        } else {
            printf("dlsym error!\n");
        }
        dlclose(handle);
    } else {
        printf("dlopen %s error: %s!\n", wltsopath, dlerror());
        return -1;
    }
    return 0;
}
int save_bmp(char *data, const int dataLen,const char *path){
    FILE *fp;

    fp = fopen(path,"wb");
    if(fp == NULL)
        return -1;
    fwrite(data,sizeof(char),dataLen,fp);
    fclose(fp);
    return 0;
}

int unpackBmp(char *rx_buffer){
    const int rgbLen = 102*126*3;
    const int bmpLen = 54+102*126*3+126*2;
    
    char reserved[54+102*126*3+126*2];

    char *rgbBuf= (char *)malloc(rgbLen*sizeof(char));
    char *bmpBuf= (char *)malloc(bmpLen*sizeof(char));
    const char *bmpFile = "id.bmp";

    int ret;

    memset(bmpBuf,0,bmpLen);
    memset(rgbBuf,0x55,rgbLen);

    ret = getbmp((char *)rx_buffer,rgbBuf);
    if(ret < 0)
        return ret;
    rgbToBmp(rgbBuf,102,126,bmpBuf);
    
    dump_data("bmpBuff-->",bmpBuf, 1024);
    save_bmp(bmpBuf,bmpLen,bmpFile);
    free(rgbBuf);
    free(bmpBuf);

    return ret;
}
#endif
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

    dc_ParseOtherInfo(0,id_data->sex,id_data->sex);
    dc_ParseOtherInfo(1,id_data->nation,id_data->nation);
#ifdef IDTWO_PHOTO
    unpackBmp(&rx_buffer[256]);
#endif
}

void dump_id_info(ID_DATA *id_data){
	
	if(Sys_TermType() == P18_L2C){
		char tmp[7][100];
		fb_disp_start();
		LCD_ClearScreen(0);		
		sprintf(tmp[0],"姓名 %s",id_data->name);
		sprintf(tmp[1],"性别 %s 民族:%s",id_data->sex,id_data->nation);
		sprintf(tmp[2],"出生 %s",id_data->birth_day);
		sprintf(tmp[3],"住址 %s",id_data->address);
		sprintf(tmp[4],"身份号 %s",id_data->id_number);
		sprintf(tmp[5],"签发单位 %s",id_data->department);
		sprintf(tmp[6],"有效期限 %s-%s",id_data->expire_start_day,	id_data->expire_end_day);
		for(int i=0;i<7;i++){
			fb_utf8_display_align(30,5, -1,100+50*i,tmp[i],RTGUI_RGB(0,0,0),DISP_CLRLINE); 
		}
		fb_disp_end();
	}else{
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(10+ 0,DISP_FONT12,"姓名 %s",id_data->name);
		LCD_Printf(10+12,DISP_FONT12,"性别 %s 民族:%s",id_data->sex,id_data->nation);
		LCD_Printf(10+24,DISP_FONT12,"出生 %s",id_data->birth_day);
		LCD_Printf(10+24,DISP_FONT12,"住址 %s",id_data->address);
		LCD_Printf(10+48,DISP_FONT12,"身份号 %s",id_data->id_number);
		
		OSTIMER_DelayMs(1000);
		LCD_ClearAll();
		LCD_Printf(24,DISP_FONT12,"签发单位 %s",id_data->department);
		LCD_Printf(48,DISP_FONT12,"有效期限 %s-%s",id_data->expire_start_day,	id_data->expire_end_day);
		OSTIMER_DelayMs(1000);

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


int h2d(unsigned char *s, int size) { 
	int a = 0;
	int i;
	for(i = 0; i < size; i++){
		a+=s[i] << 8*i;
	}
	return a;
}

extern ushort IDCARD_Get_Sam_Ver(ushort* len,unsigned char* para);

int getIdMoudleNumber(char *moudleNum){
	ushort  len;
	ushort  ret ;  

    unsigned char *version;
	int major_version, minor_version, date, major_code, minor_code;
	int base_add = 10;

    version = (unsigned char*)malloc(2048);

    ret = IDCARD_Get_Sam_Ver(&len, version);
    if(ret == 0){
        //printf("IDCARD_Get_Sam_Ver success\n");
        //dump_data("SAM_Ver-->",version,len);
        free(version);
        if((version[0] != 0xaa) || (version[1] != 0xaa)){
            printf("IDCARD_Get_Sam_Ver failed\n");
            return -1;
        }else{
			major_version = h2d(version+base_add, 2);
			minor_version = h2d(version+(base_add+2), 2);
			date = h2d(version+(base_add+4), 4);
			major_code = h2d(version+(base_add+8), 4);
			minor_code = h2d(version+(base_add+12), 4);
			printf("\nmodule number: %02u%02u%08u%010u%010u\n\n", major_version, minor_version, date, major_code, minor_code);
			sprintf((char *)moudleNum, "%02u%02u%08u%010u%010u", major_version, minor_version, date, major_code, minor_code);
			return ret;
		}
    }else{
        printf("IDCARD_Get_Sam_Ver failed\n");
        return -1;
    }
}

int idtwotest(int dumpInfo)
{
	unsigned char idtwo_getbuff[2400] = {0};
	ushort  len;
	ushort  ret ;
	
	ID_DATA id_data;
    unsigned long tick;
    
/*
    unsigned char *version;

    version = (unsigned char*)malloc(2048);

    ret = IDCARD_Get_Sam_Ver(&len, version);
    if(ret == 0){
        printf("IDCARD_Get_Sam_Ver success\n");
        dump_data("SAM_Ver-->",version,len);
        free(version);
        if((version[0] != 0xaa) || (version[1] != 0xaa)){
            printf("IDCARD_Get_Sam_Ver failed\n");
            return -1;
        }
    }else{
        printf("IDCARD_Get_Sam_Ver failed\n");
        return -1;
    }
*/
	//while(1)
	{
    #if 1
		if(dumpInfo){
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(10+ 0,DISP_FONT12,"身份证测试：");
			LCD_Printf(10+24,DISP_FONT24|DISP_CENTER,"请放身份证");
		}
        tick = OSTIMER_GetTickCount();
		pthread_spin_lock(&lock);	
		ret = IDCARD_AutoRead(&len,idtwo_getbuff);
		pthread_spin_unlock(&lock);	

		if(ret == 0){
			printf("读身份证ok[%ld ms]！！！！！！！！！！！！！！！！\n",OSTIMER_GetTickCount() - tick);
	        //Sys_Beep();
			parse_id_info((char*)&idtwo_getbuff[7],&id_data);
			if(dumpInfo)
				dump_id_info(&id_data);
			else
				dump_id_info2(&id_data);
		}else{
			printf("读身份证fail,ret=%d\n",ret);
			if(dumpInfo)
				LCD_Printf(10+48,DISP_FONT12,"测试失败 [%d]", ret);
		}
	#else
        tick = OSTIMER_GetTickCount();
		pthread_spin_lock(&lock);	
		ret = IDCARD_AutoRead_Fig(&len,idtwo_getbuff);
		pthread_spin_unlock(&lock);	
		if(ret == 0){
			printf("读身份证带指纹ok[%ld ms]！！！！！！！！！！！！！！！！\n",OSTIMER_GetTickCount() - tick);
	        //Sys_Beep();
			parse_id_info((char*)&idtwo_getbuff[9],&id_data);
			dump_id_info(&id_data);
			dump_id_info2(&id_data);
		}
	#endif
	}
	return ret;
}

int test_idtwo(int loop)
{
	int i,ret;
	
	unsigned int success = 0,total = 0;
	unsigned long tick,tick_start;
	if(loop <= 0)
		loop = 1;
	tick_start = OSTIMER_GetTickCount();
	
	for(i = 0;i < loop;i++){
		tick = OSTIMER_GetTickCount();
		ret = idtwotest(loop == 1);
		if(ret == 0)
			success += 1;
		total += 1;
		printf("============ 读身份证 [%04d/%04d %.2f%%] %3ld ms, 平均 %3ld ms============\n",
				success,total, (success * 100.0)/total,
				OSTIMER_GetTickCount() - tick,
				(OSTIMER_GetTickCount() - tick_start)/ total );
        if(ret == 0) Sys_Beep();
        PICC_Reset(0);
	}
	/*
	int ret;
	pthread_t th;
	//创建线程
 	pthread_mutex_init(&mutex, NULL);
	pthread_spin_init(&lock, 0);
	ret=pthread_create(&th,NULL,idtwotest,NULL);
    if (ret != 0){
		printf("pthread_create error.\n");
		return -1;
	}        
	ret = pthread_join(th, NULL);
	if (ret != 0){
		printf("pthread_join error.\n");
		return -1;
	}
	printf("子线程回收成功\n");
	pthread_mutex_destroy(&mutex);
	*/
	return 0;
}


