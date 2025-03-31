arm linux
OS INFO

``` bash 
# uname -a 
Linux imx6ul7d 4.9.17 #81 SMP PREEMPT Wed Dec 4 19:13:11 CST 2019 armv7l armv7l armv7l GNU/Linux

# uname -m
armv71

```

CPU INFO

``` bash 
# root@imx6ul7d:/opt/app# cat /proc/cpuinfo
processor       : 0
model name      : ARMv7 Processor rev 5 (v7l)
BogoMIPS        : 8.00
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xc07
CPU revision    : 5

Hardware        : Freescale i.MX6 UltraLite (Device Tree)
Revision        : 0000
Serial          : 0000000000000000

```

验票语音提示

* A验票成功
* B验票失败
* C已验票
* D该票不在此检票点验票
* E景点可验次数已满
* F已退票
* G门票未激活
* H设备未配置检票点
* I检票时间未到
* J检票时间已过
* K闪付验票成功
* O需二次验票
* P年卡
* Q员工卡
* T门票已过期
* Z无权限访问