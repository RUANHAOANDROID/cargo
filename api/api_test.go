package api

import (
	"cargo/internal"
	"fmt"
	"testing"
)

func TestIDTicket(t *testing.T) {
	resp, err := CheckTicket("420325199211114511", ProtoID)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	fmt.Println(resp)
}

func TestQRTicket(t *testing.T) {
	emcsUrl := "http://emcs-api.youchiyun.com"
	conf, err := internal.GetConfig(emcsUrl)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	SetConfig(emcsUrl, "a65b71425f8067a7503b48ae366a9e588e1e47e9", conf)
	resp, err := CheckTicket("98820252298378<MjAwMDAwMTkyNDIwMjQtMDYtMDQyMDM0LTA2LTA0>", ProtoQr)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	fmt.Println(resp)
}
func TestICTicket(t *testing.T) {
	resp, err := CheckTicket("0123456789", ProtoQr)
	if err != nil {
		fmt.Println(err.Error())
		t.Error(err)
	}
	fmt.Println(resp)
}
