/*
ʵ��˵����
	�����Լ�ʹ�õ�LCD1602�Ƿ����ת�Ӱ壬�������ת�Ӱ�ļ�Ϊ4λ������LCD.Hͷ�ļ���
	����#define LCD1602_4PINS�򿪣���������ʹ�õ�LCD1602��8λ������Ĭ�Ͻ��ú�ע�͡�
ʵ����ߣ�
	1��LCD1602Һ��ģ��-->��Ƭ���ܽ�
	�ο�LCD1602Һ����ʾʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	2��MQ-135������ģ��-->��Ƭ���ܽ� 
	��������ģ���Ӧ���뵽�������ϵĽӿڴ���Ȼ��ʹ�öŰ��߽�MQ135ģ���P6������AO���뿪������
	DACģ����ϵ�J52���ӵ�AIN3������
	3��ADCģ��-->��Ƭ���ܽ�
	�ο�ADCģ��ת��ʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	4����������ģ��-->��Ƭ���ܽ�
	K1-->P33
	K2-->P21
	K3-->P22
	5��������ģ��-->��Ƭ���ܽ�
	BEEP-->P15
	6��ֱ�����ģ��-->��Ƭ���ܽ�
	�ο�ֱ�����ʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	
ʵ������
	ͨ������K1�ɶ�����Ũ�ȷ�ֵ�����趨
	����һ��K1�����������趨���ٰ���K1һ�Σ��˳��趨ģʽ��������������Ũ�Ȳɼ�״̬��
	���뷧ֵ�趨ģʽ��ͨ��K2��K3������ֵ�Ӽ������ɼ�������Ũ�ȸ����趨����ֵ��������������
	�����ת���������ɼ�������Ũ�ȵ����趨����ֵ���������͵����������������������״̬��
*/

#include<reg51.h>
#include "XPT2046.h"
#include"lcd.h"

typedef unsigned char uint8;
typedef unsigned int uint16;


sbit k1=P3^3; //ģʽ
sbit k2=P2^1; //��
sbit k3=P2^2; //��

sbit moto=P1^0;
sbit beep=P1^5;


uint8 mode=0;
char temph=20;
uint8 temp,flag;

uint8 code num[10]="0123456789";

uint8 code str1[]="GAS:";  
uint8 code str2[]="SET:"; 



void delay(uint16 i)
{
	while(i--);
}

void wrc(uint8 c)	 //д����
{
	LcdWriteCom(c);
}
void wrd(uint8 dat)	  //д����
{
	LcdWriteData(dat);
}
void lcd_init()	   // LCD1602��ʼ��
{
	LcdInit();
}

void key_pros()  //����������
{
	if(k1==0)
	{
		delay(1000);
		if(k1==0)
		{
			mode++;
			flag=0;
			if(mode==2)
			{
				mode=0;
				flag=1;	
			}
			
		}
		while(!k1);
	}
	if(mode==1)		  //�Կ�����ⱨ��ֵ�趨
	{
		if(k2==0)		  //��
		{
			delay(1000);
			if(k2==0)
			{
				temph+=1;
				if(temph>=100)temph=100;
			}
			while(!k2);
		}
		if(k3==0)	   //��
		{
			delay(1000);
			if(k3==0)
			{
				temph-=1;
				if(temph<=0)temph=0;	
			}
			while(!k3);
		}
	}
	
}

void lcd_init_display()	   //LCD��ʼ����ʾ
{
	uint8 i;
	for(i=0;i<4;i++)
	{
		wrc(0x80+i);
		wrd(str1[i]);	
	}

	for(i=0;i<4;i++)
	{
		wrc(0xc0+i);
		wrd(str2[i]);	
	}
}

void data_pros()	//���ݴ�����
{
	uint8 tempbuf[3],temp_buf[3],i;
	tempbuf[0]=temp/100+0x30;
	tempbuf[1]=temp%100/10+0x30;				
	tempbuf[2]=temp%100%10+0x30;
	
	temp_buf[0]=temph/100+0x30;
	temp_buf[1]=temph%100/10+0x30;				
	temp_buf[2]=temph%100%10+0x30;

	for(i=0;i<3;i++)
	{
		wrc(0x84+i);
		wrd(tempbuf[i]);
	}

	for(i=0;i<3;i++)
	{
		wrc(0xc4+i);
		wrd(temp_buf[i]);
	}
}

void sound()
{
	uint8 i=100;
	while(i--)
	{
		beep=~beep;
		delay(100);	
	}
}
void baojinpros()	//��������
{
	if(temp>=temph&&flag==1)
	{
		moto=1;
		sound();
	}
	else
	{	
		moto=0;
		beep=1;
	}
}

void main()
{
	moto=1;
	beep=1;
	lcd_init();
	lcd_init_display();	   //LCD��ʼ����ʾ
	while(1)
	{	
		key_pros();
		data_pros();
		baojinpros();
		temp = Read_AD_Data(0x94)/40;			
	}	
}
