package internal

import (
	"cargo/pkg"
	"fmt"
	"os"
	"strconv"
	"time"
)

func SaveCount() {
	// 获取当前日期
	currentDate := time.Now().Format("2006-01-02")

	// 构建文件路径
	filePath := fmt.Sprintf("total/%s.txt", currentDate)

	// 读取文件内容
	content, err := os.ReadFile(filePath)
	if err != nil {
		// 如果文件不存在，则创建新文件
		content = []byte("0")
	}

	// 将内容转换为数字
	num, err := strconv.Atoi(string(content))
	if err != nil {
		pkg.Log.Println("Error converting content to integer:", err)
		return
	}

	// 增加数字
	num++

	// 将数字转换回字符串并写入文件
	newContent := []byte(strconv.Itoa(num))
	err = os.WriteFile(filePath, newContent, 0644)
	if err != nil {
		pkg.Log.Println("Error writing to file:", err)
		return
	}

	pkg.Log.Println("Number written to file:", num)
}
func ReadCount() int {
	// 获取当前日期
	currentDate := time.Now().Format("2006-01-02")

	// 构建文件路径
	filePath := fmt.Sprintf("total/%s.txt", currentDate)

	// 读取文件内容
	content, err := os.ReadFile(filePath)
	if err != nil {
		// 如果文件不存在，则创建新文件
		content = []byte("0")
	}

	// 将内容转换为数字
	num, err := strconv.Atoi(string(content))
	if err != nil {
		pkg.Log.Println("Error converting content to integer:", err)
		return 0
	}

	// 增加数字
	return num
}
