LIST p=18f4520
#include<p18f4520.inc>

; CONFIG1H
  CONFIG  OSC = INTIO67         ; Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOREN = SBORDIS       ; Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
  CONFIG  BORV = 3              ; Brown Out Reset Voltage bits (Minimum setting)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 32768         ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
  CONFIG  CCP2MX = PORTC        ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = ON            ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)


; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Setting file register address.
COUNT EQU 0X00	    
DIVISORH EQU 0X10   
DIVISORL EQU 0X11
REMAINDERH EQU 0X20
REMAINDERL EQU 0X21
QUOTIENT EQU 0x030 ; 
 
REM1 EQU 0x23
REM2 EQU 0x24
QUO1 EQU 0x33
QUO2 EQU 0x34
 
 L1 EQU 0X14
 L2 EQU 0X15

#define Button1Flag 0X160, 0 ; You can use it or not.
#define Button2Flag 0X170, 0 ; You can use it or not.
; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
; Teacher assistent will provide input data.
Input1H EQU 0x57;0x0F 
Input1L EQU 0x3f ;7C 
Input2  EQU 0x87 ;6F ; 0xF7C/0x6F = 0x23 ... 0x4F
; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  
ORG 0X00  
bra Init
ORG 0X08 ; setting interrupt service routine 
bra isr	
	
rrncf4time macro origf, destf ; rotate your orginal register to destination for 4 times
		clrf destf
		rrncf origf, 0	     ; Clear your destination first.
		rrncf WREG, 0
		rrncf WREG, 0
		rrncf WREG, 0
		movwf destf
	endm
	
	
; Build the delay macro 
;TIME: (2+7*199+6+3)*89+(2+7*199+6)+2
 Delay MACRO num1, num2
    local LOOP1
    local LOOP2
    movlw num2 ;d'90' ;num2
    movwf L2
    LOOP2:
	movlw num1 ;d'200' ;num1
	movwf L1
    LOOP1:
	NOP
	NOP
	NOP
	NOP
	decfsz L1, 1
	    goto LOOP1; bra ->2 goto skip ->3
	decfsz L2, 1
	    goto LOOP2
endm
	
Init:	bsf OSCCON, IRCF2 ; Internal Oscillator Frequency is set 4Mhz
	bsf OSCCON, IRCF1 ; Don't remove 3 lines !!!
	bcf OSCCON, IRCF0 
			  
    clrf TRISD
    clrf PORTD
    bsf RCON, IPEN
    
    bsf INTCON, INT0IE ; Setting button1&button2 interrupt configuration on INTx(Just pick any 2 from Int0-Int2)  
    bcf INTCON2, INTEDG0
    
    bsf INTCON3, INT1IE
    bcf INTCON2, INTEDG1
    
    bsf INTCON, GIE		  
			  ;
			  ;		  
			  ; Setting config for lighting LED (RD3-RD0)	
	bcf Button1Flag
	bcf Button2Flag
	
	rcall Divide
	movlb 1
	movff QUO1, WREG
	movwf 0x33, 1
	movff QUO2, WREG
	movwf 0x34, 1
	movff REM1, WREG
	movwf 0x23, 1
	movff REM2, WREG
	movwf 0x24, 1
	goto mainLoop
Divide:  ; divde 
	; You can write macro of divide, or not.
	; Input1 is Dividend, Input2 is Division. 
	; For example, 0xFFC / 0x11h =  0xFF0...0xC.
	; 0xFFC: Dividend (0xF: DividendH, 0xFC: DividendL). 
	; 0X11: Division.  0XFF0: Quotient. 0xc: Remainder
	; You must push result values to QUOTIENT(0X130), REMAINDERL(0x121) before you shift values.

;DIVISORH
;DIVISORL
;REMAINDERH
;REMAINDERL
;QUOTIENT
	movlw 9
	movwf COUNT
	movlw Input1H
	movwf REMAINDERH
	movlw Input1L
	movwf REMAINDERL
	movlw Input2
	movwf DIVISORH ;WREG = DIVISORH
	clrf QUOTIENT
	clrf DIVISORL
loop:
	movff DIVISORL, WREG
	subwf REMAINDERL, 1 ;remainder - divisor
	movff DIVISORH, WREG
	subwfb REMAINDERH, 1
	bn quotient_neg
	
	;quotient is positive
	rlncf QUOTIENT, 1
	bsf QUOTIENT, 0
	goto next
quotient_neg:
	movff DIVISORL, WREG
	addwf REMAINDERL, 1 ;remainder - divisor
	movff DIVISORH, WREG
	addwfc REMAINDERH, 1
	
	rlncf QUOTIENT, 1
	bcf QUOTIENT, 0
next:
	bcf STATUS, 0 ; clear carry
	rrcf DIVISORH, 1
	rrcf DIVISORL, 1
	decfsz COUNT, 1
	goto loop
Transfer:  
	; rotate register	
	; transfer values to specified file registers.
	; you can design a better method you think.
	rrncf4time QUOTIENT, QUO1
	movlw b'00001111'
	andwf QUO1, 1
	rrncf4time QUOTIENT, QUO2
	rrncf QUO2, 1
	rrncf QUO2, 1
	rrncf QUO2, 1
	rrncf QUO2, 1
	movlw b'00001111'
	andwf QUO2, 1
	
	rrncf4time REMAINDERL, REM1
	movlw b'00001111'
	andwf REM1, 1
	;movff REMAINDERL, REM2
	rrncf4time REMAINDERL, REM2
	rrncf REM2, 1
	rrncf REM2, 1
	rrncf REM2, 1
	rrncf REM2, 1
	movlw b'00001111'
	andwf REM2, 1
	return
mainLoop: 
    
    btfsc Button1Flag
    rcall q_blink
    btfsc Button2Flag
    rcall r_blink
    bra mainLoop
	  ; You must add some codes, then make your hardware is operated successfully.
	  ; about intx of button , just pick any 2 from int0-int2
	  ; Writing delay macro on interrupt service routine will get zero score.
	  ; hint: Button1Flag&Button2Flag are good partners!
	 
q_blink: 	; Designing Quotient_blink for Subroutine
    bcf Button1Flag, 0
    Delay d'200', d'360'
    movff QUO1, PORTD
    Delay d'200', d'720'
    clrf PORTD
    Delay d'200', d'360'
    movff QUO2, PORTD
    Delay d'200', d'720'
    clrf PORTD
	return 
r_blink: 	; Designing Remainder_blink for Subroutine
    bcf Button2Flag, 0
    Delay d'200', d'360'
    movff REM1, PORTD
    Delay d'200', d'720'
    clrf PORTD
    Delay d'200', d'360'
    movff REM2, PORTD
    Delay d'200', d'720'
    clrf PORTD
	return 
	
isr:	; Don't create Delay here!
	;Button1Flag
	;Button2Flag
	btfss INTCON, INT0IF
	goto testBtn2
	bcf INTCON, INT0IF
	bsf Button1Flag, 0
	retfie  
 testBtn2:
    btfsc INTCON3, INT1IF
    bcf INTCON3, INT1IF
    bsf Button2Flag, 0
    ;movlw 1
    ;movwf temp
    ;movlw count_val
    ;movwf TMR0L, 0
    
    ;bcf INTCON, TMR0IF ; clear flag bit of timer0 (will be set to 1 when timer0 goes from 256 to 0)  
    ;bcf PORTD, 1, 0
    retfie
    
    END





