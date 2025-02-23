#include <xc.h>
#include <pic18f4520.h>

// Configuration Bits
#pragma config OSC = INTIO67   // Internal oscillator, RB6 and RB7 as I/O pins
#pragma config WDT = OFF       // Watchdog Timer disabled
#pragma config PWRT = OFF      // Power-up Timer disabled
#pragma config BOREN = ON      // Brown-out Reset enabled
#pragma config PBADEN = OFF    // PORTB<4:0> pins are configured as digital I/O on Reset
#pragma config LVP = OFF       // Low-voltage programming disabled
#pragma config CPD = OFF       // Data EEPROM code protection disabled

#define _XTAL_FREQ 125000  // Oscillator frequency for delay (125 kHz)

int count;



void light(void) {
    LATA = 1;
    __delay_ms(750);
    if (LATA == 0) return; 

    LATA = 2;
    __delay_ms(750);
    if (LATA == 0) return; 

    LATA = 4;
    __delay_ms(750);
    if (LATA == 0) return; 
}

void main(void)
{
    OSCCONbits.IRCF = 0b001;      // Set internal oscillator frequency to 125 kHz

    TRISBbits.TRISB0 = 1;         // Set RB0 as input (button input)
    TRISAbits.TRISA0 = 0;         // Set RA0 as output 
    TRISAbits.TRISA1 = 0;         // Set RA1 as output 
    TRISAbits.TRISA2 = 0;         // Set RA2 as output 
    LATA = 0;                     // Clear LATA register to initialize outputs

    // Interrupt Configuration
    INTCONbits.GIE = 1;           // Enable global interrupts
    INTCONbits.PEIE = 1;          // Enable peripheral interrupts
    INTCONbits.INT0IE = 1;        // Enable external interrupt 0 (RB0 pin)
    INTCONbits.INT0IF = 0;        // Clear external interrupt 0 flag

    count = 0;

    while (1) {
        // Check if the count is even
        if (count % 2 == 0) {
            LATA = 0; // Turn off all LEDs
        } else {
            light(); // Call the light sequence function
        }
    }
}

// Interrupt Service Routine (ISR)
void __interrupt() ISR(void) {
    // Check if the interrupt is triggered by INT0 (RB0 pin)
    if (INTCONbits.INT0IF) {
        __delay_ms(25); // Debounce delay
        count++;
        LATA = 0; // Turn off all LEDs immediately
        INTCONbits.INT0IF = 0; // Clear interrupt flag
    }
}