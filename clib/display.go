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
#include "unistd.h"
*/
import "C"
import (
	"cargo/decaros"
	"cargo/pkg"
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
	pkg.Log.Printf("display Height=%v,Width=%v\n", cLedHeight, cLedWidth)
}

// ClearScreen 清理屏幕
func (d Display) ClearScreen() {
	cScreen := C.uint32_t(1)
	pkg.Log.Println("display ClearScreen")
	C.LCD_ClearScreen(cScreen)
}

// LCDRow 显示文字
func (d Display) LCDRow(text string, x int16, y int16, mode C.uint) {
	pkg.Log.Println(text, x, y, mode)
	cTitle := C.CString(text)
	defer C.free(unsafe.Pointer(cTitle))
	cX := C.uint16_t(x)
	cY := C.uint16_t(y)
	C.LCD_Display_Row(cTitle, cY, cX, mode)
}
func (d Display) showTitleArea(title string) {
	d.LCDRow(title, 2, 2, DISP_FONT24)
}
func (d Display) showContentArea(content string) {
	d.LCDRow(content, 2, 30, DISP_FONT12)
}
func (d Display) showBottomArea() {
	d.LCDRow(pkg.IPV4(), 0, 68, DISP_FONT12)
	sn, err := decaros.GetSN()
	if err != nil {
		d.LCDRow("sn err", 94, 68, DISP_FONT12)
		return
	}
	d.LCDRow(sn, 94, 68, DISP_FONT12)
}
func (d Display) Show(title string, content string) {
	d.ClearScreen()
	d.showTitleArea(title)
	d.showContentArea(content)
	d.showBottomArea()
}
