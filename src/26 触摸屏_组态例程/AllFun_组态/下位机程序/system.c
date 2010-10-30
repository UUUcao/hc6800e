#include <system.h>

unsigned char ReHC74165(void);

unsigned char idata LedUnit;
unsigned int  idata LedNumVal;
unsigned char idata LedOut[8]; 


unsigned int Tick_10ms=0;    //ʱ��Ƭ������
//*************************************************************       
//* (T0) 10 ms ʱ�ӳ��     
//************************************************************
void T0zd(void) interrupt 1 
{
  static xdata unsigned char num;

  TH0=0xC4; //18.432M
  TL0=0x00;
  //TH0=0xDC; //11.0592M
  //TL0=0x00;	    
  Tick_10ms++;

   num++;
	  if(20==num)       //����ʱ���ȡ1302�����ݡ�ʱ�������Ե���
	  {
		 ReadRTC_Flag=1; //ʹ�ñ�־λ�ж�
		 num=0;
	   }
}


unsigned char TickOut(unsigned int * tick, unsigned int tickCnt)
{
     if(Tick_10ms - *tick >= tickCnt)  
     {
        *tick = Tick_10ms;
        return 1;
     }
     return 0;
}



/*********************************************************************************************************
** ��������: ReHC74165
** ��������: 
*********************************************************************************************************/
 unsigned char  ReHC74165(void)
{  
  unsigned char i;
  unsigned char indata;
		
   IN_PL=0;
   NOP();
   IN_PL=1;
   NOP();            
   
   indata=0;     
   for(i=0; i<8; i++)
    { 
	  indata=indata<<1;
      if(IN_Dat==1)indata=indata+1;
	  SCK=1;
      NOP();
      SCK=0;	  
	} 
   IN_PL=0;	     //�˴��������Ӱ������ܵ�G��
   return(~indata);
 
}





