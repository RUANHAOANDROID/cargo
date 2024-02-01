package clib

import "C"
import (
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"net"
	"strconv"
)

const bufferSize = 1024 // 1MB 缓冲区大小

var chanMsg *chan msg.Message

// byteArrayToDecimal 将字节数组转换为十进制数
func byteArrayToDecimal(bytes []byte) int {
	result := 0
	for i := 0; i < len(bytes); i++ {
		result = result*256 + int(bytes[i])
	}
	return result
}
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
			pkg.Log.Printf("buffer len=%v buffer=%v\n", bytesRead, buffer)
			types := int(buffer[0])
			pkg.Log.Println(types)
			switch types {
			case 1:
				content := byteArrayToDecimal(buffer[1:])
				cInt := strconv.Itoa(content)
				fmt.Println(cInt)
				//chanMsg <-msg.Message{Type:int(types),Content: content}
				pkg.APlay(pkg.SoundFiles[8])
				display.LCDRow(cInt, 8, 40, DISP_FONT12)
			case 2:
				content := string(buffer[1:])
				fmt.Println(content)
				pkg.APlay(pkg.SoundFiles[9])
			default:
				fmt.Println("undefined type")
			}

			//chanMsg <- msg.Message{Type: int(packet.Type), Content: packetContent}
			//conn.Write([]byte(recvStr)) // 发送数据
		} else {
			pkg.Log.Println("bytesRead <=0 or buffer is null")
		}
	}
}

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
