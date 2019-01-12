LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF
CONFIG LVP = OFF
    
#define SWITCH PORTA, 4
    L1 EQU 0x14
    L2 EQU 0x15
    
   ORG 0X00

DELAY MACRO num1, num2
    local LOOP1
    local LOOP2
    movlw num2 ;d'180' ;num2
    movwf L2
    LOOP2:
	movlw num1 ;d'200' ;num1
	movwf L1
    LOOP1:
	NOP
	NOP
	NOP
	NOP
	NOP
	decfsz L1, 1
	bra LOOP1
	decfsz L2, 1
	bra LOOP2
endm
    
clrf PORTA
clrf LATA
bsf TRISA, 4
movlw 0x0f
movwf ADCON1
clrf TRISD
clrf LATD
	
bsf LATD, 0
LOOP_A:
    DELAY d'200', d'180'
    rlncf LATD, 1
    movlw 1
    btfsc LATD, 4
    movwf LATD
    btfsc SWITCH
    bra LOOP_A
    bra LOOP_B
    ;bcf LATD, 0
    ;end
    
LOOP_B:
    DELAY d'200', d'180'
    movlw b'10000'
    btfsc LATD, 0
    movwf LATD
    rrncf LATD, 1
    
    btfsc SWITCH
    bra LOOP_B
    bra LOOP_A
    ;bcf LATD, 0
    end
    



