#define KEY_PORT	P3			//¶¨Òå¼üÅÌ½Ó¿Ú

unsigned char scan,read,key_num;//²Î¿¼µçÆ½£»¶ÁKEY_PORT¿ÚµçÆ½£»¼ì²âµ½µÃ¼üÅÌÎ»Êı

void Delay_ms(unsigned int time);		//ÉùÃ÷ÑÓÊ±º¯Êı


unsigned char Key_Scan() //¼üÅÌÉ¨Ãè³ÌĞò
{
	key_num=0;
	KEY_PORT=0xfe;//´ÓÏÂÃæÊıÆğµÚÒ»ĞĞ
	read=KEY_PORT;
	scan=0xf0;
	scan=scan&read;//scanÓëread
	if(scan!=0xf0)//ÅĞ¶ÏÊÇ·ñÓÓĞ°´¼ü±»°´ÏÂ
		{
			Delay_ms(5);//ÑÓ³ÙÒ»¶ÎÊ±¼ä£¬Ïû¶·£¬
			read=KEY_PORT;
			scan=read&0xf0;	
			if(scan!=0xf0) // ÈôÕæµÄÓĞ¼ü°´ÏÂ£¬ÔÙ½øĞĞÅĞ¶ÏÄÄ¸ö°´¼ü°´ÏÂ
				{
					switch(scan)
						{
							case 0x70:key_num=16;break;
							case 0xb0:key_num=15;break;
							case 0xd0:key_num=14;break;
							case 0xe0:key_num=13;break;						
						}
					while(scan!=0xf0)//ËÉÊÖ¼ì²â
					{
						while(scan != 0xf0)//ËÉÊÖÏû¶¶¼ì²â
						{
							Delay_ms(5);
							read=KEY_PORT;
							scan=read&0xf0;				
						}
						Delay_ms(5);
						read=KEY_PORT;
						scan=read&0xf0;
					}
				}

		}

	KEY_PORT=0xfd;//´ÓÏÂÃæÊıÆğµÚ¶şĞĞ
	read=KEY_PORT;
	scan=0xf0;
	scan=scan&read;
	if(scan!=0xf0)
		{
			Delay_ms(5);
			read=KEY_PORT;
			scan=read&0xf0;	
			if(scan!=0xf0)
			{
				switch(scan)
				{
					case 0x70:key_num=12;break;
					case 0xb0:key_num=11;break;
					case 0xd0:key_num=10;break;
					case 0xe0:key_num=9;break;						
				}
					while(scan!=0xf0)
					{
						while(scan != 0xf0)
						{
							Delay_ms(5);
							read=KEY_PORT;
							scan=read&0xf0;				
						}
						Delay_ms(5);
						read=KEY_PORT;
						scan=read&0xf0;
					}
		    }

		}


    KEY_PORT=0xfb;//´ÓÏÂÃæÊıÆğµÚÈıĞĞ
	read=KEY_PORT;
	scan=0xf0;
	scan=scan&read;
	if(scan!=0xf0)
		{
			Delay_ms(5);
			read=KEY_PORT;
			scan=read&0xf0;	
			if(scan!=0xf0)
				{
					switch(scan)
						{
							case 0x70:key_num=8;break;
							case 0xb0:key_num=7;break;
							case 0xd0:key_num=6;break;
							case 0xe0:key_num=5;break;						
						}
						while(scan!=0xf0)
						{
							while(scan != 0xf0)
							{
								Delay_ms(5);
								read=KEY_PORT;
								scan=read&0xf0;				
							}
							Delay_ms(5);
							read=KEY_PORT;
							scan=read&0xf0;
						}
				}

		}


    KEY_PORT=0xf7;//´ÓÏÂÃæÊıÆğµÚËÄĞĞ
	read=KEY_PORT;
	scan=0xf0;
	scan=scan&read;
	if(scan!=0xf0)
		{
			Delay_ms(5);
			read=KEY_PORT;
			scan=read&0xf0;	
			if(scan!=0xf0)
				{
					switch(scan)
						{
							case 0x70:key_num=4;break;
							case 0xb0:key_num=3;break;
							case 0xd0:key_num=2;break;
							case 0xe0:key_num=1;break;						
						}
					while(scan!=0xf0)
					{
						while(scan != 0xf0)
						{
							Delay_ms(7);
							read=KEY_PORT;
							scan=read&0xf0;				
						}
						Delay_ms(5);
						read=KEY_PORT;
						scan=read&0xf0;
					}
				}

		}
	return(key_num);
}
