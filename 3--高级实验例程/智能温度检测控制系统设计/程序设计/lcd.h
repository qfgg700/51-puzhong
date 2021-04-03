#ifndef _lcd_H
#define _lcd_H

#include "public.h"


/**********************************
��ʹ�õ���4λ���ݴ����ʱ���壬
ʹ��8λȡ���������
**********************************/
#define LCD1602_4PINS


//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^7;
sbit LCD1602_RW=P2^5;
sbit LCD1602_RS=P2^6;



void LCD_WriteCmd(u8 cmd);
void LCD_WriteData(u8 dat);
void LcdInit();
void LCD_Clear();
void LCD_Dispstring(u8 x,u8 line,u8 *p);




#endif