/*
ʵ��˵����
	�����Լ�ʹ�õ�LCD1602�Ƿ����ת�Ӱ壬�������ת�Ӱ�ļ�Ϊ4λ������LCD.Hͷ�ļ���
	����#define LCD1602_4PINS�򿪣���������ʹ�õ�LCD1602��8λ������Ĭ�Ͻ��ú�ע�͡�
ʵ����ߣ�
	1��LCD1602Һ��ģ��-->��Ƭ���ܽ�
	�ο�LCD1602Һ����ʾʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	2��DS18B20�¶ȴ�����ģ��-->��Ƭ���ܽ� 
	�ο�DS18B20�¶ȴ�����ʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	3��ֱ�����ģ��-->��Ƭ���ܽ�
	IN1-->P10���ο�ֱ�����ʵ����ߣ�
	
ʵ������
	LCD1602��ʾ�¶�ֵ���¶ȱ仯�ı�ֱ�������ת�ٶȣ����������ͨ���޸ĵ�������ٶ�ʵ�ֵ��٣�
	�¶ȵ��ٷ�Χ��main����������
*/

#include<reg51.h>
#include<intrins.h>
#include"lcd.h"
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;

sbit dq=P3^7;
sbit moto=P1^0;	//ֱ�����
uint8 d[16]="   tempertury   ";
uint8 num[10]="0123456789";
uint8 a,c,temp;
void delay(uint16 i)
{
	while(i--);
}
void wrc(uint8 c)
{
	LcdWriteCom(c);
}
void wrd(uint8 dat)
{
	LcdWriteData(dat);
}
void lcd_init()
{
	LcdInit();
}
void ds18b20init()		//18b20�ĳ�ʼ��
{
	dq=1;
	delay(1);
	dq=0;
	delay(80);
	dq=1;
	delay(5);
	dq=0;
	delay(20);
	dq=1;
	delay(35);
}
void ds18b20wr(uint8 dat)	  //18b20д����
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		dq=0;
		dq=dat&0x01;
		dat>>=1;
		delay(8);//��ʱ����ֻ����һ���ʱ��Ҫ����׼ȷ������ʱ��������15us
		dq=1;
		delay(1);
	}	
}
uint8 ds18b20rd()		  //18b20������
{
	uint8 value,i;
	for(i=0;i<8;i++)	
	{
		dq=0;
		value>>=1;
		dq=1;
		if(dq==1)value|=0x80;
		delay(8);//����һ��Ҳ��ʱ��Ҫ���ر�׼ȷ�����γ���������60us		
	}
	return value;
}
uint8 readtemp()			  //��ȡ�¶�����Ҫ��λ��
{
	uint8 b;
	ds18b20init();		//��ʼ��
	ds18b20wr(0xcc);   //���ͺ���ROMָ��
	ds18b20wr(0x44);   //�����¶�ת��ָ��
	delay(100);
	ds18b20init();	   //��ʼ��
	ds18b20wr(0xcc);   //���ͺ���ROMָ��
	ds18b20wr(0xbe);   //�����ݴ���ָ��
	a=ds18b20rd();	 //�¶ȵĵͰ�λ
	b=ds18b20rd();	 //�¶ȵĸ߰�λ
	b<<=4;			 //ssss s***��sΪ��־λs=0��ʾ�¶�ֵΪ������s=1�¶�ֵΪ����
	c=b&0x80;		//�¶�������־λȷ��
	b+=(a&0xf0)>>4;
	a=a&0x0f;	  //�¶ȵ�С������
	return b;
}
void display()
{
	uint16 i,k;
	float dio;
	dio=a*0.0625;
	k=dio*10000;//ȡС�������λ��Ч����
	wrc(0x80+0x00);
	for(i=0;i<16;i++)
	{
		wrd(d[i]);
	}
	wrc(0x80+0x43);
	if(c==0x80)	//��ȡ�����¶ȼ�Ϊ���룬Ҫ����ת����Դ��
	{
		wrd('-');
		temp=temp-1;
		temp=(~temp)|0x80;//�����Ĳ��뼴Ϊ����+1���������ķ���Ϊ��Դ��ȡ�������˷���λ�������Ĳ�����������ķ������������Դ��
	}	
	else
	{
		wrd('+');
		temp=(temp);
	}			
	wrd(num[temp/100]);
	wrd(num[temp%100/10]);
	wrd(num[temp%100%10]);
	wrd('.');
	wrd(num[k/1000]);
	wrd(num[k%1000/100]);
	wrd(0xdf);
	wrd('C');
}
void motospeed(uint16 i)
{
	while(i--)
	{
		moto=~moto;
	}		
}
void main()
{
	moto=0;
	lcd_init();
	while(1)
	{
		temp=readtemp();
		display();
		//������¶Ⱥ��ٶȷ�Χ�Լ���������
		if(temp>27&&temp<29) //�ٶ�1
			motospeed(1);
		if(temp>23&&temp<24) //�ٶ�2
			motospeed(10);
		if(temp>29&&temp<35) //�ٶ�3
			motospeed(100);			
	}		
}