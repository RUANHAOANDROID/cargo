package speaker

import (
	"cargo/decaros"
	"cargo/pkg"
	"fmt"
	"strconv"
	"strings"
	"time"
)

func Speaker(msg string, success bool) {
	pkg.Log.Println(msg, success)
	if success {
		decaros.APlay("A")
	} else {
		if strings.Contains(msg, "已验票") {
			decaros.APlay("C")
			return
		}
		if strings.Contains(msg, "权限") {
			decaros.APlay("Z")
			return
		}
		decaros.APlay("B")
	}
}

func SpeakerGroup(number string) {
	num, err := strconv.Atoi(number)
	if err != nil {
		pkg.Log.Println("转换错误:", err)
	} else {
		pkg.Log.Println("整数值:", num)
	}
	if num < 10 {
		decaros.APlay("tp")
		time.Sleep(300 * time.Millisecond)
		decaros.APlay(fmt.Sprintf("%d", num))
	}
	if num > 10 && num < 100 {
		tens := num / 10  // 取十位
		units := num % 10 // 取个位
		decaros.APlay("tdp")
		time.Sleep(1400 * time.Millisecond)
		decaros.APlay(fmt.Sprintf("%d", tens))
		time.Sleep(400 * time.Millisecond)
		decaros.APlay("10")
		time.Sleep(500 * time.Millisecond)
		decaros.APlay(fmt.Sprintf("%d", units))
		time.Sleep(400 * time.Millisecond)
		decaros.APlay("ren")
	}
}
