
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <pthread.h>

#include <time.h>
#include "oslib.h"
#include "toolslib.h"


extern void LC812_test(void);
const unsigned char aucLogoCL[] =
{
64,40,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x40,0x20,0x20,
0x10,0x10,0x10,0x10,0x08,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x40,0x40,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x40,0x10,0x08,0x04,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0xC0,0x80,0x00,0x00,0x70,0xF0,0xC0,0x00,0x0C,0x3E,0xFC,
0xE0,0x80,0x00,0x00,0x00,0xC0,0x20,0x00,0x20,0x20,0x20,0x21,0x22,0x42,0xC4,0x08,
0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x78,0x87,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x87,0xFF,0xFC,0x00,0x00,0xC7,0xFF,0xFC,0x00,0x00,0x83,0xFF,0xFE,0x00,0x00,0x03,
0xFF,0xFF,0x00,0x00,0x08,0x17,0x10,0x26,0x2B,0xB1,0xE1,0x62,0x42,0x86,0x07,0x09,
0x19,0x11,0x21,0x01,0x02,0x06,0x08,0x30,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x04,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x01,0x00,0x00,0x06,0x0F,0x03,0x00,0x00,0x38,0x3F,0x0F,0x01,0xC0,0xF0,0x7F,
0x1F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x1F,0x12,0x94,0x61,0x2A,
0x24,0x38,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x01,0x02,0x0C,0x18,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x04,0x04,0x08,0x08,0x18,
0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
0x10,0x10,0x10,0x10,0x18,0x08,0x08,0x04,0x04,0x02,0x02,0x01,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
/**
 * @brief  Display GPS icon and number of satellites
 * @param[in] a Display GPS icon and number of satellites the number of searched satellites 4-29, other values clear icon。
 * @retval 0 succeed
 * @retval other fail
*********************************************************************************************************/
uint8_t LCD_GPS_Icon(uint8_t a);

uint8_t LCD_Signal_Icon(uint8_t a);

uint8_t LCD_BT_Icon(uint8_t a);

uint8_t LCD_Electricity_Icon(uint8_t a);

#define WIDTH  128//160
#define HEIGHT 64//80

#define N (WIDTH * HEIGHT / 8)

int test_movie(int argc, char* argv[])
{
	int fd1;
	unsigned char buf[N] = {0};
	size_t nbyte = 0;
	int n = 1;

	if(argc < 2)
	{
		printf("Please input Movie name,Like this: %s <movie.bin>\n",argv[0]);
		return -1;
	}
	if((fd1 = open(argv[1], O_RDONLY)) < 0)
	{
		perror("Fail to open Movie file1!\n");
		return -1;
	}
	printf("\nBegin to play movie!\n");
	while((nbyte = read(fd1, buf, N)) > 0)
	{
                LCD_DisplayLogo(0,0,WIDTH,HEIGHT,buf,1);
		printf("\rFrame %d", n++);
		fflush(stdout);
		usleep(39000);
	}
	printf("\nPlay movie Over!\n");
	close(fd1);

	printf("Exit!\n");
	return 0x00;
}
void test_drawrect(void)
{
    uint16_t x0 = 0,y0 = 0;
    do
    {
       //LCD_ClearScreen(0);
       LCD_DrawRect(x0,y0,LCD_GetWidth() - x0 * 2,LCD_GetHeight()-y0 * 2,(void*)1);
       LCD_FillRect(x0,y0,LCD_GetWidth() - x0 * 2,LCD_GetHeight()-y0 * 2,(unsigned char )1);
       LCD_DrawEllipse(LCD_GetWidth()/2,LCD_GetHeight()/2,LCD_GetWidth()/2 - x0 - 2,LCD_GetHeight()/2-y0 - 2,1);
       LCD_FillEllipse(LCD_GetWidth()/2,LCD_GetHeight()/2,LCD_GetWidth()/2 - x0 - 2,LCD_GetHeight()/2-y0 - 2,0);
        x0 ++; y0 ++;
        if(x0 >= LCD_GetWidth() / 2)
            break;
        //if(y0 >= LCD_GetHeight() / 2)
        //    break;
        usleep(500);
    }while(1);
}

#define MAX_BALANCE 9999

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

void test_sim(void){
    unsigned char atr[64],rpdu[300];
    unsigned int atr_len,rpdu_len;
    int ret;
    unsigned char apdu[] = "\x00\x84\x00\x00\x08";
    unsigned char slot = ICC_SIM1;

    ret = ICC_Init(slot);
    ret = ICC_GetATR(slot,atr,&atr_len);
    if(ret)
        printf("Atr error [%d]\n",ret);
    else
        dump_data("atr",atr,atr_len);

    ret = ICC_Exchange(slot,apdu,5,rpdu,&rpdu_len);
    if(ret)
        printf("exchange error [%d]\n",ret);
    else
        dump_data("rpdu<--",rpdu,rpdu_len);
    ICC_Close(slot);
}

int rsa_test_1984_65537(int loop_time)
{
    int i;
    unsigned char pucPublicKey[] ={
        0xCB,0xF2,0xE4,0x0F,0x08,0x36,0xC9,0xA5,0xE3,0x90,0xA3,0x7B,0xE3,0xB8,0x09,0xBD,
0xF5,0xD7,0x40,0xCB,0x1D,0xA3,0x8C,0xFC,0x05,0xD5,0xF8,0xD6,0xB7,0x74,0x5B,0x5E,
0x9A,0x3F,0xA6,0x96,0x1E,0x55,0xFF,0x20,0x41,0x21,0x08,0x52,0x5E,0x66,0xB9,0x70,
0xF9,0x02,0xF7,0xFF,0x43,0x05,0xDD,0x83,0x2C,0xD0,0x76,0x3E,0x3A,0xA8,0xB8,0x17,
0x3F,0x84,0x77,0x71,0x00,0xB1,0x04,0x7B,0xD1,0xD7,0x44,0x50,0x93,0x12,0xA0,0x93,
0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0x32,0xED,0x25,0xFE,0xD5,
0x2A,0x95,0x94,0x30,0x76,0x8C,0xC3,0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,
0x68,0xCC,0xD9,0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,
0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x82,
0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,
0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x52,0xA9,
0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x32,0xFD,0x8C,0x8A,
0xD9,0x12,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0x95,0x87,
0x69,0x75,0x89,0x97,0x89,0x78,0x94,0x30,0x76,0x8C,0xCD,0x90,0x2F,0xD3,0xF3,0x4B,
0x92,0xE7,0x92,0x4D,0x72,0x9C,0xB6,0x47,0x35,0x33,0xAE,0x2B,0x2B,0x55,0xBF,0x0E,
0x44,0x96,0x4F,0xDE,0xA8,0x44,0x01,0x17
    };
    unsigned int uiPublicKeyLen = 248;
    unsigned char pucPublicKeyExp[] = {0x01, 0x00, 0x01};
    unsigned int uiPublicKeyExpLen = 3;
    unsigned char pucIn[] ={0x00,0x01,0xE4,0x0F,0x08,0x36,0xC9,0xA5,0xE3,0x90,0xA3,0x7B,0xE3,0xB8,0x09,0xBD,
0xF5,0xD7,0x40,0xCB,0x1D,0xA3,0x8C,0xFC,0x05,0xD5,0xF8,0xD6,0xB7,0x74,0x5B,0x5E,
0x9A,0x3F,0xA6,0x96,0x1E,0x55,0xFF,0x20,0x41,0x21,0x08,0x52,0x5E,0x66,0xB9,0x70,
0xF9,0x02,0xF7,0xFF,0x43,0x05,0xDD,0x83,0x2C,0xD0,0x76,0x3E,0x3A,0xA8,0xB8,0x17,
0x3F,0x84,0x77,0x71,0x00,0xB1,0x04,0x7B,0xD1,0xD7,0x44,0x50,0x93,0x12,0xA0,0x93,
0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0x32,0xED,0x25,0xFE,0xD5,
0x2A,0x95,0x94,0x30,0x76,0x8C,0xC3,0x2E,0xD2,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,
0x68,0xCC,0xD9,0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,
0x02,0xFD,0x82,0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x82,
0x5F,0xED,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,
0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x52,0xA9,
0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0xD9,0x12,0x32,0xFD,0x8C,0x8A,
0xD9,0x12,0x52,0xA9,0x59,0x43,0x07,0x68,0xCC,0xD9,0x02,0xFD,0x8C,0x8A,0x95,0x87,
0x69,0x75,0x89,0x97,0x89,0x78,0x94,0x30,0x76,0x8C,0xCD,0x90,0x2F,0xD3,0xF3,0x4B,
0x92,0xE7,0x92,0x4D,0x72,0x9C,0xB6,0x47,0x35,0x33,0xAE,0x2B,0x2B,0x55,0xBF,0x0E,
0x44,0x96,0x4F,0xDE,0xA8,0x44,0x01,0x17
    };
    unsigned int uiInLen = 248;
    unsigned char pucOut[248];
#if 1
    unsigned char pucOut2[] = {0x62,0xEA,0x15,0x1B,0x26,0x34,0x9F,0x59,0xE7,0x41,0x07,0xE7,0x05,0xEF,0x58,0xD0,
0xCC,0x3E,0xB2,0x8A,0x80,0xDC,0x39,0xF6,0x0D,0x82,0xC6,0xC3,0x10,0x90,0xAF,0xBE,
0xD9,0x84,0x71,0x4B,0xD8,0x06,0x88,0x43,0x78,0x67,0x16,0x11,0x46,0xF8,0x7B,0x4A,
0x34,0x7A,0xAC,0x98,0x6E,0x1B,0xA7,0xE9,0xCB,0x22,0x0A,0x8E,0x91,0x28,0x26,0x7B,
0xE0,0x25,0x59,0xED,0xD4,0x3F,0x67,0xA0,0x5F,0x11,0xEA,0x72,0xB9,0x61,0x6F,0x2E,
0xA3,0xDF,0x1A,0x3A,0x56,0x59,0x33,0x3D,0x59,0x91,0x8C,0x5E,0xBA,0xBC,0xBC,0xC6,
0x88,0x68,0x5C,0xD6,0x46,0xCB,0x88,0xEF,0x7A,0x8C,0x6D,0xAD,0x5A,0xEA,0xDF,0xDD,
0x86,0xC4,0xB3,0x10,0x8A,0x3A,0x64,0xC4,0x1F,0xEC,0xDF,0x03,0xAD,0xB9,0x32,0x42,
0xBA,0x24,0xE4,0x2E,0xB9,0x8C,0x4B,0x54,0x47,0x02,0xC2,0x60,0xD8,0xA7,0x22,0xA0,
0x0F,0x0A,0x35,0x71,0x0F,0x8C,0xA2,0xB4,0x3C,0x2A,0xA5,0x46,0x06,0x3B,0xAA,0xA8,
0xDE,0xF2,0x78,0x0C,0x06,0xD6,0x59,0xDC,0x6E,0x2A,0x09,0xA7,0x88,0xE1,0x35,0xA6,
0x80,0x68,0x51,0x32,0xFE,0x9E,0x8E,0xD6,0x2B,0x28,0x24,0x65,0x6B,0xE2,0x78,0x3F,
0x24,0x46,0x1F,0xAA,0x21,0xE3,0xE6,0x6A,0x99,0x60,0xDC,0x60,0xA5,0x19,0xD9,0x75,
0xA1,0xE4,0x5B,0xE5,0x37,0x1E,0xB8,0x64,0xF9,0x6D,0x4A,0x0C,0xE7,0xEB,0x1C,0xD0,
0xF6,0x1F,0x25,0x27,0x2D,0x5F,0x84,0x6C,0x93,0x03,0x2F,0x36,0xC6,0xAE,0xD9,0x04,
0x16,0xA0,0x30,0x64,0xB1,0xB9,0xDE,0xA1
    };
#endif
    for(i = 0;i < loop_time;i++)
    {
        RSA_PublicDecrypt(pucPublicKey, uiPublicKeyLen,
                        pucPublicKeyExp, uiPublicKeyExpLen,
                        pucIn, uiInLen,
                        pucOut);
        if(memcmp(pucOut,pucOut2,uiPublicKeyLen))
        {
            printf("rsa test error\n");
            return 0x01;
        }
        //dump_data("pucOut ",pucOut,uiPublicKeyLen);
    }
    return 0x00;
}
static void dump_register(void)
{
    uint8_t i;
    uint32_t data[0x39];
    uint32_t ret;


    extern uint16_t phhalHw_ReadRegister(
    uint8_t bAddress,
    uint32_t * pValue);

    printf("reading register value!\n");

    for(i = 0;i < 0x39;i++)
    {

        ret = phhalHw_ReadRegister(i, &data[i]);
        if(ret)
        {
            printf("read reg addr[%02X]err[0x%02x]\r\n",i,ret);
            break;
        }
        printf("addr[%02X]_[0x%08X]\r\n",i,data[i]);
    }

}

void test_rsa(void){

    unsigned long tick;
    unsigned int loop_time = 200;
    tick = OSTIMER_GetTickCount();
    rsa_test_1984_65537(loop_time);
    printf("rsa_test_1984_65537 used time %ldms/%d time \n",OSTIMER_GetTickCount() - tick,loop_time);
}


void test_picc(void){
    unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
    int ret;
    unsigned int rpdu_len;
    int slot;
    int slot_num;

    if(Sys_TermType() == 2)
    	slot_num = 3;
    else
    	slot_num = 1;

    PICC_Open(0);

    //dump_register();
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"Type A card test");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Tap card");

	if(Sys_TermType() == 2)
		LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"Tap three card");

	for(slot = 0;slot < slot_num;slot++)
	{
		do{
			ret = PICC_PollCard(slot,0);
			if(!ret)
				Sys_Beep();
		}while(ret);
	}

	for(slot = 0;slot < slot_num;slot++)
	{
		dump_data("apdu--->",apdu,sizeof(apdu));
		ret = PICC_Exchange(slot, apdu,sizeof(apdu),rpdu,&rpdu_len);

		if(!ret /*&& !memcmp(rpdu[rpdu_len-2],"\x90\x00",2)*/)
		{
			dump_data("rpdu<---",rpdu,rpdu_len);
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"A card OK");
			if(Sys_TermType() == 2)
				LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_REVERT,"slot[%d]",slot);
			OSTIMER_DelayMs(500);
		}
		else
		{
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"A card Fail");
			if(Sys_TermType() == 2)
				LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_REVERT,"slot[%d]",slot);
			OSTIMER_DelayMs(500);
		 }
		PICC_Close(slot);
	}
}

