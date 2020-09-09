#include<STC12C5A60S2.h>
#include<intrins.h>
#include<key_scan.h>
#include<LCD1602.h>
#include<DS18B20.h>

void Delay(unsigned int);
void Delay_ms(unsigned int);
void blingbling_light();
void uptodown_horse_race_light();
void downtoup_horse_race_light();
void developerinfo();
void normal_custom_mode();
void LED_Init();
void slowtofast_shining_light();
void fasttoslow_shining_light();
void password_confirming();
void PlayMusic1(unsigned char);
void PlayMusic2(unsigned char);
void InterruptTmr0();

#define uint	unsigned	int
#define ulong	unsigned	long
#define	LED_PORT	P0
#define RldTmr(fr) 65536 - (11059200 /12) / (fr << 1)
#define FuDian(n) (n << 1) / 3

uint TPH;                        
uint TPL;                         
uint TP;						

sbit LED0 = P0^0;
sbit LED1 = P0^1;
sbit LED2 = P0^2;
sbit LED3 = P0^3;
sbit LED4 = P0^4;
sbit LED5 = P0^5;
sbit LED6 = P0^6;
sbit LED7 = P0^7;
sbit BUZZ = P1^4;

uint code noteFreq[] = 	//������ӦƵ��
{
    262,293,329,349,392,440,494,		//��1��7
	523,586,658,697,783,879,987,		//��1��7
    1045,1171,1316,1393,1563,1755,1971,	//��1��7
};

uint code tmrRld[] = 
{   
	RldTmr(262), RldTmr(293), RldTmr(329), RldTmr(349), RldTmr(392), RldTmr(440), RldTmr(494),
    RldTmr(523), RldTmr(586), RldTmr(658), RldTmr(697), RldTmr(783), RldTmr(879), RldTmr(987),
    RldTmr(1045), RldTmr(1171), RldTmr(1316), RldTmr(1393), RldTmr(1563), RldTmr(1755), RldTmr(1971),
};

uchar code musicNote1[] =  //���Һ��ҵ��������һ������
{    
    12,13,12,11,10,9,8,5,               //�Һ��ҵ����
	8,10,15,14,13,10,12,                //һ��Ҳ���ָܷ�
	13,14,13,12,11,10,9,6,              //�������ߵ�����
	7,6,5,12,8,9,10,	                //������һ���޸�
	12,13,12,11,10,9,8,5,				//�Ҹ質ÿһ����ɽ
	8,10,15,14,16,15,13,				//�Ҹ質ÿһ����
	15,14,13,12,13,12,11,10,7,6,5,9,8,	//�������̣�СС���䣬·��һ����
	15,16,17,16,15,13,14,13,10,12,		//�����װ������
	15,16,17,16,15,13,14,12,10,13,		//����Զ�������������
	12,11,10,9,7,6,6,5,10,11,9,8,8,		//��������ĸ�׵�����������˵
};

uchar code musicNote2[] =  //��â�֡����貿������
{    
    17,15,16,13,13,13,17,19,17,19,17,               	//һ�뵽���Ҿ�Wu~
	16,16,15,16,16,15,16,17,12,               			//���鲻�����ڿຣ��
	12,17,19,17,13,15,16,16,15,16,16,15,17,15,13,15,    //Wu~�´�ͽ���仨Ӱ����
	13,17,19,17,19,17,19,19,17,19,19,17,19,17,15,	    //Wu~��˼���ò�Цɽ�˾�
	12,20,19,19,17,19,17,15,15,13,						//Wu~ν�Һ���
};

uchar code noteDuration1[] =  //���Һ��ҵ��������һ�ν���
{      
	8,8,8,8,8,8,FuDian(4),FuDian(4),					//�Һ��ҵ����
	8,8,8,8,4,16,2,										//һ��Ҳ���ָܷ�
	8,8,8,8,8,8,FuDian(4),FuDian(4),					//�������ߵ�����
	8,8,8,8,4,16,2,										//������һ���޸�
	8,8,8,8,8,8,FuDian(4),FuDian(4),					//�Ҹ質ÿһ����ɽ
	8,8,8,8,4,16,2,			          				 	//�Ҹ質ÿһ����
	8,8,8,FuDian(4),8,8,8,FuDian(4),4,8,4,8,FuDian(4),	//�������̣�СС���䣬·��һ����
	8,8,8,8,8,8,8,FuDian(8),16,2,						//�����װ������
	8,8,8,8,8,8,8,FuDian(8),16,2,						//����Զ�������������
	8,8,8,FuDian(4),8,16,16,8,FuDian(4),FuDian(4),4,8,2,//��������ĸ�׵�����������˵	
};

