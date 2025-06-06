package internal

import (
	"bytes"
	"cargo/pkg"
	"encoding/json"
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

func CheckUpdate(dType string, version string, eqp string) {
	defer func() {
		if r := recover(); r != nil {
			pkg.Log.Error("Update panic:", r)
		}
	}()
	pkg.Log.Printf("Current Version -> %s", version)
	pkg.Log.Printf("Device No-> %s", eqp)
	pkg.Log.Printf("DeviceType-> %s", dType)
	// 定义请求体结构
	requestBody := map[string]interface{}{
		"data": map[string]string{
			"deviceId":   eqp,
			"deviceType": dType,
		},
		"sign":      "770BA7AC5DB7E20B652B2540B18BE001",
		"timestamp": 0,
	}

	// 将请求体转换为JSON格式
	requestBodyBytes, err := json.Marshal(requestBody)
	if err != nil {
		pkg.Log.Println("Error marshalling request body:", err)
		return
	}

	// 发起POST请求
	resp, err := http.Post("http://emcs-api.youchiyun.com/gateMachine/getVersion", "application/json", bytes.NewBuffer(requestBodyBytes))
	if err != nil {
		pkg.Log.Println("Error sending POST request:", err)
		return
	}
	defer resp.Body.Close()

	// 读取响应体
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		pkg.Log.Println("Error reading response body:", err)
		return
	}
	// 打印响应体
	pkg.Log.Println(string(body))
	// 解析JSON数据到结构体中
	var vr VersionResponse
	err = json.Unmarshal(body, &vr)
	if err != nil {
		pkg.Log.Println("Error parsing JSON:", err)
		return
	}
	if vr.Code == 0 {
		targetVersionStr := vr.Data.VersionCode
		// 去除版本号中的"v"和点号
		targetVersionStr = strings.TrimPrefix(targetVersionStr, "v")
		targetVersionStr = strings.ReplaceAll(targetVersionStr, ".", "")
		targetVersionInt, err := strconv.ParseInt(targetVersionStr, 10, 64)
		if err != nil {
			pkg.Log.Println("Error:", err)
			return
		}
		currentVersion := strings.TrimPrefix(version, "v")
		currentVersion = strings.ReplaceAll(currentVersion, ".", "")
		versionInt, err := strconv.ParseInt(currentVersion, 10, 64)
		if err != nil {
			pkg.Log.Println("Error:", err)
			return
		}
		pkg.Log.Println("Target version:", targetVersionInt)
		pkg.Log.Println("Current version:", versionInt)
		currentVersion = strings.ReplaceAll(currentVersion, ".", "")
		if versionInt < targetVersionInt {
			pkg.Log.Println("A new version is available")
			pkg.Log.Println(vr.Data.DownloadUrl)
			pkg.Log.Println("Start Download")
			tarPath, err := downLoad(vr.Data.DownloadUrl)
			if err != nil {
				pkg.Log.Println(err)
			}
			pkg.Log.Println(tarPath)
			runUpgradeShell()
			////err = extractTar("", tarPath)
			//tarxvf(tarPath)
			//if err != nil {
			//	pkg.Log.Println(err)
			//}
			//reboot()
		}
	}
}
func downLoad(url string) (string, error) {
	// 目标文件路径
	filePath := "cargo.tar"
	// 发起HTTP GET请求
	response, err := http.Get(url)
	if err != nil {
		pkg.Log.Println("Error:", err)
		return "", err
	}
	defer response.Body.Close()
	// 创建文件
	file, err := os.Create(filePath)
	if err != nil {
		pkg.Log.Println("Error:", err)
		return "", err
	}
	defer file.Close()

	// 将响应体写入文件
	_, err = io.Copy(file, response.Body)
	if err != nil {
		pkg.Log.Println("Error:", err)
		return "", err
	}
	pkg.Log.Println("File downloaded successfully!")
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
func tarxvf(tarFile string) {
	// 调用Linux系统的重启命令
	cmd := exec.Command("tar", "-xvf", tarFile)

	// 执行命令
	err := cmd.Run()
	if err != nil {
		pkg.Log.Println("Error rebooting system:", err)
		return
	}
	pkg.Log.Println("System is rebooting...")
}

func reboot() {
	// 调用Linux系统的重启命令
	cmd := exec.Command("reboot")

	// 执行命令
	err := cmd.Run()
	if err != nil {
		pkg.Log.Println("Error rebooting system:", err)
		return
	}
	pkg.Log.Println("System is rebooting...")
}
func runUpgradeShell() {
	// 创建一个cmd对象
	cmd := exec.Command("chmod", "+x", "upgrade.sh")

	// 执行命令
	err := cmd.Run()
	if err != nil {
		pkg.Log.Println("Error changing file permission:", err)
		return
	}

	pkg.Log.Println("File permission changed successfully!")

	// 创建一个cmd对象
	cmd = exec.Command("./upgrade.sh") // 脚本文件名为 script.sh

	// 设置工作目录，如果脚本中使用了相对路径，可以设置工作目录
	// cmd.Dir = "/path/to/directory"

	// 执行命令
	err = cmd.Run()
	if err != nil {
		pkg.Log.Println("Error executing script:", err)
		return
	}

	pkg.Log.Println("Script executed successfully!")
}
