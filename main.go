package main

import "C"
import (
	"cargo/api/icbc"
	"cargo/clib"
	"cargo/config"
	"cargo/decaros"
	"cargo/emcs"
	"cargo/msg"
	"cargo/pkg"
	"cargo/screen"
	"cargo/speaker"
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
	screen.Set(&display)
	display.Show("启动中..", "正在获取配置..")
	emcsConf, err := emcs.GetConfig(conf.ServerUrl)
	if err != nil {
		display.LCDRow(err.Error(), 60, 80, clib.DISP_FONT12)
		pkg.Log.Error(err)
		display.Show("错误..", "获取配置错误！！")
		panic("获取配置错误！")
	}
	//---------设置NTP
	display.Show("启动中..", "正在配置NTP..")
	decaros.SetNTP()
	icbc.SetConfig(conf, emcsConf)
	display.Show("请刷票", "支持二维码，IC卡验票")
	wg.Add(1)
	go func() {
		defer wg.Done()
		clib.StartTcpServer(msgChan)
	}()
	go clib.StartC(wg)
	for cMsg := range msgChan {
		pkg.Log.Printf("msg chan-> type=%v,content=%v \n", cMsg.Type, cMsg.Content)
		switch cMsg.Type {
		case msg.IC_CARD:
			pkg.Log.Printf("ic card=%s\n", cMsg.Content)
			resp, err := icbc.CheckTicket(cMsg.Content, icbc.ProtoIC)
			parseResp(err, resp)
			//pkg.Log.Println(resp)
		case msg.QRCODE:
			pkg.Log.Printf("qrocde=%s\n", cMsg.Content)
			resp, err := icbc.CheckTicket(cMsg.Content, icbc.ProtoQr)
			parseResp(err, resp)
		default:
			pkg.Log.Println("undefined type")
		}
	}
	pkg.Log.Print("End......")
}

func parseResp(err error, resp *icbc.CheckResponse) {
	if err != nil {
		pkg.Log.Error(err)
		pkg.Log.Error(resp)
		go speaker.Speaker("验票失败", false)
		screen.Show(err.Error(), false)
		return
	}
	if resp.RetCode == "0" {
		pkg.Log.Println("check ticket success")
		go speaker.Speaker(resp.RetMsg, true)
		screen.Show(resp.RetMsg, true)
	} else {
		pkg.Log.Println("check ticket fail")
		go speaker.Speaker(resp.RetMsg, false)
		screen.Show(resp.RetMsg, false)
	}
}
