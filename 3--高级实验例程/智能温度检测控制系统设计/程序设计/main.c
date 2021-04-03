/*
ʵ��˵����
	�����Լ�ʹ�õ�LCD1602�Ƿ����ת�Ӱ壬�������ת�Ӱ�ļ�Ϊ4λ������LCD.Hͷ�ļ���
	����#define LCD1602_4PINS�򿪣���������ʹ�õ�LCD1602��8λ������Ĭ�Ͻ��ú�ע�͡�
ʵ����ߣ�
	1��LCD1602Һ��ģ��-->��Ƭ���ܽ�
	�ο�LCD1602Һ����ʾʵ����ߣ������������ڶ�Ӧ��ʵ���½���ʵ�������н���˵����
	2����������ģ��-->��Ƭ���ܽ� 
	K1-->P34
	K2-->P35
	K3-->P32
	3��������ģ��-->��Ƭ���ܽ�
	BEEP-->P15
	4��LEDģ��-->��Ƭ���ܽ�
	D1-->P24
	5��LEDģ��-->��Ƭ���ܽ�
	D1-->P24
	6���̵���ģ��-->��Ƭ���ܽ�
	RELAY-->P14
	7��ֱ�����ģ��-->��Ƭ���ܽ�
	IN1-->P10���ο�ֱ�����ʵ����ߣ�
	8��EEPROMģ��-->��Ƭ���ܽ�
	�ο�EEPROMʵ�����
	9��DS18B20ģ��-->��Ƭ���ܽ�
	�ο�DS18B20�¶ȴ�����ʵ�����
	
ʵ������
	����������ʾ�������¶Ȳ������������޷�ֵ������������Ĺ��ܣ�ϵͳ�ϵ��ʱ����ʾ���ǵ�ǰ�����¶�
	���趨���¶ȷ�ֵ�����ǿ���ͨ���������޸��¶������޷�ֵ�����ǿ����������K1��������¶ȷ�ֵ����
	���棬ÿ��һ�£��л�һ�η�ֵ���ã����·�ֵ�����棬����3��ʱ�����Զ��ص������棬���ѭ����
	�ڽ����¶ȷ�ֵ��ƽ���ʱ������ͨ��K2��K3���Է�ֵ���мӼ�����������ֻ���¶��������ֽ������ã�
	С���������ǾͲ���Ҫ�ˣ������úõ������޷�ֵ���浽AT24C02��EEPROM���ڣ�����һ�ο���ϵͳʱֻ��
	��AT24C02�ڶ�ȡ����ķ�ֵ���ݣ�������Ҫ�ظ����������޷�ֵ�������Ļ���������3��������ʵ�����¶�����
	�޷�ֵ���趨�������¶ȼ�����ϵͳ�����Ĺ��ܡ��������ǰ��¶���������Ϊ32��C����������Ϊ30��C��
	���⻹�к㶨�¶ȵĹ��ܡ����趨�������޷�ֵʱ��ϵͳ����ѵ�ǰ���¶����趨�������޷�ֵ�Աȣ�
	������������¶ȣ�����ɢ�Ƚ��н��£�ͬʱ������������������¶ȣ��������ȣ�ͬʱ������
	�����ǰ�¶ȴ������޺������¶�֮��ʱ���ر�ɢ�ȡ����ȼ��������Ӷ��ɽ��¶ȿ����ڷ�ֵ�ķ�Χ�ڡ�
*/

#include "public.h"
#include "lcd.h"
#include "temp.h"
#include "i2c.h"


sbit k3=P3^2;	//�����¶�������   
sbit k1=P3^4;	//��			   
sbit k2=P3^5;	//��			  

sbit led=P2^4; //����ָʾ��

sbit beep=P1^5;	  //����������
sbit relay=P1^4;  //�����豸
sbit moto=P1^0;   //���ɢ��


char set_templ=22,set_temph=40;  //�趨�¶�������Ĭ��ֵ
u16 temp_val;  //����ʵ���¶�
u8 mode;  //�¶�ģʽ

