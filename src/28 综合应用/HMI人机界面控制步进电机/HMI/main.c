/***********************************************************************
*                                                                      *
*              HMI�˻����沽���������ϵͳ                             *
*                ��  ���пƼ�  ��                                      *
*                                                                      *
* ʵ��Ŀ�ģ�1.����TFT������������ı�д��							   *
*           2.ѧ������ȡ��ģ���Ӧ��(ȡ�ַ��ͺ�������)				   *
*			3.����ͨ��TFT��ʾƽ̨��ζԲ���������ٺͿ���ת��          *
*                                                                      *
*ʵ�����󣺲�������ʾ���֡����֡���ĸ�������˸��                      *
*                                                                      *
*����:1.�����趨ת�ٺ�Ȧ��                                             *
      2.���Ա����趨���ݵ�AT24CO2EEPROM�У�(������뿪ʱ�������Զ�����)*
*                                                                      *
* ���飺ѧԱ����������51�Ķ�ʱ�ж������� ������У�Ч���������أ�      *
***********************************************************************/
#include "system.h"
#include "lcd.h"
#include"Function.h"


bit flag,Ch_Focus_J , Inc_num_J , Dec_num_J ;
unsigned char Set_Data[5] = {0,0,0,0} ;
unsigned char Index = 0 , old_Index;
unsigned char *pold;
unsigned char xdata  pDat[16];
unsigned int  xdata D[32]; 

unsigned char xdata X[] = {0,28,94,28,94,28,94,28,94};    //�Ĵ���X����
unsigned char xdata Y[] = {0,24,24,60,60,96,96,132,132};  //�Ĵ���Y����
/***********************************************************
purpose: ϵͳ��ʼ��
/**********************************************************/
void system_Ini()
{ 
    TMOD|= 0x11;
   	IE = 0x8A;

    TH0=0xDC; //11.0592M
    TL0=0x00;
    TR0  = 1;
 
}

/**********************************************************

  ������

**********************************************************/
void main(void)
{   char i;
    

   P2 = 0xff;
   P0 = 0xff;

   LCD_init();
   LCD_clear(5);

     
   ChineseChar(4,4,1,colors[0],colors[5],1);	//��
   ChineseChar(20,4,1,colors[0],colors[5],2);	//��
   ChineseChar(36,4,1,colors[0],colors[5],3);	//��
   ChineseChar(52,4,1,colors[0],colors[5],4);	//��
   ChineseChar(68,4,1,colors[0],colors[5],5);	//һ
   
    //�������һ
   ChineseChar(2,24,0,colors[0],colors[5],1);	//ת
   ChineseChar(14,24,0,colors[0],colors[5],2);	//��
   ChineseChar(64,24,0,colors[0],colors[5],3);	//Ȧ
   ChineseChar(76,24,0,colors[0],colors[5],4);	//��

/*****************************************************************/
   ChineseChar(4,40,1,colors[0],colors[5],1);	//��
   ChineseChar(20,40,1,colors[0],colors[5],2);	//��
   ChineseChar(36,40,1,colors[0],colors[5],3);	//��
   ChineseChar(52,40,1,colors[0],colors[5],4);	//��
   ChineseChar(68,40,1,colors[0],colors[5],6);	//��
 
   //��������
   ChineseChar(2,60,0,colors[0],colors[5],1);	//Ȧ 
   ChineseChar(14,60,0,colors[0],colors[5],2);	//��
   ChineseChar(64,60,0,colors[0],colors[5],3);	//ת
   ChineseChar(76,60,0,colors[0],colors[5],4);	//��

/*****************************************************************/
   ChineseChar(4,76,1,colors[0],colors[5],1);	//��
   ChineseChar(20,76,1,colors[0],colors[5],2);	//��
   ChineseChar(36,76,1,colors[0],colors[5],3);	//��
   ChineseChar(52,76,1,colors[0],colors[5],4);	//��
   ChineseChar(68,76,1,colors[0],colors[5],7);	//��
   
    //���������
   ChineseChar(2,96,0,colors[0],colors[5],1);	//Ȧ 
   ChineseChar(14,96,0,colors[0],colors[5],2);	//��
   ChineseChar(64,96,0,colors[0],colors[5],3);	//ת
   ChineseChar(76,96,0,colors[0],colors[5],4);	//��

/*****************************************************************/
   ChineseChar(4,112,1,colors[0],colors[5],1);	//��
   ChineseChar(20,112,1,colors[0],colors[5],2);	//��
   ChineseChar(36,112,1,colors[0],colors[5],3);	//��
   ChineseChar(52,112,1,colors[0],colors[5],4);	//��
   ChineseChar(68,112,1,colors[0],colors[5],8);	//��

    //���������
   ChineseChar(2,132,0,colors[0],colors[5],1);	//Ȧ 
   ChineseChar(14,132,0,colors[0],colors[5],2);	//��
   ChineseChar(64,132,0,colors[0],colors[5],3);	//ת
   ChineseChar(76,132,0,colors[0],colors[5],4);	//��
  
    
    ReadData();  // ��ȡ�ϴ��趨����
      
    for (i = 1; i<9 ; i++)
   {
       handle_ASCII(i);
       LCD_ShowString(X[i],Y[i],colors[4],colors[7],Set_Data);
   }
 
   system_Ini();
  
   while(1)	
   {     
      
      ChangeNumber();
      changefocus(Index);
      MotorRun(D);
   }

}

/*********************************************************
*
*               �ı�����趨����
*
**********************************************************/

