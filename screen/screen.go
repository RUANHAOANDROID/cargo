package screen

import (
	"cargo/clib"
	"fmt"
	"regexp"
	"strings"
)

var display *clib.Display

func Set(dp *clib.Display) {
	display = dp
}
func Show(msg string, success bool) {
	display.ClearScreen()
	if success {
		display.ShowSuccess(convertMsg(msg))
	} else {
		if strings.Contains(msg, "已验票") {
			display.ShowError("-已验票！")
			return
		}
		if strings.Contains(msg, "权限") {
			display.ShowError("-无权限访问！")
			return
		}
		display.ShowError(msg)
	}
}
func convertMsg(msg string) string {
	// 编译正则表达式
	re := regexp.MustCompile(`门票名:(\S+)`)

	// 在字符串中查找匹配项
	match := re.FindStringSubmatch(msg)
	if match != nil {
		fmt.Println("门票名:", match[1])
		return match[1]
	} else {
		fmt.Println("未找到匹配项")
	}
	return msg
}
