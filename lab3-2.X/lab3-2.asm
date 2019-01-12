LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
	org 0x00

    ;***********
start:
    clrf WREG
    clrf LATA
    bcf STATUS, 3 ;overflow = 0
loop:
    bov 10
    incf WREG
    addwf LATA, 1
    goto loop

    nop
    nop
    nop
    goto Initial
    
Rotate:  
    movlw 0xBF ;=10111111
    movwf LATB
    bsf STATUS, 0 ;carry = 1
    RLCF LATB, 1 ;before: 1 10111111; after: 1 01111111
Initial:
    goto Rotate
end