bit enable = 1, tmrFlg = 0;
uchar T0RH = 0XFF, T0RL = 0X00;

uchar code noteDuration2[] =  //��â�֡����貿�ֽ���
{      
	16,16,8,8,8,4,8,8,8,8,4,							//һ�뵽���Ҿ�Wu~
	8,16,16,8,16,16,8,8,4,								//���鲻�����ڿຣ��
	4,8,8,8,8,4,8,16,16,8,16,16,8,8,8,8,				//Wu~�´�ͽ���仨Ӱ����
	4,8,8,8,8,4,8,16,16,8,16,16,8,8,4,					//Wu~��˼���ò�Цɽ�˾�
	4,8,8,4,4,2,8,8,4,2,								//Wu~ν�Һ���	
};

void PlayMusic1(uchar speed)//���Һ��ҵ��������һ�β��� 
{
    uchar i;
    uchar idx;
	uint key;
    uint cnt = 0;
    uint durationCnt = 0;
    uint soundCnt = 0;
	LED0 = 0;
	LED2 = 0;
	LED4 = 0;
	LED6 = 0;
	LED7 = 1;	
    for (i = 0; i < sizeof (musicNote1); ) 
	{
		key = Key_Scan();
		if(key == 16)
		{   
			i = sizeof (musicNote1) - 1; 
		}
        while (!tmrFlg) ;
        tmrFlg = 0;
        if (cnt == 0) 
		{
			LED0 = !LED0;
			LED1 = !LED1;
			LED2 = !LED2;
			LED3 = !LED3;
			LED4 = !LED4;
			LED5 = !LED5;
			LED6 = !LED6;
			LED7 = !LED7;
            idx = musicNote1[i] - 1;
            T0RH = tmrRld[idx] >> 8;
            T0RL = tmrRld[idx];
            durationCnt = (ulong)240 * (ulong)noteFreq[idx] / ((ulong)noteDuration1[i] * (ulong)speed);
            soundCnt = durationCnt - (durationCnt >> 2);
            enable = 1;
            cnt++;
        }
        else 
		{
            if (cnt == durationCnt) 
			{
                cnt = 0;
                i++;
            }
            else 
			{
                cnt++;
                if (cnt == soundCnt) 
				{
                    enable = 0;
                }
            }
        }
    }
}

void PlayMusic2(uchar speed)//��â�֡����貿�ֲ��� 
{
    uchar i;
    uchar idx;
	uint key;
    uint cnt = 0;
    uint durationCnt = 0;
    uint soundCnt = 0;
	LED0 = 0;
	LED2 = 0;
	LED4 = 0;
	LED6 = 0;
	LED7 = 1;	
    for (i = 0; i < sizeof (musicNote2); ) 
	{
		key = Key_Scan();
		if(key == 16)
		{   
			i = sizeof (musicNote2) - 1; 
		}
        while (!tmrFlg) ;
        tmrFlg = 0;
        if (cnt == 0) 
		{
			LED0 = !LED0;
			LED1 = !LED1;
			LED2 = !LED2;
			LED3 = !LED3;
			LED4 = !LED4;
			LED5 = !LED5;
			LED6 = !LED6;
			LED7 = !LED7;
            idx = musicNote2[i] - 1;
            T0RH = tmrRld[idx] >> 8;
            T0RL = tmrRld[idx];
            durationCnt = (ulong)240 * (ulong)noteFreq[idx] / ((ulong)noteDuration2[i] * (ulong)speed);
            soundCnt = durationCnt - (durationCnt >> 2);
            enable = 1;
            cnt++;
        }
        else 
		{
            if (cnt == durationCnt) 
			{
                cnt = 0;
                i++;
            }
            else 
			{
                cnt++;
                if (cnt == soundCnt) 
				{
                    enable = 0;
                }
            }
        }
    }
}

void InterruptTmr0() interrupt 1 
{
    TH0 = T0RH;
    TL0 = T0RL;
    tmrFlg = 1;
    if (enable)
        BUZZ = ~BUZZ;
    else
        BUZZ = 1;
}

void LED_Init()//LED�Ƴ�ʼ��
{
	LED0 = 1;
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	LED5 = 1;
	LED6 = 1;
	LED7 = 1;
}

