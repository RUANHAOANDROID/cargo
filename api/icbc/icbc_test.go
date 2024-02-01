package icbc

import (
	"cargo/config"
	"fmt"
	"path/filepath"
	"testing"
)

func loadConfig(t *testing.T) error {
	configFile := filepath.Join("..", "..", "config.yml")
	conf, err := config.Load(configFile)
	if err != nil {
		t.Error(err)
	}
	SetConfig(conf)
	return err
}
func TestIDTicket(t *testing.T) {
	err := loadConfig(t)
	resp, err := CheckTicket("420325199211114511", ProtoID)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	fmt.Println(resp)
}

func TestQRTicket(t *testing.T) {
	err := loadConfig(t)
	resp, err := CheckTicket("15943090529708<MjAwMDAwMTkyNDIwMjQtMDItMDEyMDI0LTAzLTAx>", ProtoQr)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	fmt.Println(resp)
}
func TestICTicket(t *testing.T) {
	err := loadConfig(t)
	resp, err := CheckTicket("0123456789", ProtoQr)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	fmt.Println(resp)
}
