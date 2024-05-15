package icbc

import (
	"bytes"
	"cargo/pkg"
	"encoding/json"
	"fmt"
	"net/http"
	"time"
)

// CheckTicket 模拟发起HTTP请求 protocolNo 类型
func CheckTicket(ticket string, protocolNo string) (*CheckResponse, error) {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Println("CheckTicket panic:", r)
		}
	}()
	pkg.Log.Printf("CheckTicket: protocolNo=%v,ticket =%v", protocolNo, ticket)
	localData := time.Now().Local()
	ticket = removeNullCharacters(ticket)
	time := localData.Format("20060102150405")
	data := CheckData{
		ClientType:   "006",
		CientTransNo: protocolNo + time + random3(),
		UpData:       ticket,
		UpDataLength: fmt.Sprint(len(ticket)),
	}
	requestEntity := CheckRequest{
		Data:         data,
		CorpId:       emcsConf.ManufacturerId1,
		CorpId2:      emcsConf.ManufacturerId1,
		StrTESn:      Authenticator(emcsConf.Yccode, emcsConf.EquipmentNo),
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
	pkg.Log.Println(emcsConf.ManufacturerId1)
	pkg.Log.Printf("request=%q\n", requestBody)
	clt := http.Client{}
	resp, err := clt.Post(emcsConf.CheckUrl+pathCheckTicket, contentType, bytes.NewBuffer(requestBody))
	if err != nil {
		pkg.Log.Error(err)
		return nil, err
	}
	defer resp.Body.Close()
	//var res map[string]interface{}
	var checkResponse CheckResponse
	err = json.NewDecoder(resp.Body).Decode(&checkResponse)
	if err != nil {
		pkg.Log.Error(err)
		return nil, err
	}
	pkg.Log.Printf("resp code=%s,msg=%s,resortId=%s\n", checkResponse.RetCode, checkResponse.RetMsg, checkResponse.ResortId)
	if checkResponse.RetCode == "0" {
		pkg.Log.Println("check ticket success! verify ticket")
		VerifyTicket(protocolNo, &checkResponse)
	}
	return &checkResponse, err
}

type CheckRequest struct {
	Data         CheckData `json:"DATA"`
	CorpId       string    `json:"corpId"`
	CorpId2      string    `json:"corpId2"`
	PrintControl string    `json:"printControl"`
	ProtocolNo   string    `json:"protocolNo"`
	StrTESn      string    `json:"strTESn"`
	SystemType   string    `json:"systemType"`
	TimeStamp    string    `json:"timeStamp"`
	Version      string    `json:"version"`
}
type CheckData struct {
	CientTransNo string `json:"cientTransNo"`
	ClientType   string `json:"clientType"`
	UpData       string `json:"upData"`
	UpDataLength string `json:"upDataLength"`
}
type CheckResponse struct {
	IDNO              interface{} `json:"ID_NO"`
	NotVerTktNum      interface{} `json:"notVerTktNum"`
	CientTransNo      string      `json:"cientTransNo"`
	RetCode           string      `json:"retCode"`
	ListNum           interface{} `json:"listNum"`
	TicketNo          string      `json:"ticketNo"`
	MemAccno          interface{} `json:"memAccno"`
	MergeTicketNoList []string    `json:"mergeTicketNoList"`
	MergeTicketNo     string      `json:"mergeTicketNo"`
	CorpId            string      `json:"corpId"`
	PrintInfo         interface{} `json:"printInfo"`
	TradeNo           interface{} `json:"tradeNo"`
	ResortId          string      `json:"resortId"`
	SpotId            string      `json:"spotId"`
	PackageTicketNo   interface{} `json:"packageTicketNo"`
	EnterType         interface{} `json:"enterType"`
	TicketType        string      `json:"ticketType"`
	TicketInfoList    interface{} `json:"ticketInfoList"`
	RetMsg            string      `json:"retMsg"`
	MerNotVerTktNum   string      `json:"merNotVerTktNum"`
	PckNotVerTktNum   interface{} `json:"pckNotVerTktNum"`
	ErrCode           string      `json:"errCode"`
	TicketNoList      interface{} `json:"ticketNoList"`
	TicketName        string      `json:"ticketName"`
}
