package speaker

import (
	"cargo/decaros"
	"cargo/pkg"
	"strings"
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
