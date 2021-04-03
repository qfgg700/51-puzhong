/*
ʵ��˵����
	
ʵ����ߣ�
	1����̬�����ģ��-->��Ƭ���ܽ�
	�ο���̬�������ʾʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	2����������ģ��-->��Ƭ���ܽ� 
	K1-->P10
	K2-->P11
	K3-->P12
	K4-->P13
	
ʵ������
	��K4�������������ã���K1��K3������˸�����ֽ��мӺͼ�����K2�������������λ����
	�ٴΰ���K4���˳����á�
*/


#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit k1=P3^0;
sbit k2=P3^1;
sbit k3=P3^2;
sbit k4=P3^3;	 //���尴���˿�

#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ

u8 DisplayData[8];
char datnum1=0,datnum2=0,datnum3=0,datnum4=0;
u8 setflag=0;
u8 wei_change=0;

/*******************************************************************************
* �� �� ��         : delay
* ��������		   : ��ʱ������i=1ʱ����Լ��ʱ10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

/*******************************************************************************
* �� �� ��         : Timer0Init
* ��������		   : ��ʱ��0��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Timer0Init()
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������

	TH0=0Xd8;	//����ʱ������ֵ����ʱ10ms
	TL0=0Xf0;	
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	//TR0=1;//�򿪶�ʱ��			
}

/*******************************************************************************
* �� �� ��         : DigDisplay
* ��������		   : ����ܶ�̬ɨ�躯����ѭ��ɨ��8���������ʾ
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ
			case(1):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
			case(2):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
			case(3):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(4):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ 
			case(5):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ 
			case(6):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ 
			case(7):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ	
		}
		P0=DisplayData[i];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
}

//mode=0 ����   1������
u8 KeyScan(u8 mode)
{
	static u8 keyen=1;
	if(mode==1)
	{
		keyen=1;
	}
	if(keyen==1&&(k1==0||k2==0||k3==0||k4==0))
	{
		delay(1000);  //��������
		keyen=0;
		if(k1==0)return KEY1;
		else if(k2==0)return KEY2;
		else if(k3==0)return KEY3;
		else if(k4==0)return KEY4;				
	}
	else if(k1==1&&k2==1&&k3==1&&k4==1)
	{
		keyen=1;
	}
	return 0;	
}

void datapros()
{
	u8 key;
	key=KeyScan(0);
	
	if(key==KEY4)	   //����
	{
		setflag++;
		TR0=1;	   //������ʱ��
		if(setflag==2)
		{
			TR0=0;		//�رն�ʱ��
			setflag=0;	
		}
	}

	if(setflag==0)	 //Ĭ��/�˳�������ʾ
	{
		DisplayData[0]=smgduan[datnum4];
		DisplayData[1]=smgduan[datnum3];
		DisplayData[2]=smgduan[datnum2];
		DisplayData[3]=smgduan[datnum1];
	}
	else	  //��������  �����ѡ����˸
	{
		if(key==KEY2)	    //����λ���ƶ��仯
		{
			wei_change++;
			if(wei_change>3)wei_change=0;
			DisplayData[0]=smgduan[datnum4];
			DisplayData[1]=smgduan[datnum3];
			DisplayData[2]=smgduan[datnum2];
			DisplayData[3]=smgduan[datnum1];	
		}

		else if(key==KEY1)	 //���ݼ�
		{
			switch(wei_change)
			{
				case 0:  datnum1++;
						if(datnum1>9)datnum1=0;
						break;
				case 1:  datnum2++;
						if(datnum2>9)datnum2=0;
						break;
				case 2:  datnum3++;
						if(datnum3>9)datnum3=0;
						break;
				case 3:  datnum4++;
						if(datnum4>9)datnum4=0;
						break;

			}	
		}
		else if(key==KEY3)	 //���ݼ�
		{
			switch(wei_change)
			{
				case 0:  datnum1--;
						if(datnum1<=0)datnum1=9;		
						break;
				case 1:  datnum2--;
						if(datnum2<=0)datnum2=9;
						break;
				case 2:  datnum3--;
						if(datnum3<=0)datnum3=9;
						break;
				case 3:  datnum4--;
						if(datnum4<=0)datnum4=9;
						break;

			}	
		}
			
	}
	
}
/*******************************************************************************
* �� �� ��       : main
* ��������		 : ������
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void main()
{	
	Timer0Init();
	while(1)
	{	
		datapros();
		DigDisplay();	
	}		
}

/*******************************************************************************
* �� �� ��         : void Timer0() interrupt 1
* ��������		   : ��ʱ��0�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Timer0() interrupt 1
{
	static u8 i;
	TH0=0Xd8;	//����ʱ������ֵ����ʱ10ms
	TL0=0Xf0;
	i++;
	if(i==50)	  //500ms
	{
		switch(wei_change)
		{
			case 0: DisplayData[3]=0x00;break;
			case 1: DisplayData[2]=0x00;break;
			case 2: DisplayData[1]=0x00;break;
			case 3: DisplayData[0]=0x00;break;
		}	
	}
	if(i==100)	   //1s
	{
		i=0;
		switch(wei_change)
		{
			case 0: DisplayData[3]=smgduan[datnum1];break;
			case 1: DisplayData[2]=smgduan[datnum2];break;
			case 2: DisplayData[1]=smgduan[datnum3];break;
			case 3: DisplayData[0]=smgduan[datnum4];break;
		}			
	}
}

