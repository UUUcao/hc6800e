;********************************************************************************
;               ���̿��ƵĻ����ʵơ�											*
;  ���ӷ�����JP10��P2�� �� JP1 ��LED�ƣ����ӣ�   							    *
;            JP11��P0�� �� JP5�������ӿڣ�����        						    *
;																				*
;��p24,p27,p26,p27,p2��8���Ʒֱ��� �ƶ����ƶ���ֹͣ ���� ����*				    *
;																				*
;********************************************************************************
;���ƿ��Ƴ���;
;************;

	ORG     0000H
	LJMP    START
	org	0030h
;************;
;  �� �� ��  ;
;************;
START:
	MOV P3,       #0FFH
      	JNB       P0.7,FUN0	;   ��ⰴ��
      	JNB       P0.4,FUN1
      	JNB       P0.5,FUN2
      	JNB       P0.6,FUN3   	;�رհ�Ŧ
      	AJMP	  START
FUN0:	LCALL      DL10MS       ;��������
     	JB         P0.7,START
WAITL0:	JNB        P0.7,WAITL0  ;�ȴ����ͷ�
FUN01:	LCALL      FUN00
	AJMP	   START
FUN1:	LCALL      DL10MS       ;��������
	JB         P0.4,START
WAITL1:	JNB        P3.3,WAITL1  ;�ȴ����ͷ�
FUN10:	LCALL      FUN11
	AJMP	   START
FUN2:	LCALL      DL10MS       ;��������
     	JB         P0.5,START
WAITL2:	JNB        P0.5,WAITL2  ;�ȴ����ͷ�
FUN20:	LCALL      FUN22
	AJMP	   START

FUN3:	LCALL      DL10MS       ;��������
     	JB         P0.6,START
WAITL3:	JNB        P0.6,WAITL3  ;�ȴ����ͷ�
     	MOV        P2,#0FFH     ;����ʾ
     	LJMP       START

FUN00:	MOV        A,#0FEH	;�����ӳ���
FUN000:	MOV        P2,A
      	LCALL      DL05S
      	JNB        ACC.7,OUT	;����Ƿ����
      	RL         A
      	AJMP       FUN000
OUT:  	RET

FUN11:	MOV        A,#07FH	;�����ӳ���
FUN111:	MOV        P2,A
      	LCALL      DL05S
      	JNB        ACC.0,OUT	;����Ƿ����
      	RR         A
      	AJMP       FUN111

FUN22:MOV         A,#0AAH	;�����ʾ�ӳ���
      MOV         P2,A
      LCALL       DL30S
      CPL         A
      MOV         P2,A
      LCALL       DL30S
      RET

;************;
;  ��ʱ����  ;
;************;
DL512:MOV       R2,#0FFH	;��ʱ500US
LOOP1: DJNZ     R2,LOOP1
       RET

DL10MS:MOV      R3,#14H		;��ʱ10MS
LOOP2: LCALL    DL512
       DJNZ     R3,LOOP2
       RET

DL05S:MOV       R4,#0AH		;��ʱ100MS
LOOP3: LCALL    DL10MS
       DJNZ     R4,LOOP3
       RET

DL30S:MOV       R5,#03H		;��ʱ300MS.
LOOP4:LCALL     DL05S
      DJNZ      R5,LOOP4
      RET     

end
