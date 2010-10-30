
;/********************************************************************************
;*  ����:                                                                       *    
;*         �������1602Һ����ʾ��ֵ                                             *    
;*         ������̶��壺                                                       *
;*         P1.1-P1.3Ϊ����,P1.4-P1.7Ϊ����                                      *    
;*        ���Ƚ�P1.5��  �������P1�ڣ�1602LCD                                   *                     
;********************************************************************************/
;--------------------------------------------------------------
           RELAY  EQU  P1.4
           BEEP   EQU  P1.5
;--------------------------------------------------------------
           RS    EQU  P2.6
           RW    EQU  P2.5
           EN    EQU  P2.7
           X     EQU  26H       ;LCD ��ַ����
;--------------------------------------------------------------
           ORG  0000H
           JMP  MAIN
;--------------------------------------------------------------
MAIN:      MOV  SP,#30H
           CLR   EN
		   CALL  SET_LCD
           CALL  MENU1
LOOP0:
           MOV  P1,#0F0H
           MOV  A,P1
           CJNE  A,#0F0H,LOOP1  ;�м�����תLOOP1
           JMP   LOOP0
LOOP1:
           CALL  KEY_IN
           CALL  CONV
           CALL  BEEP_BL
           JMP  LOOP0
;-----------------------------------------------------
;  LCD ��ʼ������
;-----------------------------------------------------
SET_LCD:
          CLR  EN
          CALL  INIT_LCD     ;��ʼ�� LCD
          CALL  DELAY1
          MOV  DPTR,#INFO1   ;ָ��ָ����ʾ��Ϣ1
          MOV  A,#1          ;��ʾ�ڵ�һ��
          CALL  LCD_SHOW
          MOV  DPTR,#INFO2   ;ָ��ָ����ʾ��Ϣ2
          MOV  A,#2          ;��ʾ�ڵڶ���
          CALL  LCD_SHOW
          RET
;-----------------------------------------------------
INIT_LCD:                 ;8λI/O���� LCD �ӿڳ�ʼ��
          MOV  A,#38H     ;˫����ʾ������5*7����
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#38H     ;˫����ʾ������5*7����
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#38H     ;˫����ʾ������5*7����
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#0CH     ;����ʾ���ع�꣬
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#01H     ;��� LCD ��ʾ��
          CALL  WCOM
          CALL  DELAY1
          RET
;----------------------------------------------------
LCD_SHOW:       ;��LCD�ĵ�һ�л�ڶ�����ʾ��Ϣ�ַ�

          CJNE  A,#1,LINE2  ;�ж��Ƿ�Ϊ��һ��
  LINE1:  MOV  A,#80H       ;���� LCD �ĵ�һ�е�ַ
          CALL  WCOM        ;д������
          CALL  CLR_LINE    ;��������ַ�����
          MOV  A,#80H       ;���� LCD �ĵ�һ�е�ַ
          CALL  WCOM        ;д������
          JMP  FILL

  LINE2:  MOV  A,#0C0H      ;���� LCD �ĵڶ��е�ַ
          CALL  WCOM        ;д������
          CALL  CLR_LINE    ;��������ַ�����
          MOV  A,#0C0H      ;���� LCD �ĵڶ��е�ַ
          CALL  WCOM
  FILL:   CLR  A            ;�����ַ�
          MOVC  A,@A+DPTR   ;����Ϣ��ȡ���ַ�
          CJNE  A,#0,LC1    ;�ж��Ƿ�Ϊ������
          RET
  LC1:    CALL  WDATA       ;д������
          INC  DPTR         ;ָ���1
          JMP  FILL         ;���������ַ�
          RET
;---------------------------------------------------
CLR_LINE:                  ;������� LCD ���ַ�
          MOV  R0,#24
   CL1:   MOV  A,#' '
          CALL  WDATA
          DJNZ  R0,CL1
          RET
;----------------------------------------------------
MENU1:                      ;LCD ��ʾ�����˵���Ϣ
         MOV   DPTR,#MENU2
         MOV   A,#1         ;�ڵ�һ����ʾ��Ϣ
         CALL  LCD_SHOW
         RET
MENU2:  DB  "   KEY NUMBER   ",0
;-----------------------------------------------------
INFO1:  DB  "                ",0  ;LCD ��һ����ʾ��Ϣ
INFO2:  DB  " KEY-CODE: -- H ",0  ;LCD �ڶ�����ʾ��Ϣ
;-----------------------------------------------------
; дָ�����ʹ���ӳ���
;-----------------------------------------------------
WCOM:                      ;дָ��ʹ��
          MOV  P0,A
          CLR RS           ;RS=L,RW=L,D0-D7=ָ���룬E=������
          CLR RW
          SETB EN
          CALL  DELAY0
          CLR EN
          RET
