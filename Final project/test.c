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
#define xbond 24
#define ybond 30
void Timer0stop(void);
void Timer0on(void);
void setServoPosition1(unsigned int dutyCycle); 
void setServoPosition2(unsigned int dutyCycle); 
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
    TRISDbits.RD0=0;
    LATC = 0;    
    LATD=0;
    //ADC
    ADCON1bits.PCFG = 0b1101;
    ADCON0bits.CHS = 0b0000;
    ADCON1bits.VCFG0 = 0;     
    ADCON1bits.VCFG1 = 0;
    ADCON2bits.ADCS = 0b010; // Fosc/32
    ADCON2bits.ACQT = 0b001;
    ADCON0bits.ADON = 1;
    ADCON2bits.ADFM = 1;
    ADCON0bits.GO = 1;
    
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
    INTCONbits.TMR0IF=0;
    INTCONbits.TMR0IE=1;
    RCONbits.IPEN = 1;      // enable Interrupt Priority mode
    //timer
    /*
    TMR0H=0;
    TMR0L=0;
    T0CONbits.T0PS=0b011;
    INTCON2bits.TMR0IP=0;
    T0CONbits.T08BIT = 0;      // Configure Timer0 as 16-bit timer
    T0CONbits.T0CS = 0;        // Use internal clock (Fosc/4)
    T0CONbits.PSA = 0;         // Enable prescaler
    */
    while (1){
         if (ADCON0bits.GO == 0) ADCON0bits.GO = 1;
    }
}

void setServoPosition1(unsigned int dutyCycle)
{
    CCPR1L = dutyCycle >> 2;                
    CCP1CONbits.DC1B = dutyCycle & 0x03;    
}
void setServoPosition2(unsigned int dutyCycle)
{
    CCPR2L = dutyCycle >> 2;                
    CCP2CONbits.DC2B = dutyCycle & 0x03;    
}

int i=0;
int x=0,y=0;
void __interrupt(high_priority) ISR(void)
{
    if (INTCONbits.INT0IF)
    {
        __delay_ms(300);
        for(int i=0;i<x;i++){
            setServoPosition2(56);
            delay(0.192);
        }
        setServoPosition2(46);
        x=0;
        for(int i=0;i<y;i++){
            setServoPosition1(36);
            delay(0.179);
        }
        setServoPosition1(46);
        y=0;
        if(LATD==1)
            LATD=0;
        else
            LATD=1;
        INTCONbits.INT0IF = 0;
        
    }
    else if (PIR1bits.ADIF) {  
        int value = (ADRESH << 8) + ADRESL;
        __delay_ms(5);
        if(i==0){
            i=1; //x direction
                if(value > 800){ //right
                    setServoPosition2(36);
                    delay(0.15);
                    x++;
                }
                else if(value < 200){ //left
                    setServoPosition2(56);
                    delay(0.169);
                    x--;
                }
                else{   //x stop
                    setServoPosition2(46);
                }
            ADCON0bits.CHS = 0b0001;
        }
        else{
            i=0; //y direction
                if(value >800){
                    setServoPosition1(56);
                    delay(0.169);
                    y++;
                }else if(value <200){
                    setServoPosition1(36);
                    delay(0.16);
                    y--;
                }else{
                    setServoPosition1(46);
                }
            ADCON0bits.CHS = 0b0000;
        }
        PIR1bits.ADIF = 0;
    }
}