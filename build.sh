#!/bin/bash
rm -f cargo
export CGO_ENABLED=1
export GOARCH=arm
export GOARM=7
export CC=arm-linux-gnueabihf-gcc
# 构建 Go 项目
go build

scp /home/hao/Desktop/workspace/cargo/cargo root@192.168.8.137:/usr/app/cargo
ssh root@192.168.8.137 'cd /usr/app/ && exec bash -l'


