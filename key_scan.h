#define KEY_PORT	P3			//������̽ӿ�

unsigned char scan,read,key_num;//�ο���ƽ����KEY_PORT�ڵ�ƽ����⵽�ü���λ��

void Delay_ms(unsigned int time);		//������ʱ����


unsigned char Key_Scan() //����ɨ�����
{
	key_num=0;
	KEY_PORT=0xfe;//�����������һ��
	read=KEY_PORT;
	scan=0xf0;
	scan=scan&read;//scan��read
	if(scan!=0xf0)//�ж��Ƿ���а���������
		{
			Delay_ms(5);//�ӳ�һ��ʱ�䣬������
			read=KEY_PORT;
			scan=read&0xf0;	
			if(scan!=0xf0) // ������м����£��ٽ����ж��ĸ���������
				{
					switch(scan)
						{
							case 0x70:key_num=16;break;
							case 0xb0:key_num=15;break;
							case 0xd0:key_num=14;break;
							case 0xe0:key_num=13;break;						
						}
					while(scan!=0xf0)//���ּ��
					{
						while(scan != 0xf0)//�����������
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

	KEY_PORT=0xfd;//����������ڶ���
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


    KEY_PORT=0xfb;//���������������
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


    KEY_PORT=0xf7;//���������������
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