WDATA:                    ;д����ʹ��
          MOV  P0,A
          SETB  RS        ;RS=H,RW=L,D0-D7=���ݣ�E=������
          CLR   RW
          SETB  EN
          CALL  DELAY0
          CLR   EN
          RET

DELAY0:   MOV  R7,#250      ;��ʱ500΢��
          DJNZ  R7,$
          RET
;---------------------------------------------------
;�� LCD �ڶ�����ʾ�ַ�
;A=ASC DATA, B=LINE X POS
;---------------------------------------------------
LCDP2:                    ;��LCD�ĵڶ�����ʾ�ַ�
         PUSH  ACC        ;
         MOV  A,B         ;������ʾ��ַ
         ADD  A,#0C0H     ;����LCD�ĵڶ��е�ַ
         CALL  WCOM       ;д������
         POP  ACC         ;�ɶ�ջȡ��A
         CALL  WDATA      ;д������
         RET
;-----------------------------------------------------
;������̼�ֵ��ȡ�ӳ���
;-----------------------------------------------------
KEY_IN:    MOV  P1,#0F0H    ;����λ��Ϊ����  ������Ϊ�ߵ�ƽ , ����Ϊ�͵�ƽ
           MOV  A,P1
           ANL  A,#0F0H		;���ε���λ
           MOV  B,A
           MOV  P1,#0FH     ;����λ��Ϊ����  ;������Ϊ�ߵ�ƽ������Ϊ�͵�ƽ
           MOV  A,P1
           ANL  A,#0FH
           ORL  A,B               ;����λ�����λ�������
           CJNE  A,#0FFH,KEY_IN1  ;0FFHΪĩ����
           RET
KEY_IN1:   MOV  B,A
           MOV  DPTR,#KEYTABLE
           MOV  R3,#0FFH
KEY_IN2:   INC  R3
           MOV  A,R3
           MOVC  A,@A+DPTR
           CJNE  A,B,KEY_IN3
           MOV  A,R3              ;�ҵ���ȡ˳����
           MOV  R5,#08H           ;��ʱ
           RET
KEY_IN3:   CJNE  A,#00H,KEY_IN2  ;ĩ�꣬������
           RET                    ;00HΪ������
;-------------------------------------------------
CONV:
          MOV   X,#11        ;������ʾ��ʼλ��
          MOV   A,R3
          ANL   A,#0F0H      ;ȡ������λ��������
          SWAP  A            ;����λ�����λ����
          PUSH  ACC          ;ѹ���ջ
          CLR   C            ;C=0
          SUBB  A,#0AH       ;��10
          POP   ACC          ;������ջ
          JC    ASCII0       ;����С��10��תASCII0
          ADD   A,#07H       ;����10��������37H
ASCII0:   ADD   A,#30H       ;С��10��������30H
          MOV   B,X
          CALL  LCDP2

          MOV   A,R3
          ANL   A,#0FH        ;ȡ������λ��������
          PUSH  ACC
          CLR   C
          SUBB  A,#0AH        ;��10
          POP   ACC
          JC    ASCII1        ;����С��10��תASCII0
          ADD   A,#07H        ;����10��������37H
ASCII1:   ADD   A,#30H        ;С��10��������30H
          INC   X
          MOV   B,X
          CALL  LCDP2
          RET
;--------------------------------------------------------
;��������һ���ӳ���
;--------------------------------------------------------
BEEP_BL:
         MOV  R6,#200
  BL1:   CALL  DEX1
         CPL   BEEP
         DJNZ  R6,BL1
         MOV  R5,#50
         CALL  DELAY
         RET
 DEX1:   MOV  R7,#220
 DEX2:   NOP
         DJNZ  R7,DEX2
         RET
DELAY:                    ;��ʱR5��10MS
         MOV  R6,#03H
  D1:    MOV  R7,#100
         DJNZ  R7,$
         DJNZ  R6,D1
         DJNZ  R5,DELAY
         RET
;-----------------------------------------------------
DELAY1:                    ;��ʱ5MS
         MOV  R6,#25
  D2:    MOV  R7,#100
         DJNZ  R7,$
         DJNZ  R6,D2
         RET
;-----------------------------------------------------
KEYTABLE:
//;	   0   1	2	   3	 4	 5	   6	 7	   8	 9	10    11	������ʾ  
DB   0EDH,0DDH,0BDH,07Dh,  0EBh,0DBh ,0BBh,07Bh, 0E7h, 0D7h,0B7h,077h,  00H
    
;-----------------------------------------------------
    END


