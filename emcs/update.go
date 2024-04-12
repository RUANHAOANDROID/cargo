package emcs

import (
	"bytes"
	"cargo/config"
	"cargo/pkg"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"os/exec"
	"strconv"
	"strings"
)

// VersionResponse 定义响应数据的结构体
type VersionResponse struct {
	Code int    `json:"code"`
	Msg  string `json:"msg"`
	Data struct {
		DeviceType  string `json:"deviceType"`
		DownloadUrl string `json:"downloadUrl"`
		DeviceID    string `json:"deviceId"`
		ProjectID   string `json:"projectId"`
		VersionCode string `json:"versionCode"`
		IsForced    int    `json:"isForced"`
	} `json:"data"`
}

func CheckUpdate(conf *config.Config, eqp string) {
	pkg.Log.Printf("current version%s", conf.Version)
	pkg.Log.Printf("Device No%s", eqp)
	pkg.Log.Printf("DeviceType No%s", conf.DeviceType)
	// 定义请求体结构
	requestBody := map[string]interface{}{
		"data": map[string]string{
			"deviceId":   eqp,
			"deviceType": conf.DeviceType,
		},
		"sign":      "770BA7AC5DB7E20B652B2540B18BE001",
		"timestamp": 0,
	}

	// 将请求体转换为JSON格式
	requestBodyBytes, err := json.Marshal(requestBody)
	if err != nil {
		fmt.Println("Error marshalling request body:", err)
		return
	}

	// 发起POST请求
	resp, err := http.Post("http://emcs-api.youchiyun.com/gateMachine/getVersion", "application/json", bytes.NewBuffer(requestBodyBytes))
	if err != nil {
		fmt.Println("Error sending POST request:", err)
		return
	}
	defer resp.Body.Close()

	// 读取响应体
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("Error reading response body:", err)
		return
	}
	// 打印响应体
	fmt.Println(string(body))
	// 解析JSON数据到结构体中
	var vr VersionResponse
	err = json.Unmarshal(body, &vr)
	if err != nil {
		fmt.Println("Error parsing JSON:", err)
		return
	}
	if vr.Code == 0 {
		targetVersionStr := vr.Data.VersionCode
		// 去除版本号中的"v"和点号
		targetVersionStr = strings.TrimPrefix(targetVersionStr, "v")
		targetVersionStr = strings.ReplaceAll(targetVersionStr, ".", "")
		targetVersion, err := strconv.ParseInt(targetVersionStr, 10, 64)
		if err != nil {
			fmt.Println("Error:", err)
			return
		}
		currentVersion := strings.TrimPrefix(conf.Version, "v")
		currentVersion = strings.ReplaceAll(currentVersion, ".", "")
		version, err := strconv.ParseInt(currentVersion, 10, 64)
		if err != nil {
			fmt.Println("Error:", err)
			return
		}
		pkg.Log.Println("Integer:", targetVersion)
		pkg.Log.Println("Integer:", version)
		currentVersion = strings.ReplaceAll(currentVersion, ".", "")
		if currentVersion > vr.Data.VersionCode {
			pkg.Log.Println("A new version is available")
			pkg.Log.Println(vr.Data.DownloadUrl)
			pkg.Log.Println("Start Download")
			tarPath, err := downLoad(vr.Data.DownloadUrl)
			if err != nil {
				pkg.Log.Println(err)
			}
			err = extractTar("", tarPath)
			if err != nil {
				pkg.Log.Println(err)
			}
			reboot()
		}
	}
}
func downLoad(url string) (string, error) {
	// 目标文件路径
	filePath := "car.tar"
	// 发起HTTP GET请求
	response, err := http.Get(url)
	if err != nil {
		fmt.Println("Error:", err)
		return "", err
	}
	defer response.Body.Close()
	// 创建文件
	file, err := os.Create(filePath)
	if err != nil {
		fmt.Println("Error:", err)
		return "", err
	}
	defer file.Close()

	// 将响应体写入文件
	_, err = io.Copy(file, response.Body)
	if err != nil {
		fmt.Println("Error:", err)
		return "", err
	}
	fmt.Println("File downloaded successfully!")
	return filePath, nil
}
func extractTar(targetDir string, tarFile string) error {
	source, err := os.Open(tarFile)
	if err != nil {
		return err
	}
	defer source.Close()
	err = pkg.UnTar(targetDir, tarFile, false)
	if err != nil {
		pkg.Log.Println("Extract Error! ")
		pkg.Log.Println(err)
		return err
	}
	pkg.Log.Println("Extract End!")
	return nil
}
func reboot() {
	// 调用Linux系统的重启命令
	cmd := exec.Command("reboot")

	// 执行命令
	err := cmd.Run()
	if err != nil {
		fmt.Println("Error rebooting system:", err)
		return
	}
	fmt.Println("System is rebooting...")
}
