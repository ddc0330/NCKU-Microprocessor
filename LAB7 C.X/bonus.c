#include <xc.h>

// Configuration Bits
#pragma config OSC = INTIO67       // Internal oscillator
#pragma config FCMEN = OFF         // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF          // Oscillator Switchover disabled
#pragma config PWRT = OFF          // Power-up Timer disabled
#pragma config BOREN = SBORDIS     // Brown-out Reset enabled in hardware only
#pragma config BORV = 3            // Brown-out Reset Voltage minimum setting
#pragma config WDT = OFF           // Watchdog Timer disabled
#pragma config WDTPS = 32768       // Watchdog Timer Postscale 1:32768
#pragma config CCP2MX = PORTC      // CCP2 input/output multiplexed with RC1
#pragma config PBADEN = ON         // PORTB<4:0> pins are analog inputs on Reset
#pragma config LPT1OSC = OFF       // Timer1 configured for higher power operation
#pragma config MCLRE = ON          // MCLR pin enabled
#pragma config STVREN = ON         // Stack full/underflow causes Reset
#pragma config LVP = OFF           // Single-Supply ICSP disabled
#pragma config XINST = OFF         // Instruction set extension disabled

#define _XTAL_FREQ 250000  // Oscillator frequency (250 kHz)

// Global Variables
volatile unsigned char COUNTER = 0;
volatile unsigned char TEMP = 0;
volatile unsigned char STATEL = 1;
volatile unsigned char STATET = 1;

void __interrupt() ISR(void) {
    if (INTCONbits.INT0IF) { // Button interrupt
        STATEL++;
        STATET++;

        // STATEL cycle
        if (STATEL > 3) {
            STATEL = 1;
        }

        // STATET cycle
        if (STATET > 2) {
            STATET = 1;
        }

        // Update PR2 based on STATET
        switch (STATET) {
            case 1:
                PR2 = 61;
                break;
            case 2:
                PR2 = 122;
                break;
        }

        // Check STATEL
        if (STATEL == 3) {
            COUNTER = 15;
        } else {
            COUNTER = 0;
        }

        INTCONbits.INT0IF = 0; // Clear interrupt flag
    }

    if (PIR1bits.TMR2IF) { // Timer interrupt
        switch (STATEL) {
            case 1:
                COUNTER++;
                if (COUNTER > 7) {
                    COUNTER = 0;
                }
                break;
            case 2:
                COUNTER++;
                if (COUNTER > 15) {
                    COUNTER = 0;
                }
                break;
            case 3:
                if (COUNTER == 0) {
                    COUNTER = 15;
                } else {
                    COUNTER--;
                }
                break;
        }
        LATA = COUNTER;
        PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag
    }
}


void main(void) {
    // Initialization
    ADCON1 = 0x0F; // Configure pins as digital I/O
    
    TRISA = 0x00;  // Configure PORTA as output
    LATA = 0x00;   // Clear PORTA
    TRISBbits.TRISB0 = 1; // Configure RB0 as input

    RCONbits.IPEN = 1; // Enable interrupt priority
    INTCONbits.GIEH = 1; // Enable global interrupts
    INTCONbits.INT0IE = 1; // Enable INT0 interrupt
    INTCONbits.INT0IF = 0; // Clear INT0 interrupt flag

    PIE1bits.TMR2IE = 1; // Enable Timer2 interrupt
    IPR1bits.TMR2IP = 1; // Set Timer2 to high priority
    PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag

    T2CON = 0xFF; // Set Timer2 with Prescaler and Postscaler to 1:16
    PR2 = 61;     // Set PR2 to define Timer2 period
    OSCCON = 0x20; // Set internal oscillator frequency to 250 kHz

    STATEL = 1;
    STATET = 1;
    COUNTER = 0;
    TEMP = 0;

    while (1) {
        // Main loop does nothing; all work done in ISR
    }
}
