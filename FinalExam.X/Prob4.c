/*
 * File:   Prob4.c
 * Author: febri
 *
 * Created on December 22, 2018, 12:57 PM
 */

//control LED freq with potentiometer

/*
 * File:   Problem3.c
 * Author: febri
 *
 * Created on December 20, 2018, 1:23 AM
 */


// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
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


#include<xc.h>
#include<string.h>

void adc_init_2_1(void);
void adc_init_2_2(void);
void ccp2_init(void);
void tmr_init(void);
void INTERRUPT_Initialize (void);
void UART_Initialize();
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer();
void MyusartRead();
int ADC_Read(int channel);
void PrintVoltage(int digital);

void LED_Init();

char mystring[10];
int lenStr = 0;
char str[10];
int count = 0; //for Prob3: count 4 times, print average
int sum = 0;

int mode = 0; //0=increment, 1=decrement (for LED)

void Mode1() // print "Hello world"
{
    ClearBuffer();
    // TODO 
    // Output the result on Command-line Interface.
    UART_Write_Text("\nHello World\n");
}

void Mode2_1() {
    TRISD = 0;
    ClearBuffer();
    count = 0;
    sum = 0;
    adc_init_2_1();
    ccp2_init();
    tmr_init();
}

void Mode2_2() { // Output Voltage 
    ClearBuffer();
    adc_init_2_2();
    int digital = 0;
    //float voltage = 0.0;
    while(strstr(mystring, ".") == NULL) // TODO design a condition. Return to main function when the while loop is over.
    {
        digital=ADC_Read(7);
         // TODO
        // Output the voltage on CLI.
        // The voltage must have the second digit after the decimal point.
        PrintVoltage(digital);
       
        for(int i = 0 ; i < 10000 ; i++) ; // a delay time
    }
    
    ClearBuffer();
}

