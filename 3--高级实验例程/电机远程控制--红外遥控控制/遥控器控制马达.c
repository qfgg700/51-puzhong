/*
ʵ��˵����
	�����Լ�ʹ�õ�LCD1602�Ƿ����ת�Ӱ壬�������ת�Ӱ�ļ�Ϊ4λ������LCD.Hͷ�ļ���
	����#define LCD1602_4PINS�򿪣���������ʹ�õ�LCD1602��8λ������Ĭ�Ͻ��ú�ע�͡�
ʵ����ߣ�
	1��LCD1602Һ��ģ��-->��Ƭ���ܽ�
	�ο�LCD1602Һ����ʾʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	2���������ģ��-->��Ƭ���ܽ� 
	�ο�����ң��ʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	3��ֱ�����ģ��-->��Ƭ���ܽ�
	IN1-->P10���ο�ֱ�����ʵ����ߣ�
	
ʵ������
	LCD1602��ʾң������ֵ��������ʾ���״̬��ON/OFF����ͨ������һ���������Կ�������������ֹͣ
*/


#include<reg51.h>
#include"lcd.h"

typedef unsigned char uint8;
typedef unsigned int uint16;


sbit moto=P1^0;

uint8 d1[8]=" RED IR ";
uint8 d2[12]="MOTO STATE: ";
uint8 num[16]="0123456789ABCDEF";

uint8 irtime;	  //����ʱ��
uint8 startflag;  //��ʼ���
uint8 bitnum;	  //bitλ��
uint8 irdata[33]; //�Ĵ�û�и�bit��ʱ��
uint8 irreceok;	  //������ɱ�־λ
uint8 ircode[4];  //�����յ��ĸ��ֽ����ݱ���
uint8 irprosok;	  //�������ݴ����־λ
uint8 irdisp[8];  //���ĸ��ֽڵ�16����ͨ��ת�����ʮ���ƴ洢
uint8 dat1,dat2;
uint8 keynum;
uint8 motoflag;
void delay(uint16 i)
{
	while(i--);
}
void lcdwrc(uint8 c)
{
	LcdWriteCom(c);
}
void lcdwrd(uint8 dat)
{
	LcdWriteData(dat);	
}
void lcd_init()
{
	LcdInit();
}
void int0init()
{
	EA=1;
	EX0=1;
	IT0=1;
}
void time0init()
{
	TMOD=0X02;	//���ö�ʱ��0ģʽ2.��ģʽΪ�Զ�װ��ģʽ
	TH0=0X00;
	TL0=0X00;//�趨������ֵ��ÿ��TL0������255ʱ��TH0�����Լ������ݸ�TL0�������¼���
	TR0=1;
	ET0=1;
	EA=1;	
}
void irpros() //����������ݴ��� ������������0����1
{
	uint8 i,j,value;
	uint8 k=1;	//������ȥ����������k=1��
	for(j=0;j<4;j++)  //ȡ����һ֡�����е��ĸ��ֽ�����
	{
		for(i=0;i<8;i++)	//ȡ����һ���ֽڵ�����		
		{
			value>>=1;
			if(irdata[k]>6)
			{
				value=value|0x80;
			}
			k++;
		}
		ircode[j]=value;
	}
	irprosok=1;
}
void irwork()  //���ĸ��ֽڵ�����ת����10��������ʾ
{
	irdisp[0]=ircode[0]/16;//��ʾ��λ
	irdisp[1]=ircode[0]%16;//��ʾ��λ  
	irdisp[2]=ircode[1]/16;
	irdisp[3]=ircode[1]%16;	 //ͬһ��ң������2���ֽڵ������������ǲ���ı�ģ��ı��ֻ�������뼰����
	irdisp[4]=ircode[2]/16;
	irdisp[5]=ircode[2]%16;
	irdisp[6]=ircode[3]/16;
	irdisp[7]=ircode[3]%16;
	dat1=irdisp[4];
	dat2=irdisp[5];
	keynum++;
}
void display()
{
	uint8 i;
	lcdwrc(0x00+0x80);
	for(i=0;i<8;i++)
	{
		lcdwrd(d1[i]);
	}	
	for(i=4;i<6;i++)
	{
		lcdwrd(num[irdisp[i]]);
	} 
	lcdwrc(0x40+0x80);
	for(i=0;i<12;i++)
	{
		lcdwrd(d2[i]);
	}
	if(motoflag==1)			//���״̬��ʾ
	{
		lcdwrd('O');
		lcdwrd('N');
		lcdwrd(' ');
	}
	else
	{
		lcdwrd('O');
		lcdwrd('F');
		lcdwrd('F');	
	}
}
void motopros()
{
	if((dat1==4)&&(dat2==5)&&(keynum==1))  //���µ�һ�ΰ��������
	{			
		dat1=0;
		dat2=0;
		motoflag=1;
		moto=1;						
	}
			
	if((dat1==4)&&(dat2==5)&&(keynum>=2))	//���µڶ��δ˰���ʱ�ر����
	{
		dat1=0;
		dat2=0;
		keynum=0;
		motoflag=0;
		moto=0;			
	}				
}
void main()
{
	moto=0;	
	lcd_init();
	int0init();
	time0init();
	while(1)
	{
		if(irreceok)
		{
			irreceok=0;
			irpros();
			if(irprosok)
			{
				irwork();
			}
		}
		motopros();
		display();
	}		
}
void time0() interrupt 1
{
	irtime++;//ÿ����һ�ξ�˵����ʱʱ��Ϊ256us��	
}
void init0() interrupt 0
{
	if(startflag)
	{
		if(irtime>32)//��������룬������������ʱ�����һ�μ���ʱ�䣬����Ҫ���ٴ�
		{
			bitnum=0;	
		}
		irdata[bitnum]=irtime;
		irtime=0;
		bitnum++;
		if(bitnum==33)
		{
			bitnum=0;
			irreceok=1;//һ֡�������ݽ�����ɱ�־
		}
	}
	else
	{
		startflag=1;//����ʼ��־λ��1���ȵ��´ν����жϼ��ɽ���if���
		irtime=0;//����ʼ֮ǰ�ļ�����ʱ�����㡣�ȵ��´ν����жϵ�ʱ�����������������ʱ��
	}
}