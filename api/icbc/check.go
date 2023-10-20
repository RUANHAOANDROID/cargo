package icbc

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