void test_picc_single(void){
    unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
    int ret ;
    unsigned int rpdu_len;
    int slot;
    int slot_num;

    if(Sys_TermType() == 2)
    	slot_num = 3;
    else
    	slot_num = 1;

    PICC_Open(0);

    //dump_register();
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"Type A card test");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"Tap card");

 	do{
 		for(slot = 0; slot <= slot_num;slot ++){

 			ret = PICC_PollCard(slot,0);
 			if(!ret){
 				Sys_Beep();
 				break;
 			}
 		}
	}while(ret);

	dump_data("apdu--->",apdu,sizeof(apdu));
	ret = PICC_Exchange(slot, apdu,sizeof(apdu),rpdu,&rpdu_len);

	if(!ret /*&& !memcmp(rpdu[rpdu_len-2],"\x90\x00",2)*/)
	{
		dump_data("rpdu<---",rpdu,rpdu_len);
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"A card OK");
		if(Sys_TermType() == 2)
			LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"slot[%d]",slot);
		OSTIMER_DelayMs(500);
	}
	else
	{
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"A card Fail");
		if(Sys_TermType() == 2)
			LCD_Printf(60,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"slot[%d]",slot);
		OSTIMER_DelayMs(500);
	 }
	PICC_Close(slot);
}

void test_mifare(void){
    int ret = -1;
    uint8_t key[] = "\xFF\xFF\xFF\xFF\xFF\xFF";
    uint8_t data[16],data_len = 16;
    uint8_t snr[16],snr_len;
    int i;

    PICC_Open(0);

    for(i = 0 ;i < 16;i++){
        if(ret) ret = Mifare_PowerOn(0,snr,&snr_len);
        printf("\n==========Block[%2d]==========\n",i);
        if(!ret) ret = Mifare_AuthenBlock(i * 4,0,key);

        if(!ret) ret = Mifare_ReadBlock(0 + i * 4,data);
        if(!ret) dump_data("Mifare Read0",data,data_len);

        if(!ret) ret = Mifare_ReadBlock(1 + i * 4,data);
        if(!ret) dump_data("Mifare Read1",data,data_len);


        if(!ret) ret = Mifare_ReadBlock(2 + i * 4,data);
        if(!ret) dump_data("Mifare Read2",data,data_len);

    }

}
void test_led(void){
    LCD_ClearScreen(0);
    //LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_REVERT,"led test");

    printf("--------led test--------\n");
    LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"led all on");
    LED_Control(LED_ALL,1);
    OSTIMER_DelayMs(1000);
    LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"led all off");
    LED_Control(LED_ALL,0);
    OSTIMER_DelayMs(1000);
    //LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24,"led flicker");
    LED_Flicker(LED_ALL,100,100,10);
    OSTIMER_DelayMs(2500);
    //LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT32,"led flicker");

    LED_Flicker(LED_BLUE,100,100,10);
    LED_Flicker(LED_YELLOW,500,500,10);
    LED_Flicker(LED_GREEN,1000,1000,10);
    LED_Flicker(LED_RED,2000,2000,10);
}
void test_LedSeg(void){
    int i;
    for(i = 0 ;i < MAX_BALANCE + 1;i++){
        LedSeg_Balance(i);
    }
    LedSeg_Display(0,"012345678901.8.8.8.8.");
}
void test_lcd(void){
    int i;
    LCD_ClearScreen(0);
    LCD_DisplayIcon(ICON_SIGNAL,28);
    LCD_DisplayIcon(ICON_NETWORK,2);sleep(1);Sys_ErrorBeep(10);
    LCD_DisplayIcon(ICON_NETWORK,3);sleep(1);
    LCD_DisplayIcon(ICON_NETWORK,4);sleep(1);
    for(i = 0 ;i < 35;i++){
        LCD_DisplayIcon(ICON_SIGNAL,i);
        //usleep(100000);
        OSTIMER_DelayUs(100000);
        LCD_DisplayIcon(ICON_GPS,i);

        LCD_DisplayIcon(ICON_BT,i % 3);
    }
    LCD_DisplayIcon(ICON_SIGNAL,29);
    LCD_DisplayIcon(ICON_GPS,28);

}

