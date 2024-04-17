package internal

import (
	"os"
	"strconv"
	"time"
)

func today() string {
	return time.Now().Format("20060102")
}
func todayFileName() string {
	return "data/" + today() + ".txt"
}

// ReadPassedCount 获取当前日期文件的数值
func ReadPassedCount() (int, error) {
	// 如果文件不存在，则创建文件
	if _, err := os.Stat(todayFileName()); os.IsNotExist(err) {
		f, err := os.Create(todayFileName())
		if err != nil {
			return 0, err
		}
		f.Close()
	}
	// 读取文件内容
	content, err := os.ReadFile(todayFileName())
	if err != nil {
		return 0, err
	}

	// 将文件内容转换为整数
	number, err := strconv.Atoi(string(content))
	if err != nil {
		return 0, err
	}

	return number, nil
}

// WritePassedCount 写入当前日期文件的数值
func WritePassedCount() (int, error) {
	number, err := ReadPassedCount()
	if err != nil {
		return 0, err
	}
	number += 1
	// 将整数转换为字符串
	content := strconv.Itoa(number)
	// 写入文件内容
	err = os.WriteFile(todayFileName(), []byte(content), 0644)
	return number, err
}
