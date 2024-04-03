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
		display.ShowTitleArea("验票成功")
		display.ShowContentArea(msg)
	} else {
		display.ShowTitleArea("验票失败")
		if strings.Contains(msg, "已验票") {
			display.ShowContentArea("已验票")
			return
		}
		if strings.Contains(msg, "权限") {
			display.ShowContentArea("无权限访问")
			return
		}
		display.ShowContentArea(msg)
	}
}
