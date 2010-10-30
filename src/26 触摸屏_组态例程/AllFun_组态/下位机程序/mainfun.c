/******************************************************************************************************************
����:  �ۺϳ���																	                                  *
* 	 �˳�������˰��д󲿷ֵĹ��ܣ������λ��һ��ʹ��  ����ǰ��˫��Keillib.exe									  * 
*	 �������ӷ�����JP8��J12 JP5��JP6 ���м̵����ͷ������쳣�죬�뽫J2 ��J8����·ñ�Ͽ�							  *
*    ע�⣺��JP165��·ñ �Ͽ� �����Ӱ�� �����С����                                                             *
*																												  *
*******************************************************************************************************************/
     
#include <system.h>

#define  NOP() _nop_()  /* �����ָ�� */

// �˱�Ϊ LED ����ģ            // 0    1    2    3    4    5    6   7    8    9    A    b    c    d    E    -    L   P    U    Hidden  _ (20)
unsigned char code  Disp_Tab[] =  { ~0xC0,~0xF9,~0xA4,~0xB0,~0x99,~0x92,~0x82,~0xF8,~0x80,~0x90,~0x88,~0x83,~0xC6,~0xA1,~0x86,~0xbf,~0xc7,~0x8c,~0xc1, ~0xff,~0xf7}; 
unsigned char code LED7Code[]  =  { ~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
uchar   dispbit[8]={0xe3,0xe7, 0xeb,0xef,0xf3, 0xf7,0xfb,0xff};    //λ��		0x14,	0x08,  //
unsigned char bdata X0,X1,Y0,Key;

sbit K0=Key^0;		  //����λ����
sbit K1=Key^1;		  //����λ����
sbit K2=Key^2;
sbit K3=Key^3;
sbit K4=Key^4;
sbit K5=Key^5;
sbit K6=Key^6;
sbit K7=Key^7;

bit K0J,K1J,K2J,K3J;

bit M0,M1,M2,M3;   //����λ����

unsigned char  idata M[32];	  //��������
unsigned short xdata D[32];



#define UCHAR unsigned char	  //�궨��
void delay(unsigned int i)	  //����ʵ��
{
    UCHAR j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}

  
unsigned int TestDelay1, TestDelay2, TestDelay3;

/***********************************************************
purpose: ϵͳ��ʼ��
/**********************************************************/
void system_Ini()
{
    TMOD|= 0x11;
	//TH0=0xC4; //18.432M
    //TL0=0x00;
    TH0=0xDC; //11.0592M
    TL0=0x00;	
	IE    = 0x8A;	
    TR0  = 1;

//	TH1 = 0xfe; //11.0592
//	TL1 = 0x33;
//	TR1  = 1; 
}



//***********************************************************
main()
{ 
	system_Ini();
    InitMotor();
	InitTimer();  //д��ʱ��ֵ�����ʹ�ñ��õ��ʱ�򣬲���Ҫûÿ���ϵ�д�룬�˳���Ӧ������	  
    InitLcd();

  
				// ����	������ վ�� �м�̵���  ���ݼĴ���
	Modbus_Init(120000, 38400, 1, &M[0], &D[0]);
  	GetTemp();    //D[5]
	
	while(1)
	{  
	  xdata unsigned char i;
	  unsigned char Input;

	  Input = ReHC74165();
	  D[18] = Input;

 	    
	   if(TickOut(&TestDelay1,50))  LedNumVal++;  //10ms*10

	   D[23]= LedNumVal;
	 
	  	if(ReadRTC_Flag )
		{
		  ReadRTC_Flag=0;
		  Read_RTC();
		 }	 
	 
	   if(TickOut(&TestDelay3,10)) //10ms*10
	    {
			if((K6)&&(D[4]<1000)) ++D[4];
	        if((K7)&&(D[4]>0))    --D[4];

			if((K4)&&(D[12]<100)) ++D[12];
	        if((K5)&&(D[12]>0))   --D[12];
		    
		    LcdLeftIn();		
			ad_da();         //D[AD=0..3, DA=4]
			GetTemp();       //D[5]

	  }
 
	  SetMotor(); //D[12]
	  Modbus_Handle(Tick_10ms);

	  //========================IIC ��ȡ
	   if(D[13]==66)
	   { unsigned char i;
	     unsigned char pDat[8];

	     D[13]=0;
		 IRcvStr(AT24C02, 0 , &pDat[0], 8); 
		 for (i=0; i<4; i++)
	     {
		   D[14+i]=pDat[i*2+0]+pDat[i*2+1]*0x100;
		 }	     
	   }

	   //========================IIC ����
	   if(D[13]==88)
	   { unsigned char i;
	     unsigned char pDat[8];

	     D[13]=0;
		 for (i=0; i<4; i++)
	     {
		   pDat[i*2+0]=D[14+i];
		   pDat[i*2+1]=D[14+i]>>8;
		 }
	      ISendStr(AT24C02, 0 , &pDat[0], 8); 
	   }

  	  
  	  D[20] = P2;
	  D[23]=LedNumVal;
    
      LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
      LedOut[1]=Disp_Tab[LedNumVal%1000/100]|0x80;
      LedOut[2]=Disp_Tab[LedNumVal%100/10];
      LedOut[3]=Disp_Tab[LedNumVal%10]; 

	  LedOut[4]=Disp_Tab[LedNumVal%10000/1000];
      LedOut[5]=Disp_Tab[LedNumVal%1000/100]|0x80;
      LedOut[6]=Disp_Tab[LedNumVal%100/10];
      LedOut[7]=Disp_Tab[LedNumVal%10]; 

	
 	  for(i=0; i<8; i++) 
	  {		
 		P1 = LedOut[i];
			
    	P2 = dispbit[i];   //ʹ�ò������������

		//����ʹ��switch ������
	/*	switch(i)					  
         {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
            case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
            case 2:LS138A=0; LS138B=1; LS138C=0; break; 
            case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break;	
		
         }	*/
		 
		 
		delay(1);
	  }
	  	P1 = 0;	
	  
	}	
}




