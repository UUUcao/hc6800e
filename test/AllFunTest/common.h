#ifndef ___COMMON_H_H___
#define ___COMMON_H_H___
#include < reg51.h >
#include < intrins.h >
#include "key.h"

#define uchar unsigned char
#define uint  unsigned int

#define delayNOP() ; {_nop_() ;_nop_() ;_nop_() ;_nop_() ;} ; //4us

//�����Լ��Ľ�������
sbit BEEP    = P1^5 ;         //������������
sbit LED     = P2^0 ;		  //�Զ����LED������˿�
sbit LCD_RS  = P2^6 ;         //1602 LCD RS �˿�   
sbit LCD_RW  = P2^5 ;		  //1602 LCD RW �˿�   
sbit LCD_EN  = P2^7 ;		  //1602 LCD EN �˿�   
//138�������ܽŶ���
sbit LS138A  = P2^2;          //138�ܽŶ���
sbit LS138B  = P2^3;
sbit LS138C  = P2^4; 
sbit IRIN    = P3^2 ;         //���������������
sbit DQ      = P3^7 ;         //����DS18B20�˿�DQ  

//������������
void delay_ms(int ms);            //ms*1ms
void delay_us(unsigned int num);  //num*1us
void IRdelay(char x);             //x*0.14ms
void IRInit(void);				  //����˿ڳ�ʼ��

void lcd_init(void);			  //LCD1602�˿ڳ�ʼ��
void led_display(unsigned char value);											       //led��ʾ
void lcd_display(unsigned char value, unsigned char pos, unsigned char is_decimal );   //lcd��ʾ ������������ λ�� ��ʮ���Ʊ�ʶ��
void process_message(void);		  //������Ϣ������������ѭ���У�
uchar is_quit(void);			  //�ж��Ƿ����˳�������
void lcd_temp_menu(void);
void lcd_welcome_menu(void);
void beep(void);

//ȫ��ң��������ֵ������
extern unsigned char  ir_key_value;	 //ȫ��ң����������ֵ����
extern unsigned char  ir_key_last;	 //TODO����δʹ�� 

#endif
