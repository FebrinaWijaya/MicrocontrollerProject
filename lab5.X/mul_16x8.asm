#include "xc.inc"
GLOBAL _mul
    
PSECT mytext, local, class=CODE, reloc=2

_mul_16x8:
 ;multiplicand: 0x01, 0x02
 ;multiplier: 0x03
 
 movlw 8 ;W = 8
 movwf LATD ;LATD = count(do shift for 8 times)
 
 clrf LATC ;LATC = how many negative numbers
 btfsc 0x02, 7
 rcall invert1
 btfsc 0x03, 7
 rcall invert2
 goto loop
 
 invert1:
    bcf STATUS, 0 ; carry = 0
    incf LATC
    movlw 0xFF
    xorwf 0x01, f
    xorwf 0x02, f
    incf 0x01
    
    clrf WREG
    addwfc 0x02, f
    return
    
 invert2:
    incf LATC
    movlw 0xFF ; b'11111111'
    xorwf 0x03, f
    incf 0x03
    return
    
 loop:
    btfss 0x03, 0
    goto rotate
    movff 0x05, WREG
    addwf 0x01, W
    movwf 0x05
    
    movff 0x06, WREG
    addwfc 0x02, W
    movwf 0x06
    
    rotate:
	bcf STATUS, 0 ; carry = 0
	rlcf 0x01, f
	rlcf 0x02, f
	rrncf 0x03, f
    
    decfsz LATD, f
    goto loop
    
    
    movff 0x05, 0x01
    movff 0x06, 0x02
    
    btfsc LATC, 0 ;check if result should be negative
    rcall invert1
 
    RETURN


