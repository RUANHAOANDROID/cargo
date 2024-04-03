package speaker

import (
	"cargo/decaros"
	"strings"
)

func Speaker(msg string, success bool) {

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
