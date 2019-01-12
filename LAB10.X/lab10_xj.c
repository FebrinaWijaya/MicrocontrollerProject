#include <xc.h>
#pragma config OSC = INTIO67, WDT=OFF, LVP=OFF
#pragma PBADEN = 1 //set AN0~AN12 as analog input

void adc_init(void);
void ccp2_init(void);
void tmr_init(void);
int array[8];
int i=0;

void __interrupt(high_priority) Hi_ISR(void)
{
    //deal ccp2 interrupt and adc interrupt
    if(PIR1bits.ADIF){//AD conversion finish
        PIR1bits.ADIF = 0;
        ADCON0bits.ADON = 1;//on
        NOP();
        array[i] = ADRES;
        if(i <  7){
            i = i+1;
        }else{
            i = 0;
        } 
    }
    else if(PIR2bits.CCP2IF){ //special event triger
        PIR2bits.CCP2IF = 0;
        CCPR2 = 31250;  // 1M/4/8hz
    }
    return ;
}

void main(void) {
    //change OSC if you want
    //OSCCONbits.IRCF = ??;
    adc_init();
    ccp2_init();
    tmr_init();
    while(1);
    return;
}

void adc_init(void){
    //datasheet p232 TABLE 19-2
    //Configure the A/D module
    ADCON1bits.PCFG  = 0x00 ;  //Setting A/D Port Configuration ControlADCON1bits.PCFG  = 0x00 ;
    //ADCON0
    //select analog channel
    ADCON0bits.CHS = 7 ;    //setting input channelADCON0bits.CHS = 7 ;
    //set TRIS
    TRISE =  0x04   ;         //setting port as input TRISE =  0x02 ; // RE2
    //Turn on A/D module
    ADCON0bits.ADON = 1;    //turn on ad module ADCON0bits.ADON = 1;
    //ADCON1 //set refer voltage
    ADCON1bits.VCFG1 = 0 ;  //setting vref-
    ADCON1bits.VCFG0 = 0 ;  //setting vref+ADCON1bits.VCFG1 = 0 ;

    //ADCON2
    //A/D Conversion Clock
    ADCON2bits.ADCS = 0X000;
    //Acquisition Time
    ADCON2bits.ACQT = 0X001;
    //left or right justified
    ADCON2bits.ADFM = 0;
    //Configure A/D interrupt(optional)
    //enable Interrupt Priority mode
    ADIF = 0;
    ADIE = 1;
    GIE = 1;
    IPEN = 1;
}

void ccp2_init(void){
    //Configure CCP2 mode,ref datasheet p139
    CCP2M3 = 1;
    CCP2M2 = 0;
    CCP2M1 = 1;
    CCP2M0 = 1;
    //configure CCP2 interrupt
    CCP2IE = 1;
    CCP2IP = 1;
    //configure CCP2 comparator value
    CCPR2 = 31250;
}

void tmr_init(void){
    //set up timer3, ref datasheet p135
    T3CCP1 = 1;
    T3CCP2 = 0;
    TMR3ON = 1;
    T3CONbits.RD16 = 1;
    //no need to turn up timer3 interrupt
}