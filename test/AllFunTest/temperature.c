/*************************�˲���Ϊ18B20����������*************************************/
#include "common.h"

bit presence  ;
unsigned char data  temperture_data[2]        = {0x00,0x00} ;
unsigned char data  temperture_display[5]     = {0x00,0x00,0x00,0x00,0x00} ;
unsigned char code  myditab[16]               = {0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,
                                                 0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09} ;
extern void lcd_pos(uchar pos);
extern void lcd_wdat(uchar dat);
/*��ʼ��ds1820                                      */
/*******************************************************************/
Init_DS18B20(void)
{  
     DQ = 1 ;           //DQ��λ
     delay_us(8) ;      //������ʱ
     DQ = 0 ;           //��Ƭ����DQ����
     delay_us(90) ;     //��ȷ��ʱ ���� 480us
     DQ = 1 ;           //��������
     delay_us(8) ;
     presence = DQ ;    //���=0���ʼ���ɹ� =1���ʼ��ʧ��
     delay_us(100) ;
     DQ = 1 ; 
     return(presence) ; //�����źţ�0=presence,1= no presence
}

/* ��һ���ֽ�                     */
/*******************************************************************/
ReadOneChar(void)
{
	unsigned char i = 0 ;
	unsigned char dat = 0 ;
	for (i = 8 ; i > 0 ; i--)
	{
    	DQ = 0 ; // �������ź�
    	dat >>= 1 ;
    	DQ = 1 ; // �������ź�
		if(DQ)
			dat |= 0x80 ;
		delay_us(4) ;
	}
    return (dat) ;
}

/* дһ���ֽ�                                         */
/*******************************************************************/
WriteOneChar(unsigned char dat)
{
	unsigned char i = 0 ;
	for (i = 8 ; i > 0 ; i--)
	{
    	DQ = 0 ;
    	DQ = dat&0x01 ;
    	delay_us(5) ;
	    DQ = 1 ;
    	dat>>=1 ;
  	}
}

/* ��ȡ�¶�                            */
/*******************************************************************/
Read_Temperature(void)
{
     Init_DS18B20() ;
  
     WriteOneChar(0xCC) ;  // ����������кŵĲ���
     WriteOneChar(0x44) ;  // �����¶�ת��

     Init_DS18B20() ;
     WriteOneChar(0xCC) ;  //����������кŵĲ���
     WriteOneChar(0xBE) ;  //��ȡ�¶ȼĴ���

     temperture_data[0] = ReadOneChar() ;   //�¶ȵ�8λ
     temperture_data[1] = ReadOneChar() ;   //�¶ȸ�8λ 
}

/* ����ת�����¶���ʾ                                              */
/*******************************************************************/
Disp_Temperature()
{
	temperture_display[4]=temperture_data[0]&0x0f ;
  	temperture_display[0]=myditab[temperture_display[4]]+0x30 ;     //����С��λ��ֵ
  
  	temperture_display[4]=((temperture_data[0]&0xf0)>>4)|((temperture_data[1]&0x0f)<<4) ;
  	temperture_display[3]=temperture_display[4]/100+0x30 ;
  	temperture_display[1]=temperture_display[4]%100 ;
  	temperture_display[2]=temperture_display[1]/10+0x30 ;
  	temperture_display[1]=temperture_display[1]%10+0x30 ;

    if(temperture_display[3]==0x30)        //��λΪ0������ʾ
   	{ 
     	temperture_display[3]=0x20 ;              
     	if(temperture_display[2]==0x30)      //�θ�λΪ0������ʾ
     	temperture_display[2]=0x20 ;
   	}

    lcd_pos(0x48) ;             
    lcd_wdat(temperture_display[3]) ;        //��λ����ʾ 
    lcd_pos(0x49) ;             
    lcd_wdat(temperture_display[2]) ;        //ʮλ����ʾ 
    lcd_pos(0x4a) ;             
    lcd_wdat(temperture_display[1]) ;        //��λ����ʾ 
    lcd_pos(0x4c) ;             
    lcd_wdat(temperture_display[0]) ;        //С��λ����ʾ 
}

/* ������     */
/************************************/
void test_18B20()
{
    lcd_temp_menu();
	do
	{
		Read_Temperature() ;
   		Disp_Temperature() ;
	}while(is_quit()&&!presence);
	do
	{
		Init_DS18B20() ;
    	beep() ;
	}while(is_quit()&&presence);

}

