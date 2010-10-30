/*************************************************************************************
*  ����:                   ������Ʋ����������										 *
* 																					 *
*    ͨ���������˽ⲽ�����ʹ�ü����������д										 *
*;   ��˫���Ĺ�����ʽ��                                                           	 *
*;   A-AB-B-BC-C-CD-D-DA-A (��һ������,ת 3.75 ��)      							 *
*    J14��·ð��Ͽ� ,ͨ���������Ӳ���������ݽӿ�                 	                 *
*    ��ѧԱһ��Ҫ�����������̣����ָ��ľ���Ӧ�ã�                       	         *
* ����5����������Բ�ͬ���ٶ����У����е���ͬȦ����ֹͣ                    	     *                        									 *
**************************************************************************************/  
#include "reg52.h"

unsigned char TickOut(unsigned int * tick, unsigned int tickCnt);
unsigned char  Acceleration( unsigned int *Base_Tim,  unsigned int Delay_Tim,unsigned int * Flag , unsigned char* Speed) ;

unsigned int Tick_10ms=0;    //ʱ��Ƭ������

unsigned char code FFW_L[8]={~0xfe,~0xfc,~0xfd,~0xf9,~0xfb,~0xf3,~0xf7,~0xf6};   //����λ �������仯��
unsigned char code FFW_H[8]={~0xef,~0xcf,~0xdf,~0x9f,~0xbf,~0x3f,~0x7f,~0x6f};   //����λ �������仯��

unsigned int  MotorTimer1 = 0 , MotorTimer2 = 0, MotorTimer3 = 0, MotorTimer4 = 0, MotorTimer5 = 0;	      //���ת�ٻ�ʱ
unsigned int  MotorDelay1 = 0 , MotorDelay2 = 0, MotorDelay3 = 0, MotorDelay4 = 0, MotorDelay5 = 0; TIM,CT;	//���ת��

unsigned  int xdata Delay_Tim1 =0,	Delay_Tim2 =0,	 Delay_Tim3 =0,   Delay_Tim4 =0,   Delay_Tim5 =0;  //�����ٶ�
unsigned  int Base_T1 = 0,	Base_T2 = 0,	 Base_T3 = 0,	  Base_T4 = 0,   Base_T5 = 0;	 //�����ٶȻ�ʱ
unsigned char  speed1 = 8,	speed2 = 8,	  speed3 = 8,	   speed4 = 8,    speed5 = 8;  //���ת��
unsigned int  Acc_Flg1 =0,	Acc_Flg2 =0,  Acc_Flg3 =0,	Acc_Flg4 =0,  Acc_Flg5 =0;	   //�Ӽ��ٱ�־


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

/*****************************************************
*	    �������1							         *
******************************************************/
 
unsigned char SetMotor1(unsigned int step,unsigned int Speed ) //step ���Ĳ���  Speed����ٶ�
{
   static unsigned int Circle; 	  //��ﲽ��
   static unsigned char ret ;
   MotorDelay1 = 70-Speed;
    if (Circle <= step)
   {  
       if (TickOut(&MotorTimer1 , MotorDelay1))
	   { static char i;
	   	 ret = (FFW_L[i]&0x0f);	  //���ȡ�������
		 if (i++ == 7) i = 0;
		 ++Circle;
	   }
    }

   return ret;
}

/*****************************************************
*	    �������2							         *
******************************************************/
unsigned char SetMotor2(unsigned int step,unsigned int Speed )	 //step ���Ĳ���  Speed����ٶ�
{  static unsigned int Circle ;  //��ﲽ��
   static unsigned char ret ;
   MotorDelay2 = 70-Speed;
  if (Circle <= step)
  {   if(TickOut(&MotorTimer2 , MotorDelay2))
     { static char i;
	    ret = (FFW_H[i]&0xF0);	 //���ȡ�������
	  	if (i++ == 7) i = 0;
	    ++Circle;
	 }
  }

  return  ret;
}

 /*****************************************************
*	    �������3							         *
******************************************************/
unsigned char SetMotor3(unsigned int step,unsigned int Speed)	 //step ���Ĳ���  Speed����ٶ�
{  static unsigned int Circle ;   //��ﲽ��
   static unsigned char ret ;
   MotorDelay3 = 70-Speed;
   if(Circle <= step)
   { if (TickOut(&MotorTimer3 , MotorDelay3))
     { static char i;
	   ret = FFW_L[i]&0x1f;	   //���ȡ�������
	   if (i++ == 7) i = 0;
	    ++Circle;
     }
   }
   return ret;
}

