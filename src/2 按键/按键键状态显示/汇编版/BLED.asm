/******************************************************************/
/* 	      ����״̬��ʾ����										  *
/*	             												  *
/*  ���ӷ���������JP10��P0)��JP5����ť�ӿ�)          			  *
/*			      JP11 (P2)��JP1(LED�ӿ�) 						  *
																  *
/*���������LED�ƣ�8·ָʾ�ƽ�p0��         						  *
/******************************************************************/
        K1  BIT  P0.0	  ;LED ��
        K2  BIT  P0.1
        K3  BIT  P0.2
        K4  BIT  P0.3
		K5  BIT  P0.4	 ;���ȿ���
		K6  BIT  P0.5    ;�̵���      
;*********************************************************
        ORG  0000H
        JMP  MAIN
        ORG  0030H        
;*********************************************************
MAIN:
        MOV  SP,#60H
		
LOOP:
  	   	JNB  K1,K1_Prog
        JNB  K2,K2_Prog
        JNB  K3,K3_Prog
        JNB  K4,K4_Prog
		MOV  P2,#0FFH       ;�ر�����LED
		JNB  K5,K5_Prog
		SETB P1.5			;�رշ�����
		JNB  K6,K6_Prog
		SETB P1.4			 ;�رռ̵���

        JMP  LOOP

K1_Prog:
        MOV  P2,#0FEH       ;D00����
        JMP  LOOP
K2_Prog:
        MOV  P2,#0FDH       ;D01����
        JMP  LOOP
K3_Prog:
        MOV  P2,#0FBH       ;D02����
        JMP  LOOP
K4_Prog:
        MOV  P2,#0F7H       ;D03����
        JMP  LOOP

K5_Prog:CLR  P1.5           ;����
        JMP  LOOP

K6_Prog:CLR  P1.4           ;�̵���
        JMP  LOOP

;*********************************************************
        END
