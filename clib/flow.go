package clib

import "C"
import (
	"cargo/msg"
	"cargo/pkg"
	"fmt"
	"math/big"
	"net"
	"strings"
)

const bufferSize = 512 // 缓冲区大小

var chanMsg chan msg.Message

// byteArrayToDecimal 将字节数组转换为十进制数
func byteArrayToDecimal(bytes []byte) int {
	result := 0
	for i := 0; i < len(bytes); i++ {
		result = result*256 + int(bytes[i])
	}
	return result
}
func bytesToHexString(bytes []byte) string {
	var hexString string
	for _, b := range bytes {
		hexString += fmt.Sprintf("%02X", b)
	}
	return hexString
}

func StartTcpServer(cm chan msg.Message) {
	chanMsg = cm
	listen, err := net.Listen("tcp", "0.0.0.0:9999")
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
func process(conn net.Conn) {
	defer conn.Close() // 关闭连接
	buffer := make([]byte, bufferSize)
	for {
		// 读取数据包
		bytesRead, err := conn.Read(buffer)
		if err != nil {
			fmt.Println("Error reading data:", err)
			return
		}
		if bytesRead > 0 {
			pkg.Log.Printf("buffer len=%v ", len(buffer))
			if buffer[1] != 0 {
				trimmedBuffer := trimTrailingZeros(buffer[:bytesRead])
				pkg.Log.Printf("trimmed buffer len=%v buffer=%v\n", len(trimmedBuffer), trimmedBuffer)
				types := int(trimmedBuffer[0])
				pkg.Log.Println("ticket type", types)
				switch types {
				case msg.IC_CARD:
					content := bytesToHexString(trimmedBuffer[1:])
					content, err = swapHexOrder(content)
					if err != nil {
						fmt.Println("Error swapping hex order:", err)
						return
					}
					content, err = hexToDecimal(content)
					if err != nil {
						fmt.Println("Error converting hex to decimal:", err)
						return
					}
					chanMsg <- msg.Message{Type: msg.IC_CARD, Content: content}
				case msg.QRCODE:
					content := string(trimmedBuffer[1:])
					chanMsg <- msg.Message{Type: msg.QRCODE, Content: content}

				case msg.ID_CARD:
					content := bytesToHexString(trimmedBuffer[1:])
					chanMsg <- msg.Message{Type: msg.ID_CARD, Content: content}
				default:
					fmt.Println("undefined type")
				}
			} else {
				pkg.Log.Printf("buffer len=%v ", buffer)
			}

		} else {
			pkg.Log.Println("bytesRead <=0 or buffer is null")
		}
	}
}
func swapHexOrder(hexString string) (string, error) {
	swappedHex := strings.Builder{}
	for i := 6; i >= 0; i -= 2 {
		swappedHex.WriteString(hexString[i : i+2])
	}
	return swappedHex.String(), nil
}
func hexToDecimal(hexString string) (string, error) {
	hexInt, success := new(big.Int).SetString(hexString, 16)
	if !success {
		return "", fmt.Errorf("无法将十六进制字符串 %s 转换为大整数", hexString)
	}
	// 将大整数转换为十进制字符串
	decimalStr := hexInt.String()

	return decimalStr, nil
}

// trimTrailingZeros 去除字节切片后面的0
func trimTrailingZeros(data []byte) []byte {
	for i := len(data) - 1; i >= 0; i-- {
		if data[i] != 0 {
			return data[:i+1]
		}
	}
	return []byte{}
}
