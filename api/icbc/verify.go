package icbc

type VerifyRequest struct {
	CorpId     string     `json:"corpId"`
	CorpId2    string     `json:"corpId2"`
	Data       VerifyData `json:"DATA"`
	ProtocolNo string     `json:"protocolNo"`
	StrTESn    string     `json:"strTESn"`
	TimeStamp  string     `json:"timeStamp"`
	Version    string     `json:"version"`
	ResortId   string     `json:"resortId"`
}
type VerifyData struct {
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
}

type VerifyResponse struct {
	CientTransNo string `json:"cientTransNo"`
	ErrCode      string `json:"errCode"`
	RetCode      string `json:"retCode"`
	RetMsg       string `json:"retMsg"`
	TicketNo     string `json:"ticketNo"`
}