// Button to suspend（500ms time out）， Button to Start
static int wait_key_suspend(uint32_t timeout){
    int key ;
    key = Sys_WaitKey(timeout);
    if(key > 0)
        key = Sys_WaitKey(0);
    return key;
}
void test_lcd_ng(void)
{
	int i;

	do
	{
        LCD_ClearScreen(1);
		wait_key_suspend(1000);
		LCD_ClearScreen(0);
        wait_key_suspend(1000);
        LCD_Signal_Icon(29);
		LCD_GPS_Icon(29);
		LCD_Display_Row("ticket",        8, 88,DISP_FONT32);
		LCD_Display_Row("30",       24, 24,DISP_FONT24);
		LCD_Display_Row("￥1.5",      40,80,DISP_FONT32);
		LCD_Display_Row("20:20",      72, 0,DISP_FONT6X8);

		wait_key_suspend(1000);
        LCD_ClearScreen(0);
        LCD_Display_Row("hello world", 8,0,DISP_CENTER);
		LCD_Display_Row("hello world",24,0,DISP_CENTER);
        LCD_Display_Row("hello world",40,0,DISP_CENTER|DISP_CLRLINE);
		LCD_Display_Row("hello world",56,0,DISP_CENTER);
        LCD_Display_Row("hello world",72,0,DISP_CENTER);

		wait_key_suspend(1000);
		LCD_ClearScreen(0);
		LCD_Display_Row("card hight test:",8,0,DISP_CENTER);
		LCD_Display_Row("tap card>>",24,0,DISP_CENTER);
		for(i =0; i<3; i++)
		{
			LCD_Clear_rows(56, 16);
			wait_key_suspend(500);
			LCD_Display_Row("card ok",56,0,DISP_CENTER);

			Sys_BeepMs(100);
			wait_key_suspend(500);
		}
        if(wait_key_suspend(500) == 'R')
            break;
	}while(1);
    {
    uint8_t revert = DISP_REVERT;
    if(revert) revert = DISP_REVERT;
    LCD_ClearScreen(0);
    lcd_clearblock(0,0,160,16,!!revert);
    LCD_Signal_Icon(29|revert);
	LCD_BT_Icon(1|revert);
	LCD_GPS_Icon(25|revert);
	LCD_Electricity_Icon(50|revert);
	LCD_Display_Row("12:34",        4,110,DISP_FONT6X8|revert);
    }
	LCD_Display_Row("1.00 RMB",      16, 32,DISP_FONT32);
	LCD_Display_Row("tap card",  48, 0,DISP_CENTER|DISP_FONT12);
	LCD_Display_Row("2017/12/29",  72, 6,DISP_FONT6X8);
	LCD_Display_Row("66666/888888",72,80,DISP_FONT6X8);
}
void test_lcd_speed(void){
    int i;
    unsigned long start;
#define _LCD_LOOP_TIME 10
    start = OSTIMER_GetTickCount();
    for(i = 0;i < _LCD_LOOP_TIME;i++){
        //LCD_ClearAll();
        //LCD_ClearIconLine();
        LCD_ClearScreen(0);
        LCD_DisplayIcon(ICON_BATTERY,88);
        LCD_DisplayIcon(ICON_SIGNAL,31);

        LCD_Printf(16,DISP_REVERT,"      Display test      ");

        //LCD_Display_Row("Welcome to iMX6UL World",0,0,0);
        LCD_Printf(32,DISP_UNDERLINE,"Welcome to iMX6UL World");
        LCD_Printf(64,0,"<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
        LCD_DrawRect(0,0,LCD_GetWidth(),LCD_GetHeight(),(void*)1);
    }
    printf("all %ld ms/%d time\n",OSTIMER_GetTickCount() - start,_LCD_LOOP_TIME);
}
void test_lcd_logo(void){
    LCD_ClearScreen(0);
    LCD_DisplayLogo(16,16,aucLogoCL[0],aucLogoCL[1],(unsigned char *)&aucLogoCL[2],0);
}
void test_buzzer(void){
    Sys_BeepBeep(3,100,200);
}
void test_speaker(void){
#if 1
	system("aplay /opt/sound/yycs.wav 2> /dev/null &");
	OSTIMER_DelayMs(1000);
	system("aplay /opt/sound/7499.wav 2> /dev/null &");
#endif

}

void test_rtc(void){
    char curr_time[15];
    int ret;

    Sys_GetRealTime((char*)curr_time);
    printf("GetRealTime:%s\n",curr_time);
    printf("current time:%.4s-%.2s-%.2s %.2s:%.2s:%.2s\n",
            &curr_time[0],&curr_time[4],&curr_time[6],
            &curr_time[8],&curr_time[10],&curr_time[12]);
    strcpy(curr_time,"20181226144444");
    ret = Sys_SetRealTime(curr_time);
    printf("try to reset time to %s[%d]\n",curr_time,ret);

    strcpy(curr_time,"20381426144461");
    ret = Sys_SetRealTime(curr_time);
    printf("try to reset time to %s[%d]\n",curr_time,ret);
}

int system_init(void){

    //char *font_file = "/opt/font/RTWSYueRoudGoG0v1-Regular.ttf";
    char *font_file = "/opt/font/SIMSUN.TTC";
    char *bg_file = "/opt/app/png/bg1.png";
	char term_name[64];

    fb_init(font_file,bg_file);

	LCD_Init();
	Sys_Gps_Init("/dev/ttymxc4","9600,8,n,1");
	LedSeg_Init();
	Beep_Init();
    Sys_Beep();
    Sys_TermName(term_name);
    printf("Sys_TermName =|%s|\n",term_name);
	return 0x00;
}

char test_QRcode(void)
{
	int qrfd1,qrfd2,ret;
	unsigned char TmpBuff[1024];
    qrfd1 = QRCode_Open(0);
    qrfd2 = QRCode_Open(1);

	printf("\n******** QR test ********>>\n");
    printf("Scan now>>\r\n");
    while(1)
	{
		ret = QRCode_RxStr(qrfd1, TmpBuff, 1024, 100);
		if(ret <= 0)
			ret = QRCode_RxStr(qrfd2, TmpBuff, 1024, 100);

		if(ret > 0)
		{
			Sys_BeepMs(100);
			printf("\nString : [%s]\r\n",TmpBuff);
			printf("length: [%d]\n",ret);
		}
	}
}

char test_GPS(void)
{
	int i;
	int err=0;
    int time=0;
    GNRMC *mes_gps;
	//LCD_ClearScreen(0);
	//LCD_ClearAll();
	//LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"gps测试:");
	printf("\n********* GPS test **********>>\n");
	Sys_Gps_Debug(1);
	for(i=0; i<10000; i++)
	{
		mes_gps = Sys_Gps_QueryLaLO("Asia/Shanghai");
		if(mes_gps == NULL)
		{
			printf("now data\n");
            err++;
		}
		else
		{
            if(mes_gps->sate_num == 0)
                err++;
			printf("\nvalid data:\n");
			printf("latitude:%08f\n",mes_gps->latitude_wgs84);
			printf("longitude:%08f\n",mes_gps->longitude_wgs84);
			printf("time:%s\n",mes_gps->date_time);
			printf("satenumber:%d\n",mes_gps->sate_num);
			printf("*******************************\n");
			printf("*****    Test GPS   OK   *****\n");
			printf("*******************************\n");
		}
        time++;
        printf("err/time:%d/%d\n",err,time);
        OSTIMER_DelayUs(813456);
	}
	if(i == 100)
	{
		printf("###############################\n");
		printf("#####    GPS test fail   #####\n");
		printf("###############################\n");
		err = (err*2) | 1 ;
		OSTIMER_DelayMs(3000);
	}
	return err;
}
static char *get_icc_name(uint8_t slot){

	switch(slot)
	{
		case ICC_SIM1:
			return "ICC_SIM1";
		case ICC_SIM2:
			return "ICC_SIM2";
		case ICC_SIM3:
			return "ICC_SIM3";
		case ICC_SIM4:
			return "ICC_SIM4";
        case ICC_ESAM:
        	return "ICC_ESAM";
		case ICC_SIM5:
			return "ICC_SIM5";
	}
	return "unknown";
}
static char *get_icc_bps(uint8_t slot){

	switch(slot)
	{
	case BAUD_9600:    return "9600";
	case BAUD_19200:   return "19200";
	case BAUD_38400:   return "38400";
	case BAUD_57600:   return "57600";
	case BAUD_115200: return "115200";
	case BAUD_55800:   return "55800";
	case BAUD_111600: return "111600";
	case BAUD_223200: return "223200";
	case BAUD_446400: return "446400";
	}
	return "unknown";
}
static uint8_t get_icc_slot(uint8_t slot)
{
	switch(slot)
	{
		case 1:
			slot = ICC_SIM1;
			break;
		case 2:
			slot = ICC_SIM2;
			break;
		case 3:
			slot = ICC_SIM3;
			break;
		case 4:
			slot = ICC_SIM4;
			break;
		case 5:
			slot = ICC_ESAM;
			break;
		default:
			slot = ICC_SIM5;
			break;
	}
	return slot;
}
void test_Sam(uint8_t slot,uint8_t sambps)
{
	unsigned char atr[64],rpdu[300];
    unsigned int atr_len,rpdu_len;
    int ret;
    unsigned char apdu[] = "\x80\x60\x00\x00\x00";

	slot = get_icc_slot(slot);
	printf("slot = [%d] %s\n",slot,get_icc_name(slot));
	printf("sambps = [%d](%s)\n",sambps,get_icc_bps(sambps));

	ret = ICC_Init(slot);
	ret = ICC_PowerUp(slot,sambps,VOLT_5V,atr,&atr_len);
	if(ret)
		printf(" ICC_SIM Atr error [%d]\n",ret);
	else
		dump_data("ICC_SIM atr",atr,atr_len);
	if(ret == 0){
		ret = ICC_Exchange(slot,apdu,5,rpdu,&rpdu_len);
		if(ret)
			printf("ICC_SIM exchange error [%d]\n",ret);
		else
			dump_data("ICC_SIM rpdu<--",rpdu,rpdu_len);
	}
	Sys_BeepBeep(1,100,200);
	ICC_Remove(slot);
}

int test_eeprom(void)
{
#define DATA_LEN 8192
#define EEPROM_NUM 2
    int i;
    uint8_t *buf_write = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    uint8_t *buf_read  = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    int ret = 0;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT12|DISP_UNDERLINE,"eeprom test:");

    if((buf_write == 0)||(buf_read == 0)){
            printf("malloc buffer error\n");
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test fail");
            goto _error;
    }

    for(i = 0;i < DATA_LEN;i++)
    {
        buf_write[i] = (uint8_t)i + '0';
    }
    for(i = 0;i < EEPROM_NUM;i++){

        dc_swr_eeprom(i,0,DATA_LEN,buf_write);
        memset(buf_read,0,DATA_LEN);
        dc_srd_eeprom(i,0,DATA_LEN,buf_read);

        //dump_data("buf_read",buf_read,DATA_LEN);

        if(!memcmp(buf_read,buf_write,DATA_LEN)){
            printf("eeprom [%d] readwrite success\n",i + 1);
            LCD_Printf(32 + 24 * i,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"[%d]test OK",i + 1);
        }
        else{
            printf("eeprom [%d] readwrite error\n",i + 1);
            LCD_Printf(32 + 24 * i,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"[%d]test fail",i + 1);
            ret |= (1 << i);
        }
    }
_error:
    if(buf_read) free(buf_read);
    if(buf_write)free(buf_write);
	OSTIMER_DelayMs(500);

    return ret;
}
void print_help(void)
{

	printf("------------------------help-------------------------\n");
	printf("command  P1   P2            P3  \n");
	printf("- - - -   1    1(or2)	        type A card test\n");
	printf("- - - -   2    1-5	  0         PSAM card test p2=slot,P3=baud:9600=0,19200=1,38400=2,57600=3,115200=4,55800=5,111600=6,223200=7,446400=8\n");
	printf("- - - -   3    1000(or2)        BEEP test\n");
	printf("- - - -   4    1(or2)	        LCD test\n");
	printf("- - - -   5    1(or2)	        GPS test\n");
	printf("- - - -   6    1(or2)	        QRcode test\n");
	printf("- - - -   7    1	        segled and key test\n");
	printf("- - - -   8    1-2	        FRAM-RTC test 1:RTC R&W 2:RTC Read\n");
	printf("- - - -   9    0	        LED test\n");
	printf("- - - -   0    0		    sound test\n");
	printf("- - - -   a    0	        net test\n");
	printf("- - - -   i    0	        4G module\n");
	printf("- - - -   j    0	        blutools\n");
	printf("- - - -   k    100000	0-1     can test P2 baud，P3 model\n");
	printf("- - - -   l    0	        COM5-485 test(ttymxc4)\n");
	printf("- - - -   n    0	        COM2-232 test(ttymxc1)\n");
	printf("- - - -   p    0	        power down protection test\n");
	printf("- - - -   t    0	        TCP/IP test\n");
	printf("- - - -   q    0	        eeprom test\n");
}



