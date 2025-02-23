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
#define _XTAL_FREQ 125000  // Oscillator frequency for delay (125 kHz)

// Global Variables
volatile unsigned char STATE = 0;
volatile unsigned char MAXNUM = 0;
volatile unsigned char START_FLAG = 0; // Rename START to avoid conflict
unsigned char COUNT = 0; // Change COUNT to unsigned char

void __interrupt() ISR(void) {
    if (INT0IF) {
         __delay_ms(25); 
        STATE++;
        if (STATE > 3) {
            STATE = 1;
        }
        switch (STATE) {
            case 1:
                COUNT = 0;
                MAXNUM = 3;
                break;
            case 2:
                COUNT = 0;
                MAXNUM = 7;
                break;
            case 3:
                COUNT = 0;
                MAXNUM = 15;
                break;
        }

        START_FLAG = 1; // Use START_FLAG instead
        INT0IF = 0; // Clear interrupt flag
    }
}

void main(void) {
    // Initialization
    ADCON1 = 0x0F;   // Configure pins as digital I/O
    
    TRISA = 0x00;    // Configure PORTA as output
    LATA = 0x00;
    TRISBbits.TRISB0 = 1;         // Configure RB0 as input
    
    OSCCONbits.IRCF = 0b001;      // Set internal oscillator frequency to 125 kHz
    RCONbits.IPEN = 0;            // Disable interrupt priority
    // Interrupt Configuration
    INTCONbits.GIE = 1;           // Enable global interrupts
    INTCONbits.PEIE = 1;          // Enable peripheral interrupts
    INTCONbits.INT0IE = 1;        // Enable external interrupt 0 (RB0 pin)
    INTCONbits.INT0IF = 0;        // Clear external interrupt 0 flag

    

    while (1) {
        if (START_FLAG == 1) { 
            COUNT = 0;

            while (1) {
                COUNT++;
                if (COUNT > MAXNUM) {
                    break;
                }

                LATA = COUNT; // Update LEDs or output
                __delay_ms(500); 
            }

            START_FLAG = 0; // Use START_FLAG instead
        }
    }
}
