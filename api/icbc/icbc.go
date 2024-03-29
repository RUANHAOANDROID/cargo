package icbc

import (
	"cargo/config"
	"fmt"
	"math/rand"
	"strings"
	"time"
)

var contentType = "application/json; charset=utf-8"
var pathCheckTicket = "/ticket/checkTicket"
var pathVerifyTicket = "/ticket/verifyTicket"
var conf *config.Config

func SetConfig(config *config.Config) {
	conf = config
}

const (
	ProtoQr    = "002" //二维码
	ProtoID    = "003" //身份证
	ProtoIC    = "004" //景区IC
	ProtoICAll = "009" //雪场雪卡+IC
)

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
