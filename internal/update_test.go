package internal

import (
	"cargo/config"
	"fmt"
	"testing"
)

func TestCheckUpdate(t *testing.T) {
	conf, err := config.Load("./config.yml")
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(conf)
}
