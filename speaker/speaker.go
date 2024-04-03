package speaker

import (
	"cargo/decaros"
	"strings"
)

func Speaker(msg string, success bool) {
	if success {
		decaros.APlay("A")
	} else {
		if !strings.HasSuffix(msg, "已验票") {
			decaros.APlay("F")
			return
		}
		if !strings.HasSuffix(msg, "权限") {
			decaros.APlay("Z")
			return
		}
		decaros.APlay("B")
	}
}
