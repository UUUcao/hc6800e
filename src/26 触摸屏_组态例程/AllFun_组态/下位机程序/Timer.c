#include "system.h"

tab_1302[7]={0,11,1,29,7,2,8};     //��ʼ��ʱʱ����������

unsigned char l_tmpdisplay[8];

bit ReadRTC_Flag;//�����DS1302��־

code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //���ʱ�������� ���λ��дλ
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  
code unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40}; 
			         //��������� 0-9  '-' 'Ϩ�𡮱�

 void Write_Ds1302_byte(unsigned char temp); 
 void Write_Ds1302( unsigned char address,unsigned char dat );
 unsigned char Read_Ds1302 ( unsigned char address );
 void Read_RTC(void);//read RTC 
 void InitTimer(void); //set RTC 
 //void InitTIMER0(void);//inital timer0

/******************************************************************/
/*                   дһ���ֽ�                                   */
/******************************************************************/
void Write_Ds1302_Byte(unsigned  char temp) 
{
 xdata unsigned char i;
 for (i=0;i<8;i++)     	//ѭ��8�� д������
  { 
   SCK=0;
     SData=temp&0x01;     //ÿ�δ�����ֽ� 
     temp>>=1;  		//����һλ
     SCK=1;
   }
}   
/******************************************************************/
/*                  д��DS1302                                    */
/******************************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();                    //����
 	Write_Ds1302_Byte(address);	//���͵�ַ
 	Write_Ds1302_Byte(dat);		//��������
 	RST=0;  		            //�ָ�
}
/******************************************************************/
/*                   ����DS1302����                               */
/******************************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
  xdata	 unsigned  char i,temp=0x00;
 	RST=0;
	_nop_();
	_nop_();
 	SCK=0;
	_nop_();
	_nop_();
 	RST=1;
	_nop_();
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 		//ѭ��8�� ��ȡ����
 	{		
 		if(SData)
 		temp|=0x80;			//ÿ�δ�����ֽ�
		SCK=0;
		temp>>=1;			//����һλ
		_nop_();
	   _nop_();
	   _nop_();
 		SCK=1;
	} 
 	RST=0;
	_nop_();	          	//����ΪDS1302��λ���ȶ�ʱ��
 	_nop_();
	RST=0;
	SCK=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCK=1;
	_nop_();
	_nop_();
	SData=0;
	_nop_();
	_nop_();
	SData=1;
	_nop_();
	_nop_();
	return (temp);			//����
}
/******************************************************************/
/*                   ��ʱ������                                   */
/******************************************************************/
void Read_RTC(void)	        //��ȡ ����
{
 xdata unsigned char i,*p;
 p=read_rtc_address; 	    //��ַ����
 for(i=0;i<7;i++)		    //��7�ζ�ȡ ���ʱ��������
 {
  D[6+i]=Read_Ds1302(*p);
  p++;
 }
}
/******************************************************************/
/*                  �趨ʱ������                                  */
/******************************************************************/
void InitTimer(void)		    //�趨 ����
{
 xdata unsigned char i,*p;//tmp;
/*	for(i=0;i<7;i++){       //BCD����
		tmp=l_tmpdate[i]/10;
		l_tmpdate[i]=l_tmpdate[i]%10;
		l_tmpdate[i]=l_tmpdate[i]+tmp*16;
	}	 */
 	Write_Ds1302(0x8E,0X00);   //����д��
	
 	p=write_rtc_address;	//����ַ	
 	for(i=0;i<7;i++)		//7��д�� ���ʱ��������
 	{
		 // Write_Ds1302(*p,l_tmpdate[i]);
		 Write_Ds1302(*p,tab_1302[i]);
		 
 		 p++;  
	 }
	 Write_Ds1302(0x8E,0x80);  //��ֹд��

}
