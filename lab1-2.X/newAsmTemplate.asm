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
here:
    incf TRISD,1
    addwf TRISD,0
    btfss TRISD,4
    goto here
    
end


