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

void setServoPosition(unsigned int dutyCycle); // Function to set the servo position

unsigned char currentPosition = 0; // 0: -90°, 1: 0°, 2: +90°, 3: back to -90°

void main(void)
{
    // PWM Initialization
    TRISBbits.TRISB0 = 1;         // Set RB0 as input (button input)
    TRISCbits.TRISC2 = 0;         // Set RC2 as output (PWM output pin)
    LATC = 0;                     // Clear LATC register to initialize outputs
    
    T2CONbits.TMR2ON = 1;         // Enable Timer2 for PWM operation
    T2CONbits.T2CKPS = 0b01;      // Set Timer2 prescaler to 4
    OSCCONbits.IRCF = 0b001;      // Set internal oscillator frequency to 125 kHz
    CCP1CONbits.CCP1M = 0b1100;   // Configure CCP1 module in PWM mode
    T2CON = 0b00000101;           // Configure T2CON: Timer2 enabled, prescaler 4
    PR2 = 0x9B;                   // Set PWM period for desired frequency
    CCPR1L = 0;                   // Initialize PWM duty cycle to 0 (servo off)

    // Interrupt Configuration
    INTCONbits.GIE = 1;           // Enable global interrupts
    INTCONbits.PEIE = 1;          // Enable peripheral interrupts
    INTCONbits.INT0IE = 1;        // Enable external interrupt 0 (RB0 pin)
    INTCONbits.INT0IF = 0;        // Clear external interrupt 0 flag

    // Initialize servo to -90° position
    setServoPosition(16); // -90° position (1 ms pulse width)

    while (1){}
}

// Function to set the servo position using PWM
void setServoPosition(unsigned int dutyCycle)
{
    CCPR1L = dutyCycle >> 2;                // Load the high 8 bits of duty cycle
    CCP1CONbits.DC1B = dutyCycle & 0x03;    // Load the low 2 bits of duty cycle
}

// Interrupt Service Routine (ISR)
void __interrupt() ISR(void)
{
    // Check if the interrupt is triggered by INT0 (RB0 pin)
    if (INTCONbits.INT0IF)
    {
        __delay_ms(25);
        for (int x = 16; x <= 75; x++) // Move to +90°
        {
            setServoPosition(x);
            __delay_ms(30); // Smooth transition
        }
        setServoPosition(16);
        // Clear the interrupt flag
        INTCONbits.INT0IF = 0;
    }
}