void Mode3() {
    ClearBuffer();
    adc_init_2_2();
    int digital = ADC_Read(7);
    PrintVoltage(digital);
    UART_Write('\n');
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
    
    INTERRUPT_Initialize();
    UART_Initialize();
    ClearBuffer();
    
    //for LED
    LED_Init();
    Mode2_1();
    while(1) {
        // TODO 
        // "clear" > clear UART Buffer()
        // "mode1" > start Mode1 function as above
        // "mode2" > start Mode2 function as above
        
//        if(strstr(mystring, "clear") != NULL)
//            ClearBuffer();
//        if(strstr(mystring, "print") != NULL) //print
//            Mode3();
//        if(strstr(mystring, "auto") != NULL) //auto
//            Mode2_1();

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
void PrintVoltage(int digital)
{
    float voltage = 0.0;
    voltage = digital* ((float)5/1023); // 5v / 2^10-1  (10bits)
        // TODO
        // Output the voltage on CLI.
        // The voltage must have the second digit after the decimal point.
        int temp = voltage*100;
        char c;
        c = '\n';
        UART_Write(c);
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
    
//        c = '\n';
//        UART_Write(c);
}

void __interrupt(high_priority) Hi_ISR(void)
{
    //deal ccp2 interrupt and adc interrupt
    if(PIR1bits.ADIF){//AD conversion finish
        PIR1bits.ADIF = 0;
        ADCON0bits.ADON = 1;//on
        //NOP();
        //ClearBuffer();
        if(strstr(mystring, ".") != NULL)
        {
            CCP2IE = 0;
            TMR3ON = 0;
        }
//        int digital = 0;
//        digital=ADRES;
//        PrintVoltage(digital);
        sum+=ADRES;
        count++;
        if(count == 4)
        {
            PrintVoltage(sum/4);
            sum = 0;
            count = 0;
        }
    }
    else if(PIR2bits.CCP2IF){ //special event triGger
        PIR2bits.CCP2IF = 0;
        CCPR2 = 31250;  // 1M/4/8hz
        //ADCON0bits.GO = 1;
        //TMR3 = 0;
    }
    return ;
}



void adc_init_2_1(void){
    ADCON1bits.VCFG1 = 0; //setting vref- = Vss
    ADCON1bits.VCFG0 = 0; //setting vref+ = Vdd
    ADCON1bits.PCFG = 0; //setting A/D port configuration control(AN0~AN12 = analog)
    ADCON0bits.CHS = 7; //setting input channel = AN7
    TRISEbits.TRISE2 = 1; //setting RE2/AN7 port as input
    //TRISE =  0x02   ;
    ADCON2bits.ADFM = 1; //setting RIGHT justified

    ADCON2bits.ACQT = 1; //setting acquisition time ACQT<2:0> 2Tad = 2.4 us (Tacqt should be >=2.4us)
    ADCON2bits.ADCS = 3; //conversion time Frc (Tad = 1.2 us) //because fosc = 1MHz
    ADCON0bits.ADON = 1; // enable AD module

    PIR1bits.ADIF = 0;//clear ADIF
    PIE1bits.ADIE = 1; //set ADIE
    INTCONbits.GIE = 1;//set GIE
    IPEN = 1;
    ADIP = 1;

}

void adc_init_2_2(void) { // without using ccp2
    TRISA = 0xff;		// Set as input port
    TRISEbits.TRISE2 = 1;
    //ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin 
    // ADCON2 = 0x92;  	
    ADFM = 1 ;          // Right Justified
    ADCON2bits.ADCS = 7; // clock source = Frc
    ADRESH=0;  			// Flush ADC output Register
    ADRESL=0;  
    
    ADCON1bits.VCFG1 = 0; //setting vref- = Vss
    ADCON1bits.VCFG0 = 0; //setting vref+ = Vdd
    ADCON1bits.PCFG = 0; //setting A/D port configuration control(AN0~AN12 = analog)
    
    ADCON2bits.ACQT = 1; //setting acquisition time ACQT<2:0> 2Tad = 2.4 us (Tacqt should be >=2.4us)
    ADCON2bits.ADCS = 3; //conversion time Frc (Tad = 1.2 us) //because fosc = 1MHz
    ADCON0bits.ADON = 1; // enable AD module
    
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
    //T3CONbits.T3CKPS = 3; //Prescaler 1:8
    //T3CONbits.T3CKPS = 2; //Prescaler 1:4
    T3CONbits.T3CKPS = 1; //Prescaler 1:2
    //no need to turn up timer3 interrupt
}

void INTERRUPT_Initialize (void)
{
    RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 1;     //disable low priority interrupt
}

void UART_Initialize() {
        
    /*portC pin6/7 is mult with usart setting their tris*/    
    TRISCbits.TRISC6 = 1;            // Setting by data sheet 
    TRISCbits.TRISC7 = 1;            //  
    
    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */      
    //  Setting baud rate
    TXSTAbits.SYNC = 0;             //choose the async moode
    BAUDCONbits.BRG16 = 0 ;          //Read Baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 51;    
//    SPBRG = 207; 
    
   //   Serial enable
    RCSTAbits.SPEN = 1;              //open serial port
    PIR1bits.TXIF = 1;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;             //Enable Tx
    RCSTAbits.CREN = 1;             //Enable Rx
    //setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //disable Tx interrupt
    IPR1bits.TXIP = 0;              //Setting Tx as low priority interrupt
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 0;              //Setting Rc as low priority interrupt
    }

void UART_Write(unsigned char data)  // Output on Terminal
{
    while(!TXSTAbits.TRMT);
    TXREG = data;              //write to TXREG will send data 
}

void UART_Write_Text(char* text) { // Output on Terminal, limit:10 chars
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

void ClearBuffer(){
    for(int i = 0; i < 10 ; i++)
        mystring[i] = '\0';
    lenStr = 0;
}

void MyusartRead()
{
    mystring[lenStr] = RCREG;
    UART_Write(mystring[lenStr]);
    lenStr++;
    lenStr %= 10;
//    if(RCREG == '^') TRISC6 = 1;
//    if(RCREG == '$') TRISC6 = 0;
    return ;
}

int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.CHS = channel ; // Select Channel
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}


// old version: 
// void interrupt low_priority Lo_ISR(void)
void __interrupt(low_priority)  Lo_ISR(void)
{
    if(RCIF)
    {
        if(RCSTAbits.OERR)
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
    }
    if(TMR1IF)
    {
        TMR1IF = 0;
        if(mode == 0 && CCPR1L < 16)
        {
            CCPR1L = CCPR1L + 1;
            if(CCPR1L == 16) mode = 1;
        }
        else if(mode == 1 && CCPR1L > 0)
        {
            CCPR1L = CCPR1L - 1;
            if( CCPR1L == 0) mode = 0;
        }
        float temp = (ADRES*5/1023)*200/32;
        TMR1 =  65535 - temp*1000;//34285; // 65535 - 1MHz/32 //65535 - 1000*200*(ADRES*5/1023)/32;
    }
    
   // process other interrupt sources here, if required
    return;
}

void LED_Init() //set as low priority
{
    TRISCbits.TRISC2 = 0; //use RC2 port as output (CCP1)
    
//    OSCCONbits.IRCF2 = 0; //use 500kHz oscillator
//    OSCCONbits.IRCF1 = 1;
//    OSCCONbits.IRCF0 = 1;
    
    T2CON = 0x07; //TMR2ON = 1, T2CKPS<1:0> = 11 (1:16 prescaler)
    TMR2 = 0;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    PR2 = 16; //timer 2 is compared to PR2=16 at each cycle //period = 2ms
    CCPR1L = 0; //duty cycle = 0~2 ms
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 0;
    
    CCP1CONbits.CCP1M3 = 1; //set CCP to PWM mode
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;
    
    //for TMR1
    T1CONbits.RD16 = 1; //use 16 bits
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 0; //Clock source = internal clock
    
    TMR1 = 34285; //65535 - 500kHz/16
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 0; //low priority
    PIR1bits.TMR1IF = 0;
}
