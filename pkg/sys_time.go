package pkg

import (
	"encoding/json"
	"net/http"
	"os/exec"
)

type SNTime struct {
	SysTime1 string `json:"sysTime1"`
	SysTime2 string `json:"sysTime2"`
}

func SetSysTime() {
	clit := &http.Client{}
	resp, err := clit.Get("https://quan.suning.com/getSysTime.do")
	if err != nil {
		Log.Println(err)
		return
	}
	var snTime SNTime

	err = json.NewDecoder(resp.Body).Decode(&snTime)
	if err != nil {
		Log.Println("JSON unmarshal error:", err)
		return
	}
	Log.Println("sysTime1:", snTime.SysTime1)
	Log.Println("sysTime2:", snTime.SysTime2)

	cmd := exec.Command("date", "--set=", snTime.SysTime2)
	cmd.Stderr = nil
	if err := cmd.Start(); err != nil {
		Log.Println("cmd run error:", err)
		return
	}
}