void Delay_ms(uint time)
{
	uint i,j;
	for(i = 0;i < time;i ++)
		for(j = 0;j < 930;j ++);
}

void Delay(uint time)
{
	uint i;
	for(i = time;i> 0;i --);
}

void uptodown_horse_race_light()//SW1:���϶��µ�ѭ��7�������
{
	uchar temp = 0xFE,i = 0;
	uint j=0;
	LCD_1602_Init();
	Write_1602_String("Mode 1",0X80);
	Write_1602_String("Showing...",0XC0);
	while(j<8)
	{
		while(i<7)
		{
			LED_PORT = temp;
			Delay_ms(40);
			temp=_crol_(temp,1);
			i++;			
		}
		j++;
		i=0;
	}
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);			
}

void downtoup_horse_race_light()//SW2:���¶��ϵ�ѭ��7�������
{
	uchar temp = 0x7F,i = 7;
	uint j=0;
	LCD_1602_Init();
	Write_1602_String("Mode 2",0X80);
	Write_1602_String("Showing...",0XC0);
	while(j<8)
	{
		while(i>0)
		{
			LED_PORT = temp;
			Delay_ms(40);
			temp=_cror_(temp,1);
			i--;		
		}
		j++;
		i=7;
	}
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);			
}

void blingbling_light()//SW3:Լ5���blingbling��
{
	uint temp , i , t;
	LCD_1602_Init();
	Write_1602_String("Mode 3",0X80);
	Write_1602_String("Showing...",0XC0);
	for(t = 0;t <= 13;t ++)
	{ 
		for(temp = 0;temp <= 100;temp ++)
		{
			for(i=0;i<10;i++)
			{
				LED0 = 1;
				LED1 = 0;
				LED2 = 1;
				LED3 = 0;
				LED4 = 1;
				LED5 = 0;
				LED6 = 1;
				LED7 = 0;
				Delay(temp);		
				LED0 = 0;
				LED1 = 1;
				LED2 = 0;
				LED3 = 1;
				LED4 = 0;
				LED5 = 1;
				LED6 = 0;
				LED7 = 1; 
				Delay(100-temp);			
			}
		}
		for(temp = 100;temp > 0;temp--)
		{
			for(i=0;i<10;i++)
			{
				LED0 = 1;
				LED1 = 0;
				LED2 = 1;
				LED3 = 0;
				LED4 = 1;
				LED5 = 0;
				LED6 = 1;
				LED7 = 0;
				Delay(temp);		
				LED0 = 0;
				LED1 = 1;
				LED2 = 0;
				LED3 = 1;
				LED4 = 0;
				LED5 = 1;
				LED6 = 0;
				LED7 = 1; 
				Delay(100-temp);			
			}			
		}		
	}
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);		
}

void shining_light()//SW4:ѭ��3�εĴ����ߵ��м����˸��
{
	uint i = 0;
	LCD_1602_Init();
	Write_1602_String("Mode 4",0X80);
	Write_1602_String("Showing...",0XC0);
	LED_Init();
	Delay_ms(500);
	while(i<3)
	{
		LED0 = !LED0;
		LED7 = !LED7;
		Delay_ms(200);
		LED0 = !LED0;
		LED7 = !LED7;
		Delay_ms(200);
		LED1 = !LED1;
		LED6 = !LED6;
		Delay_ms(200);
		LED1 = !LED1;
		LED6 = !LED6;
		Delay_ms(200);
		LED2 = !LED2;
		LED5 = !LED5;
		Delay_ms(200);
		LED2 = !LED2;
		LED5 = !LED5;
		Delay_ms(200);
		LED3 = !LED3;
		LED4 = !LED4;
		Delay_ms(200);
		LED3 = !LED3;
		LED4 = !LED4;
		Delay_ms(200);
		i++;
	}
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);
}

