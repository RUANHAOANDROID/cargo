int idtwotest(int dumpInfo)
{
	unsigned char idtwo_getbuff[2400] = {0};
	ushort  len;
	ushort  ret ;

	ID_DATA id_data;
    unsigned long tick;
	{
    #if 1
        tick = OSTIMER_GetTickCount();
		pthread_spin_lock(&lock);
		ret = IDCARD_AutoRead(&len,idtwo_getbuff);
		pthread_spin_unlock(&lock);
		if(ret == 0){
			printf("读身份证ok[%ld ms]！！！！！！！！！！！！！！！！\n",OSTIMER_GetTickCount() - tick);
	        //Sys_Beep();
			parse_id_info((char*)&idtwo_getbuff[7],&id_data);
			dump_id_info2(&id_data);
		}else{
			printf("读身份证fail,ret=%d\n",ret);
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