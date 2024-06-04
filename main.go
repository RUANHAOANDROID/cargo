package main

import "C"
import (
	"cargo/api"
	"cargo/clib"
	"cargo/config"
	"cargo/decaros"
	"cargo/internal"
	"cargo/msg"
	"cargo/pkg"
	"cargo/screen"
	"cargo/speaker"
	"strconv"
	"sync"
	"time"
)

var display *clib.Display

// 自启动至少需要10秒，等待系统组件加载完毕
var sleepTime = 0 * time.Second

// main  -lpos -lm -lpng -lfontconfig -lfreetype -liconv
func main() {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Error("main panic:", r)
		}
	}()
	//go func() {
	//	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
	//		// 设置HTTP头部，指示这是一个SSE连接
	//		w.Header().Set("Content-Type", "text/event-stream")
	//		w.Header().Set("Cache-Control", "no-cache")
	//		w.Header().Set("Connection", "keep-alive")
	//
	//		// 向客户端发送数据
	//		for {
	//			var memStats runtime.MemStats
	//			runtime.ReadMemStats(&memStats)
	//			M := memStats.Alloc / 1024 / 1024
	//			KB := memStats.Alloc / 1024
	//			fmt.Fprintf(w, "Goroutines: %d, Memory: %d MB , %d KB\n", runtime.NumGoroutine(), M, KB)
	//			w.(http.Flusher).Flush() // 强制发送到客户端
	//			time.Sleep(1 * time.Second)
	//		}
	//	})
	//	http.ListenAndServe(":6060", nil)
	//}()
	var wg sync.WaitGroup
	conf, err := config.Load("config.yml")
	if err != nil {
		panic("not find config.yml")
	}
	msgChan := make(chan msg.Message)
	display = clib.NewDisplay()
	display.ClearScreen()
	screen.Set(display)
	display.Show("启动中..", "正在等待系统组件加载..")
	time.Sleep(sleepTime)
	display.Show("启动中..", "正在获取配置..")
	emcsConf, err := internal.GetConfig(conf.ServerUrl)
	if err != nil {
		display.LCDRow(err.Error(), 60, 80, clib.DISP_FONT12)
		pkg.Log.Error(err)
		display.Show("错误..", "获取配置错误！！")
		time.Sleep(sleepTime)
		display.Show("正在重试..", "获取配置中..")
		time.Sleep(sleepTime)
		emcsConf, err = internal.GetConfig(conf.ServerUrl)
		if err != nil {
			display.LCDRow(err.Error(), 60, 80, clib.DISP_FONT12)
			pkg.Log.Error(err)
			display.Show("错误..", "获取配置错误！！")
			panic("获取配置错误！")
		}
	}
	//---------设置NTP
	display.Show("启动中..", "正在配置NTP..")
	decaros.SetNTP()
	api.SetConfig(conf.ServerUrl, conf.Sha, emcsConf)
	showNormal(0)
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
			resp, err := api.CheckTicket(cMsg.Content, api.ProtoIC)
			parseResp(err, resp)
		case msg.QRCODE:
			resp, err := api.CheckTicket(cMsg.Content, api.ProtoQr)
			parseResp(err, resp)
		default:
			pkg.Log.Println("undefined type")
		}
	}
	pkg.Log.Print("End......")
}

func parseResp(err error, resp *api.CheckResponse) {
	if err != nil {
		pkg.Log.Error(err)
		pkg.Log.Error(resp)
		speaker.Speaker("验票失败", false)
		screen.Show(err.Error(), false)
		return
	}
	var passedCount = 0
	if resp.RetCode == "0" {
		go screen.Show(resp.RetMsg, true)
		if resp.MerNotVerTktNum != "" && resp.MerNotVerTktNum != "1" {
			go speaker.SpeakerGroup(resp.MerNotVerTktNum)
			go display.ShowSuccess("团队票 可通行" + resp.MergeTicketNo + " 人")
		} else {
			go speaker.Speaker(resp.RetMsg, true)
		}
		passedCount, err = internal.WritePassedCount()
		if err != nil {
			pkg.Log.Println(err)
		}
	} else {
		pkg.Log.Println("Check ticket Fail!")
		go screen.Show(resp.RetMsg, false)
		go speaker.Speaker(resp.RetMsg, false)
	}
	time.Sleep(3 * time.Second)
	showNormal(passedCount)
}
func showNormal(pCount int) {
	pkg.Log.Printf("passed count=%d\n", pCount)
	if pCount == 0 {
		passedCount, err := internal.ReadPassedCount()
		if err != nil {
			pkg.Log.Println(err)
		}
		pCount = passedCount
	}
	str := strconv.Itoa(pCount)
	display.ShowNormal(str)
	api.SyncDayCount(int32(pCount))
}
