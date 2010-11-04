/********************************************************************************
*  ����:                                                                        *
*         �������ʾ ң�ؼ�ֵ��ȡ��                                             *
*          ң������p3.2                                                         *
*  ���󣺵���ң����ʱ�������� ����ܻ�����˸��Ϊʲô����˸��                    *
*                                                                               *
*         ���ӷ�����ʹ�ú��⹦��ʱ J1���߶̽�                                   *
*****************************************************************************8**/
#include "common.h"

unsigned char  IRCOM[7];

/**********************************************************/
void IR_IN(void) interrupt 0 
{
  unsigned char j,k,N=0;
  EX0 = 0;  

  IRdelay(5);
  if (IRIN==1) 
  { 
   EX0 =1;
   return;
  } 
                            //ȷ��IR�źų���
  while (!IRIN)             //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
  {IRdelay(1);}

  for (j=0;j<4;j++)         //�ռ���������
  { 
   for (k=0;k<8;k++)        //ÿ��������8λ
   {
    while (IRIN)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
    {IRdelay(1);}
    while (!IRIN)           //�� IR ��Ϊ�ߵ�ƽ
    {IRdelay(1);}
    while (IRIN)            //����IR�ߵ�ƽʱ��
    {
     IRdelay(1);
     N++;           
     if (N>=30)
	 { 
	  EX0=1;
	  return;
	 }                      //0.14ms���������Զ��뿪��
    }                       //�ߵ�ƽ�������                
    IRCOM[j]=IRCOM[j] >> 1;                  //�������λ����0��
    if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //�������λ����1��
    N=0;
   }//end for k
  }//end for j
 if (IRCOM[2]!=~IRCOM[3])
 { 
  EX0=1;
  return; 
 }
 EX0 = 1; 
 ir_key_value =  IRCOM[2] ; 
} 

/*******************************************************************/
void IRInit(void)
{
    IE   |= 0x81;               //�������ж��ж�,ʹ�� INT0 �ⲿ�ж�
    TCON |= 0x01;               //������ʽΪ���帺���ش���
    IRIN  = 1;                  //I/O�ڳ�ʼ��
}

//�жϵ�ǰң�������µİ����Ƿ����˳�������
//���ڸ�����ģ��ѭ���Ƿ��˳����ж�����
uchar is_quit(void)
{
	uchar flag = 0xFF;
	if ( (ir_key_value&0xff) == MY_KEY_ESC )
	{
		flag = 0x00;
	}
	return flag;
}

//���Գ���
void test_ir(void)
{
  	//IRInit();
	LED = 1;
	while(is_quit())
	{ 
    	if ( (ir_key_value&0xff) == MY_KEY_MODE )
		{
      		LED = 0;
	  		led_display( ir_key_value );
		}
		else
		{
	  		LED = 1;
      		led_display( ir_key_value );
		}
		//display(IRCOM[2]);
   	}
}

