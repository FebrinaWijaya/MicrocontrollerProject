LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
	org 0x00

    ;***********
MOVLF macro k, f
    movlw k
    movwf f
    endm
start:
    clrf WREG
    MOVLF 0, 0x12
    MOVLF 1, 0x13
    MOVLF 9, LATD
    rcall Fib
    rcall Finish
Fib: ;n is passed in LATD	
	movff 0x12, WREG
	addwf 0x13, 0, 0
	movff WREG, 0x14
	movff 0x13, 0x12
	movff 0x14, 0x13
	decf LATD, 1, 0
	movff LATD, WREG
	dcfsnz WREG, 0, 0
	return
	MOVLF 12, PCL
Finish:
end


