/*
 * File:   Prob5.c
 * Author: febri
 *
 * Created on December 22, 2018, 3:18 PM
 */
// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784
#define TOTAL 30

int per_arrp[30] = { G5, E5, E5, 0, F5, D5, D5, 0, C5, D5, E5, F5, G5, G5, G5, 0,
G5, E5, E5, 0, F5, D5, D5, 0, C5, E5, G5, G5, E5, 0
};

int cnt = 0;
int pos = 0;

void main(void) {
    
    TRISCbits.TRISC2 = 0; //use RC2 port as output (CCP1)
    
//    OSCCONbits.IRCF2 = 0; //use 500kHz oscillator
//    OSCCONbits.IRCF1 = 1;
//    OSCCONbits.IRCF0 = 1;
    
    //T2CON = 0x04; //TMR2ON = 1, T2CKPS<1:0> = 11 (1:1 prescaler)
    T2CON = 0x07;
    TMR2 = 0;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    PR2 = 16; //timer 2 is compared to PR2=16 at each cycle //period = 1ms
    CCPR1L = 0; //duty cycle = 0~1 ms
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 0;
    
    CCP1CONbits.CCP1M3 = 1; //set CCP to PWM mode
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;
    
    //for TMR1
    T3CONbits.RD16 = 1; //use 16 bits
    T3CONbits.TMR3ON = 1;
    T3CONbits.TMR3CS = 0; //Clock source = internal clock
    
    TMR3 = 34285; //65535 - 1 MHz/4/16
    PIE2bits.TMR3IE = 1;
    IPR2bits.TMR3IP = 1;
    PIR2bits.TMR3IF = 0;
    cnt = 0;
    
    while(1);
}

//void interrupt tc_int(void)
void __interrupt () my_isr_routine (void)
{
    if(TMR3IF)
    {
        TMR3IF = 0;
        cnt++;
        if(cnt == 4)
        {
            cnt = 0;
            if(per_arrp[pos] == 0)
            {
                pos = pos+1;
                PR2 = 0;
                CCPR1L = 0;
            }
            else
            {
                PR2 = 15625/per_arrp[pos++] - 1;
                CCPR1L = 0.5*PR2;
            }
            
            if(pos>=TOTAL)
            {
                TMR3ON = 0;
                PR2 = 0;
                CCPR1L = 0;
            }
        }
        TMR3 = 34285; //65535 - 1 MHz/32 
    }
}


