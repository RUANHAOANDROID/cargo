package emcs

import (
	"bytes"
	"cargo/decaros"
	"cargo/pkg"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"time"
)

type Config struct {
	TrueVoice1    string `json:"trueVoice1"`
	HeartbeatTime int    `json:"heartbeatTime"`
	TrueVoice3    string `json:"trueVoice3"`
	TrueVoice2    string `json:"trueVoice2"`
	DeFalseVoice  struct {
		Other   string `json:"other"`
		Invalid string `json:"invalid"`
		Used    string `json:"used"`
		Error   string `json:"error"`
	} `json:"deFalseVoice"`
	ConfigNo          string `json:"configNo"`
	WriteOffDelayTime int    `json:"writeOffDelayTime"`
	InvalidTime       int    `json:"invalidTime"`
	EquipmentNo       string `json:"equipmentNo"`
	Yccode            string `json:"yccode"`
	ManufacturerId2   string `json:"manufacturerId2"`
	ManufacturerId1   string `json:"manufacturerId1"`
	DeTrueVoice       struct {
		Normal    string `json:"normal"`
		Other     string `json:"other"`
		YearCard1 string `json:"yearCard1"`
		YearCard2 string `json:"yearCard2"`
	} `json:"deTrueVoice"`
	WriteOffUrl  string `json:"writeOffUrl"`
	NumUpUrl     string `json:"numUpUrl"`
	HeartbeatUrl string `json:"heartbeatUrl"`
	FalseVoice3  string `json:"falseVoice3"`
	NumUpTime    int    `json:"numUpTime"`
	FalseVoice4  string `json:"falseVoice4"`
	WelcomeMsg   string `json:"welcomeMsg"`
	TrueVoice4   string `json:"trueVoice4"`
	CheckUrl     string `json:"checkUrl"`
	FalseVoice1  string `json:"falseVoice1"`
	FalseVoice2  string `json:"falseVoice2"`
}
type Response struct {
	Code int             `json:"code"`
	Msg  string          `json:"msg"`
	Data json.RawMessage `json:"data"`
}

func GetConfig(url string) (*Config, error) {
	pkg.Log.Println(url)
	sn, err := decaros.GetSN()
	if err != nil {
		pkg.Log.Println("获取SN失败:", err)
		return nil, err
	}
	pkg.Log.Println(sn)
	// 构建请求体
	requestBody, err := json.Marshal(map[string]interface{}{
		"data": map[string]string{
			"serialNum": sn,
		},
		"sign":      "D715EFDE3F4E509F3FE7FB11CAD3F677",
		"timestamp": time.Now().Unix(),
	})
	if err != nil {
		fmt.Println("Error marshalling request body:", err)
		return nil, err
	}

	// 发起 POST 请求
	resp, err := http.Post(url+"/gateMachine/getConfig", "application/json", bytes.NewBuffer(requestBody))
	if err != nil {
		fmt.Println("Error sending request:", err)
		return nil, err
	}
	defer resp.Body.Close()

	// 读取响应
	var response Response
	if err := json.NewDecoder(resp.Body).Decode(&response); err != nil {
		fmt.Println("Error decoding response:", err)
		return nil, err
	}
	if response.Code == 0 {
		var conf Config
		err = json.Unmarshal(response.Data, &conf)
		if err == nil {
			return &conf, nil
		}
	} else {
		return nil, errors.New(response.Msg)
	}
	return nil, err
}