/*****************************************************
*	    �������4							         *
******************************************************/
unsigned char SetMotor4(unsigned int step,unsigned int Speed)   //step ���Ĳ���  Speed����ٶ�
{  static unsigned int Circle ;   //��ﲽ��
   static unsigned char ret ;
   MotorDelay4 = 70-Speed;
    if(Circle <= step)
	{   if (TickOut(&MotorTimer4 , MotorDelay4))
	   { static char i;
		 ret = FFW_H[i]&0xf0;	//���ȡ�������
		 if (i++ == 7) i = 0;
		  ++Circle;
	   }
	}
   return ret;
}

/*****************************************************
*	    �������5							         *
******************************************************/
unsigned char SetMotor5(unsigned int step,unsigned int Speed)	 //step ���Ĳ���  Speed����ٶ�
{  static unsigned int Circle ;	 //��ﲽ��
   static unsigned char ret ; 	 
   MotorDelay5 = 70-Speed;
   if(Circle <= step)
   {    if (TickOut(&MotorTimer5 , MotorDelay5))
	   { static char i;
		 ret = FFW_L[i]&0x0f;	 //���ȡ�������
		if (i++ == 7) i = 0;
		++Circle;
	  }
	  
	}
   return ret;
}

main()
{ 
  system_Ini();
  
  while (1) 
  { unsigned char ret1 , ret2 , ret3 , ret4 , ret5;
   
    //**************************�������1����**************************
	Delay_Tim1 = 50;
	if(speed1 == 61)  Acc_Flg1 = 2;
	if(speed1 == 8)	  Acc_Flg1 = 1 ;
	Acc_Flg1 = 1;
	Acceleration(&Base_T1 , Delay_Tim1, &Acc_Flg1  , &speed1);
    ret1 = SetMotor1(8*5000 , speed1 ) ;	 //�������ò���  8������30��*100=3000��

	//**************************�������2����**************************
	Delay_Tim2 = 250;
	Acc_Flg2 = 1;
	Acceleration(&Base_T2 , Delay_Tim2, &Acc_Flg2  , &speed2);
 	ret2 = SetMotor2(8*1000 , speed2) ;	 //���2
	P1 = ret1 | ret2 ;

	//**************************�������3����**************************
	Delay_Tim3 = 500;
	Acc_Flg3 = 1;
	ret3 = SetMotor3(8*1500 , Acceleration(&Base_T3 , Delay_Tim3, &Acc_Flg3  , &speed3)) ;	 //���3
	
	//**************************�������4����**************************
	Delay_Tim4 = 750;
	Acc_Flg4 = 1;
	ret4 = SetMotor4(8*2000 , Acceleration(&Base_T4 ,  Delay_Tim4 ,&Acc_Flg4  , &speed4)) ;	 //���4
 	P0 =  ret3 | ret4 ;
	
	//**************************�������5����**************************
	Delay_Tim5 = 1000;
	Acc_Flg5 = 1;
	ret5 = SetMotor5(8*2500 , Acceleration(&Base_T5 , Delay_Tim5, &Acc_Flg5  , &speed5));    //���5
	P2 = ret5; 

	}
 
}

/**************************************************************************
*																		  *
*               �������Ļ�ʱ���൱�ڶ����ʱ����				          *
* ע�⣺���ָ����÷� ���๤��������ͬʱ�������������                   *
**************************************************************************/
unsigned char TickOut(unsigned int * tick, unsigned int tickCnt)
{
     if(Tick_10ms - *tick >= tickCnt)  
     {
        *tick = Tick_10ms;
        return 1;
     }
     return 0;
}


/*************************************************************************
*																		 *
*                        �������ٶ�										 *
* ע�⣺���ָ����÷�                                                   *
**************************************************************************/
unsigned char  Acceleration(unsigned int *Base_Tim, unsigned int Delay_Tim,  unsigned int * Flag , unsigned char* Speed)
{
   	if (TickOut(*&Base_Tim , Delay_Tim))	 
	{  
	  if((*Flag == 1) && (*Speed <= 61)) 
	  (*Speed)++ ;

	  if((*Flag == 2) && (*Speed >= 8)) 
	  (*Speed)--;
	 }
	  
   	return  *Speed;
}


/************************************************
*												*
*	      ��ʱ�ж���ʱ						    *
************************************************/
  void Tzd(void) interrupt 1
{
   TH0 = 0xfe;    //11.0592
   TL0 = 0x33;

   Tick_10ms++;	   //10MSʱ��

}
