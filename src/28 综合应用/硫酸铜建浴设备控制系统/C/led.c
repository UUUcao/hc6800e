/***********************************************************************************
*  ����:                 ����ͭ��ԡ�豸����ϵͳ                                    *
*																			       *
*  ���ӷ�����P1��J12 ��8PIN��������											       *
*            P3��JP5��ť����                                                       *
             P0��JP1(LED������)												       *
			 P3.4Ϊ + ��ť   P3.5Ϊ - ��ť    P3.7Ϊ��갴ť				       *
			 P3.0 ��������� P3.1 ˮ�ÿ��    P3.2 �������					       *
�������Ҫ��: 1.���� P3.4��P3.5���� ���� P3.7 ���ɽ�������ģʽ���ٴΰ����˳�����ģʽ 			
			  �����λ���������ߵĿ�ʼ��˸���������λ����ܲ����ڴ˿���ϵͳ     *
			  ���¼ӻ�� �� A b C ���� �ֱ��������������					       *
			  ��һ������(A)�����������ʱ��									       *
			  �ڶ�������(b)��Һλ�źű���ʱ��								       *
			  ����������(C): Ϊ����											       *
																			       *
	         ������������STC89C52��Ƭ�����ڲ�EEPROM��						       *
																			       *
����ϸ����Ҫ��ο� <<���ƹ��ܱ�>>											       *			
************************************************************************************/

#include "system.h"

//�˱�Ϊ LED ����ģ, ��������� 0-9  - 
unsigned char code Disp_Tab[] = {~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x6f,~0x40}; 


							   //
unsigned char code Disp_alph[] ={~0x88,~0x83,~0xC6,~0xA1,~0x86,~0xbf,~0xc7,~0x8c,~0xc1};
								//P     R/A	  E	    S
unsigned char code alarm_Tab[] ={~0x0c ,~0x88 ,~0x86 ,~0x92 }; 

unsigned int  D[10] , LedOut[8] , LedNumVal , user_T1 , temp_val;
unsigned char user_T , pos,val  , pos_alph , monitor_Number;
unsigned int temp,Tick_10ms,Tick1_10ms,Tick_1min,Tick2_10ms ;
bit M0, M1,M3,M4,SET_J ,Jiao_ban_J, shui_beng_J,Flag_Tim,Flag_C,Flag_set,alarm_flag,liq_alarm;
char val2;
void delay(unsigned int i);      //��������
void Display();
void set_mode();
void Act_Contrl();
void Run_mode(unsigned char run_monitor);
unsigned int distribute_Data(unsigned char pos_alph );
void Data_Change(unsigned int lednmval, unsigned char position);
void MUL_display(unsigned char one ,unsigned char two,unsigned char three,unsigned char four);


main()
{   TMOD|= 0x11;
    TH1=0xD8;   //12.000M
    TL1=0xF0;	
	IE = 0x88;	
    TR1  = 1; 
	
 	rdData(0x0000, 10);

	pos = 0;
	val = 0;
	Flag_set = 0;
  
  while(1)
  {	 
   	char i;
    
	  //ʵ����������
    if(~SET&SET_J)  
	{   
   	  if (!ADD & !SUB) { M0 =~M0 ; pos = 1;}   //ͬʱ���� �� ��������ťSET��ť��������ģʽ 
	   
	  if (Flag_set == 1 & ADD & SUB ) pos ++;
   	} 
  	SET_J=~(~SET);

    if (pos > 4) pos = 1; 	 //���ƹ��ѭ���ƶ�

	 if (M0) { 	set_mode();	  Flag_set = 1;	}  //��������ģʽ
	 
     if (Flag_set & ~M0)   //�˳�����ģʽ
	 {
	   Flag_set = 0; //������ñ�־λ
	   pos = 0; 	 //������λ��
	   val = 0;		 //����ı�������ñ���
	   temp = 0 ;
	   for (i = 0 ; i<4; i++) W_buffer[i] = D[i];
	   earseSector(0x0000);            //����EEPROM
	   wrData(0x0000, 10);             //д��EEPROM

	 }

   	Act_Contrl();  //���ö������Ƴ���
   	Display();	   //������ʾ����
  }	 

}

