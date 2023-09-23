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
*/
import "C"
import (
	"cargo/msg"
	"fmt"
)

type Scanner struct {
	QRChan chan msg.Message
}

func NewScanner(msgChan chan msg.Message) *Scanner {
	return &Scanner{
		QRChan: msgChan,
	}
}

func (q *Scanner) Receive() {
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
			q.QRChan <- msg.Message{Type: msg.QRCODE, Content: hexString}
		}
	}
}
