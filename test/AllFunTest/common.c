#include "common.h"


//ȫ��ң��������ֵ����
unsigned char  ir_key_value = 0x00;
unsigned char  ir_key_last  = 0x00;

/*ms����ʱ����                                        */
/*******************************************************************/
void delay_ms(int ms)		     //ms*1ms
{
	unsigned char y ;
	while(ms--)
	{
		for(y = 0 ; y<250 ; y++)
		{
			_nop_() ;
			_nop_() ;
			_nop_() ;
			_nop_() ;
		}
	}
}

/*us����ʱ����                                        */
/*******************************************************************/
void delay_us(unsigned int num)	 //num*1us
{
	while( --num ) ;
}

/*0.14ms����ʱ����                                        */
/**********************************************************/
void IRdelay(unsigned char x)    //x*0.14ms
{
	unsigned char i;
	while(x--)
	{
 		for (i = 0; i<13; i++) {}
	}
}




 




