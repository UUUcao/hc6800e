/********************************************************************************
;*         	        �����ʵ��(������LED)                                       *
;																				*
;     ��ʱʵ��LED��ˮ��Ч��   p2�ڰ˸���������ơ������˼Ĵ������м���	        *
;     ���ӷ����� JP11��P2����JP1(LED��) ��8PIN������������					    *
;********************************************************************************/
#include "common.h"

void ledflash(void)			        
{
	LED = 0;
	do 
	{
    	LED = ~LED;
    	delay_ms(300) ;
	}while( is_quit());
}

void ledon(void)
{
	do 
	{
    	LED = 0;
    	delay_ms(300) ;
	}while( is_quit());
}

void ledoff(void)
{
	do 
	{
    	LED = 1;
    	delay_ms(300) ;
	}while( is_quit());
}

void paomadeng(void)
{
	unsigned char i;
	char LED_STATUS = 0xFE;
	do 
	{
		LED_STATUS = 0xFE;
		for (i = 0 ;i < 8 ; i++)
		{ 
			P2  = LED_STATUS;
			delay_ms(100);
			LED_STATUS = LED_STATUS << 1;	 // ����
			LED_STATUS = LED_STATUS | 0x01;	 // ��λ�󣬺����λΪ�ߵ�ƽ
			if (LED_STATUS == 0x7f)	 break;	 //��ǰ�˳� FOR ѭ��
		}
		for (i = 0 ;i < 8 ; i++)
		{
			P2  = LED_STATUS;
			delay_ms(100);
			LED_STATUS = LED_STATUS >> 1;	 // ����
			LED_STATUS = LED_STATUS | 0x80;	 // ��λ�󣬺����λΪ�ߵ�ƽ
		}
	}while(is_quit());
}

