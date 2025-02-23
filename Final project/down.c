#include <xc.h>
#include <pic18f4520.h>

// Configuration Bits
#pragma config OSC = INTIO67   
#pragma config WDT = OFF      
#pragma config PWRT = OFF      
#pragma config BOREN = ON      
#pragma config PBADEN = OFF    
#pragma config LVP = OFF       
#pragma config CPD = OFF       

#define _XTAL_FREQ 125000  // Oscillator frequency for delay (125 kHz)
#define stop 46
#define delay(t) __delay_ms(t * 1000);
#define up 80
#define down 16
#define open 50
#define close 40

void setServoPosition1(unsigned int dutyCycle); 
void setServoPosition2(unsigned int dutyCycle); 
void godown(void);
void goup(void);
void getitem(void);
void main(void)
{
    //osc
    OSCCONbits.IRCF = 0b001;
    
    //IO port
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISCbits.TRISC2 = 0; 
    TRISCbits.TRISC1 = 0; 
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB2 = 1;
    LATC = 0;          
    TRISDbits.TRISD0=1;
    LATD=0;
    
    //PWM
    T2CONbits.TMR2ON = 1;         // Enable Timer2 for PWM operation
    T2CONbits.T2CKPS = 0b01;      // Set Timer2 prescaler to 4
    CCP1CONbits.CCP1M = 0b1100;   // Configure CCP1 module in PWM mode
    CCP2CONbits.CCP2M = 0b1100;   // Configure CCP1 module in PWM mode
    T2CON = 0b00000101;           // Configure T2CON: Timer2 enabled, prescaler 4
    PR2 = 0x9B;                   // Set PWM period for desired frequency
    CCPR1L = 0;                   // Initialize PWM duty cycle to 0 (servo off)
    CCPR2L = 0;
    
    // Interrupt Configuration
    PIE1bits.ADIE = 1;      // Enable ADC interrupt
    PIR1bits.ADIF = 0;      // Clear ADC flag
    INTCONbits.GIE = 1;     // Enable global interrupts
    INTCONbits.PEIE = 1;    // Enable peripheral interrupts
    INTCONbits.INT0IE = 1;  // Enable external interrupt 0 (RB0 pin)
    INTCONbits.INT0IF = 0;  // Clear external interrupt 0 flag
    RCONbits.IPEN = 1;      // enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    // enable high priority interrupt
    INTCONbits.GIEL = 1;    // disable low priority interrupt
    
    setServoPosition2(50); //set
    while (1){ 
        if (ADCON0bits.GO == 0) ADCON0bits.GO = 1;
    }
}
void godown(void){
setServoPosition1(down); 
              delay(4.05);
        setServoPosition1(stop);
}
void goup(void){
        setServoPosition1(up); 
        delay(3.9);
        setServoPosition1(stop);

}
void getitem(void){
    setServoPosition2(close);
}
void setServoPosition1(unsigned int dutyCycle)
{
    CCPR1L = dutyCycle >> 2;                // Load the high 8 bits of duty cycle
    CCP1CONbits.DC1B = dutyCycle & 0x03;    // Load the low 2 bits of duty cycle
}
void setServoPosition2(unsigned int dutyCycle)
{
    CCPR2L = dutyCycle >> 2;                // Load the high 8 bits of duty cycle
    CCP2CONbits.DC2B = dutyCycle & 0x03;    // Load the low 2 bits of duty cycle
}
int i=0,prex=0,prey=0;
int distance_x=0,distance_y=0;
int throw;
void __interrupt() ISR(void)
{
    // Check if the interrupt is triggered by INT0 (RB0 pin)
    if (INTCONbits.INT0IF)
    {   
         __delay_ms(300);
        // Clear the interrupt flag
         if(LATD==1)
             LATD=0;
         else
             LATD=1;
        setServoPosition2(open);
        godown();
        getitem();
        goup();
         while(1){
             if(PORTBbits.RB2==0){
                 setServoPosition2(open);
                 break;
             }
         }
        INTCONbits.INT0IF = 0;
    }
}
