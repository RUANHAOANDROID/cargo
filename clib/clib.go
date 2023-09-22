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

void 	dump_data(char *str,unsigned char *text,int len)
{
	int i;
    printf("%s(%d):",str,len);
    for(i = 0;i < len;i++)
    {
        printf("%02X ",(unsigned char)text[i]);
    }
    printf("\n");
}
// test
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
*/
import "C"
import (
	"cargo/pkg"
	"fmt"
	"unsafe"
)

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

func (d Display) ClearScreen() {
	cScreen := C.uint32_t(1)
	pkg.Log.Println("display ClearScreen")
	C.LCD_ClearScreen(cScreen)
}

func (d Display) LCDRow(text string, x int16, y int16, mode C.uint) {
	pkg.Log.Println(text, x, y, mode)
	cTitle := C.CString(text)
	defer C.free(unsafe.Pointer(cTitle))
	cX := C.uint16_t(x)
	cY := C.uint16_t(y)
	C.LCD_Display_Row(cTitle, cY, cX, mode)
}

type Scanner struct {
	QrChan chan string
}

func NewScanner() *Scanner {
	return &Scanner{
		QrChan: make(chan string),
	}
}
func (s *Scanner) Receive() {
	index := C.int(0)
	qrfd0 := C.QRCode_Open(index)
	//index = C.int(1)
	//qrfd1 := C.QRCode_Open(index)
	C.Beep_Init()
	var tmpBuff [1024]C.uchar
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
			s.QrChan <- hexString
		}
	}

}

type CardM1 struct {
	CmChan chan string
}

func NewCardM1() *CardM1 {
	return &CardM1{
		CmChan: make(chan string),
	}
}
func (c CardM1) TestMifare() {
	C.test_mifare()
}
func (c CardM1) Receive() {
	ret := C.int(-1)
	key := [6]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
	//data := [16]C.uint8_t{} // 初始化一个长度为16的数组
	//data_len := C.uint8_t(16)
	var snr C.uchar
	var data C.uchar // 初始化一个长度为16的数组
	var snrLen C.uchar
	//var sectorAddr C.uint8_t = 0
	var keyType C.ENUM_MIFARE_KEY_TYPE = C.MIFARE_KEY_A
	C.PICC_Open(0)
	for i := 0; i < 16; i++ {
		if ret == 1 {
			ret = C.Mifare_PowerOn(0, &snr, &snrLen)
		}
		if ret != 1 {
			fmt.Println((*C.uint8_t)(&key[0]))
			C.Mifare_AuthenBlock(keyType, C.uint8_t(0), (*C.uint8_t)(&key[0]))
		}

		//if ret != 1 {
		C.Mifare_ReadBlock(C.uint8_t(4), &data)
		//}
		fmt.Println(data)
	}
}
