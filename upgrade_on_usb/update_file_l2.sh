usb_dir=/var/run/media/sda1/linux
#SNFILE="/opt/config/sn"
#SNNUB=$(cat /opt/config/sn)

echo "update now..."
amixer set Headphone 80% #临时设置音量
#tar -cjvf backup$SNNUB.tar.bz2 /moudle/ /home/root/ /lib/libhardware.so /usr/sn #压缩机具内部数据
#cp backup$SNNUB.tar.bz2 $usb_dir/	#备份机具内部数据到U盘
#rm backup$SNNUB.tar.bz2	#删除过度文件

#tar -xjvf $usb_dir/p18l2update.tar.bz2 -C / #更新tar包

#cp -f /boot/* /var/run/media/mmcblk1p1/
#cp -f $usb_dir/u-boot.imx /var/run/media/mmcblk1p1/ #更新UBOOT

#cp -f $usb_dir/zImage /var/run/media/mmcblk1p1/ #更新内核

#dd if=/dev/zero of=/dev/mmcblk1 bs=1k seek=768 conv=fsync count=8

#dd if=./u-boot.imx of=/dev/mmcblk1 bs=512 seek=2 conv=fsync

#cp -f $usb_dir/libpos.so /opt/lib/ #更新库

#cp -f $usb_dir/gprs /etc/ppp/peers/ #更新拨号文件

if [ -f "/opt/script/rc_usr_usr.sh" ];then
	mv /opt/script/rc_usr_usr.sh /opt/script/rc_usr.sh
	aplay $usb_dir/yyhfcg.wav
else
	mv /opt/script/rc_usr.sh /opt/script/rc_usr_usr.sh
	cp -f $usb_dir/rc_usr.sh /opt/script/ #更新启动文件
	chmod +x /opt/script/rc_usr.sh #修改权限
	aplay /opt/sound/7499.wav
fi
# 拷贝生产文件
cp -f $usb_dir/upgrade.sh /uchi/upgrade.sh
cp -f $usb_dir/config.yml /uchi/config.yml
cp -f $usb_dir/runner /uchi/runner
# 赋予运行权限
chmod +x /uchi/runner
chmod +x /uchi/upgrade.sh

#cp -f $usb_dir/start_app.sh /opt/script/ #更新自动启动应用脚本
#chmod +x /opt/script/start_app.sh #修改权限

#cp -f $usb_dir/auto_4g.sh /opt/script/ #自动拨号脚本
#chmod +x /opt/script/auto_4g.sh #修改权限

#cp -f $usb_dir/test_tcpip.sh /opt/script/ #自动拨号脚本
#chmod +x /opt/script/test_tcpip.sh #修改权限

#cp -f $usb_dir/ppp_router.sh /opt/script/ #自动拨号脚本
#chmod +x /opt/script/ppp_router.sh #修改权限

#cp -f $usb_dir/test_api /opt/app/ #更新生产测试程序
#chmod +x /opt/app/test_api #修改权限

#cp -f $usb_dir/dc_wdog /opt/app/ #更新看门狗测试程序
#chmod +x /opt/app/dc_wdog #修改权限

#cp -f $usb_dir/wpa_supplicant.conf /etc/ #wifi配置文件


sync
sleep 1
