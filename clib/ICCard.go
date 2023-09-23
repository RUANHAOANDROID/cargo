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
void ic_read(void) {
    int ret = -1;
    uint8_t key[] = "\xFF\xFF\xFF\xFF\xFF\xFF";
    uint8_t data[16], data_len = 16;
    uint8_t snr[16], snr_len;
    //PICC_Open(0);
    int i = 0;
    if (ret) ret = Mifare_PowerOn(0, snr, &snr_len);
    if (!ret) ret = Mifare_AuthenBlock(0, 0, key);
    if (!ret) ret = Mifare_ReadBlock(0, data);
    if (!ret) {
		dump_data("read ic ",data,data_len);
    }
}
*/
import "C"
import (
	"time"
)

type ICCarder struct {
	CmChan chan string
}

func NewICCarder() *ICCarder {
	return &ICCarder{
		CmChan: make(chan string),
	}
}
func (c ICCarder) ICReadGO() {
	C.PICC_Open(0)

	for {
		time.Sleep(time.Millisecond * 500)
		C.test_mifare()
	}
}
