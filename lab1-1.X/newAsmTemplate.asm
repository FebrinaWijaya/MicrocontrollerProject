LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
	org 0x00
Initial:
    ;***********
start:
    clrf WREG
    clrf TRISD
    
    incf WREG
    movlw 0x03
    addlw 0x06
    addwf TRISD, 1
    clrf TRISD
here:
    incf TRISD
    cpfseq TRISD
    goto here
continue:
    btfss TRISD, 0
    addwf TRISD, 1
    
end