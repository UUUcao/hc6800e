/**************************************************************************************
*																					  *
*  ��P1�ں�JP5(��������)��������													  *
*																					  *
****************************************************************************************/

#include "SYSTEM.H"

/*--------------------------------------------------------------*/
SYSTEMTIME CurrentTime;

unsigned char k = 0;	//��ֵ
unsigned char K1,K2,K3,M1,M2,M3,K1J,K2J,K3J,K4J;	//��ֵ
unsigned char Set;	//����ֵ
char Set_Mode = 0;

void Keysan(void);
void delay200ms(void);


//������
void main(void)
{	
	lcd_init();
	//lcd_sef_chr();						//д���Զ����ַ���
	DS1302_Initial();

	 lcd_pos(0,0);	   //���� Date:��ʾλ��
	 prints("Date:");  //��ʾ Date:�ַ�
	 lcd_pos(0,1);	   //���� Time:��ʾλ��
   	 prints("Time:");  
	k = 6; 	//���ı�ʱ��  
    while(1)
	{
	  Keysan();	   //�������
	 
	    DS1302_GetTime(&CurrentTime);  //���DS1302ʱ������
		DateToStr(&CurrentTime);	   //����ת�����ַ�
		TimeToStr(&CurrentTime);	   //ʱ��ת�����ַ�
	  
	  
		lcd_pos(6,0);  //������ʾλ��
		prints(CurrentTime.DateString);
		lcd_pos(6,1);  //������ʾλ��
		prints(CurrentTime.TimeString);

		//lcd_pos(7,0);
	    //lcd_wcmd(0x0E);

		delay200ms();	//��ʱ�ӳ���
		
	}
}


/*--------------------------------------------------------------*/
//����ɨ��
void Keysan(void)
{
	
    if(~P1_2&K1J)   { k++;  }
 	 K1J=~(~P1_2);

    if(~P1_3&K2J)    { Set++; }
 	  K2J=~(~P1_3);

	if(~P1_4&K3J)    { Set--; }
 	 K3J=~(~P1_4);
	
   	if(k == 7)	{k = 0; }

	switch(k)
	{
			case 0:	DS1302_SetTime(DS1302_SECOND, Set&0x3f); break;	  //������
			case 1: DS1302_SetTime(DS1302_MINUTE, Set&0x3f); break;	  //���÷�
			case 2:	DS1302_SetTime(DS1302_HOUR  , Set&0x1f); break;	  //����ʱ
			case 3:	DS1302_SetTime(DS1302_DAY   , Set&0x0f); break;	  //������
			case 4:	DS1302_SetTime(DS1302_MONTH , Set&0x0f); break;	  //������
			case 5:	DS1302_SetTime(DS1302_YEAR  , Set&0x7f); break;	  //������
			default: break;
	}
}


/*--------------------------------------------------------------*/
//��ʱ200000+0us ��������
void delay200ms(void)
{
  	unsigned char i,j,k;
  	for(i=201;i>0;i--)
  	for(j=32;j>0;j--)
  	for(k=14;k>0;k--);
}


/*--------------------------------------------------------------*/