void ChangeNumber()
{  static unsigned int C_Inc , C_Dec;
     //��ť���崦��
   if(~Ch_Focus & Ch_Focus_J)  
	{   
   	   old_Index = Index;  //�����ϴι���λ��
       pold = Set_Data;    //�����ϴι������
    
       Index++; //�ı���
       if(Index > 8) Index = 0;

       if (X[old_Index] != 0)  LCD_ShowString(X[old_Index],Y[old_Index],colors[4],colors[7],pold); // ��ʾǰһ���������
     
       SaveData(); //���ı�ʱ ��������
      
   	} 
     Ch_Focus_J=~(~Ch_Focus);

/**************************************************************************/
 
	 if(Index > 0)  
	 {    // ������ ��ť���崦��
	   if(~Inc_num&Inc_num_J)  
		{   
	   		 //D[Index]++;
         if( D[Index]< 9999)  D[Index]++;
         if(D[Index]> 9999) D[Index] = 0;
	   	} //if
	  	Inc_num_J=~(~Inc_num);

         // ������ ��ť���崦��
       if(~Dec_num&Dec_num_J)  
		{   
	   	//	 D[Index]--;
         if( D[Index]> 0 ) D[Index]--;
         if(D[Index]> 9999) D[Index] = 0;
	   	} //if
	  	Dec_num_J=~(~Dec_num);
	}//if
/**********************************************************/
//       ���ټӼ���������   
   if(!Inc_num & Dec_num) 
   {
      if( C_Inc <= 30)   C_Inc++;
      else 
      {  if( D[Index]< 9999)  D[Index]++;
         if(D[Index]> 9999) D[Index] = 0;
      }
   }//if
   else   C_Inc = 0;

   if(!Dec_num & Inc_num) 
   {
      if(C_Dec <= 30) C_Dec++;
      else 
       { if( D[Index]> 0 ) D[Index]--;
         if(D[Index]> 9999) D[Index] = 0;
       }
   }//if  
   else C_Dec = 0;

  

/**********************************************************************************/



}



/********************************************************
*                                                       *
*                �ı���λ��                           *
*                                                       *
*********************************************************/
void changefocus(unsigned char index)
{
  handle_ASCII(Index);
 switch(index)
 {
   case 1:
   Focus_Flash(X[1],Y[1]);  
   break;

   case 2:
   Focus_Flash(X[2],Y[2]);
   break;
   
   case 3:
   Focus_Flash(X[3],Y[3]);
   break;

   case 4:
   Focus_Flash(X[4],Y[4]);
   break;

   case 5:
   Focus_Flash(X[5],Y[5]);
   break;
   
   case 6:
   Focus_Flash(X[6],Y[6]);
 
   break;

   case 7:
   Focus_Flash(X[7],Y[7]);
  
   break;

   case 8:
   Focus_Flash(X[8],Y[8]);
   break;

   }

}


/*********************************************************
*
*      �����ݷֽ�ASCII��
*
**********************************************************/

void handle_ASCII(unsigned char index)
{    unsigned int i;
     
     i = D[index];

     Set_Data[0] = i%10000/1000  + 0x30;	   //ǧλ+0x30 ת����ASCII����
     Set_Data[1] = i%1000/100  + 0x30;         //��λ+0x30 ת����ASCII����
     Set_Data[2] = i%100/10 + 0x30;            //ʮλ
     Set_Data[3] = i%10 + 0x30;                //��λ
     Set_Data[4] = '\0';

}

/********************************************************
*                                                       *
*  �����˸����                                         *
*                                                       *
*********************************************************/
void Focus_Flash(unsigned char x, unsigned char y)
{ 

 if(flag)
   
   LCD_ShowString(x,y,colors[4],colors[7],Set_Data);
 
 else

   LCD_ShowString(x,y,colors[7],colors[0],Set_Data);
 
}




/********************************************************
*                  ��������                             *
*                                                       *
*********************************************************/
void SaveData()
{ char i ;
 
  for (i=0; i<8; i++)
 {
    pDat[i*2+0]=D[1+i];
    pDat[i*2+1]=D[1+i]>>8;
 }
   ISendStr(AT24C02, 0 , pDat, 8); 
   ISendStr(AT24C02+1, 0 , pDat+8, 8); 

}

/********************************************************
*                  ��ȡ�趨����                         *
*                                                       *
*********************************************************/
void ReadData()
{ unsigned char i;
    IRcvStr(AT24C02, 0 , pDat, 8);   
    IRcvStr(AT24C02+1, 0 , pDat+8, 8); 
    for (i=0; i<8; i++)
	{
	   D[1+i]=pDat[i*2+0] + pDat[i*2+1]*0x100;
 	}

}

/*************************************
 [ t1 (1ms)�ж�] �ж�

*************************************/
void T1zd(void) interrupt 1      //3��ʱ��1���жϺ�  1��ʱ��0���жϺ� 0�ⲿ�ж�1 2�ⲿ�ж�2  4�����ж�
{ static unsigned int LedNumVal;
	// TH1 = 0x1c;    //12.000
	// TL1 = 0x18;

    TH0 = 0xfe;    //11.0592
    TL0 = 0x33;

     // TH0 = 0x1c;    //11.0592
     // TL0 = 0x18;
    
    if(LedNumVal++ > 1100)  //�ı���˸Ƶ��
    {
     flag = ~flag;  //�ı����־
     LedNumVal = 0 ;

      
    }

  
 Tick_10ms++;
	
 
}