char select_test(void)
{
	int fd,ret,i;
	unsigned char TmpBuff[2048];
#define QR_SCAN_LOOP 100 // x100ms
	printf("\n******** Scan the code to select test items*******>>\n");
	memset(TmpBuff, 0, sizeof(TmpBuff));
	//LCD_Clrscr();
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT, "Scan the code to select test items:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"10s After entering automatic test");

	fd = QRCode_Open(0);
    //QRCode_Default(fd);

	printf("scan now>>\r\n");
	for(i = 0; i < QR_SCAN_LOOP; i++)
	{
		ret = QRCode_RxStr(fd,TmpBuff,1024,100);
		if(ret > 0)
		{
			Sys_Beep();
			printf("QR recive : [%s]\n",TmpBuff);
			OSTIMER_DelayMs(1);
			return TmpBuff[ret -1];
		}
	}
	if(i == QR_SCAN_LOOP)
	{
		printf("auto test....\r\n");
		OSTIMER_DelayMs(1000);
		return 'f';
	}
	return 0x00;
}
char select_aging(void)
{
	int fd,ret,i;
	unsigned char TmpBuff[2048];
	LCD_ClearScreen(0);
	LCD_ClearAll();
    fd = QRCode_Open(0);
	LCD_Printf(0, DISP_LEFT|DISP_FONT12,"After entering automatic test, scan any TWO-DIMENSIONAL code to exit aging:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"10s aging test");
	for(i = 0; i<100; i++)
	{
		ret = QRCode_RxStr(fd, TmpBuff, 1024, 100);
		if(ret > 0)
		{
			Sys_BeepMs(100);
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(16,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"exit aging");
			OSTIMER_DelayMs(1000);
			return 0;
		}
	}
	if(i == 100)
	{
		return 'f';
	}
	return 0x00;
}
void aging_test(void)
{
	int ret;
	unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
	char dispbuff[100] = {0};
	unsigned int rpdu_len;
	int i,numbok = 0,numbfi = 0;
	char err;
	int slot = 0;

//////////////////////////////////aging test//////////////////////////////////////////////>

	printf("\n\n********* aging test *********\n");
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT, "aging test:");
	LCD_Printf(25, DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE, "tap card");
	while(1){
		PICC_Open(0);
		ret = PICC_PollCard(slot,0);
		//no card
		if(ret){
			PICC_Close(0);
			continue;
		}
		for(i=0; i<30; i++){
			ret = PICC_Exchange(slot,apdu,sizeof(apdu),rpdu,&rpdu_len);
		    if((!ret) && (!memcmp(&rpdu[rpdu_len-2],"\x90\x00",2))){
				err = 0;
			}
		    else{
				err = 1;
				break;
			}
		}
		if(err == 0){
			LCD_Clear_rows(25,16);
			LCD_Printf(25,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"type A card OK");
			system("aplay /opt/sound//7499.wav > /dev/null 2&>1 &");
			numbok++;
		}
		else{
			LCD_Clear_rows(25,16);
			LCD_Printf(25,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"type A card fail");
			system("aplay /opt/sound/sksb.wav > /dev/null 2&>1 &");
			numbfi++;
		}
		memset(dispbuff, 0, sizeof(dispbuff));
		sprintf(dispbuff,"OK:%d ERR:%d",numbok,numbfi);
		LCD_Clear_rows(50,16);
		LCD_Printf(50,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,dispbuff);
		PICC_Close(0);
		OSTIMER_DelayMs(200);
	}
}



