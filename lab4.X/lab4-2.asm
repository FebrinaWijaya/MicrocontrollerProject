LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
	org 0x00
MOVLF macro k, f, a
    movlw k
    movwf f, a
    endm

Initial:
    LFSR FSR0, 100H ;FSR0 contains the address of top of stack
start:
    movlb 1
    MOVLF 8, 0x40, 1
    MOVLF 7, 0x16, 1
    MOVLF 6, 0x54, 1
    
    rcall mclear    
    
    rcall Finish
mclear:
    ;push values in 0x140,0x116,0x154 to stack
    movff 0x140, INDF0
    movff 0x116, PREINC0
    movff 0x154, PREINC0
    
    movff 0x140, WREG
    mulwf 0x116
    movff PRODL, WREG
    mulwf 0x154
    
    clrf 0x140
    clrf 0x116
    clrf 0x154
    
    ;resume the values from stack to 0x140,0x116,0x154
    movff POSTDEC0, 0x154
    movff POSTDEC0, 0x116
    movff POSTDEC0, 0x140
    return
Finish:
end


