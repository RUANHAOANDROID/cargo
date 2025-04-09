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

void ShowNetworkIcon(void){
	LCD_DisplayIcon(ICON_SIGNAL,50);
	//LCD_DisplayIcon(ICON_NETWORK,3);//网络类型
	LCD_DisplayIcon(ICON_WIFI,3);//wifi
}
*/
import "C"
import (
	"cargo/config"
	"cargo/pkg"
	"fmt"
	"time"
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
	Width, Height uint16
}

func NewDisplay() *Display {
	pkg.Log.Println("display Init")
	C.LCD_Init()
	cLedHeight := C.LCD_GetHeight()
	cLedWidth := C.LCD_GetWidth()
	pkg.Log.Printf("Display Height=%v,Width=%v\n", cLedHeight, cLedWidth)
	return &Display{Height: uint16(cLedHeight), Width: uint16(cLedWidth)}
}

// ClearScreen 清理屏幕
func (d *Display) ClearScreen() {
	fmt.Println("--------------clear screen----------------")
	cScreen := C.uint32_t(0)
	C.LCD_ClearScreen(cScreen)
	C.LCD_ClearAll()
	C.ShowNetworkIcon()
}

func (d *Display) AddTopLine(y uint16) {
	cTitle := C.CString("---------------------------")
	defer C.free(unsafe.Pointer(cTitle))
	cX := C.uint16_t(0)
	cY := C.uint16_t(y)
	C.LCD_Display_Row(cTitle, cY, cX, DISP_FONT6X8)
}

// LCDRow 显示文字
func (d *Display) LCDRow(text string, x int16, y int16, mode C.uint) {
	pkg.Log.Println(text, x, y, mode)
	cTitle := C.CString(text)
	defer C.free(unsafe.Pointer(cTitle))
	cX := C.uint16_t(x)
	cY := C.uint16_t(y)
	C.LCD_Display_Row(cTitle, cY, cX, mode)
}

func (d *Display) showTitleArea(title string) {
	//l := int16(len(title) * 24)
	//x := 160 - l/2
	d.LCDRow(title, 1, 8+6, DISP_FONT24)
}
func (d *Display) ShowPassedCount(count string) {
	showContext := " " + count
	textWidth := (len([]rune(showContext)) + 2) * 12
	pkg.Log.Printf("内容%s,宽度%d\n", showContext, textWidth)
	x := 160 - textWidth
	pkg.Log.Println(x)
	d.LCDRow(showContext, 0, int16(d.Height-8), DISP_FONT12)
	//d.LCDRow(showContext, int16(x), int16(80-12), DISP_FONT12)
}
func (d *Display) ShowTime() {
	currentTime := time.Now()
	formattedTime := currentTime.Format("06/01/02 15:04")
	// 打印格式化后的时间
	pkg.Log.Println("当前时间:", formattedTime)
	dataLen := len(formattedTime) * 6
	C.LCD_Clear_rows(C.uint16_t(160), C.uint16_t(8))
	d.LCDRow(formattedTime, int16(160-dataLen), 1, DISP_FONT6X8)
}
func (d *Display) showContentArea(content string) {
	C.LCD_Clear_rows(C.uint16_t(160), C.uint16_t(34))
	y := 8 + 6 + 24 + 2
	d.LCDRow(content, 1, int16(y), DISP_FONT12)
}

func (d *Display) showBottomArea() {
	//d.LCDRow(pkg.IPV4(), 0, int16(d.Height-8), DISP_FONT6X8)
	d.LCDRow("v"+config.Version, int16(d.Width-30), int16(d.Height-10), DISP_FONT6X8)
}
func (d *Display) Show(title string, content string) {
	d.ClearScreen()
	d.ShowTime()
	d.AddTopLine(9)
	d.showTitleArea(title)
	d.showContentArea(content)
	d.AddTopLine(60)
	d.showBottomArea()

}
func (d *Display) ShowNormal(passedCount string) {
	d.ClearScreen()
	d.ShowTime()
	d.AddTopLine(9)
	d.showTitleArea("请刷票")
	d.showContentArea("扫描二维码或刷IC卡")
	d.AddTopLine(60)
	d.ShowPassedCount(passedCount)
	d.showBottomArea()
}
func (d *Display) ShowError(content string) {
	d.ClearScreen()
	d.AddTopLine(9)
	d.ShowTime()
	d.showTitleArea("验票失败")
	d.showContentArea(content)
	d.AddTopLine(60)
}
func (d *Display) ShowSuccess(content string) {
	d.ClearScreen()
	d.ShowTime()
	d.AddTopLine(9)
	d.showTitleArea("验票成功")
	d.showContentArea(content)
	d.AddTopLine(60)
	d.showBottomArea()
}
