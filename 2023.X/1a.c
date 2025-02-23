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

#define _XTAL_FREQ 250000          // Oscillator frequency (250 kHz)

// Global Variables
int STATE = 1;
int light = 0;
void __interrupt(high_priority) ISR(void) {
    if (PIR1bits.TMR2IF) {
        if(light ==0){
            LATA = 0xFF;
            light = 1;
        }
        else{
            LATA = 0;
            light = 0;
        }
        PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag
    }
    if (INTCONbits.INT0IF) { // Button interrupt
        STATE++;
        __delay_ms(25); 
        // STATEL cycle
        if (STATE > 3) {
            STATE = 1;
        }
        switch (STATE) {
            case 1:
                PR2 = 244;
                break;
            case 2:
                PR2 = 122;
                break;
            case 3:
                PR2 = 61;
            break;
        }
        INTCONbits.INT0IF = 0; // Clear interrupt flag
    }
}


void main(void) {
    // Initialization
    ADCON1 = 0x0F; // Configure pins as digital I/O
    
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
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
    PR2 = 244;     // Set PR2 to define Timer2 period
    OSCCON = 0x20; // Set internal oscillator frequency to 250 kHz

    while (1) {
        // Main loop does nothing; all work done in ISR
    }
}
