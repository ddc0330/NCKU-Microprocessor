    LIST p=18f4520
    #include <p18f4520.inc>
    
    CONFIG OSC = INTIO67
    CONFIG WDT = OFF
    
    ORG 0x00 ; Program Counter start at 0x00
    
    ; Define registers
    al EQU 0x10    ; Lower byte of first number
    ah EQU 0x11    ; Higher byte of first number
    bl EQU 0x12    ; Lower byte of second number
    bh EQU 0x13    ; Higher byte of second number
    res_l EQU 0x01 ; Result lower byte
    res_h EQU 0x00 ; Result higher byte
    cor EQU 0x20
    ; Load BCD values into registers
    MOVLW 0x12     ; Load high byte of first BCD number
    MOVWF ah
    MOVLW 0x78     ; Load low byte of first BCD number
    MOVWF al
    MOVLW 0x11     ; Load high byte of second BCD number
    MOVWF bh
    MOVLW 0x56     ; Load low byte of second BCD number
    MOVWF bl
    MOVLW 0x9A
    MOVWF cor
    MOVFF bl,WREG
    CPFSLT al
	GOTO sub
    DECF ah
    ; Start 16-bit BCD subtraction
    ; Subtract lower byte with BCD adjustment
    sub:
    MOVFF bl, WREG
    SUBWF cor,W
    ADDWF al,W          ; W = al - bl
    DAW
    MOVWF res_l           ; Store result in lower byte of result
    MOVFF bh, WREG
    SUBWF cor,W
    ADDWF ah,W          ; W = ah - bh
    DAW 
    MOVWF res_h           ; Store result in higher byte of result
    END
