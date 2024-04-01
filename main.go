package main

import "C"
import (
	"cargo/api/icbc"
	"cargo/clib"
	"cargo/config"
	"cargo/emcs"
	"cargo/msg"
	"cargo/pkg"
	"sync"
)

// main  -lpos -lm -lpng -lfontconfig -lfreetype -liconv
func main() {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Error("main panic:", r)
		}
	}()
	pkg.SetSysTime()
	var wg sync.WaitGroup
	conf, err := config.Load("config.yml")
	if err != nil {
		panic("not find config.yml")
	}

	pkg.Log.Print("Start......1")
	msgChan := make(chan msg.Message)
	display := clib.Display{}
	display.Init()
	display.ClearScreen()

	display.LCDRow("C Test D", 8, 8, clib.DISP_FONT24)
	display.LCDRow(pkg.NowTimeStr(), 8, 40, clib.DISP_FONT12)
	emcsConf, err := emcs.GetConfig(conf.ServerUrl)
	if err != nil {
		display.LCDRow(err.Error(), 60, 80, clib.DISP_FONT12)
		pkg.Log.Error(err)
		panic("获取配置错误！")
	}
	icbc.SetConfig(conf, emcsConf)
	display.LCDRow(emcsConf.EquipmentNo, 68, 64, clib.DISP_FONT12)
	//display.LCDRow("-hao88.cloud", 80, 60, clib.DISP_FONT12)
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
			pkg.Log.Printf("ic card=%s\n", cMsg.Content)
			resp, err := icbc.CheckTicket(cMsg.Content, icbc.ProtoIC)
			if err != nil {
				pkg.APlay("sksb.wav")
			}
			if resp.RetCode == "0" {
				pkg.Log.Println("check ticket success")
				pkg.APlay("skcg.wav")
			} else {
				pkg.Log.Println("check ticket fail")
				pkg.APlay("feifaka.wav")
			}
			//pkg.Log.Println(resp)
		case msg.QRCODE:
			pkg.Log.Printf("qrocde=%s\n", cMsg.Content)
			resp, err := icbc.CheckTicket(cMsg.Content, icbc.ProtoQr)
			if err != nil {
				pkg.Log.Error(err)
				pkg.APlay("sksb.wav")
			}
			if resp.RetCode == "0" {
				pkg.Log.Println("check ticket success")
				pkg.APlay("skcg.wav")
			} else {
				pkg.Log.Println("check ticket fail")
				pkg.APlay("feifaka.wav")
			}
			//pkg.Log.Println(resp)
		default:
			pkg.Log.Println("undefined type")
		}
	}
	pkg.Log.Print("End......")
}
