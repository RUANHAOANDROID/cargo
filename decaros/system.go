package decaros

import (
	"bytes"
	"cargo/pkg"
	"os/exec"
)

func GetSN() (string, error) {
	return "02211101538", nil //第一部设备
	//return "02211101541", nil //第二部带身份证的
	cmd := exec.Command("cat", "/opt/config/sn")
	var out bytes.Buffer
	cmd.Stdout = &out
	err := cmd.Run()
	sn := out.String()
	return sn, err
}
func SetNTP() {
	// 执行 Linux 命令 02211101501
	cmd := exec.Command("ntpdate", "ntp7.aliyun.com")
	// 捕获命令输出
	var out bytes.Buffer
	cmd.Stdout = &out
	// 执行命令
	err := cmd.Run()
	if err != nil {
		pkg.Log.Println(err)
	}
	// 输出 SN 码
	outText := out.String()
	pkg.Log.Print(outText)
}
