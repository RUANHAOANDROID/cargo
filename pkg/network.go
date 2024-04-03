package pkg

import (
	"fmt"
	"net"
)

func NetAvailable() bool {
	conn, err := net.Dial("tcp", "223.6.6.6:80")
	if err != nil {
		Log.Println("无法连接到网络:", err)
		return false
	}
	defer conn.Close()
	Log.Println("网络连接正常")
	return true
}
func IPV4() string {
	interfaces, err := net.Interfaces()
	if err != nil {
		fmt.Println("无法获取网络接口列表:", err)
		return ""
	}

	for _, iface := range interfaces {
		addrs, err := iface.Addrs()
		if err != nil {
			fmt.Println("无法获取网络地址:", err)
			continue
		}
		for _, addr := range addrs {
			// 检查是否是IP地址
			if ipnet, ok := addr.(*net.IPNet); ok && !ipnet.IP.IsLoopback() {
				// 检查IP地址的类型，只输出IPv4地址
				if ipnet.IP.To4() != nil {
					Log.Println("IPv4地址:", ipnet.IP.String())
					return ipnet.IP.String()
				}
			}
		}
	}
	return ""
}
