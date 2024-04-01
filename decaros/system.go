package decaros

import (
	"bytes"
	"os/exec"
)

func GetSN() (string, error) {
	// 执行 Linux 命令
	cmd := exec.Command("cat", "/opt/config/sn")
	// 捕获命令输出
	var out bytes.Buffer
	cmd.Stdout = &out
	// 执行命令
	err := cmd.Run()
	// 输出 SN 码
	sn := out.String()
	return sn, err
}