char auto_Speaker(void)
{
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT, "sound test:");
	//system("amixer set Headphone 50%");
	system("aplay /opt/sound/yycs.wav 2> /dev/null &");
	OSTIMER_DelayMs(1000);
	system("aplay /opt/sound/7499.wav 2> /dev/null &");
	OSTIMER_DelayMs(1000);
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"sound test end");
	OSTIMER_DelayMs(2000);
	return 0;
}
char auto_Sam(void)
{
	unsigned char atr[64],rpdu[300];
	unsigned int atr_len,rpdu_len;
	int ret;
	unsigned char apdu[] = "\x00\x84\x00\x00\x08";
	unsigned char slot;
	unsigned char simnum;
	unsigned char err = 0;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT, "sam card test:");
	for(simnum = 1;simnum <= 5;simnum++){
		switch(simnum)
		{
			case 1:
				slot = ICC_SIM1;
				break;
			case 2:
				slot = ICC_SIM2;
				break;
			case 3:
				slot = ICC_SIM3;
				break;
			case 4:
				slot = ICC_SIM4;
				break;
            case 5:
                slot = ICC_ESAM;
                break;
			default:
				slot = ICC_SIM1;
				break;
		}
		ret = ICC_Init(slot);
    	ret = ICC_GetATR(slot,atr,&atr_len);
		ret = ICC_Exchange(slot,apdu,5,rpdu,&rpdu_len);
		if((!ret) && (!memcmp(&rpdu[rpdu_len-2],"\x90\x00",2))){
			dump_data("ICC_SIM rpdu<--",rpdu,rpdu_len);
			if(slot == ICC_ESAM)
				LCD_Printf(50,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"esam card ok");
			else
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"sam card ok");
			//err = err*2;
			OSTIMER_DelayMs(1000);
		}
		else{
			if(slot == ICC_ESAM)
				LCD_Printf(50,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"esam card fail");
			else
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"sam card fail");
			err += (1<<(simnum-1));
			OSTIMER_DelayMs(1000);
			//err =
		}
		ICC_Remove(slot);
	}
	if((err & 0x1F) == 0){
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"sam card ok");
	}
#if 0
	if((err & 0x10) == 0){
		LCD_Printf(48,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"esam");
	}
	else{
		LCD_Printf(48,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"esam");
	}
#endif
	OSTIMER_DelayMs(2000);
	return err;
}
char auto_eeprom(void)
{
#define EEPROM_DATA_LEN 10
#define EEPROM_NUM 2
    int i;
    uint8_t *buf_write = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    uint8_t *buf_read  = (uint8_t *)malloc((DATA_LEN*EEPROM_NUM + 3)*sizeof(uint8_t));
    int ret = 0;
	char lcdzkbuff[4][100] = {"main eeprom OK","core eeprom OK"
						      ,"main eeprom fail","core eeprom fail"};

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0,DISP_LEFT,"eeprom test:");

    if((buf_write == 0)||(buf_read == 0)){
            printf("malloc buffer error\n");
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test fail");
            goto _error;
    }

    for(i = 0;i < EEPROM_DATA_LEN;i++)
    {
        buf_write[i] = (uint8_t)i + '0';
    }
    for(i = 0;i < EEPROM_NUM;i++){

        dc_swr_eeprom(i,0,EEPROM_DATA_LEN,buf_write);
        memset(buf_read,0,EEPROM_DATA_LEN);
        dc_srd_eeprom(i,0,EEPROM_DATA_LEN,buf_read);

        //dump_data("buf_read",buf_read,EEPROM_DATA_LEN);

        if(!memcmp(buf_read,buf_write,EEPROM_DATA_LEN)){
            LCD_Printf(25 +24* i,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,lcdzkbuff[i]);
        }
        else{
            LCD_Printf(25 + 24 * i,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,lcdzkbuff[i+2]);
            ret |= (1 << i);
        }
    }
_error:
    if(buf_read) free(buf_read);
    if(buf_write)free(buf_write);
	OSTIMER_DelayMs(3000);

    return ret;
}

