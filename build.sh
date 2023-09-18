#!/bin/bash
rm -f cargo
export CGO_ENABLED=1
export GOARCH=arm
export GOARM=7
export CC=arm-linux-gnueabihf-gcc
# 构建 Go 项目
go build


