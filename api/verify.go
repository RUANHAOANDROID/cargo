package api

import (
	"bytes"
	"cargo/pkg"
	"encoding/json"
	"net/http"
)

// VerifyTicket 核销票据
func VerifyTicket(protocolNo string, ctr *CheckResponse) error {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Println("VerifyTicket panic:", r)
		}
	}()
	vrt := VerifyRequest{
		CorpId:     emcsConf.ManufacturerId1,
		CorpId2:    emcsConf.ManufacturerId2,
		ProtocolNo: protocolNo,
		StrTESn:    Authenticator(emcsConf.Yccode, emcsConf.EquipmentNo),
	}
	vrt.Data.CientTransNo = ctr.CientTransNo
	vrt.Data.SpotId = ctr.SpotId
	vrt.Data.ResortId = ctr.ResortId
	vrt.Data.TicketNo = ctr.TicketNo
	vrt.Data.TicketType = ctr.TicketType
	vrt.Data.ClientType = "006"
	vrt.Data.MergeTicketNo = ctr.MergeTicketNo
	vrt.Data.MerNotVerTktNum = ctr.MerNotVerTktNum
	clt := &http.Client{}
	requestBody, err := json.Marshal(vrt)
	if err != nil {
		pkg.Log.Error(err)
		return err
	}
	pkg.Log.Printf("verify request =%q\n", requestBody)
	resp, err := clt.Post(emcsConf.CheckUrl+pathVerifyTicket, contentType, bytes.NewBuffer(requestBody))
	if err != nil {
		pkg.Log.Error(err)
		return err
	}
	defer resp.Body.Close()
	var verifyResp VerifyResponse
	status := 2
	if resp.StatusCode == 200 {
		status = 1
		err = json.NewDecoder(resp.Body).Decode(&verifyResp)
		if err != nil {
			return err
		}
	}
	pkg.Log.Printf("verify response=%q\n", verifyResp)
	jsonResp, err := json.Marshal(verifyResp)
	if err != nil {
		pkg.Log.Error(err)
	}
	upVerifyLog(string(requestBody), string(jsonResp), uint32(status))
	return err
}

type VerifyResponse struct {
	CientTransNo string `json:"cientTransNo"`
	ErrCode      string `json:"errCode"`
	RetCode      string `json:"retCode"`
	RetMsg       string `json:"retMsg"`
	TicketNo     string `json:"ticketNo"`
}
type VerifyRequest struct {
	CorpId  string `json:"corpId"`
	CorpId2 string `json:"corpId2"`
	Data    struct {
		CientTransNo    string      `json:"cientTransNo"`
		ClientType      string      `json:"clientType"`
		EnterType       interface{} `json:"enterType"`
		IDNO            interface{} `json:"ID_NO"`
		InOutDirect     string      `json:"inOutDirect"`
		MemAccno        interface{} `json:"memAccno"`
		MerNotVerTktNum interface{} `json:"merNotVerTktNum"`
		MergeTicketNo   interface{} `json:"mergeTicketNo"`
		NotVerTktNum    interface{} `json:"notVerTktNum"`
		PackageTicketNo interface{} `json:"packageTicketNo"`
		PckNotVerTktNum interface{} `json:"pckNotVerTktNum"`
		ResortId        string      `json:"resortId"`
		SpotId          string      `json:"spotId"`
		TicketNo        string      `json:"ticketNo"`
		TicketType      string      `json:"ticketType"`
		TradeNo         interface{} `json:"tradeNo"`
	} `json:"DATA"`
	ProtocolNo string `json:"protocolNo"`
	StrTESn    string `json:"strTESn"`
	TimeStamp  string `json:"timeStamp"`
	Version    string `json:"version"`
}
