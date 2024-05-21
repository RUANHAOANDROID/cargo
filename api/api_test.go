package api

import (
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
	resp, err := CheckTicket("86465099669896<MjAwMDAwMTkyNDIwMjQtMDItMDIyMDI0LTAzLTAy>", ProtoQr)
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
