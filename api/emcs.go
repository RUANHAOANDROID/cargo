package api

import (
	"bytes"
	"cargo/config"
	"cargo/emcs"
	"cargo/pkg"
	"google.golang.org/protobuf/proto"
	"net/http"
	"strconv"
	"time"
)

//emcs API Post Request

func SyncDayCount(count int32) {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Error("同步天人数失败", r)
		}
	}()

	upDayCOunt := &emcs.NumUploadRequest{
		DeviceId:  emcsConf.EquipmentNo,
		TimeStamp: strconv.FormatInt(time.Now().Unix(), 10),
		PeopleNum: count,
	}
	data, err := proto.Marshal(upDayCOunt)
	if err != nil {
		pkg.Log.Fatalf("Failed to marshal example: %v", err)
	}

	url := "http://emcs-api.youchiyun.com"
	req, err := http.NewRequest("POST", url, bytes.NewBuffer(data))
	if err != nil {
		pkg.Log.Fatalf("Failed to create request: %v", err)
	}

	req.Header.Set("Content-Type", "application/x-protobuf")

	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		pkg.Log.Fatalf("Failed to send request: %v", err)
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		pkg.Log.Fatalf("Bad response: %v", resp.Status)
	}
	pkg.Log.Println("Request sent successfully!")
}

func upCheckLog(request string, resp string, status uint32) {
	upLogs(strconv.Itoa(1), request, resp, status)
}
func upVerifyLog(request string, resp string, status uint32) {
	upLogs(strconv.Itoa(2), request, resp, status)
}
func upLogs(upType string, requestContent string, responseContent string, status uint32) {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Error("上传日志失败", r)
		}
	}()
	apiLogRequest := &emcs.ApiLogRequest{
		DeviceId:        emcsConf.EquipmentNo,
		Timestamp:       strconv.FormatInt(time.Now().Unix(), 10),
		AppVersion:      config.Version,
		Type:            upType,
		Status:          status,
		RequestUrl:      emcsConf.CheckUrl,
		RequestContent:  requestContent,
		ResponseContent: responseContent,
	}

	data, err := proto.Marshal(apiLogRequest)
	if err != nil {
		pkg.Log.Fatalf("Failed to marshal example: %v", err)
	}

	url := "http://emcs-api.youchiyun.com"
	req, err := http.NewRequest("POST", url, bytes.NewBuffer(data))
	if err != nil {
		pkg.Log.Fatalf("Failed to create request: %v", err)
	}

	req.Header.Set("Content-Type", "application/x-protobuf")

	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		pkg.Log.Fatalf("Failed to send request: %v", err)
	}
	defer resp.Body.Close()

	// 检查响应状态
	if resp.StatusCode != http.StatusOK {
		pkg.Log.Fatalf("Bad response: %v", resp.Status)
	}
	pkg.Log.Println("Request sent successfully!")
}
