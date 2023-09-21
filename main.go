package main

import (
	"cargo/pkg"
)

// main  -lpos -lm -lpng -lfontconfig -lfreetype -liconv
func main() {
	pkg.Log.Print("Start......")
	display := Display{}
	display.Init()
	display.ClearScreen()
	display.LCDRow("CGO Test", 8, 8, DISP_FONT24)
	display.LCDRow("Test oooo", 8, 40, DISP_FONT12)
	display.LCDRow("-hao88.cloud", 80, 60, DISP_FONT12)
	scanner := Scanner{}
	scanner.Receive()
	pkg.Log.Print("End......")
}