void Temp_DataPros()
{
	short temp;
	u8 temp_buf[5];
	temp=Ds18b20ReadTemp();
	temp_val=temp;	
	if(temp<0)
	{
		temp=-temp;
		LCD_Dispstring(2+5,0,"-");
	}
	else
	{
		LCD_Dispstring(2+5,0," ");
	}

	temp_buf[0]=temp/100+0x30;
	temp_buf[1]=temp%100/10+0x30;
	temp_buf[2]='.';
	temp_buf[3]=temp%100%10+0x30;
	temp_buf[4]='\0';
	LCD_Dispstring(2+6,0,temp_buf);		 //��ʾ�����¶�xx.x

	temp_buf[0]=set_temph/10+0x30;
   	temp_buf[1]=set_temph%10+0x30;
	temp_buf[2]='\0';
	LCD_Dispstring(5,1,temp_buf);	  //��ʾ�趨���¶�����ֵxx

	temp_buf[0]=set_templ/10+0x30;
   	temp_buf[1]=set_templ%10+0x30;
	temp_buf[2]='\0';
	LCD_Dispstring(14,1,temp_buf);	 //��ʾ�趨���¶�����ֵxx
}



#define  K1_MODE  1
#define  K2_ADD  2
#define  K3_DEC  3

//mode:  0-����ɨ��   1-����ɨ��
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key&&(k1==0||k2==0||k3==0))
	{
		delay(1000);   //����
		key=0;
		if(k3==0)
		{
			return K1_MODE;
		}
		else if(k1==0)
		{
			return K2_ADD;
		}
		else if(k2==0)
		{
			return K3_DEC;	
		}
	}
	else if(k1==1&&k2==1&&k3==1)
	{
		key=1;	
	}
	if(mode)
	{
		key=1;
	}
	return 0;	
}

void KEY_Pros()
{
	u8 key;
	u8 temph_buf[3];

	key=KEY_Scan(0);
	
	if(key==K1_MODE)   //ģʽѡ��
	{
		mode++;
		LCD_Clear();

		if(mode==1)
		{
			LCD_Dispstring(0,0,"SETH:   C");		
		}
		else if(mode==2)
		{
			LCD_Dispstring(0,1,"SETL:   C");	
		}
		else 
		{
			mode=0;
			LCD_Dispstring(2,0,"Temp:     C");
			LCD_Dispstring(0,1,"SETH:  ");
			LCD_Dispstring(9,1,"SETL:  ");	
		}
	}
	if(mode==1)	 //�¶���������
	{
		switch(key)	
		{
			case K2_ADD: 	   //��
						set_temph++;
						if(set_temph>=80)set_temph=80;
						break;
			case K3_DEC: 	 //��
						set_temph--;
						if(set_temph<=0)set_temph=0;
						break;
		}

		temph_buf[0]=set_temph/10+0x30;
		temph_buf[1]=set_temph%10+0x30;
		temph_buf[2]='\0';
	   	LCD_Dispstring(6,0,temph_buf);
		At24c02Write(0,set_temph);
	}

	else if(mode==2)  //�¶���������
	{
		switch(key)	
		{
			case K2_ADD: 	   //��
						set_templ++;
						if(set_templ>=80)set_templ=80;
						break;
			case K3_DEC: 	 //��
						set_templ--;
						if(set_templ<=0)set_templ=0;
						break;
		}

		temph_buf[0]=set_templ/10+0x30;
		temph_buf[1]=set_templ%10+0x30;
		temph_buf[2]='\0';
	   	LCD_Dispstring(6,1,temph_buf);
		At24c02Write(2,set_templ);
	}
	


}



void sound()
{
	u8 i=50;
	while(i--)
	{
		beep=!beep;
		delay(10);
	}
}
void TempData_Compare()
{
	if(temp_val>set_temph*10)	//ʵ���¶ȸ�������ֵ   ����  ɢ��
	{
		led=1;
		moto=1;
		relay=1;
		sound();		
	}
	else if(temp_val<set_templ*10)	 //ʵ���¶ȵ�������ֵ   ����  ����
	{
		led=1;
		moto=0;
		relay=0;
		sound();		
	}
	else		      //ʵ���¶�������ֵ������ֵ֮��   ȡ������  ȡ������  ȡ��ɢ��
	{
		moto=0;
		led=0;
		relay=1;
	}
}

void kai_display()
{
	if(At24c02Read(255)!=18)
	{
		At24c02Write(0,set_temph);
		At24c02Write(2,set_templ);
		At24c02Write(255,18);	
	}
	else
	{
		set_temph=At24c02Read(0);
		set_templ=At24c02Read(2);	
	}

	LCD_Dispstring(2,0,"Temp:     C");
	LCD_Dispstring(0,1,"SETH:  ");
	LCD_Dispstring(9,1,"SETL:  ");
}

void main()
{
	moto=0;
	led=0;
	relay=1;

	LCD_Init();
	kai_display();
	while(1)
	{
		if(mode==0)
			Temp_DataPros();
		KEY_Pros();	
		TempData_Compare();				
	}
}


