LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
	org 0x00
Initial:
    ;***********
start:
    clrf WREG ;Array value and count for the loop
    LFSR FSR0, 120H
loop:
    incf WREG ;Array value
    MOVWF POSTINC0
    btfss WREG,3
    goto loop
    
clrf WREG
LFSR FSR0, 120H
LFSR FSR1, 137H
loop2:
    incf WREG ;count
    MOVFF POSTINC0, POSTDEC1
    btfss WREG,3
    goto loop2
end


