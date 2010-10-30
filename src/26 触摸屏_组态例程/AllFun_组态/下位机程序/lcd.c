#include <system.h>

void ADDA_ConvertASCII (void);

//����LCD
unsigned char xdata  XPOS,YPOS;
unsigned char xdata	temp[16],AD[16];
//��æ
bit LcdStartRun = 0;
void LcdWaitIdle(void)
{ 
   P0=0xff; 
   LCDRSPIN=0;    //1 ѡ�����ݼĴ���  O ѡ��ָ��Ĵ���
   LCDRWPIN=1; 	  //��д���� 1 ��  O д
   LCDEPIN=1;     //1 LCM���Զ�д����
   if(LcdStartRun == 1)
   {
      LcdStartRun=1;
	  delay(100);
   }

   LCDEPIN=0;
}

//дָ����� ������æ��
void LcdWCN(unsigned char c)
{  LCDRSPIN=0; 				
   LCDRWPIN=0; 
   P0=c;
   LCDEPIN=1;
   _nop_(); 
   LCDEPIN=0; 
}

//дָ����루��æ��
void LcdWC(unsigned char c)
{  LcdWaitIdle();
   LcdWCN(c); 
}

//д����
void LcdWD(unsigned char d)
{  LcdWaitIdle(); 
   LCDRSPIN=1; 
   LCDRWPIN=0; 
   P0=d; 
   LCDEPIN=1;
   _nop_(); 
   LCDEPIN=0; 
}

//�ڲ����ݵ�ַָ�붨λ
void LcdPos(void)
{  XPOS&=0x0f; 
   YPOS&=0x03;
   if(YPOS==0x00)
      LcdWC(XPOS|0x80);
   else if(YPOS==0x01)
      LcdWC((XPOS+0x40)|0x80);
   else if(YPOS==0x02)
      LcdWC((XPOS+0x10)|0x80);
}		 

//��ʼ��
void InitLcd()
{  
   LCDEPIN=0;
   delay(15);
   LcdWCN(0x38);      //����8λ��ʽ,2��,2*16
   delay(1);

   LcdWC(0x08);        ///�����ʾ
   delay(1);
   LcdWC(0x06);       //�趨���뷽ʽ,��������λ
   delay(1);
   LcdWC(0x0c);    //������ʾ,�ع��,����˸
   delay(1);
}


void LcdWDA(unsigned char x,unsigned char y,unsigned char *s)
{
    YPOS=y;
    for(XPOS=x;XPOS<16 && *s != 0;XPOS++)
    {  LcdPos();
      LcdWD(*s);
      s++;
    }
}
char  LcdStr[17]={'P','R','E','C','H','I','N',' ','C','5','1',0x00};
unsigned char * str1;//=LcdStr;
unsigned char * str2;

char CharPos=-11;
char CharLen = 12;      //�������ַ��ĸ���
bit LcdPause = 0;     //�Ƿ�ֹͣ����
UINT LcdTick=0;

//����str1����߽���
void LcdLeftIn()
{ char i;
  ConvertASCII();
  ADDA_ConvertASCII ();
  
 
   	

  for (i=0;i<16;i++)
  { 
        *str2= temp[i];
	    LcdWDA(i+1,1,str2);
   
   }

   	
   for (i=0;i<16;i++)
   {    *str1= AD[i];
       LcdWDA(i+1,0,str1);
   }

     

}


/*********************************************
											  
�˺�����ʮ����ת����ASCII����ַ�(��ʵʱʱ����ʾ��Һ������)
											   
//*********************************************/
void ConvertASCII (void)
{ 
 
   temp[0]=D[11]%100/10+0x30;
   temp[1]=D[11]%10+0x30;

   temp[2]='/';

   temp[3]=D[9]%100/10+0x30;
   temp[4]=D[9]%10+0x30;

   temp[5]=' ';
   temp[6]=' ';

   temp[7]=D[8]%100/10+0x30;
   temp[8]=D[8]%10+0x30;

   temp[9]=':';

   temp[10]=D[7]%100/10+0x30;
   temp[11]=D[7]%10+0x30;

   temp[12]=':';

   temp[13]=D[6]%100/10+0x30;
   temp[14]=D[6]%10+0x30;

						 

 }
 
 void ADDA_ConvertASCII (void)
{

 //*********��/��********

   AD[0]=D[0]%1000/100+0x30; 
   AD[2]=0x2e; 
   AD[1]=D[0]%100/10+0x30;
   AD[3]=D[0]%10+0x30;

   AD[4]='/';

   AD[5]=D[1]%1000/100+0x30;
   AD[6]=0x2e;    
   AD[7]=D[1]%100/10+0x30;    
   AD[8]=D[1]%10+0x30;
   AD[9]='V';


   AD[10] = ' ';
   AD[11] = D[5]%100/10+0x30;
   AD[12] = D[5]%10+0x30;

   AD[13] =0xdf;
   AD[14] ='C' ;
 
}
