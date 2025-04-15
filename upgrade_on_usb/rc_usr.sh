#! /bin/bash
. /opt/script/function.sh

function run_script(){
    
    script=$1
    echo "script name = $script"
    if [ -f "$script" ];then
        kill_and_wait `basename $script`
        chmod +x $script
        cd `dirname $script`
        #echo "to run script $script"
        $1
        return 0
    else
        echo "script $script not exist"
        return 5
    fi
}
ppp_dial(){
#关闭反向路由检查

echo 0 > /proc/sys/net/ipv4/conf/all/rp_filter
echo 0 >/dev/null 2>&1 > /proc/sys/net/ipv4/conf/eth0/rp_filter
echo 0 >/dev/null 2>&1 > /proc/sys/net/ipv4/conf/wlan0/rp_filter 
echo 0 >/dev/null 2>&1 > /proc/sys/net/ipv4/conf/ppp0/rp_filter
/opt/script/auto_4g.sh 2>&1 > /dev/null &
}

usb_autorun=/run/media/sda1/linux/update_file_l2.sh
get_term_name=/opt/script/get_term_name.sh
rc_usr=/opt/script/rc_usr.sh
export PATH=$PATH:/opt/app
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/lib:/opt/qt-4.8.7/lib
export QT_QWS_FONTDIR=/opt/qt-4.8.7/lib/fonts
export DISPLAY=:0

update_success(){
 if [ $? -eq 0 ]; then
 aplay /opt/sound/upsjcg.wav 
 sync
 cd /
 #ps ax
 #fuser -m /dev/sda1
 umount /dev/sda1
 fi
}

install_driver(){
if [ "x$1" = "xstart" ]; then
	#sleep 2 	
	echo "A" > /dev/watchdog
	#insmod /opt/driver/8723bu.ko 2&>/dev/null
	#insmod /opt/driver/rtl8723_du.ko 2>/dev/null

	#GobiNet#
	#insmod /opt/driver/GobiNet.ko   2>/dev/null
	insmod /opt/driver/sm1616.ko    2>/dev/null
	insmod /opt/driver/st7565.ko    2>/dev/null
	insmod /opt/driver/ds1302.ko    2>/dev/null
	insmod /opt/driver/halicc.ko    2>/dev/null
	#insmod /opt/driver/idtwo.ko    	2>/dev/null
	echo "V" > /dev/watchdog
fi
}

disp_eth0_ip()
{
	echo  eth0 ip is `ifconfig eth0|grep inet|grep -v inet6|awk '{print $2}'|tr -d "addr:"`
}
install_driver $1
run_script $get_term_name
run_script /opt/script/update_app.sh
run_script $usb_autorun
update_success                                  
                         
ppp_dial 

#GobiNet#
#quectel-CM -s cmnet -n 2&                 
                                          
#run_script $get_term_name



/opt/script/check_powerdown_int.sh &
kill_and_wait A2tpumain
disp_eth0_ip
echo "2 4 1 7" > /proc/sys/kernel/printk

#GobiNet#
#/opt/script/ppp_router.sh
/opt/app/test_api Z 0 &

#run_script /opt/script/start_app.sh
#/home/root/A2tpumain 0 &
/uchi/runner &