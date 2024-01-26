package clib

import "C"
import (
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"net"
)

const bufferSize = 1024 * 2 // 1MB 缓冲区大小

func process(conn net.Conn) {

	defer conn.Close() // 关闭连接
	buffer := make([]byte, bufferSize)
	for {
		// 读取数据包
		pkg.Log.Println("--------go tcp read data--------")
		bytesRead, err := conn.Read(buffer)
		if err != nil {
			fmt.Println("Error reading data:", err)
			return
		}
		//pkg.Log.Println("data type=", bytesRead, "content =", bytesRead)
		pkg.Log.Println("data ", bytesRead, "buffer", &buffer)
		pkg.Log.Println(string(buffer[1:]))
		pkg.Log.Println(string(buffer[:1]))
		//chanMsg <- msg.Message{Type: int(packet.Type), Content: packetContent}
		//conn.Write([]byte(recvStr)) // 发送数据
	}
}

var chanMsg chan msg.Message

func StartFlow(cm chan msg.Message) {
	chanMsg = cm
	listen, err := net.Listen("tcp", "127.0.0.1:9999")
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
