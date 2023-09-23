package main

import (
	"cargo/clib"
	"cargo/pkg"
	"fmt"
)

// main  -lpos -lm -lpng -lfontconfig -lfreetype -liconv
func main() {
	pkg.Log.Print("Start......")
	display := clib.Display{}
	display.Init()
	display.ClearScreen()

	display.LCDRow("C Test", 8, 8, clib.DISP_FONT24)
	display.LCDRow(pkg.NowTimeStr(), 8, 40, clib.DISP_FONT12)
	display.LCDRow("-hao88.cloud", 80, 60, clib.DISP_FONT12)
	qrCoder := clib.NewScanner()
	go func() {
		qrCoder.Receive()
	}()
	//card := clib.NewCardM1()
	card := clib.NewICCarder()
	go func() {
		card.ICReadGO()
	}()

	go func() {
		for number := range card.CmChan {
			fmt.Print("chan->")
			fmt.Println(number)
		}
	}()
	for qrCode := range qrCoder.QrChan {
		fmt.Print("chan->")
		fmt.Println(qrCode)
	}
	pkg.Log.Print("End......")
}
