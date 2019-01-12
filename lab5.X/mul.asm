#include "xc.inc"
GLOBAL _mul
    
PSECT mytext, local, class=CODE, reloc=2

_mul:
 ;multiplicand: 0x01
 ;multiplier: 0x03
 
 movlw 8 ;W = 8
 movwf LATD ;LATD = count(do shift for 8 times)
 
 loop:
    btfss 0x03, 0
    goto rotate
    movff 0x06, WREG
    addwf 0x01, W
    movwf 0x06
    
    rotate:
	bcf STATUS, 0 ; carry = 0
	rrcf 0x05, f
	rrcf 0x06, f
	rrncf 0x03, f
    
    decfsz LATD, f
    goto loop
    
    
    movff 0x05, 0x01
    movff 0x06, 0x02
    
    RETURN





