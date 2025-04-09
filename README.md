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

``` bash

ssh -o HostKeyAlgorithms=ssh-rsa root@192.168.x.x
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

```c 
int idtwotest(int dumpInfo)
{
	unsigned char idtwo_getbuff[2400] = {0};
	ushort  len;
	ushort  ret ;
	
	ID_DATA id_data;
    unsigned long tick;
    
/*
    unsigned char *version;

    version = (unsigned char*)malloc(2048);

    ret = IDCARD_Get_Sam_Ver(&len, version);
    if(ret == 0){
        printf("IDCARD_Get_Sam_Ver success\n");
        dump_data("SAM_Ver-->",version,len);
        free(version);
        if((version[0] != 0xaa) || (version[1] != 0xaa)){
            printf("IDCARD_Get_Sam_Ver failed\n");
            return -1;
        }
    }else{
        printf("IDCARD_Get_Sam_Ver failed\n");
        return -1;
    }
*/
	//while(1)
	{
    #if 1
		if(dumpInfo){
			LCD_ClearScreen(0);
			LCD_ClearAll();
			LCD_Printf(10+ 0,DISP_FONT12,"身份证测试：");
			LCD_Printf(10+24,DISP_FONT24|DISP_CENTER,"请放身份证");
		}
        tick = OSTIMER_GetTickCount();
		pthread_spin_lock(&lock);	
		ret = IDCARD_AutoRead(&len,idtwo_getbuff);
		pthread_spin_unlock(&lock);	

		if(ret == 0){
			printf("读身份证ok[%ld ms]！！！！！！！！！！！！！！！！\n",OSTIMER_GetTickCount() - tick);
	        //Sys_Beep();
			parse_id_info((char*)&idtwo_getbuff[7],&id_data);
			if(dumpInfo)
				dump_id_info(&id_data);
			else
				dump_id_info2(&id_data);
		}else{
			printf("读身份证fail,ret=%d\n",ret);
			if(dumpInfo)
				LCD_Printf(10+48,DISP_FONT12,"测试失败 [%d]", ret);
		}
	#else
        tick = OSTIMER_GetTickCount();
		pthread_spin_lock(&lock);	
		ret = IDCARD_AutoRead_Fig(&len,idtwo_getbuff);
		pthread_spin_unlock(&lock);	
		if(ret == 0){
			printf("读身份证带指纹ok[%ld ms]！！！！！！！！！！！！！！！！\n",OSTIMER_GetTickCount() - tick);
	        //Sys_Beep();
			parse_id_info((char*)&idtwo_getbuff[9],&id_data);
			dump_id_info(&id_data);
			dump_id_info2(&id_data);
		}
	#endif
	}
	return ret;
}
```

```bash
ssh -o HostKeyAlgorithms=+ssh-rsa user@192.168.8.24
```