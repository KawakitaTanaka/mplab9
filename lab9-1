#include <xc.h>
#include <pic18f4520.h>

#pragma config OSC = INTIO67    // Internal oscillator
#pragma config WDT = OFF        // Disable Watchdog Timer
#pragma config PWRT = OFF       // Disable Power-up Timer
#pragma config BOREN = ON       // Enable Brown-out Reset
#pragma config PBADEN = OFF     // PORTB digital I/O
#pragma config LVP = OFF        // Disable Low Voltage Programming
#pragma config CPD = OFF        // Disable Data EEPROM Code Protection
#define _XTAL_FREQ 125000

const unsigned char student_id_digits[] = {7, 4, 1, 1, 6, 7, 3, 8};

// ????
void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_ms(1);
    }
}

void __interrupt(high_priority) H_ISR(){
    // ?? A/D ????
    int value = ADRESH; // ????
    
    LATB = student_id_digits[value / 32]; // ?? LED
    
    // ?? A/D ????
    PIR1bits.ADIF = 0;

    // ???? 2Tad ????? A/D ??
    delay_ms(1);  // ?? Tad = 2us
    ADCON0bits.GO = 1;

    return;
}

void main(void) 
{
    // ??????? IO ?
    OSCCONbits.IRCF = 0b100; // 1MHz ??
    TRISAbits.RA0 = 1;       // AN0 ?????A/D ???
    TRISB = 0;    // ?? RB0 ?????? LED?
    LATB = 0;      // ?? LED ???????
    
    // A/D ???
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110; // AN0 ?????????????
    ADCON0bits.CHS = 0b0000;  // ?? AN0 ?? A/D ????
    ADCON2bits.ADCS = 0b000;  // A/D ?????1MHz ?????? Fosc/2?
    ADCON2bits.ACQT = 0b001;  // ?? Acquisition Time ? 2Tad
    ADCON0bits.ADON = 1;      // ?? A/D ??
    ADCON2bits.ADFM = 0;      // ?????

    // ?? A/D ??
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    // ????? A/D ??
    ADCON0bits.GO = 1;

    while(1);
    return;
}
