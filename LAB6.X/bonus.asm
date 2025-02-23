
LIST p=18f4520
#include<p18f4520.inc>
    CONFIG OSC = INTIO67 ; Set internal oscillator to 1 MHz
    CONFIG WDT = OFF     ; Disable Watchdog Timer
    CONFIG LVP = OFF     ; Disable Low Voltage Programming
    L1 EQU 0x14         ; Define L1 memory location
    L2 EQU 0x15         ; Define L2 memory location
    S1 EQU 0x16
    S2 EQU 0x17
    org 0x00            

DELAY macro num1, num2
    local LOOP1         
    local LOOP2      
    MOVLW num2         
    MOVWF L2           
    LOOP2:
    MOVLW num1          
    MOVWF L1  
    LOOP1:
    NOP      
    NOP
    NOP
    NOP
    NOP
    DECFSZ L1, 1        
    BRA LOOP1          
    DECFSZ L2, 1        
    BRA LOOP2           
endm

DELAY1 macro num1, num2
    local LOOP1         
    local LOOP2   
    MOVLW num2          
    MOVWF L2          
    LOOP2:
    MOVLW num1          
    MOVWF L1  
    LOOP1:
    BTFSS PORTB,0             
    BRA state2
    NOP
    NOP
    NOP
    DECFSZ L1, 1        
    BRA LOOP1       
    DECFSZ L2, 1        
    BRA LOOP2           
endm

DELAY2 macro num1, num2
    local LOOP1        
    local LOOP2         
    MOVLW num2          
    MOVWF L2            
    LOOP2:
    MOVLW num1          
    MOVWF L1  
    LOOP1:
    BTFSS PORTB,0                
    BRA state0
    NOP
    NOP
    NOP
    DECFSZ L1, 1        
    BRA LOOP1        
    DECFSZ L2, 1     
    BRA LOOP2           
endm
    
init:
    MOVLW 0x0f          ; Set ADCON1 register for digital mode
    MOVWF ADCON1        ; Store WREG value into ADCON1 register
    CLRF PORTB          ; Clear PORTB
    BSF TRISB, 0        ; Set RB0 as input (TRISB = 0000 0001)
    CLRF LATA           ; Clear LATA
    BCF TRISA, 0        ; Set RA0 as output (TRISA = 0000 0000)
    BCF TRISA, 1        ; Set RA1 as output (TRISA = 0000 0000)
    BCF TRISA, 2        ; Set RA2 as output (TRISA = 0000 0000)
    CLRF S1
    CLRF S2
; Button check
state0:      
    CLRF S2
    BCF LATA , 0
    BCF LATA , 1
    BCF LATA , 2
    DELAY d'111', d'70' 
    BTFSC PORTB, 0      ; Check if PORTB bit 0 is low (button pressed)
    BRA state0  
    BRA state1
state1:
    BSF LATA , 0
    BCF LATA , 2
    TSTFSZ S1
    GOTO notfirst1
    DELAY d'111', d'70' 
    DELAY1 d'111', d'70' 
    INCF S1
    GOTO state12
    notfirst1:
    DELAY1 d'111', d'70' 
    DELAY1 d'111', d'70'
    state12:
    BSF LATA , 1
    BCF LATA , 0
    DELAY1 d'111', d'70' 
    DELAY1 d'111', d'70'
    BSF LATA , 2
    BCF LATA , 1
    DELAY1 d'111', d'70' 
    DELAY1 d'111', d'70'
    BRA state1
state2:
    CLRF S1
    BCF LATA , 0
    BCF LATA , 1
    BCF LATA , 2
    BSF LATA , 0
    TSTFSZ S2
    GOTO notfirst2
    DELAY d'111', d'70' 
    DELAY2 d'111', d'70' 
    DELAY2 d'111', d'70' 
    DELAY2 d'111', d'70' 
    INCF S2
    GOTO state22
    notfirst2:
    DELAY2 d'111', d'70' 
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    state22:
    BSF LATA , 1
    DELAY2 d'111', d'70' 
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    BCF LATA , 0
    BCF LATA , 1
    BSF LATA , 2
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    BCF LATA , 2
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    BSF LATA , 2
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    BCF LATA , 2
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    BSF LATA , 2
    DELAY2 d'111', d'70'
    DELAY2 d'111', d'70'
    BRA state2
end