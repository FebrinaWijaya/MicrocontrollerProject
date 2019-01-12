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
    movlw 9
    rcall Fib
    rcall Finish
Fib: ;n is passed in WREG
    movwf LATD
    movlw 1
    subwf LATD, 1, 0
    MOVLF 0, 0x13
    MOVLF 1, 0x14
    loop:
	movff 0x13, 0x12
	movff 0x14, 0x13
	movff 0x12, WREG
	addwf 0x13, 0, 0
	movff WREG, 0x14
	dcfsnz LATD, 1, 0
	return
	MOVLF 16, PCL
Finish:
end


