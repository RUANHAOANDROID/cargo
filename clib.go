package main

/*
#cgo LDFLAGS: -L./sdk/lib -lpos -lpng -lfreetype -liconv -lm -lz -lrt
#cgo CFLAGS: -I./sdk/inc
#include "sdk/inc/Felica.h"
#include "sdk/inc/mc_led.h"
#include "sdk/inc/oslib.h"
#include "sdk/inc/toolslib.h"
#include "stdlib.h"
#include "stdio.h"
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
	qrChan chan string
}

func NewScanner() *Scanner {
	return &Scanner{
		qrChan: make(chan string),
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
			fmt.Println(uChars)
			// Convert the C array to a Go []byte
			byteSlice := make([]byte, len(uChars))
			for i, v := range uChars {
				byteSlice[i] = byte(v)
			}
			hexString := string(byteSlice)
			fmt.Println(hexString)
			s.qrChan <- hexString
		}
	}

}
