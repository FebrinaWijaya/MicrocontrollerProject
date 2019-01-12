#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
// using namespace std;

void adc_init(void);
void ccp2_init(void);
void tmr_init(void);
void PrintVoltage(int digital);
char str[10];
int count = 0;

void Mode1() // print "Hello world"
{
    ClearBuffer();
    // TODO 
    // Output the result on Command-line Interface.
    UART_Write_Text("\nHello World\n");
}

void Mode2() {
    TRISD = 0;
    count = 0;
    ClearBuffer();
    UART_Write('\n');
    adc_init();
    ccp2_init();
    tmr_init();

}

//void Mode2() { // Output Voltage 
//    ClearBuffer();
//    int digital = 0;
//    float voltage = 0.0;
//    while(???????????????????) // TODO design a condition. Return to main function when the while loop is over.
//    {
//        digital=ADC_Read(0);
//        voltage = digital* ((float)5/1023); // 5v / 2^10-1  (10bits)
//        // TODO
//        // Output the voltage on CLI.
//        // The voltage must have the second digit after the decimal point.
//        for(int i = 0 ; i < 10000 ; i++) ; // a delay time
//    }
//    
//    ClearBuffer();
//}

void main(void) 
{
    
    SYSTEM_Initialize() ;
    ClearBuffer();
    //Mode1();
    
    while(1) {
        // TODO 
        // "clear" > clear UART Buffer()
        // "mode1" > start Mode1 function as above
        // "mode2" > start Mode2 function as above
        
//        if(strstr(GetString(), "clear") != NULL)
//            ClearBuffer();
//        if(strstr(GetString(), "mode1") != NULL)
//            Mode1();
//        if(strstr(GetString(), "mode2") != NULL)
//            Mode2();
        
        char *mystring;
        mystring = GetString();
        
        if(mystring[4] == 'r')
            ClearBuffer();
        if(mystring[4] == '1')
            Mode1();
        if(mystring[4] =='2')
            Mode2();
    }
    return;
    
}

// old version: 
// void interrupt high_priority Hi_ISR(void)

//void __interrupt(high_priority) Hi_ISR(void)
//{
//    if(PIR1bits.CCP1IF == 1) {
//        RC2 ^= 1;
//        PIR1bits.CCP1IF = 0;
//        TMR3 = 0;
//    }
//    return ;
//}

void __interrupt(high_priority) Hi_ISR(void)
{
    //deal ccp2 interrupt and adc interrupt
    if(PIR1bits.ADIF){//AD conversion finish
        PIR1bits.ADIF = 0;
        ADCON0bits.ADON = 1;//on
        //NOP();
        
        int digital = 0;
        digital=ADRES;
        PrintVoltage(digital);
        count = count+1;
        if(count >= 30)
        {
            CCP2IE = 0;
            TMR3ON = 0;
        }
//        char *temp = GetString();
//        if(temp[0] == '.'){
//            CCP2IE = 0;
//            TMR3ON = 0;
//        }
//        else
//            ClearBuffer();
        ClearBuffer();
        
    }
    else if(PIR2bits.CCP2IF){ //special event triGger
        PIR2bits.CCP2IF = 0;
        CCPR2 = 31250;  // 1M/4/8hz
        //ADCON0bits.GO = 1;
        //TMR3 = 0;
    }
    return ;
}

void adc_init(void){
    ADCON1bits.VCFG1 = 0; //setting vref- = Vss
    ADCON1bits.VCFG0 = 0; //setting vref+ = Vdd
    ADCON1bits.PCFG = 0; //setting A/D port configuration control(AN0~AN12 = analog)
    ADCON0bits.CHS = 7; //setting input channel = AN7
    TRISEbits.TRISE2 = 1; //setting RE2/AN7 port as input
    //TRISE =  0x02   ;
    ADCON2bits.ADFM = 1; //setting RIGHT justified

    //ADCON2bits.ADCS = 3; //conversion time Frc (Tad = 1.2 us) //USE THIS IF FOSC = 1MHz
    ADCON2bits.ADCS = 4; //conversion time 4Tosc (Tad = 0.25x4 = 1 us) //USE THIS IF FOSC = 4MHz
    //ADCON2bits.ACQT = 1; //setting acquisition time ACQT<2:0> 2Tad = 2.4 us (Tacqt should be >=2.4us) //USE THIS IF FOSC = 1MHz
    ADCON2bits.ACQT = 2; //setting acquisition time ACQT<2:0> 4Tad = 4 us (Tacqt should be >=2.4us) //USE THIS IF FOSC = 4MHz
    
    ADCON0bits.ADON = 1; // enable AD module

    PIR1bits.ADIF = 0;//clear ADIF
    PIE1bits.ADIE = 1; //set ADIE
    INTCONbits.GIE = 1;//set GIE
    IPEN = 1;
    ADIP = 1;

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
    CCPR2 = 31250; // 1M/4/8hz
}

void tmr_init(void){
    //set up timer3, ref datasheet p135
    T3CCP1 = 1;
    T3CCP2 = 0;
    TMR3ON = 1;
    T3CONbits.RD16 = 1; //16-bit timer
    
    //T3CONbits.T3CKPS = 2; //Prescaler 1:4
    //no need to turn up timer3 interrupt
}

void PrintVoltage(int digital)
{
    float voltage = 0.0;
    voltage = digital* ((float)5/1023); // 5v / 2^10-1  (10bits)
        // TODO
        // Output the voltage on CLI.
        // The voltage must have the second digit after the decimal point.
        int temp = voltage*100;
        char c;
        if(temp/100 > 0)
        {
            c = temp/100 + '0';
            temp = temp - temp/100*100;
            UART_Write(c);
            
            UART_Write('.');
        
            c = temp/10 + '0';
            temp = temp - temp/10*10;
            UART_Write(c);
        }
        else if(temp/10 > 0)
        {
            UART_Write('0');
            UART_Write('.');
            
            c = temp/10 + '0';
            temp = temp - temp/10*10;
            UART_Write(c);
        }
        else
        {
            UART_Write('0');
            UART_Write('.');
            UART_Write('0');
        }
        
        c = temp + '0';
        UART_Write(c);
    
        c = '\n';
        UART_Write(c);
}