/*************************************************************************************
*          ���������Ƴ���															 *				 
*																					 *
**************************************************************************************/
void Act_Contrl()
{
    
	P0 = 0xff;
	P2 = 0xff;
	monitor_Number = 9;
   
	if(~liq_level_low)
	{
		if(Tick2_10ms >= D[1]*100)	{liq_alarm = 1;}
	}
	else
	{Tick2_10ms = 0;
	liq_alarm = 0;
	}
   

   //�������崦��
   if(~Jiao_ban&Jiao_ban_J)  
	{   
   		 M3 = ~M3;
   	} 
  	Jiao_ban_J=~(~Jiao_ban);


	//ˮ�����崦�� 
	 if(~shui_beng&shui_beng_J)  
	{   
   	     M4 = ~M4;
   	} 
  	shui_beng_J=~(~shui_beng);


		//ϵͳ����
	if(jiao_relay & shui_relay) {monitor_Number =9;}


	 //���������
	if(M3) {jiao_relay = 0; monitor_Number =1; }

	

	 //���������
	if(~jiao_ban_Err) {alarm_flag = 1; M3 = 0; monitor_Number =2; }
		
     //ˮ������
	if(M4) {shui_relay = 0; monitor_Number =3;}

	  //ˮ�ù���
	if(~shui_beng_Err) {alarm_flag = 1; M4 = 0; monitor_Number =4;}
   	
	//�������ˮ��ͬʱ����
	if(M3 & M4 ) {monitor_Number =5;}

	//�������ˮ��ͬʱ����
	if(~jiao_ban_Err & ~shui_beng_Err)	{monitor_Number =6;}

	 //����Һλ��
	if(liq_alarm ) {alarm_flag = 1; M4 = 0; monitor_Number =7;}

	 //��Դ����
	if(~power_Err)	{alarm_flag = 1; monitor_Number =8;}
  
	 //�������ˮ��ͬʱ���� ����ʾ��ǰʱ��
	 if(M3)
	 { if (Tick_10ms >= 6000 )  {Tick_1min++;Tick_10ms = 0;}
		   if (D[0]!=0)
		   { if (Tick_1min == D[0])  {M3 = 0;Tick_1min = 0;Tick_10ms = 0;}
			 if (M3&~M4) monitor_Number =10;
		     if (M3&M4)  monitor_Number =11;
		   }
		   else{// �Է�����ʱ�ı����
		    Tick_1min = 0;
		    Tick_10ms = 0;
		   }
	 }
	 else
	 {
	   Tick_10ms = 0;
	   Tick_1min = 0;
	 }

	  // �������
	  if(!alarm_remove & alarm_flag) alarm_flag = 0;
   	  if(jiao_ban_Err & shui_beng_Err & power_Err & liq_level_low & alarm_flag)
	  {
	    alarm_flag = 0;
	  }
	  if(alarm_flag)	beep = 0;

	  //���Ա�����
	  if(!alarm_remove) beep = 0; 

	 if (~Flag_set) Run_mode(monitor_Number);   //�������м�����ʾ
	 

}

