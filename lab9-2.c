#include <xc.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67    // Internal oscillator
#pragma config WDT = OFF        // Disable Watchdog Timer
#pragma config PWRT = OFF       // Disable Power-up Timer
#pragma config BOREN = ON       // Enable Brown-out Reset
#pragma config PBADEN = OFF     // PORTB digital I/O
#pragma config LVP = OFF        // Disable Low Voltage Programming
#pragma config CPD = OFF        // Disable Data EEPROM Code Protection
#define _XTAL_FREQ 125000000

const unsigned int odd[] = {1, 3, 5, 7, 9, 11, 13, 15};
const unsigned int even[] = {0, 2, 4, 6, 8, 10, 12, 14};
int current = 0;
// ????
void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_ms(1);
    }
}
void __interrupt(high_priority)H_ISR(){
    
    //step4
    int value = ADRESH;
    if (value > current)LATB = odd[value/32];
    else if (value < current) LATB = even[value/32];
    //do things
    current = value;
    
    //clear flag bit
    PIR1bits.ADIF = 0;
    
    
    //step5 & go back step3
    /*
    delay at least 2tad
    ADCON0bits.GO = 1;
    */
    delay_ms(1);  // ?? Tad = 2us
    ADCON0bits.GO = 1;
    return;
}

void main(void) 
{
    //configure OSC and port
    OSCCONbits.IRCF = 0b100; //1MHz
    TRISAbits.RA0 = 1;       //analog input port
    TRISB = 0;
    LATB = 0;
    //step1
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110; //AN0 ?analog input,???? digital
    ADCON0bits.CHS = 0b0000;  //AN0 ?? analog input
    ADCON2bits.ADCS = 0b000;  //????000(1Mhz < 2.86Mhz)
    ADCON2bits.ACQT = 0b001;  //Tad = 2 us acquisition time?2Tad = 4 > 2.4
    ADCON0bits.ADON = 1;
    ADCON2bits.ADFM = 0;    //left justified 
    
    
    //step2
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


    //step3
    ADCON0bits.GO = 1;
    
    while(1);
    
    return;
}
