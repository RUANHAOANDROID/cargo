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

	display.LCDRow("CGO Test", 8, 8, clib.DISP_FONT24)
	display.LCDRow(pkg.NowTimeStr(), 8, 40, clib.DISP_FONT12)
	display.LCDRow("-hao88.cloud", 80, 60, clib.DISP_FONT12)
	scanner := clib.NewScanner()
	go func() {
		scanner.Receive()
	}()
	card := clib.NewCardM1()
	go func() {
		card.TestMifare()
	}()

	go func() {
		for number := range card.CmChan {
			fmt.Print("chan->")
			fmt.Println(number)
		}
	}()
	for qrCode := range scanner.QrChan {
		fmt.Print("chan->")
		fmt.Println(qrCode)
	}
	pkg.Log.Print("End......")
}