char auto_Beep(void)
{
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"beep test:");
	Sys_BeepMs(2000);
	OSTIMER_DelayMs(1000);
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"beep test end");
	OSTIMER_DelayMs(2000);
	return 0;
}
char auto_QRcode(void)
{
	int err=0;
	int i,fd,ret;
	unsigned char TmpBuff[1024];
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"QR code test:");
	fd = QRCode_Open(0);
	for(i = 0; i<1000; i++)
	{
		ret = QRCode_RxStr(fd, TmpBuff, 1024, 100);
		if(ret > 0)
		{
			Sys_BeepMs(100);
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test OK");
			err = 0;
			OSTIMER_DelayMs(2000);
			break;
		}
	}
	if(i == 1000)
	{
		err = 1;
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"test fail");
		OSTIMER_DelayMs(3000);
	}
	return err;
}
char auto_Lcd(void)
{
	char i;
	for(i=0; i<2; i++)
	{
        LCD_ClearScreen(1);
		LCD_ClearScreen(0);
        LCD_Display_Row("Hello world", 8,0,DISP_CENTER);
		LCD_Display_Row("Hello world",24,0,DISP_CENTER);
        LCD_Display_Row("Hello world",40,0,DISP_CENTER|DISP_CLRLINE);
		LCD_Display_Row("Hello world",56,0,DISP_CENTER);
        LCD_Display_Row("Hello world",72,0,DISP_CENTER);
		OSTIMER_DelayMs(2000);
		LCD_ClearScreen(1);
		LCD_ClearScreen(0);
        LCD_Signal_Icon(29);
		LCD_GPS_Icon(29);
		LCD_Display_Row("tick",        8, 88,DISP_FONT32);
		LCD_Display_Row("30",       24, 24,DISP_FONT24);
		LCD_Display_Row("￥1.5",      40,80,DISP_FONT32);
		LCD_Display_Row("20:20",      72, 0,DISP_FONT6X8);
        OSTIMER_DelayMs(2000);
	}
	return 0;
}
char auto_TypeAB(void)
{
	unsigned char apdu[5] = "\x00\x84\x00\x00\x08";
    unsigned char rpdu[300];
    int ret;
    unsigned int rpdu_len;
	unsigned char testnum = 0;
	int slot = 0;

    PICC_Open(0);

    dump_register();
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"type A test:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"tap card");
	while(1){
		ret = PICC_PollCard(slot,0);
		if(!ret)
			break;
		testnum++;
		if(testnum>10)
			break;
		OSTIMER_DelayMs(500);
	}

    dump_data("apdu--->",apdu,sizeof(apdu));
    ret = PICC_Exchange(slot,apdu,sizeof(apdu),rpdu,&rpdu_len);

    if((!ret) && (!memcmp(&rpdu[rpdu_len-2],"\x90\x00",2))){
		dump_data("rpdu<---",rpdu,rpdu_len);
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test OK");
		OSTIMER_DelayMs(2000);
	}
    else{
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"test fail");
		OSTIMER_DelayMs(2000);
		return 1;
	 }
	return 0;
}
char auto_GPS(void)
{
	int i;
    int time=0;
    GNRMC *mes_gps;
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"gps test:");
	Sys_Gps_Debug(1);
	for(i=0; i<100; i++)
	{
		mes_gps = Sys_Gps_QueryLaLO("Asia/Shanghai");
		if(mes_gps == NULL)
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"gps test fail");
		}
		else
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"gps test ok");
			OSTIMER_DelayMs(2000);
			break;
		}
        time++;
        OSTIMER_DelayUs(813456);
	}
	if(i == 100)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"gps test fail");
		OSTIMER_DelayMs(3000);
		return 1;
	}
	return 0;
}
char auto_SmKey(void)
{
	int i;
	int err = 0;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"segled test:");
    LedSeg_Display(0,"012345678901.8.8.8.8.");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"see segled");
	OSTIMER_DelayMs(1000);
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"right key");
	for(i = 0; i < 60; i++)
	{
		if(Sys_WaitKey(500) == 'R')
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"right key OK");
			err = (err*2);
			break;
		}
		OSTIMER_DelayUs(100000);
	}
	if(i == 60)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"right key fail");
		err = (err*2) | 1 ;
		OSTIMER_DelayMs(3000);
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"left key");
	for(i = 0; i < 40; i++)
	{
		if(Sys_WaitKey(500) == 'L')
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"left key Ok");
			err = (err*2);
			break;
		}
		OSTIMER_DelayUs(100000);
	}
	if(i == 40)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"left key fail");
		err = (err*2) | 1 ;
		OSTIMER_DelayMs(3000);
	}
	if((err & 0x03) == 0)
	{
		Sys_BeepMs(100);
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"OK");
		OSTIMER_DelayMs(2000);
	}
	else
	{
		LCD_Clear_rows(32, 24);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"fail");
		OSTIMER_DelayMs(3000);
	}
	return err;
}
char auto_RTC(void)
{
	char realtimem[15] = {"20180228161010"} ;//string '\0'

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"RTC测试:");
	Sys_SetRealTime(realtimem);
	OSTIMER_DelayMs(2000);
	Sys_GetRealTime(realtimem);
	if(realtimem[9] == '6')
	{
		Sys_BeepMs(100);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"RTC OK");
		OSTIMER_DelayMs(2000);
		return 0;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"RTC fail");
		OSTIMER_DelayMs(2000);
		return 1;
	}
}
char auto_Led(void)
{
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"led test:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"led on");
    LED_Control(LED_ALL,1);
    OSTIMER_DelayMs(1000);
    LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"led off");

    LED_Control(LED_ALL,0);
    OSTIMER_DelayMs(1000);
	LED_Control(LED_ALL,1);
	return 0;
}
void auto_RTC_Read(void)
{
	char realtimem[32] = {0} ;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"RTC test:");

	Sys_GetRealTime(realtimem);
	if((realtimem[6] != 0x00) )
	{
		Sys_BeepMs(100);
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"RTC ok");
		OSTIMER_DelayMs(2000);
		return;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"RTC fail");
		OSTIMER_DelayMs(3000);
		return;
	}

}

char auto_High()
{
	unsigned char i;
	int ret;
	char pollfirstok = 0;
	int slot = 0;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"card hight:");
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"tap card");
	for(i=0;i<100;i++){

		PICC_Open(0);
		ret = PICC_PollCard(slot,0);
		PICC_Close(slot);
		if(ret)
		{
			if(pollfirstok)
				Sys_BeepMs(100);
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"no card");
			OSTIMER_DelayMs(50);
			continue;
		}
		pollfirstok = 1;
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"ok");
		OSTIMER_DelayMs(50);
	}
	return 0;
}
char auto_USB(void)
{
	int usbfd;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"u disk test:");
	usbfd = open("/run/media/sda1/autoUSB.txt",O_RDWR);
	if(usbfd < 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"u disk fail");
		OSTIMER_DelayMs(3000);
		return 1;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"u disk OK");
		Sys_BeepMs(100);
		OSTIMER_DelayMs(2000);
	}
	return 0;
}
char auto_TF(void)
{
	int tffd;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"TF card test:");
	tffd = open("/run/media/mmcblk0p1/TF_test.txt",O_RDWR);
	if(tffd < 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TF card fail");
		OSTIMER_DelayMs(3000);
		return 1;
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TF card OK");
		Sys_BeepMs(100);
		OSTIMER_DelayMs(2000);
	}
	return 0;
}
char auto_4G(void)
{
	int state;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"4G test:");
	state = system("/opt/script/test_tcpip.sh ppp0");
	if(state == 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"4G OK");
		Sys_BeepMs(100);
		OSTIMER_DelayMs(2000);
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"4G fail");
		OSTIMER_DelayMs(3000);
		return 1;
	}
	return 0;
}

