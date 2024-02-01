package icbc

import (
	"bytes"
	"cargo/pkg"
	"encoding/json"
	"fmt"
	"log"
	"math/rand"
	"net/http"
	"time"
)

var contentType = "application/json; charset=utf-8"
var pathCheckTicket = "/ticket/checkTicket"
var pathVerifyTicket = "/ticket/verifyTicket"

// CheckTicket 模拟发起HTTP请求 protocolNo 类型
func CheckTicket(ticket string, protocolNo string) (CheckResponse, error) {
	localData := time.Now().Local()
	data := CheckData{
		ClientType:   "006",
		CientTransNo: protocolNo + localData.Format("20060102150405") + random3(),
		UpData:       ticket,
		UpDataLength: string(len(ticket)),
	}
	requestEntity := CheckRequest{
		Data:         data,
		CorpId:       conf.Icbc.CorpId,
		CorpId2:      conf.Icbc.CorpId2,
		StrTESn:      Authenticator("yccode", conf.Uchi.EqpCode),
		Version:      "1",
		PrintControl: "0",
		TimeStamp:    pkg.Fmt2HMS(localData),
		ProtocolNo:   protocolNo,
		SystemType:   "1",
	}

	requestBody, err := json.Marshal(requestEntity)
	if err != nil {
		log.Fatal(err)
	}
	clt := http.Client{}
	resp, err := clt.Post(conf.Icbc.CheckUrl+pathCheckTicket, contentType, bytes.NewBuffer(requestBody))
	pkg.Log.Error(resp)
	if err != nil {
		pkg.Log.Error(err)
		pkg.Log.Error(resp)
	}
	//var res map[string]interface{}
	var checkResponse CheckResponse
	err = json.NewDecoder(resp.Body).Decode(&checkResponse)
	pkg.Log.Info(checkResponse)
	go func() {
		verifyRequest := VerifyRequest{
			CorpId:     conf.Icbc.CorpId,
			CorpId2:    conf.Icbc.CorpId2,
			ProtocolNo: protocolNo,
			StrTESn:    Authenticator("yccode", conf.Uchi.EqpCode),
		}
		VerifyTicket(verifyRequest)
	}()
	return checkResponse, err
}

// VerifyTicket 核销票据
func VerifyTicket(request VerifyRequest) (VerifyResponse, error) {
	clt := http.Client{}
	jsonRequest, err := json.Marshal(request)
	if err != nil {
		log.Fatal(err)
	}
	resp, err := clt.Post(conf.Icbc.VerifyUrl+pathVerifyTicket, contentType, bytes.NewBuffer(jsonRequest))
	pkg.Log.Info(resp)
	var verifyResp VerifyResponse
	if resp.StatusCode != 200 {
		verifyResp.RetCode = resp.Status
		verifyResp.RetMsg = resp.Status
	} else {
		err = json.NewDecoder(resp.Body).Decode(&verifyResp)
	}
	pkg.Log.Info(verifyResp)
	return verifyResp, err
}
func Authenticator(ycCode string, deviceId string) string {
	//return ycCode + deviceId
	return "BCSSHecsun0001E" + deviceId
}

/**
 * 随机三位数数
 */
func random3() string {
	random := rand.New(rand.NewSource(time.Now().UnixNano())).Int31n(100)
	return fmt.Sprintf("%03v", random)
}
