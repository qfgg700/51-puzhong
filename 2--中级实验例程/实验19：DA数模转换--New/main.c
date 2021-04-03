/*
ʵ����ߣ�
	AD/DACģ��-->��Ƭ���ܽ�
	J50-->P21
ʵ������
	AD/DACģ���ϵ�DA1ָʾ�Ƴʺ�����Ч��
*/


#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���


//���������ͽ�����������
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//--����ʹ�õ�IO��--//
sbit PWMOUTLED=P2^1;

//#define SYSCLOCK	11059200		//����Ƶ����11.0592MHZ
#define SYSCLOCK	12000000		//����Ƶ����12MHZ

u8 heighHR0, heighLR0;//PWMһ�������иߵ�ƽ���ֽں͵��ֽ�����ֵ
u8 lowHR0, lowLR0;//PWMһ�����������е͵�ƽʱ���ֽں͵��ֽ�����ֵ



/*ʹ��T0����PWM�ߵ͵�ƽ��Ƶ��*/
void PWM_Config(u16 fr, u8 dc)  //frΪPWM��Ƶ�ʣ�100HZ����˸��dc��PWM��ռ�ձ�(�ߵ�ƽռ�İٷֱ�)
{
	u32 tmp;
	u16 heigh, low;
	
	tmp = SYSCLOCK/12;//һ����������Ƶ��
	tmp = tmp / fr;  //һ������������fr��Ƶ���������ֵ(�����ߵ�ƽ�͵͵�ƽ)
	heigh = (tmp * dc) / 100;//�ߵ�ƽռdc%���������ֵ
	low   = tmp - heigh;//�͵�ƽ�������ֵ
	heigh = 65536-heigh;//�ߵ�ƽ������ֵ
	low   = 65536-low;//�͵�ƽ������ֵ
	
	heighHR0 = (u8)(heigh >> 8);//�ߵ�ƽװ���ֵ�ĸ��ֽ�
	heighLR0 = (u8)heigh;    //�ߵ�ƽװ���ֵ�ĵ��ֽ�
	lowHR0   = (u8)(low >> 8);  //�͵�ƽװ���ֵ�ĸ��ֽ�
	lowLR0   = (u8)low;    //�͵�ƽװ���ֵ�ĵ��ֽ�
	
	TMOD &= 0xF0;//����T0����λ
	TMOD |= 0x01;//�趨T0������ʽ1,16λ���趨ʱ��
	TH0 = heighHR0;
	TL0 = heighLR0;//װ��ߵ�ƽ������ֵ
	PWMOUTLED = 1;//��ʼʱPWM����ߵ�ƽ
	TR0 = 1;
	ET0 = 1;
	EA  = 1;
}

/*�ر�PWM*/
void PWM_Close()
{
	TR0 = 0;//�ر�Timer0
	ET0 = 0;//�ر�T0�ж�
	PWMOUTLED = 1;//PWM����ߵ�ƽ
}

void main()
{
	u16 i=0;
	int cont=0;
	u8 fx=0;

	while (1)     //������Ч��
	{
		if(fx==0)
		{
			cont+=10;
			if(cont>=90)
				fx=1;	
		}
		else
		{
			cont-=10;
			if(cont<=0)
				fx=0;	
		}
		
		
		//LED��->��
		PWM_Config(100, cont);//����PWMƵ��Ϊ100HZ,ռ�ձ�Ϊ10%
		for (i = 0; i < 30000; i++); //���������ʱ��Լ76ms

		PWM_Close();
	} 
}


/*��ʱ��T0�жϷ���*/
void Timer0_ISP() interrupt 1
{
	//�ж�PWM����������Ǹߵ�ƽ���ǵ͵�ƽ
	if (PWMOUTLED)
	{
		TH0 = lowHR0;
		TL0 = lowLR0;//װ�ص͵�ƽ������ֵ
		PWMOUTLED = 0;//PWM����͵�ƽ
	}
	else
	{
		TH0 = heighHR0;
		TL0 = heighLR0;//��װװ�ظߵ�ƽ������ֵ
		PWMOUTLED = 1;//PWM����ߵ�ƽ
	}
}