char auto_Wifi(void)
{
	int state;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"WIFI test:");
	state = system("/opt/script/test_tcpip.sh wlan0");
	if(state == 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"WIFI OK");
		Sys_BeepMs(100);
		OSTIMER_DelayMs(2000);
	}
	else
	{
		system("killall udhcpc");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"WIFI fail");
		OSTIMER_DelayMs(3000);
		return 1;
	}
	system("ifconfig wlan0 down");
	return 0;
}
char auto_TCP(void)
{
	int state;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"TCP/IP test:");
	state = system("/opt/script/test_tcpip.sh eth0");
	if(state == 0)
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TCP/IP OK");
		Sys_BeepMs(100);
		OSTIMER_DelayMs(2000);
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"TCP/IP fail");
		OSTIMER_DelayMs(3000);
		return 1;
	}
	return 0;
}
char sn_down(void)
{
	struct timeval ;
	int ret,i;
    int fd;
	unsigned char TmpBuff[1028]={"\0"},snbuf[200]={"\0"};

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"SN load:");
	fd = QRCode_Open(0);
	LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"scan sn>>");
	for(i = 0; i<5000; i++)
	{
		ret = QRCode_RxStr(fd, TmpBuff, 1023, 100);
		if((ret == 8) || (ret == 9))
		{
			Sys_BeepMs(100);
			ret = Sys_SetSN((char*)TmpBuff);
			ret = Sys_GetSN((char*)snbuf);
			if(!memcmp(snbuf,TmpBuff,strlen((char*)TmpBuff)))
			{
				LCD_Clear_rows(32,32);
				LCD_Printf(32, DISP_CENTER,"load ok>>");
				LCD_Printf(50, DISP_CENTER,snbuf);
				OSTIMER_DelayMs(3000);
			}
			else
			{
				LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"load fail");
				OSTIMER_DelayMs(3000);
				return 1;
			}
			system("sync");
			OSTIMER_DelayMs(3000);
			return 0;
		}
	}
	if(i == 5000)
	{
		LCD_Printf(32, DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"scan fail");
		OSTIMER_DelayMs(3000);
		//return 'f';
		return 1;
	}
	return 0x00;
}
unsigned char auto_can(int mode,unsigned int bitrate)
{
	int can_fd,i,nbytes,state,com5fd,com2fd,ret,j,sendnumber=0,canerr=0;
    unsigned int rcv_len;
	unsigned char wtbuff[10], rdbuff[10];
	unsigned char pBuffer[1024]={0};
	unsigned char dispbuff[20] = {0};

	can_fd = Can_Init("can0",bitrate);

	com5fd = OSUART_Init("/dev/ttymxc2","115200,n,8,1");
	com2fd = OSUART_Init("/dev/ttymxc1","115200,n,8,1");
	if(mode)
	{
		printf("\n********* CAN 485 test **********>>\n");
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(10, DISP_LEFT,"test:");
		while(1)
    	{
    		nbytes = Can_Read(can_fd, 0x11, &rcv_len, rdbuff,100); //
    		//printf("read [%d]",nbytes);
    		//
    		if(nbytes > 0)
    		{
    			sendnumber++;
				LCD_Clear_rows(32,32);
    			LCD_Printf(32, DISP_CENTER,"CAN read<<");
				sprintf((char*)dispbuff,"%d",sendnumber);
				LCD_Printf(48, DISP_CENTER,dispbuff);
    			nbytes = Can_Write(can_fd, 0x11, rcv_len, rdbuff);
    		}
			memset(pBuffer, 0, sizeof(pBuffer));
			ret = OSUART_RxStr(com5fd,pBuffer,sizeof(pBuffer),100);
			if(ret>0)
			{
				LCD_Clear_rows(32,32);
				LCD_Printf(32, DISP_CENTER,"485-232 data<<");
				OSUART_TxStr(com5fd, pBuffer,sizeof(pBuffer));
				LCD_Printf(48, DISP_CENTER,"485-232 data>>");
				OSTIMER_DelayMs(1000);
			}
			ret = OSUART_RxStr(com2fd,pBuffer,sizeof(pBuffer),100);
			if(ret>0)
			{
				LCD_Clear_rows(32,32);
				LCD_Printf(32, DISP_CENTER,"COM2 data<<");
				OSUART_TxStr(com2fd, pBuffer,sizeof(pBuffer));
				LCD_Printf(48, DISP_CENTER,"COM2 data>>");
				OSTIMER_DelayMs(1000);
			}
    	}
	}
	else
	{
		LCD_ClearScreen(0);
		LCD_ClearAll();
		LCD_Printf(0, DISP_LEFT,"CAN test:");
		memset(wtbuff, 0x23, 8);
		memset(rdbuff, 0x00, 8);
		for(j=0; j<50; j++)
		{
			Can_Write(can_fd,0x11,8,wtbuff);
			for(i=0;i<100000;i++)
    		{
    			nbytes = Can_Read(can_fd, 0x11, &rcv_len, rdbuff,100); //
    			if(nbytes > 0)
    			{
					state = memcmp(rdbuff, wtbuff, 8);
					if(state == 0)
					{
						canerr = 0|canerr;
						break;
					}
					else
					{
						canerr++;
						break;
					}
    			}
    		}
			if(i == 100000)
			{
				canerr++;
			}
		}
		if(canerr)
		{
			LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"CAN fail");
			OSTIMER_DelayMs(3000);
			Can_Close(can_fd);
			return 1;
		}
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"CAN OK");
		Sys_BeepMs(100);
		OSTIMER_DelayMs(2000);
		return 0;
	}
}

unsigned char auto_485(void)
{
	int com3fd;
	int ret;
	int i;
	unsigned char pBuffer[1024]={0};
	unsigned char wbuffer[1024]={0};
	printf("\n\n******* COM3_485 test *******\n");
	com3fd = OSUART_Init("/dev/ttymxc2","115200,n,8,1");
	if(com3fd == 0)
	{
		printf("open com3 err!\n");
	}
	ret = system("echo 74 > /sys/class/gpio/export");
	if(ret < 0)
	{
		printf("export err\n");
	}
	ret = system("echo out > /sys/class/gpio/gpio74/direction");
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"COM3_485 test:");
	memset(wbuffer, 'k', 100);
	ret = system("echo 1 > /sys/class/gpio/gpio74/value");
	OSUART_TxStr(com3fd, wbuffer, 100);
	ret = system("echo 0 > /sys/class/gpio/gpio74/value");
	memset(pBuffer, 0, sizeof(pBuffer));
	for(i = 0; i < 20; i++)
	{
		ret = OSUART_RxStr(com3fd,pBuffer,sizeof(pBuffer),100);
		if(ret>0)
    	{
			ret = memcmp(pBuffer, wbuffer, 100);
			if(ret == 0)
			{
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"485 OK");
				Sys_BeepMs(100);
				OSTIMER_DelayMs(2000);
				OSUART_Flush(com3fd);
				return 0;
			}
		}
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"485 fail");
	OSTIMER_DelayMs(3000);
	OSUART_Flush(com3fd);
	return 1;
}

unsigned char auto_232(void)
{
	int com2fd;
	int ret;
	int i;
	unsigned char pBuffer[1024]={0};
	unsigned char wbuffer[1024]={0};

	com2fd = OSUART_Init("/dev/ttymxc1","115200,n,8,1");
	if(com2fd == 0)
	{
		printf("open com2 err!\n");
	}
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"com2 test:");
	memset(wbuffer, 'k', 1023);

	OSUART_TxStr(com2fd, wbuffer, 1023);
	memset(pBuffer, 0, sizeof(pBuffer));
	for(i = 0; i < 20; i++)
	{
		ret = OSUART_RxStr(com2fd,pBuffer,sizeof(pBuffer),100);
		if(ret>0)
    	{
			ret = memcmp(pBuffer, wbuffer, 1023);
			if(ret == 0)
			{
				LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"com2 OK");
				Sys_BeepMs(100);
				OSTIMER_DelayMs(2000);
				OSUART_Flush(com2fd);
				return 0;
			}
		}
	}
	LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_FONT24|DISP_UNDERLINE,"com2 fail");
	OSTIMER_DelayMs(3000);
	OSUART_Flush(com2fd);
	return 1;

}

void err_log(int derr)
{
	int err;
	err = derr;

	printf("err = %4x \n",err);

	printf("++--------+-----++--------+-----++--------+-----++\n");
	printf("||  fun  | res||  fun  | res||  fun  | res||\n");
	printf("++--------+-----++--------+-----++--------+-----++\n");
//	printf("|  GPS   | OK  ||# wifi #|#ERR#||   USB  | OK  |\n");
////////////////////////////////////one row/////////////////////////////////////////////>
	if((err & 0x01) == 0)
	{
		printf("||   GPS  | OK  |");
	}
	else
	{
		printf("||#  GPS #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|   485  | OK  |");
	}
	else
	{
		printf("|## 485 #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|   CAN  | OK  ||\n");
	}
	else
	{
		printf("|#  CAN #|#ERR#||\n");
	}
	printf("++--------+-----++--------+-----++--------+-----++\n");

////////////////////////////////////one row/////////////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("||  COM2  | OK  |");
	}
	else
	{
		printf("||##COM2 #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|  WIFI  | OK  |");
	}
	else
	{
		printf("|# WIFI #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|   4G   | OK  ||\n");
	}
	else
	{
		printf("|## 4G ##|#ERR#||\n");
	}
	printf("++--------+-----++--------+-----++--------+-----++\n");
//////////////////////////////////////////two row////////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("||   TF   | OK  |");
	}
	else
	{
		printf("||## TF ##|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|   USB  | OK  |");
	}
	else
	{
		printf("|#  USB #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("| QR | OK  ||\n");
	}
	else
	{
		printf("|#QR#|#ERR#||\n");
	}
	printf("++--------+-----++--------+-----++--------+-----++\n");
//////////////////////////////////////three row////////////////////////////////////////>
	printf("||  LCD   | OK  |");
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("| left KEY | OK  |");
	}
	else
	{
		printf("|#left KEY#|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("| right KEY | OK  ||\n");
	}
	else
	{
		printf("|#rigth KEY#|#ERR#||\n");
	}
	printf("++--------+-----++--------+-----++--------+-----++\n");
//////////////////////////////////////four row/////////////////////////////////////////>
	printf("||   LED  | OK  ||  BEEP  | OK  ||  sound  | OK  ||\n");
 	printf("++--------+-----++--------+-----++--------+-----++\n");

//////////////////////////////////////five row/////////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("||   RTC  | OK  |");
	}
	else
	{
		printf("||## RTC #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|  SAM1  | OK  |");
	}
	else
	{
		printf("|# SAM1 #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|  SAM2  | OK  ||\n");
	}
	else
	{
		printf("|# SAM2 #|#ERR#||\n");
	}
	printf("++--------+-----++--------+-----++--------+-----++\n");
