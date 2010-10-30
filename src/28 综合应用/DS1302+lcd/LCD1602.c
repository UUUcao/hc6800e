# include "SYSTEM.H"													 /*--------------------------------------------------------------*/

/*--------------------------------------------------------------*/
//#ifndef __LCD1602_8_H__
//#define __LCD1602_8_H__



/*--------------------------------------------------------------*/
//LCD�ӿڶ���					
sfr		 io	= 0x80;				//P0-0x80,P1-0x90,P2-0xA0,P3-0xB0;
sbit	 rs = P2^6;				//LCD����/����ѡ���(H/L)
sbit	 rw = P2^5;				//LCD��/дѡ���(H/L)
sbit	 ep = P2^7;				//LCDʹ�ܿ���
sbit     bz = io^7;				//LCDæ��־λ


/*--------------------------------------------------------------*/
//��������		
void lcd_busy(void);					//����LCDæµ״̬����
void lcd_wcmd(unsigned char cmd);		//д��ָ�LCD����
void lcd_wdat(unsigned char dat);		//д�����ݵ�LCD����
void lcd_pos (unsigned char x, bit y);	//LCD����ָ��λ�ó���
void lcd_sef_chr();
//void printc(unsigned char CHAR);		//��ʾ�ַ�

void lcd_init(void);					//LCD��ʼ���趨����


/*--------------------------------------------------------------*/
//����LCDæµ״̬
void lcd_busy(void)
{	
	do{
		ep = 0;
		rs = 0;		//ָ��
		rw = 1;		//����
		io = 0xff;
		ep = 1;
		_nop_();	//�ߵ�ƽ����	1us	
	}while(bz);		//bz=1��ʾæ,bz=0��ʾ����
	ep = 0;		
}


/*--------------------------------------------------------------*/
//д��ָ�LCD
void lcd_wcmd(unsigned char cmd)
{							
	lcd_busy();	//���æ
	rs = 0;		//ָ��
	rw = 0;		//д��
	ep = 1;	
	io = cmd;	//ָ��
	ep = 0;		//�½�����Ч	
}


/*--------------------------------------------------------------*/
//д�����ݺ���
void lcd_wdat(unsigned char Data)
{
	lcd_busy();  //���æ
	rs = 1;		 //����
	rw = 0;		 //д��
	ep = 1;
	io = Data;	 //����
	ep = 0;		 //�½�����Ч
}


/*--------------------------------------------------------------*/
//LCD����ָ��λ�ó���
void lcd_pos(unsigned char x, bit y)
{						
	if(y)lcd_wcmd(x|0xc0);	//y=1,�ڶ�����ʾ;y=0,��һ����ʾ		0<=x<16
	else lcd_wcmd(x|0x80);	//����ָ��=80+��ַ��(00H~27H,40H~67H)
		
}


/*--------------------------------------------------------------*/
//��ʾ�ַ�
/*
void printc(unsigned char CHAR)
{
	lcd_wdat(CHAR);
}
*/


/*--------------------------------------------------------------*/
//��ʾ�ַ���
void prints(unsigned char *string)
{
	/*unsigned char i;
	//do{lcd_wdat(string[i]);i++;}while(string[i]!='\0');//���ɴ����forѭ����
	for(i=0;string[i]!='\0';i++)lcd_wdat(string[i]);	 //��ʱ��forѭ��Ҳ�ù�*/

	while(*string) {lcd_wdat(*string);string++;}

}

 


/*--------------------------------------------------------------*/
//LCD��ʼ���趨
void lcd_init()
{						
	lcd_wcmd(0x38);		//����LCDΪ16X2��ʾ,5X7����,��λ���ݽӿ�
	lcd_wcmd(0x06);		//LCD��ʾ����ƶ�����(����ַָ���1,������ʾ���ƶ�)
	lcd_wcmd(0x0c);		//LCD����ʾ���������(��겻��˸,����ʾ"_")
	lcd_wcmd(0x01);		//���LCD����ʾ����


	


}


/*--------------------------------------------------------------*/
//#endif
