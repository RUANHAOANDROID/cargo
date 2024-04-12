package emcs

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
	CheckUpdate(conf, "EQP20240401000001")
}
