package clib

/*
#cgo LDFLAGS: -L../sdk/lib -lpos -lpng -lfreetype -liconv -lm -lz -lrt
#cgo CFLAGS: -I../sdk/inc
#include "clib.h"
*/
import "C"
import (
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"sync"
	"time"
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

var tmpBuff [512]C.uchar

func (q *Scanner) Receive() {
	index := C.int(0)
	qrfd0 := C.QRCode_Open(index)
	C.Beep_Init()

	for {
		ret := C.QRCode_RxStr(qrfd0, &tmpBuff[0], 512, 100)
		if ret > 0 {
			C.Sys_BeepMs(200)
			uChars := tmpBuff[:ret]
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

func StartC(wg sync.WaitGroup) {
	pkg.Log.Println("Start C TCP Client wait 1 Second---")
	time.Sleep(time.Second)
	C.start_tcp()
	wg.Add(1)
	go func() {
		defer wg.Done()
		C.read_data(1, 0)
	}()
	//go func() {
	//	defer wg.Done()
	//	C.qr_read()
	//}()
	//wg.Add(1)
	//go func() {
	//	defer wg.Done()
	//	C.ic_read()
	//}()
	//wg.Add(1)
	//go func() {
	//	defer wg.Done()
	//	C.id_read()
	//}()
}

func StopC() {
	pkg.Log.Println("close tcp connection")
	defer C.close_connection(-1)
}
