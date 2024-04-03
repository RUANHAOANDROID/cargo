package icbc

import (
	"cargo/config"
	"cargo/emcs"
	"cargo/pkg"
	"fmt"
	"math/rand"
	"strings"
	"time"
)

var contentType = "application/json; charset=utf-8"
var pathCheckTicket = "/ticket/checkTicket"
var pathVerifyTicket = "/ticket/verifyTicket"
var conf *config.Config
var emcsConf *emcs.Config

func SetConfig(config *config.Config, emcsConfig *emcs.Config) {
	conf = config
	emcsConf = emcsConfig
	pkg.Log.Printf("version=%s,url=%s,sha=%s", config.Version, config.ServerUrl, config.Sha)
	pkg.Log.Printf("url=%s,eqp=%s,ycode=%s", emcsConf.CheckUrl, emcsConf.EquipmentNo, emcsConf.Yccode)
}

const (
	ProtoQr    = "002" //二维码
	ProtoID    = "003" //身份证
	ProtoIC    = "004" //景区IC
	ProtoICAll = "009" //雪场雪卡+IC
)

func Authenticator(uchiCode, deviceId string) string {
	//return ycCode + deviceId
	return uchiCode + deviceId
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
