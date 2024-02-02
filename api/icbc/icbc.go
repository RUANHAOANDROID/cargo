package icbc

import (
	"bytes"
	"cargo/pkg"
	"encoding/json"
	"fmt"
	"math/rand"
	"net/http"
	"strings"
	"time"
)

var contentType = "application/json; charset=utf-8"
var pathCheckTicket = "/ticket/checkTicket"
var pathVerifyTicket = "/ticket/verifyTicket"

const (
	ProtoQr    = "002" //二维码
	ProtoID    = "003" //身份证
	ProtoIC    = "004" //景区IC
	ProtoICAll = "009" //雪场雪卡+IC
)

// CheckTicket 模拟发起HTTP请求 protocolNo 类型
func CheckTicket(ticket string, protocolNo string) (CheckResponse, error) {
	pkg.Log.Printf("CheckTicket: protocolNo=%v,ticket =%v", protocolNo, ticket)
	localData := time.Now().Local()
	ticket = removeNullCharacters(ticket)
	data := CheckData{
		ClientType:   "006",
		CientTransNo: protocolNo + localData.Format("20060102150405") + random3(),
		UpData:       ticket,
		UpDataLength: fmt.Sprint(len(ticket)),
	}
	requestEntity := CheckRequest{
		Data:         data,
		CorpId:       conf.Icbc.CorpId,
		CorpId2:      conf.Icbc.CorpId2,
		StrTESn:      Authenticator(conf.Uchi.EqpCode),
		Version:      "01",
		PrintControl: "0",
		TimeStamp:    pkg.Fmt2HMS(localData),
		ProtocolNo:   protocolNo,
		SystemType:   "1",
	}

	requestBody, err := json.Marshal(requestEntity)
	if err != nil {
		pkg.Log.Error(err)
	}
	pkg.Log.Printf("request=%q\n", requestBody)
	clt := http.Client{}
	resp, err := clt.Post(conf.Icbc.CheckUrl+pathCheckTicket, contentType, bytes.NewBuffer(requestBody))
	if err != nil {
		pkg.Log.Error(err)
		pkg.Log.Error(resp)
	}
	defer resp.Body.Close()
	//var res map[string]interface{}
	var checkResponse CheckResponse
	err = json.NewDecoder(resp.Body).Decode(&checkResponse)
	if err != nil {
		pkg.Log.Error(err)
	}
	pkg.Log.Printf("resp code=%s,msg=%s\n", checkResponse.RetCode, checkResponse.RetMsg)
	if checkResponse.RetCode == "0" {
		pkg.Log.Println("check ticket success! verify ticket")
		go func() {
			verifyRequest := VerifyRequest{
				CorpId:     conf.Icbc.CorpId,
				CorpId2:    conf.Icbc.CorpId2,
				ResortId:   checkResponse.ResortId,
				ProtocolNo: protocolNo,
				StrTESn:    Authenticator(conf.Uchi.EqpCode),
			}
			VerifyTicket(verifyRequest)
		}()
	}
	return checkResponse, err
}

// VerifyTicket 核销票据
func VerifyTicket(request VerifyRequest) (VerifyResponse, error) {
	clt := &http.Client{}
	jsonRequest, err := json.Marshal(request)
	if err != nil {
		pkg.Log.Error(err)
	}
	resp, err := clt.Post(conf.Icbc.VerifyUrl+pathVerifyTicket, contentType, bytes.NewBuffer(jsonRequest))
	if err != nil {
		pkg.Log.Error(err)
	}
	defer resp.Body.Close()
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
func Authenticator(deviceId string) string {
	//return ycCode + deviceId
	return "BCSSHecsun0001" + deviceId
}
func removeNullCharacters(s string) string {
	return strings.Replace(s, "\x00", "", -1)
}

/**
 * 随机三位数数
 */
func random3() string {
	random := rand.New(rand.NewSource(time.Now().UnixNano())).Int31n(100)
	return fmt.Sprintf("%03v", random)
}
