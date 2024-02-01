package clib

import "C"
import (
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"net"
)

const bufferSize = 1024 // 1MB 缓冲区大小

func process(conn net.Conn) {
	display := Display{}
	defer conn.Close() // 关闭连接
	buffer := make([]byte, bufferSize)
	for {
		// 读取数据包
		bytesRead, err := conn.Read(buffer)
		if err != nil {
			fmt.Println("Error reading data:", err)
			return
		}
		if bytesRead > 0 && buffer[1] != 0 {
			types := string(buffer[:1])
			content := string(buffer[1:])
			pkg.Log.Printf("buffer len=%v type=%v,data=%v\n", bytesRead, types, content)
			display.LCDRow(content, 8, 40, DISP_FONT12)
			pkg.APlay(pkg.SoundFiles[9])
			//chanMsg <- msg.Message{Type: int(packet.Type), Content: packetContent}
			//conn.Write([]byte(recvStr)) // 发送数据
		} else {
			pkg.Log.Println("bytesRead <=0 or buffer is null")
		}
	}
}

var chanMsg *chan msg.Message

func StartTcpServer(cm *chan msg.Message) {
	chanMsg = cm
	listen, err := net.Listen("tcp", "127.0.0.1:9999")
	defer listen.Close()
	if err != nil {
		fmt.Println("Listen() failed, err: ", err)
		return
	}
	for {
		conn, err := listen.Accept() // 监听客户端的连接请求
		if err != nil {
			fmt.Println("Accept() failed, err: ", err)
			continue
		}
		go process(conn) // 启动一个goroutine来处理客户端的连接请求
	}
}
