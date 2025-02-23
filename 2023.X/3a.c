#include <pic18f4520.h>
#include <stdio.h>
#include <xc.h>
#define _XTAL_FREQ 1000000

#pragma config OSC = INTIO67  // Oscillator Selection bits
#pragma config WDT = OFF      // Watchdog Timer Enable bit
#pragma config PWRT = OFF     // Power-up Enable bit
#pragma config BOREN = ON     // Brown-out Reset Enable bit
#pragma config PBADEN = OFF   // Watchdog Timer Enable bit
#pragma config LVP = OFF      // Low Voltage (single -supply) In-Circute Serial Pragramming Enable bit
#pragma config CPD = OFF      // Data EEPROM?Memory Code Protection bit (Data EEPROM code protection off)

void setServoPosition(unsigned int dutyCycle);

int a;

void __interrupt(high_priority) H_ISR() {
    // step4
    int value = (ADRESH<<2);
    a = (value*30)/1024 + 45; //map 0-1024 to 45-75
    setServoPosition(a);
    // clear flag bit
    PIR1bits.ADIF = 0;
    // step5 & go back step3
    __delay_ms(5);  // delay at least 2tad
    ADCON0bits.GO = 1;
}
void setServoPosition(unsigned int dutyCycle)
{
    CCPR1L = dutyCycle >> 2;                // Load the high 8 bits of duty cycle
    CCP1CONbits.DC1B = dutyCycle & 0x03;    // Load the low 2 bits of duty cycle
}
void main(void) {
    // PWM Initialization
    T2CONbits.TMR2ON = 1;         // Enable Timer2 for PWM operation
    T2CONbits.T2CKPS = 0b01;      // Set Timer2 prescaler to 4
    OSCCONbits.IRCF = 0b001;      // Set internal oscillator frequency to 125 kHz
    CCP1CONbits.CCP1M = 0b1100;   // Configure CCP1 module in PWM mode
    TRISCbits.TRISC2 = 0;         // Set RC2 as output (PWM output pin)
    LATC = 0;                     // Clear LATC register to initialize outputs
    T2CON = 0b00000101;           // Configure T2CON: Timer2 enabled, prescaler 4
    PR2 = 0x9B;                   // Set PWM period for desired frequency
    CCPR1L = 0;                   // Initialize PWM duty cycle to 0 (servo off)
    
    // Set RA0(AN0) as analog input(variable resistor)
    TRISAbits.RA0 = 1;         // Set RA0 as input port
    ADCON1bits.PCFG = 0b1110;  // AN0 as analog input, others as digital
    ADCON0bits.CHS = 0b0000;   // Select AN0 channel
    // step1
    ADCON1bits.VCFG0 = 0;     // Vref+ = Vdd
    ADCON1bits.VCFG1 = 0;     // Vref- = Vss
    ADCON2bits.ADCS = 0b000;  // ADC clock Fosc/2
    ADCON2bits.ACQT = 0b001;  // Tad = 2 us acquisition time set 2Tad = 4 > 2.4
    ADCON0bits.ADON = 1;      // Enable ADC
    ADCON2bits.ADFM = 0;      // left justified

    // step2
    PIE1bits.ADIE = 1;    // Enable ADC interrupt
    PIR1bits.ADIF = 0;    // Clear ADC interrupt flag
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    INTCONbits.GIE = 1;   // Enable global interrupts
    // step3
    ADCON0bits.GO = 1;  // Start ADC conversion
    
    
    setServoPosition(30);
    while (1);

    return;
}