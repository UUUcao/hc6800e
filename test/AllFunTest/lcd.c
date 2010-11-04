/*******************************************************************/
/*  LCD1602��ʾ����                                                */
/*  ���ӷ�����JP10 ��P0����LCD1602�ӿ�����                         */
/*  ��573��������138���������ƺ������                             */
/*******************************************************************/
#include "common.h"

uchar code cdis1[ ] = {" REN WEN LONG   "} ;
uchar code cdis2[ ] = {" WENDU:    .  C "} ;
uchar code cdis3[ ] = {" DS18B20  ERR0R "} ;
uchar code cdis4[ ] = {"  PLEASE CHECK  "} ;
uchar code cdis5[ ] = {" REN WEN LONG   "} ;
uchar code cdis6[ ] = {" IR CODE: --H "} ;
uchar code cdis7[ ] = {"Welcome To Here!"} ;

unsigned char display[5]        = {0x00,0x00,0x00,0x00,0x00} ;
unsigned char code mytab[8]     = {0x0C,0x12,0x12,0x0C,0x00,0x00,0x00,0x00} ;

/******************************************************************/
/*���LCDæ״̬                                                   */
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�   */
/******************************************************************/ 
bit lcd_busy()
{                          
    bit result ;
    LCD_RS = 0 ;
    LCD_RW = 1 ;
    LCD_EN = 1 ;
    delayNOP() ;
    result = (bit)(P0&0x80) ;
    LCD_EN = 0 ;
    return(result) ; 
}
/*дָ�����ݵ�LCD                                                  */
/*RS=L��RW=L��E=�����壬D0-D7=ָ���롣          */
/*******************************************************************/
void lcd_wcmd(uchar cmd)
{                          
   while(lcd_busy()) ;
    LCD_RS = 0 ;
    LCD_RW = 0 ;
    LCD_EN = 0 ;
    _nop_() ;
    _nop_() ; 
    P0 = cmd ;
    delayNOP() ;
    LCD_EN = 1 ;
    delayNOP() ;
    LCD_EN = 0 ;  
}
/*******************************************************************/
/*д��ʾ���ݵ�LCD                            */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�       */
/*******************************************************************/
void lcd_wdat(uchar dat)
{                          
   while(lcd_busy()) ;
    LCD_RS = 1 ;
    LCD_RW = 0 ;
    LCD_EN = 0 ;
    P0 = dat ;
    delayNOP() ;
    LCD_EN = 1 ;
    delayNOP() ;
    LCD_EN = 0 ; 
}

/*  LCD��ʼ���趨                                                  */
/*******************************************************************/
void lcd_init()
{ 
    delay_ms(15) ;   
    lcd_wcmd(0x01) ;      //���LCD����ʾ����            
    lcd_wcmd(0x38) ;      //16*2��ʾ��5*7����8λ����
    delay_ms(5) ;
    lcd_wcmd(0x38) ;         
    delay_ms(5) ;
    lcd_wcmd(0x38) ;         
    delay_ms(5) ;
    lcd_wcmd(0x0c) ;      //��ʾ�����ع��
    delay_ms(5) ;
    lcd_wcmd(0x06) ;      //�ƶ����
    delay_ms(5) ;
    lcd_wcmd(0x01) ;      //���LCD����ʾ����
    delay_ms(5) ;
}
/*  �趨��ʾλ��                                    */
/*******************************************************************/
void lcd_pos(uchar pos)
{                          
  lcd_wcmd(pos | 0x80) ;  //����ָ��=80+��ַ����
}
/*�Զ����ַ�д��CGRAM                                   */
/*******************************************************************/
void  writetab()  
{  
    unsigned char i ;
    lcd_wcmd(0x40) ;            //дCGRAM
    for (i = 0 ; i< 8 ; i++)       
    lcd_wdat(mytab[ i ]) ;        
}


//ͨ�ó��������������д������
void lcd_display(unsigned char value, unsigned char pos, unsigned char is_decimal )
{ 
	if ( is_decimal & 0xFF )
	{
		display[1] = (value/10) + 0x30;	 //ʮλ
		display[0] = (value%10) + 0x30;	 //��λ
	}
	else
	{
		display[1] = (value>>4) & 0x0F;
		if (display[1]>9)
		  display[1] = display[1] + 0x37;
		else
		  display[1] = display[1] + 0x30;
		display[0] = value & 0x0F;
		if (display[0]>9)
		  display[0] = display[0] + 0x37;
		else
		  display[0] = display[0] + 0x30;
	}

    /*if(display[1]==0x30)        //��λΪ0������ʾ
	{ 
		display[1] = 0x20 ;              
	}*/
	lcd_pos( (pos&0xFF) );
	lcd_wdat(display[1]);//�߰�λ
	lcd_wdat(display[0]);//�Ͱ�λ
}

void lcd_temp_menu(void)
{
    uchar  m ;
    lcd_init() ;                //��ʼ��LCD 
            
    lcd_pos(0) ;                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
    m = 0 ;
    while(cdis1[m] != '\0')
    {                           //��ʾ�ַ�
       lcd_wdat(cdis1[m]) ;
       m++ ;
    }
    lcd_pos(0x40) ;             //������ʾλ��Ϊ�ڶ��е�1���ַ�
    m = 0 ;
    while(cdis2[m] != '\0')
    {
       lcd_wdat(cdis2[m]) ;     //��ʾ�ַ�
       m++ ;
    }
    writetab() ;                //�Զ����ַ�д��CGRAM
    delay_ms(5) ;
    lcd_pos(0x4d) ;
    lcd_wdat(0x00) ;            //��ʾ�Զ����ַ�
}

void lcd_ir_menu(void)
{
    uchar  m ;
    lcd_init() ;                //��ʼ��LCD 
            
    lcd_pos(0) ;                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
    m = 0 ;
    while(cdis5[m] != '\0')
    {                           //��ʾ�ַ�
       lcd_wdat(cdis5[m]) ;
       m++ ;
    }
    lcd_pos(0x40) ;             //������ʾλ��Ϊ�ڶ��е�1���ַ�
    m = 0 ;
    while(cdis6[m] != '\0')
    {
       lcd_wdat(cdis6[m]) ;     //��ʾ�ַ�
       m++ ;
    }
}

void lcd_welcome_menu(void)
{
    uchar  m ;
    lcd_init() ;                //��ʼ��LCD 
    lcd_pos(0) ;                //������ʾλ��Ϊ��һ�еĵ�1���ַ�
    m = 0 ;
    while(cdis5[m] != '\0')
    {                           //��ʾ�ַ�
       lcd_wdat(cdis5[m]) ;
       m++ ;
    }
    lcd_pos(0x40) ;             //������ʾλ��Ϊ�ڶ��е�1���ַ�
    m = 0 ;
    while(cdis7[m] != '\0')
    {
       lcd_wdat(cdis7[m]) ;     //��ʾ�ַ�
       m++ ;
    }
}

/* ������     */
/************************************/
void test_1602()
{
	lcd_ir_menu();
	while(is_quit())
	{
		if ( ir_key_last != ir_key_value )
		{
			lcd_display( ir_key_value, 0x4A, 0x00 );
			ir_key_last = ir_key_value ;
		}
		else
		{
			delay_ms(10);
		}
	}
}
