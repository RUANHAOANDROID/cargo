#!/bin/bash
rm -f cargo
export CGO_ENABLED=1
export GOARCH=arm
export GOARM=7
export CC=arm-linux-gnueabihf-gcc
export GODEBUG=cgocheck=2

# 构建 Go 项目
go build
#列出
pwd
ls -l
scp /drone/src/cargo root@192.168.8.39:/usr/app/cargo