//////////////////////////////////////////six row///////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("||  SAM3  | OK  |");
	}
	else
	{
		printf("||# SAM3 #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|  SAM4  | OK  |");
	}
	else
	{
		printf("|# SAM4 #|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|  ESAM  | OK  ||\n");
	}
	else
	{
		printf("|# ESAM #|#ERR#||\n");
	}
	printf("++--------+-----++--------+-----++--------+-----++\n");
	//////////////////////////////////////////seven row///////////////////////////////////////>
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|| core eeprom | OK  |");
	}
	else
	{
		printf("||#core eeprom#|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("| main eeprom | OK  |");
	}
	else
	{
		printf("|#main eeprom#|#ERR#|");
	}
	err = err>>1;
	if((err & 0x01) == 0)
	{
		printf("|   A卡  | OK  ||\n");
	}
	else
	{
		printf("|#  A卡 #|#ERR#||\n");
	}
	printf("++--------+-----++--------+-----++--------+-----++\n");
}

void auto_test(void)
{
	int err=0;
	int ret;
	char retu;

	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"auto test:");
printf("\n\n=================== auto test ====================\n");
////////////////////////////////A card test///////////////////////////////////////////////>
	ret = auto_TypeAB();
	err = ret;
////////////////////////////////////////eeprom//////////////////////////////////////////>
	ret = auto_eeprom();
	err = (err<<2)|ret;
/////////////////////////////////SAM ////////////////////////////////////////////////////>
	ret = auto_Sam();
	err = (err<<5)|ret;
/////////////////////////////RTC test////////////////////////////////////////////////////>
	ret = auto_RTC();
	err = (err<<1)|ret;
///////////////////////////////sound test/////////////////////////////////////////////////>
	ret = auto_Speaker();
///////////////////////////////beep test///////////////////////////////////////////////>
	ret = auto_Beep();
//////////////////////////////LED test//////////////////////////////////////////////////>
	ret = auto_Led();
////////////////////////////////segled test//////////////////////////////////////////>
	ret = auto_SmKey();
	err = (err<<2)|ret;
//////////////////////////////////////LCD test///////////////////////////////////////////>
	ret = auto_Lcd();
/////////////////////////////////////QR code test//////////////////////////////////////////>
	ret = auto_QRcode();
	err = (err<<1)|ret;
////////////////////////////////////////U disk test/////////////////////////////////////////>
	ret = auto_USB();
	err = (err<<1)|ret;
///////////////////////////////////////TF card test/////////////////////////////////////////>
	ret = auto_TF();
	err = (err<<1)|ret;
/////////////////////////////////net test////////////////////////////////////////////////>
	ret = auto_4G();
	err = (err<<1)|ret;
	ret = auto_Wifi();
	err = (err<<1)|ret;
//////////////////////////////////////////BT test/////////////////////////////////////////>
	ret = auto_232();
	err = (err<<1)|ret;
////////////////////////////////////////CAN test//////////////////////////////////////////>
	ret = auto_can(0,100000);
	err = (err<<1)|ret;
////////////////////////////////////////485 test//////////////////////////////////////////>
	ret = auto_485();
	err = (err<<1)|ret;
	ret = auto_High();
	////////////////////////////////////////GPS test//////////////////////////////////////////>
	ret = auto_GPS();
	err = (err<<1)|ret;


	printf("\n\n================ the end =================\n");
	LCD_ClearScreen(0);
	LCD_ClearAll();
	LCD_Printf(0, DISP_LEFT,"auto test end");
	if(err == 0)
	{
		//system("aplay -q /home/root/sound/7499.wav &");
		printf("*************************************************\n");
		printf("**************    all fun   OK    ***********\n");
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"all fun OK");
		err_log(err);
		OSTIMER_DelayMs(3000);
		sn_down();
	}
	else
	{
		LCD_Printf(32,DISP_CENTER|DISP_CLRLINE|DISP_UNDERLINE,"all fun fail");
		printf("**************    all fun fail  ***********\n");
		err_log(err);
		OSTIMER_DelayMs(3000);
	}
//////////////////////////////////////aging test///////////////////////////////////////////>
	retu = select_aging();
	if((retu == 'f') || (retu == 'e'))
	{
		aging_test();
	}
}

void menu_test(void)
{
	char select;
	while(1)
	{
		select = select_test();
		printf("select = %c\n", select);
		switch(select)
		{
			case '0':
				auto_Speaker();
				break;
			case '1':
				auto_TypeAB();
				break;
			case '2':
				auto_Sam();
				break;
			case '3':
				auto_Beep();
				break;
			case '4':
				auto_Lcd();
				break;
			case '5':
				auto_GPS();
				break;
			case '6':
				auto_QRcode();
				break;
			case '7':
				auto_SmKey();
				break;
			case '8':
				auto_RTC();
				break;
			case '9':
				auto_Led();
				break;
			case 'a':
				auto_4G();
				break;
			case 'b':
				auto_Wifi();
				break;
			case 'c':
				auto_USB();
				break;
			case 'd':
				auto_TF();
				break;
			case 'e':
				auto_High();
				break;
			case 'f':
				auto_test();
				break;
			case 'g':
				sn_down();
				break;
			case 'h':
				auto_RTC_Read();
				break;
			case 'i':
				aging_test();
				break;
			case 'j':
				//auto_bluetooth();
				break;
			case 'k':
				auto_can(0,100000);
				break;
			case 'l':
				auto_485();
				break;
			case 'n':
				auto_232();
				break;
			case 'y':
				auto_can(1,100000);
				break;
			case 't':
				auto_TCP();
				break;
			default:
				auto_test();
				break;
		}
	}
}
//extern int test_other( int argc, char**  argv );
int main(int argc,char **argv)
{
    argc = (int)argc;
    argv = (char**)argv;

    if(argc < 3)
    {
	    print_help();
	    return -1;
    }
    system_init();
	switch(*argv[1])
	{
		case '0':
			test_speaker();
			break;
		case '1':
		{
			test_picc_single();
			break;
		}
		case 'b':
			//Test_B_card();
			break;
		  case 'c':
			//Test_C_card();
			break;
		case 'm':
			test_mifare();
			break;
		case 't':
			auto_TCP();
			break;
		case 'g':
			//Rf24g_Test();
			break;
		case '2':
            if(argv[3])
            	test_Sam(atoi(argv[2]),atoi(argv[3]));
			else
				test_Sam(atoi(argv[2]),0);
			break;
		case '3':
			test_buzzer();
			break;
		case '4':
			//test_lcd();
            test_lcd_speed();
			break;
		case '5':
			test_GPS();
			break;
		case '6':
			test_QRcode();
			break;
		case '7':
			test_LedSeg();
			break;
		case '8':
			//fram_rtctest(atoi(argv[2]));
			test_rtc();
			break;
		case '9':
			test_led();
			break;
		case 'a':
			//net_test();
			auto_4G();
			auto_Wifi();
			break;
		case 'i':
			//gprs_cmd_test();
			test_GPS();
			break;
		case 'j':
			//auto_bluetooth();
			break;
		case 'k':
			auto_can(atoi(argv[3]),atoi(argv[2]));
			break;
		case 'l':
			auto_485();
			break;
		case 'n':
			auto_232();
			break;
		case 'o':
			test_lcd_logo();
			break;
		case 'p':
			//pwr_pro_test();
			break;
		case 'q':
			test_eeprom();
			break;
		case 'r':
			auto_test();
			break;
		case 'z':
			menu_test();
			break;
        case 'A':
            LC812_test();
            break;
		case 'B':
		{
            test_picc();
			break;
		}
		default:{
			int ret ;

			//ret = test_other(argc,argv);
			if(ret < 0)
				print_help();
			break;
		}
	}
    return 0;
}



