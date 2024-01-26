package main

import (
	"cargo/clib"
	"cargo/msg"
	"cargo/pkg"
	"sync"
)

// main  -lpos -lm -lpng -lfontconfig -lfreetype -liconv
func main() {
	var wg sync.WaitGroup
	pkg.Log.Print("Start......1")
	msgChan := make(chan msg.Message)
	display := clib.Display{}
	display.Init()
	display.ClearScreen()

	display.LCDRow("C Test D", 8, 8, clib.DISP_FONT24)
	display.LCDRow(pkg.NowTimeStr(), 8, 40, clib.DISP_FONT12)
	display.LCDRow("-hao88.cloud", 80, 60, clib.DISP_FONT12)
	wg.Add(1)
	go clib.StartTcpServer(&msgChan)
	go clib.StartC()
	for msg := range msgChan {
		pkg.Log.Println("msg chan->", msg.Type, msg.Content)
	}
	pkg.Log.Print("End......")
	wg.Wait()
}
