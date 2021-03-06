LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF
CONFIG LVP = OFF
    
#define SWITCH PORTA, 4
    L1 EQU 0x14
    L2 EQU 0x15
    
   ORG 0X00

DELAYA MACRO num1, num2
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
	btfss SWITCH
	bra PRESS_A
	decfsz L1, 1
	bra LOOP1
	decfsz L2, 1
	bra LOOP2
endm
DELAYB MACRO num1, num2
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
	btfss SWITCH
	bra PRESS_B
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
    DELAYA d'200', d'180'
    rlncf LATD, 1
    movlw 1
    btfsc LATD, 4
    movwf LATD
    bra LOOP_A
    ;bcf LATD, 0
    ;end
    
LOOP_B:
    DELAYB d'200', d'180'
    movlw b'10000'
    btfsc LATD, 0
    movwf LATD
    rrncf LATD, 1
    
    bra LOOP_B
    ;bcf LATD, 0
    
PRESS_A:
    btfss SWITCH
    bra PRESS_A
    bra LOOP_B
    
PRESS_B:
    btfss SWITCH
    bra PRESS_B
    bra LOOP_A
end
    



