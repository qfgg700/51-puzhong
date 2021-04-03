#include "lcd.h"

/*******************************************************************************
* �� �� ��         : Lcd1602_Delay1ms
* ��������		   : ��ʱ��������ʱ1ms
* ��    ��         : c
* ��    ��         : ��
* ˵    ��         : �ú�������12MHZ�����£�12��Ƶ��Ƭ������ʱ��
*******************************************************************************/

void Lcd1602_Delay1ms(uint c)   //��� 0us
{
    uchar a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
    	
}

#ifndef 	LCD1602_4PINS	 //��û�ж������LCD1602_4PINSʱ
void LCD_WriteCmd(uchar com)	  //д������
{
	LCD1602_E = 0;     //ʹ��
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	
	LCD1602_DATAPINS = com;     //��������
	Lcd1602_Delay1ms(1);		//�ȴ������ȶ�

	LCD1602_E = 1;	          //д��ʱ��
	Lcd1602_Delay1ms(5);	  //����ʱ��
	LCD1602_E = 0;
}
#else 
void LCD_WriteCmd(uchar com)	  //д������
{
	LCD1602_E = 0;	 //ʹ������
	LCD1602_RS = 0;	 //ѡ��д������
	LCD1602_RW = 0;	 //ѡ��д��

	LCD1602_DATAPINS = com;	//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

//	Lcd1602_Delay1ms(1);
	LCD1602_DATAPINS = com << 4; //���͵���λ
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	 //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
/*******************************************************************************
* �� �� ��         : LcdWriteData
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/		   
#ifndef 	LCD1602_4PINS		   
void LCD_WriteData(uchar dat)			//д������
{
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //д��ʱ��
	Lcd1602_Delay1ms(5);   //����ʱ��
	LCD1602_E = 0;
}
#else
void LCD_WriteData(uchar dat)			//д������
{
	LCD1602_E = 0;	  //ʹ������
	LCD1602_RS = 1;	  //ѡ��д������
	LCD1602_RW = 0;	  //ѡ��д��

	LCD1602_DATAPINS = dat;	//����4λ�Ľ����ǽӵ�P0�ڵĸ���λ�����Դ��͸���λ���ø�
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4; //д�����λ
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;	  //д��ʱ��
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
/*******************************************************************************
* �� �� ��       : LcdInit()
* ��������		 : ��ʼ��LCD��
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/		   
#ifndef		LCD1602_4PINS
void LcdInit()						  //LCD��ʼ���ӳ���
{
 	LCD_WriteCmd(0x38);  //����ʾ
	LCD_WriteCmd(0x0c);  //����ʾ����ʾ���
	LCD_WriteCmd(0x06);  //дһ��ָ���1
	LCD_WriteCmd(0x01);  //����
	LCD_WriteCmd(0x80);  //��������ָ�����
}
#else
void LCD_Init()						  //LCD��ʼ���ӳ���
{
	LCD_WriteCmd(0x32);	 //��8λ����תΪ4λ����
	LCD_WriteCmd(0x28);	 //����λ���µĳ�ʼ��
	LCD_WriteCmd(0x0c);  //����ʾ����ʾ���
	LCD_WriteCmd(0x06);  //дһ��ָ���1
	LCD_WriteCmd(0x01);  //����
	LCD_WriteCmd(0x80);  //��������ָ�����
}
#endif



void LCD_Clear()
{
	LCD_WriteCmd(0x01);
	LCD_WriteCmd(0x80);	
}

//���κ�λ����ʾ�ַ���

void LCD_Dispstring(u8 x,u8 line,u8 *p)	  
{
	char i=0;
	if(line<1)	   //��һ����ʾ
	{	
		while(*p!='\0')
		{
			if(i<16-x)
			{
				LCD_WriteCmd(0x80+i+x);	
			}
			else
			{
				LCD_WriteCmd(0x40+0x80+i+x-16);	
			}
			LCD_WriteData(*p);
			p++;
			i++;	
		}	
	}
	else		   //��2����ʾ
	{
		while(*p!='\0')
		{
			if(i<16-x)
			{
				LCD_WriteCmd(0x80+0x40+i+x);	
			}
			else
			{
				LCD_WriteCmd(0x80+i+x-16);	
			}
			LCD_WriteData(*p);
			p++;
			i++;	
		}	
	}		
}