/*************************************************************************************
*          ����ģʽ����ʾ��ǰ�������												 *				 
*																					 *
**************************************************************************************/
void Run_mode(unsigned char run_monitor)	 //���м�����ʾ
{

     switch (run_monitor)
	{
	  case 1:  //�����������  PR01
	  {MUL_display(alarm_Tab[0] , alarm_Tab[1] , Disp_Tab[0] , Disp_Tab[1]) ;
	   break;
	  }

	  case 2:  //���������	   PE01
	  {MUL_display(alarm_Tab[0] , alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[1]) ;
	   break;
	  }
 
	  case 3:  //����ˮ��������	 PR02
	  {MUL_display(alarm_Tab[0] , alarm_Tab[1] , Disp_Tab[0] , Disp_Tab[2]) ;
	   break;
	  }

	  case 4:  //����ˮ�ù���	 PE02
	  {MUL_display(alarm_Tab[0] , alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[2]) ;
	   break;
	  }

	  case 5:  //�������ˮ��ͬʱ������	   PR12
	  {MUL_display(alarm_Tab[0] , alarm_Tab[1] , Disp_Tab[1] , Disp_Tab[2]) ;
	   break;
	  }
										  
	  case 6:  //�������ˮ��ͬʱ����  PE12
	  {MUL_display(alarm_Tab[0] , alarm_Tab[2] , Disp_Tab[1] , Disp_Tab[1]) ;
	   break;
	  }

	  case 7:  //���޵�Һλ		 E001
	  {MUL_display(alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[0] , Disp_Tab[1]) ;
	   break;
	  }

	  case 8:  //��Դ����	   E002
	  {MUL_display(alarm_Tab[2] , Disp_Tab[0] , Disp_Tab[0] , Disp_Tab[2]) ;
	   break;
	  }

	  case 9:  //ϵͳ����״̬ S000
	  {MUL_display(alarm_Tab[3] ,Disp_Tab[0] , Disp_Tab[0] , Disp_Tab[0]) ;
	   break;
	  }

	  case 10:  //����������� ��ʾ��ǰʱ��
	  {MUL_display(alarm_Tab[0], Disp_Tab[(D[0]-Tick_1min)%1000/100], Disp_Tab[(D[0]-Tick_1min)%100/10], Disp_Tab[(D[0]-Tick_1min)%10]);
	   break;
	  }

	  case 11:  //�������ˮ�������У���������ʱ����
	  {MUL_display(alarm_Tab[1], Disp_Tab[(D[0]-Tick_1min)%1000/100], Disp_Tab[(D[0]-Tick_1min)%100/10], Disp_Tab[(D[0]-Tick_1min)%10]);
	   break;
	  }

	}  
}


/*************************************************************************************
*          �趨ģʽ																	 *				 
*																					 *
**************************************************************************************/
void set_mode()		//�趨ģʽ
{   
   switch (pos)
	{
	   case 1:
	   {pos_alph = val;  
	    temp_val = distribute_Data (pos_alph);	   //�ַ�����
	   	LedNumVal = temp_val ;
	    break;
	    }
	 	    
	   case 2:
	   {LedNumVal = temp_val + (val2*100); 
	    Data_Change(LedNumVal , pos_alph);
        break;
	   }

	   case 3:
	   {LedNumVal = temp_val + (val2*10);    
	   	Data_Change(LedNumVal , pos_alph);
		break;
	   }

	   case 4:
	   {LedNumVal = temp_val + val2;
	    Data_Change(LedNumVal , pos_alph);
	   break;
	   }
	}

   MUL_display(0, 0, 0, 0);
}

void Data_Change(unsigned int lednmval, unsigned char position)
{
	 if(lednmval >= 999) lednmval = 999;
     D[position] = lednmval;
	 temp_val = lednmval;
	 val2 = 0;
	 

}

unsigned int distribute_Data(unsigned char pos_alph )
{
  unsigned int temp_val;	
	switch (pos_alph)
   {
	 case 0:
	 { temp_val = D[0];
	  
	 break;
	 }

	 case 1:
	 { temp_val = D[1];
	  
	  break;
	 }

	 case 2:
	 { temp_val = D[2];
	   
	  break;
	 }
   }

    return temp_val;
}


/*************************************************************************************
*          ���ع��̱�����ʾ�����ӳ���												 *				 
*																					 *
**************************************************************************************/
void MUL_display(unsigned char one ,unsigned char two,unsigned char three,unsigned char four)
{
    if(Flag_set)  
  {	 
	 
	 LedOut[0]=Disp_alph[pos_alph];	   //ǧλ
     LedOut[1]=Disp_Tab[LedNumVal%1000/100];      //��λ��С����
     LedOut[2]=Disp_Tab[LedNumVal%100/10]&0x7f;   //ʮλ
     LedOut[3]=Disp_Tab[LedNumVal%10];            //��λ

	 LedOut[4]=Disp_Tab[LedNumVal%10000/1000];	   //ǧλ
     LedOut[5]=Disp_Tab[LedNumVal%1000/100]&0x7f;
     LedOut[6]=Disp_Tab[LedNumVal%100/10];
     LedOut[7]=Disp_Tab[LedNumVal%10];
  }
  else
  {
     		
	LedOut[0]=one;      //Disp_Tab[monitor_Number];	 
    LedOut[1]=two;
    LedOut[2]=three;
    LedOut[3]=four;	 

	LedOut[4]=Disp_Tab[Tick2_10ms%10000/1000];	   //ǧλ
    LedOut[5]=Disp_Tab[Tick2_10ms%1000/100]&0x7f;
    LedOut[6]=Disp_Tab[D[1]%100/10];
    LedOut[7]=Disp_Tab[D[1]%10];
  }

}