void slowtofast_shining_light()//SW5:��������ļ����˸��
{
	uint i,j;
	LCD_1602_Init();
	Write_1602_String("Mode 5",0X80);
	Write_1602_String("Showing...",0XC0);
	LED_Init();	
	for(j=400;j>101;j-=50)
	{		
		LED0 = 0;
		LED1 = 1;
		LED2 = 0;
		LED3 = 1;
		LED4 = 0;
		LED5 = 1;
		LED6 = 0;
		LED7 = 1;
		Delay_ms(j);
		LED0 = 1;
		LED1 = 0;
		LED2 = 1;
		LED3 = 0;
		LED4 = 1;
		LED5 = 0;
		LED6 = 1;
		LED7 = 0;
		Delay_ms(j);		
	}
	for(i=0;i<10;i++)
	{		
		LED0 = 0;
		LED1 = 1;
		LED2 = 0;
		LED3 = 1;
		LED4 = 0;
		LED5 = 1;
		LED6 = 0;
		LED7 = 1;
		Delay_ms(100);
		LED0 = 1;
		LED1 = 0;
		LED2 = 1;
		LED3 = 0;
		LED4 = 1;
		LED5 = 0;
		LED6 = 1;
		LED7 = 0;
		Delay_ms(100);		
	}
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);	
}

void fasttoslow_shining_light()//SW6:�ӿ쵽���ļ����˸��
{
	uint i,j;
	LCD_1602_Init();
	Write_1602_String("Mode 6",0X80);
	Write_1602_String("Showing...",0XC0);
	LED_Init();
	for(i=0;i<10;i++)
	{		
		LED0 = 0;
		LED1 = 1;
		LED2 = 0;
		LED3 = 1;
		LED4 = 0;
		LED5 = 1;
		LED6 = 0;
		LED7 = 1;
		Delay_ms(100);
		LED0 = 1;
		LED1 = 0;
		LED2 = 1;
		LED3 = 0;
		LED4 = 1;
		LED5 = 0;
		LED6 = 1;
		LED7 = 0;
		Delay_ms(100);		
	}	
	for(j=50;j<399;j+=50)
	{		
		LED0 = 0;
		LED1 = 1;
		LED2 = 0;
		LED3 = 1;
		LED4 = 0;
		LED5 = 1;
		LED6 = 0;
		LED7 = 1;
		Delay_ms(j);
		LED0 = 1;
		LED1 = 0;
		LED2 = 1;
		LED3 = 0;
		LED4 = 1;
		LED5 = 0;
		LED6 = 1;
		LED7 = 0;
		Delay_ms(j);		
	}
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);	
}

void normal_custom_mode()//SW7:��ͨ�Զ����
{
	uchar key = 0;
	LCD_1602_Init();
	Write_1602_String("Mode 7",0X80);
	Write_1602_String("Press x To Exit",0XC0);
	LED_Init();
	while(key != 16)
	{
		key = Key_Scan();	 
		switch(key)	
		{
			case 1:
			LED0 = !LED0;break;
			case 2:
			LED1 = !LED1;break;
			case 3:
			LED2 = !LED2;break;
			case 4:
			LED3 = !LED3;break;
			case 5:
			LED4 = !LED4;break;
			case 6:
			LED5 = !LED5;break;
			case 7:
			LED6 = !LED6;break;
			case 8:
			LED7 = !LED7;break;	
		}
	}
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);
}

void musicPlay1()//SW8:���š��Һ��ҵ��������һ�� 
{
	LCD_1602_Init();
	Write_1602_String("Playing...",0X80);
	Write_1602_String("Press x To Stop",0XC0);
    EA = 1;
    TMOD = 0X01;
    TH0 = T0RH;
    TL0 = T0RL;
    ET0 = 1;
    TR0 = 1;
    PlayMusic1(55);
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);
}

void musicPlay2()//SW9:���š�â�֡����貿�� 
{
	LCD_1602_Init();
	Write_1602_String("Playing...",0X80);
	Write_1602_String("Press x To Stop",0XC0);
    EA = 1;
    TMOD = 0X01;
    TH0 = T0RH;
    TL0 = T0RL;
    ET0 = 1;
    TR0 = 1;
    PlayMusic2(55);
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);
}

void developerinfo()//SW10:Լ3��Ŀ�������Ϣ��ʾ
{
	LCD_1602_Init();
	Write_1602_String("Developer:",0X80);
	Write_1602_Data(0x06);
	Write_1602_Data(0x07);
	Write_1602_Data(0x02);
	Write_1602_Data(0x03);
	Write_1602_Data(0x04);
	Write_1602_Data(0x05);
	Write_1602_String("ID:20192333075",0XC0);
	Delay_ms(3000);
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);
}	 

