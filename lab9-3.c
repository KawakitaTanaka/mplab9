#include <xc.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67    // Internal oscillator
#pragma config WDT = OFF        // Disable Watchdog Timer
#pragma config PWRT = OFF       // Disable Power-up Timer
#pragma config BOREN = ON       // Enable Brown-out Reset
#pragma config PBADEN = OFF     // PORTB digital I/O
#pragma config LVP = OFF        // Disable Low Voltage Programming
#pragma config CPD = OFF        // Disable Data EEPROM Code Protection
#define _XTAL_FREQ 100000

// ??????????
void __interrupt(high_priority) H_ISR() {
    // ?? ADRESH?????????
    int value = ADRESH; 
    
    // ? ADRESH ????? PWM ????
    if(value < 127)CCPR1L = value / 2; // ? 8 ?????? 7 ???? PWM
    else CCPR1L = 127 - (value / 2);
    
    // ?? ADC ?????
    PIR1bits.ADIF = 0;

    // ???? 2 Tad ????? ADC
    __delay_us(4); // ? Tad = 2us??? 2 Tad = 4us
    ADCON0bits.GO = 1; // ????? ADC
}

void main(void) {
    // ????????/??
    OSCCONbits.IRCF = 0b100; // 1MHz
    TRISAbits.RA0 = 1;       // ?? RA0 ???????????
    TRISCbits.RC2 = 0;       // ?? RC2 ????PWM ???

    // ?? ADC
    ADCON1bits.VCFG0 = 0;    // ???? Vdd
    ADCON1bits.VCFG1 = 0;    // ???? Vss
    ADCON1bits.PCFG = 0b1110; // AN0 ???????????
    ADCON0bits.CHS = 0b0000;  // ?? AN0 ?? ADC ??
    ADCON2bits.ADCS = 0b000;  // ?? ADC ??? Fosc/2??? 1MHz?
    ADCON2bits.ACQT = 0b001;  // ??????? 2 Tad
    ADCON0bits.ADON = 1;      // ?? ADC ??
    ADCON2bits.ADFM = 0;      // ?????

    // ?? ADC ??
    PIE1bits.ADIE = 1; 
    PIR1bits.ADIF = 0; 
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    // ?? PWM
    PR2 = 255;              // ?? PWM ??
    CCP1CONbits.CCP1M = 0b1100; // ?? CCP1 ? PWM ??
    CCPR1L = 0;             // ?????? 0
    T2CONbits.T2CKPS = 0b01; // ?? Timer2 ????? 4
    T2CONbits.TMR2ON = 1;   // ?? Timer2

    // ?? ADC
    ADCON0bits.GO = 1;

    while (1) ;
    
    return;
}
