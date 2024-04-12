#!/bin/bash

# 获取当前日期时间，格式为YYYY-MM-DD-HH-MM
current_date_time=$(date +"%Y-%m-%d-%H-%M")

# 备份目录
backup_dir="backup/$current_date_time"

# 创建备份目录
mkdir -p "$backup_dir"

# 复制当前文件夹下的所有文件到备份目录
cp -r ./config.yml ./runner "$backup_dir/"
# 解压 cargo.tar 文件到当前目录
tar -xvf cargo.tar

# 删除cargo.tar文件
rm -f cargo.tar

# 检查是否解压成功
if [ $? -eq 0 ]; then
    echo "文件解压成功！准备重启系统..."
    # 重启系统
    reboot
else
    echo "文件解压失败！"
fi
