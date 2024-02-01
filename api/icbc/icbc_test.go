package icbc

import (
	"cargo/config"
	"fmt"
	"path/filepath"
	"testing"
)

func TestCheckTicket(t *testing.T) {
	configFile := filepath.Join("..", "..", "config.yml")
	conf, err := config.Load(configFile)
	if err != nil {
		t.Error(err)
	}
	SetConfig(conf)
	resp, err := CheckTicket("420325199211114511", ProtoID)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	fmt.Println(resp)
}
