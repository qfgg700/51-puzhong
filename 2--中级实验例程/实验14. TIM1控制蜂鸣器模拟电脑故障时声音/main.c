/* ��ʱһ��ʱ�������Ʒ�������������,ģ����Թ���ʱ������  */
//BEEP-->P15
#include<reg52.h>
typedef unsigned char u8;
typedef unsigned int u16;
sbit beep=P1^5;
void tim1init()//��ʱ��1��ʼ��
{
	TMOD=0X10;	//t1 ��ʽ1
	TH1=0XFC;
	TL1=0X18;//��ʱ1ms
	TR1=1;	 //��t1��ʱ��
}
void main()
{
	u16 j;
	tim1init();
	while(1)
	{
		if(TF1==1)
		{
			TF1=0;
			TH1=0XFC;
			TL1=0X18;//��ʱ1ms
			j++;
			if(j==1)	 //1ms
			{	
				j=0;
				beep=~beep;
			}
		}
	}	
}
