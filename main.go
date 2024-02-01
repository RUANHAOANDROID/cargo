package main

import "C"
import (
	"cargo/api/icbc"
	"cargo/clib"
	"cargo/config"
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"sync"
)

// main  -lpos -lm -lpng -lfontconfig -lfreetype -liconv
func main() {
	var wg sync.WaitGroup
	conf, err := config.Load("config.yml")
	if err != nil {
		panic("not find config.yml")
	}
	icbc.SetConfig(conf)
	pkg.Log.Print("Start......1")
	msgChan := make(chan msg.Message)
	display := clib.Display{}
	display.Init()
	display.ClearScreen()

	display.LCDRow("C Test D", 8, 8, clib.DISP_FONT24)
	display.LCDRow(pkg.NowTimeStr(), 8, 40, clib.DISP_FONT12)
	display.LCDRow("-hao88.cloud", 80, 60, clib.DISP_FONT12)
	wg.Add(1)
	go func() {
		wg.Done()
		clib.StartTcpServer(msgChan)
	}()
	go clib.StartC(wg)
	for cMsg := range msgChan {
		pkg.Log.Printf("msg chan-> type=%v,content=%v \n", cMsg.Type, cMsg.Content)
		switch cMsg.Type {
		case msg.IC_CARD:
			resp, err := icbc.CheckTicket(cMsg.Content, icbc.ProtoIC)
			if err != nil {
				pkg.APlay("sksb.wav")
			}
			if resp.RetCode == "0" {
				pkg.APlay("skcg.wav")
			} else {
				pkg.APlay("feifaka.wav")
			}

			pkg.Log.Println(resp)
		case msg.QRCODE:
			resp, err := icbc.CheckTicket(cMsg.Content, icbc.ProtoQr)
			if err != nil {
				pkg.APlay("sksb.wav")
			}
			if resp.RetCode == "0" {
				pkg.APlay("skcg.wav")
			} else {
				pkg.APlay("feifaka.wav")
			}
			pkg.Log.Println(resp)
		default:
			fmt.Println("undefined type")
		}
	}
	pkg.Log.Print("End......")
}