/*************************************************************************************
*          �������ʾ����															 *				 
*																					 *
**************************************************************************************/

void Display()
{   
  	unsigned int i; 
	 for( i=0; i<8; i++) 
	 {	
			
	  switch(i)					  
	     {	    
			case 0:
			if ((pos == 1 )&Flag_Tim & (ADD&SUB))
			{LedOut[0] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=0; LS138C=0; 
			 //L1 = 0;  L2 = 1;  L3 = 1; L4 = 1;
			break; 
			} 
			       
	        case 1:
			if ((pos == 2 )&Flag_Tim& (ADD&SUB))
			{LedOut[1] = 0xff;
			break;
			}
			else
			{LS138A=1; LS138B=0; LS138C=0; 
			 //L1 = 1;  L2 = 0;  L3 = 1; L4 = 1;
			break;}
			             	
	        case 2:
			if ((pos == 3 )&Flag_Tim& (ADD&SUB))
			{LedOut[2] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=1; LS138C=0; 
			 //L1 = 1;  L2 = 1;  L3 = 0; L4 = 1;
			break;} 

	        case 3:
			if ((pos == 4 )&Flag_Tim& (ADD&SUB))
			{LedOut[3] = 0xff;
			break;
			}
			else 
			{LS138A=1; LS138B=1; LS138C=0; 
			 //L1 = 1;  L2 = 1;  L3 = 1; L4 = 0;
			break;}
			 
			case 4:
			if ((pos == 1 )&Flag_Tim& (ADD&SUB))
			{LedOut[4] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=0; LS138C=1; 
			 //L1 = 1;  L2 = 0;  L3 = 0; L4 = 0;
			 break;}

			case 5:
			if ((pos == 2 )&Flag_Tim& (ADD&SUB))
			{LedOut[5] = 0xff;
			break;
			}
			else
			{LS138A=1; LS138B=0; LS138C=1; 
			// L1 = 0;  L2 = 1;  L3 = 0; L4 = 0;
			break;}

			case 6:
			if ((pos == 3 )&Flag_Tim& (ADD&SUB))
			{LedOut[6] = 0xff;
			break;
			}
			else
			{LS138A=0; LS138B=1; LS138C=1; 
			 //L1 = 0;  L2 = 0;  L3 = 1; L4 = 0;
			break;}

			case 7:
			if ((pos == 4  )&Flag_Tim& (ADD&SUB))
			{  LedOut[7] = 0xff;
		    	break;
			}
			else
			{LS138A=1; LS138B=1; LS138C=1; 
			 //L1 = 0;  L2 = 0;  L3 = 0; L4 = 1;
			break; }  	
			
	     }
		   P1 = LedOut[i];
		  
		delay(150);
	  }
	     P1 = 0xff;
}
	
/*************************************************************************************
*          ��ʱ����														       	     *				 
*																					 *
**************************************************************************************/
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


 /*************************************
 [ t1 (10ms)�ж�] �ж�

*************************************/
void T1zd(void) interrupt 3      //3��ʱ��1���жϺ�  1��ʱ��0���жϺ� 0�ⲿ�ж�1 2�ⲿ�ж�2  4�����ж�
{

	 TH1 = 0xD8;    //12.000
	 TL1 = 0xF0;

	 Tick_10ms++;
	 Tick1_10ms++;
	 Tick2_10ms++;

 	if (++user_T == 30)  
	{Flag_Tim = ~Flag_Tim; 
	 user_T = 0;
	 }

   if (++user_T1 == 30) 
   {   
 
	if (Flag_set)  
	{   if (pos == 1)
	   {
	   	if (!ADD &SUB)  val = (++temp)%3;
        if (!SUB &ADD ) val = (--temp)%3;
	   }
  	   else  
	   {
	   	if (!ADD & SUB & LedNumVal <999)  val2++ ;
        if (!SUB & ADD & val2 >=-1 ) val2-- ;
	   }
	 }	

	  user_T1 = 0;
	}

}