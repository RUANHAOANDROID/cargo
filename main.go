package main

import "C"
import (
	"cargo/api/icbc"
	"cargo/clib"
	"cargo/config"
	"cargo/internal"
	"cargo/msg"
	"cargo/pkg"
	"cargo/screen"
	"cargo/speaker"
	"sync"
)

var display *clib.Display

// main  -lpos -lm -lpng -lfontconfig -lfreetype -liconv
func main() {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Error("main panic:", r)
		}
	}()
	var wg sync.WaitGroup
	conf, err := config.Load("config.yml")
	if err != nil {
		panic("not find config.yml")
	}
	msgChan := make(chan msg.Message)
	display = clib.NewDisplay()
	display.ClearScreen()
	screen.Set(display)
	display.Show("启动中..", "正在获取配置..")
	emcsConf, err := internal.GetConfig(conf.ServerUrl)
	if err != nil {
		display.LCDRow(err.Error(), 60, 80, clib.DISP_FONT12)
		pkg.Log.Error(err)
		display.Show("错误..", "获取配置错误！！")
		panic("获取配置错误！")
	}
	//---------设置NTP
	display.Show("启动中..", "正在配置NTP..")
	//decaros.SetNTP()
	icbc.SetConfig(conf.ServerUrl, conf.Sha, emcsConf)
	display.Show("请刷票", "支持二维码，IC卡验票")
	display.ShowTime()
	internal.CheckUpdate(conf.DeviceType, config.Version, emcsConf.EquipmentNo)
	wg.Add(1)
	go func() {
		defer wg.Done()
		clib.StartTcpServer(msgChan)
	}()
	go internal.StartTimer(conf, display)
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
			go parseResp(err, resp)
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
		pkg.Log.Println("Check ticket SUCCESS!")
		go speaker.Speaker(resp.RetMsg, true)
		screen.Show(resp.RetMsg, true)
		internal.SaveCount()
	} else {
		pkg.Log.Println("Check ticket Fail!")
		go speaker.Speaker(resp.RetMsg, false)
		screen.Show(resp.RetMsg, false)
	}
}
