#!/bin/bash

REMOTE_URL="http://emcs-api.youchiyun.com//gateMachine/getVersion"  # 存放新版本号的文本文件
INSTALL_DIR="/path/to/install"                            # 安装目录
BACKUP_DIR="/path/to/backup"                              # 备份目录
CHECK_INTERVAL=60                                        # 30分钟的秒数
DEVICE_TYPE="1467360836350640149"
while true; do
    # 下载远程文本文件，里面存放着新版本号
    REMOTE_VERSION=$(curl -s $REMOTE_URL)

    # 获取本地已安装的版本号
    LOCAL_VERSION=$(cat $INSTALL_DIR/version.txt 2>/dev/null)

    # 如果版本不一致，则进行更新
    if [ "$REMOTE_VERSION" != "$LOCAL_VERSION" ]; then
        echo "New version detected. Updating..."

        # 备份旧版本
        mkdir -p $BACKUP_DIR
        cp -r $INSTALL_DIR $BACKUP_DIR/backup_$(date +"%Y%m%d%H%M%S")

        # 下载新版本
        wget -O latest_version.tar.gz https://your-server.com/latest_version.tar.gz

        # 解压新版本
        tar -xzf latest_version.tar.gz -C $INSTALL_DIR

        # 清理临时文件
        rm latest_version.tar.gz

        # 更新本地版本号文件
        echo $REMOTE_VERSION > $INSTALL_DIR/version.txt

        echo "Update completed successfully."
    else
        echo "No new version available. Sleeping for $CHECK_INTERVAL seconds..."
    fi

    # 等待一段时间后再次检测
    sleep $CHECK_INTERVAL
done
