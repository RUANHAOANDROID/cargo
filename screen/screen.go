package screen

import (
	"cargo/clib"
	"strings"
)

var display *clib.Display

func Set(dp *clib.Display) {
	display = dp
}
func Show(msg string, success bool) {
	display.ClearScreen()
	if success {
		display.Show("验票成功", msg)
	} else {
		if strings.Contains(msg, "已验票") {
			display.Show("验票失败", "已验票")
			return
		}
		if strings.Contains(msg, "权限") {
			display.Show("验票失败", "无权限访问")
			return
		}
		display.Show("验票失败", msg)
	}
}