void temperature_showing()//SW11:��ǰ�¶���ʾ
{  	
	uchar key = 0;
	LCD_1602_Init();
	Write_1602_String("Temperature:",0X80);
	Write_1602_String("Press x To Exit",0XC0);			
    while(key != 16)
    {
		key = Key_Scan();
	    DS18B20_Reset();               
	    DS18B20_WriteByte(0xCC);       
	    DS18B20_WriteByte(0x44);             	
		Delay_ms(20);				  	
	    DS18B20_Reset();              
	    DS18B20_WriteByte(0xCC);       
	    DS18B20_WriteByte(0xBE);        
	    TPL = DS18B20_ReadByte();      
	    TPH = DS18B20_ReadByte();     
		TP=((TPH<<8)|TPL)*0.0625;		
		Write_Num(TP,0x80+13);			  
    }
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);	
}

void password_confirming()//�жϿ�������
{
	uint i=0,j,judge;
	uint temp;
	uchar defaultpassword[4]={5,2,1,3};
	uchar enterpassword[4];
	Write_1602_String("Enter Password:",0X80);
	while(1)
	{
		if(judge==1)
		{
			break;
		}
		j=0;
		temp=Key_Scan();
		if(temp>0&&temp<11)
		{
			switch(temp)
			{
				case 1:enterpassword[i]=1;i++;Write_1602_String("1",0XC0);Delay_ms(300);break;
				case 2:enterpassword[i]=2;i++;Write_1602_String("2",0XC0);Delay_ms(300);break;
				case 3:enterpassword[i]=3;i++;Write_1602_String("3",0XC0);Delay_ms(300);break;
				case 4:enterpassword[i]=4;i++;Write_1602_String("4",0XC0);Delay_ms(300);break;
				case 5:enterpassword[i]=5;i++;Write_1602_String("5",0XC0);Delay_ms(300);break;
				case 6:enterpassword[i]=6;i++;Write_1602_String("6",0XC0);Delay_ms(300);break;
				case 7:enterpassword[i]=7;i++;Write_1602_String("7",0XC0);Delay_ms(300);break;
				case 8:enterpassword[i]=8;i++;Write_1602_String("8",0XC0);Delay_ms(300);break;
				case 9:enterpassword[i]=9;i++;Write_1602_String("9",0XC0);Delay_ms(300);break;
				case 10:enterpassword[i]=0;i++;Write_1602_String("0",0XC0);Delay_ms(300);break;
			}
		}
		Write_1602_String(" ",0XC0);
		if(i==4)
		{
			Delay_ms(600);
			LCD_1602_Init();
			for(i=0;i<4;i++)
			{
				if(enterpassword[i]!=defaultpassword[i])
				{
					Write_1602_String("Incorrect",0X80);
					Write_1602_String("Try Again",0XC0);
					Delay_ms(1200);
					LCD_1602_Init();
					Write_1602_String("Enter Password:",0X80);
					judge = 0;	
					break;	
				}
				if(i==3)
				{
					Write_1602_String("Password Correct",0X80);
					judge = 1;
					Delay_ms(1200);
				}
			}
			i=0;
		}
	}
}

void main()
{
	uint key = 0;
	LCD_1602_Init();
	Write_1602_String("--LED PLAYER--",0X81);
	Delay_ms(1000);
	Write_1602_String("POWER ON",0XC4);
	Delay_ms(1000);
	LCD_1602_Init();
	password_confirming();
	LCD_1602_Init();
	Write_1602_String("Please Press A",0X80);
	Write_1602_String("Button",0XC0);
	while(1)
	{
		LED_Init();		
		key = Key_Scan();
		switch(key)	
		{
			case 1:
			uptodown_horse_race_light();break;	//Mode1�����϶��µ�ѭ��7�������
			case 2:
			downtoup_horse_race_light();break;	//Mode2�����¶��ϵ�ѭ��7�������
			case 3:
			blingbling_light();break;			//Mode3��Լ5���blingbling��
			case 4:
			shining_light();break;				//Mode4��ѭ��3�εĴ����ߵ��м����˸��
			case 5:
			slowtofast_shining_light();break;	//Mode5����������ļ����˸��
			case 6:
			fasttoslow_shining_light();break;	//Mode6���ӿ쵽���ļ����˸��
			case 7:
			normal_custom_mode();break;			//Mode7����ͨ�Զ����
			case 8:
			musicPlay1();break;					//Mode8�����š��Һ��ҵ��������һ��
			case 9:
			musicPlay2();break;					//Mode9�����š�â�֡����貿��
			case 10:
			developerinfo();break;				//info��Լ3��Ŀ�������Ϣ��ʾ
			case 11:
			temperature_showing();break;		//temperature����ǰ�¶���ʾ	
		}
	}
}