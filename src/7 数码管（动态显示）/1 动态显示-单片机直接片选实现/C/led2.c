/**********************************************************************************
*  ����:                  �������������ʾ����( ��Ƭ��ֱ��ʵ��λѡ ������)        *
*																			      *
*  ���ӷ�����P0��J12 ��8PIN��������	 P1��JP16 ����������  						  *
***********************************************************************************
*                         	       										          *
* ��ѧԱ�������������̣���573���������ƺ͵�Ƭ����ֱ��λѡ����(������������)����� *
***********************************************************************************/


#include <reg51.h> 
#include <intrins.h>

void delay(unsigned int i);      //��������

char DelayCNT;//�������
 
//�˱�Ϊ LED ����ģ, ��������� 0-9  - 
unsigned char code   Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};  //�������

//�˱�Ϊ8�������λѡ����, ��������� 1-8��  - 
unsigned char code   dispbit[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdF,0xbF,0x7F};    //λѡ����   ���ķ�������

/************������**********************/ 
main()
{  
   unsigned int i,LedNumVal=1 ;	   //��������
   unsigned int LedOut[10];		   //��������
   
   DelayCNT=0;

  while(1)
  {	 
     if(++DelayCNT>=20)	  //�������ֱ仯�ٶ�
	 {
	 	DelayCNT=0;		  //20��ɨ����������һ��
	    ++LedNumVal;	  //ÿ��20��ɨ�����ڼ�һ��
	 }

	 LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
     LedOut[1]=Disp_Tab[LedNumVal%1000/100]|0x80;
     LedOut[2]=Disp_Tab[LedNumVal%100/10];
     LedOut[3]=Disp_Tab[LedNumVal%10];
	 
	 LedOut[4]=Disp_Tab[LedNumVal%10000/1000];	   //ǧλ
     LedOut[5]=Disp_Tab[LedNumVal%1000/100]|0x80;  //��λ��С����
     LedOut[6]=Disp_Tab[LedNumVal%100/10];		   //ʮλ
     LedOut[7]=Disp_Tab[LedNumVal%10];             //��λ	 

   
	
	 for( i=0; i<9; i++) 
	 {	 
	  P0 = LedOut[i];
	  P1 = dispbit[i];   //ʹ�ò������λѡ 
	  			
/*	  switch(i)			   //ʹ��switch ������λѡ 	  
	     {	    
			case 0:P1 = 0x7F;  break;         
	        case 1:P1 = 0xbF;  break;             	
	        case 2:P1 = 0xdF;  break; 
	        case 3:P1 = 0xeF;  break; 
			case 4:P1 = 0xf7;  break;
			case 5:P1 = 0xfb;  break;
			case 6:P1 = 0xfd;  break;
			case 7:P1 = 0xfe;  break;
		  
	     }	   */
		 
		delay(150);		 //ɨ����ʱ��	  ̫��������ܻ�����˸��
	  }
	 
  }   
}	


void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


