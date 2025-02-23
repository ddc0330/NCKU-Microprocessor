List p=18f4520

    #include<p18f4520.inc>
        CONFIG OSC = INTIO67
        CONFIG WDT = OFF
        org 0x00 
MOVLW 0xCC
MOVWF 0x00
MOVLW 0x01
MOVWF 0x10

start:
    BTFSC 0x00,0
    GOTO m1
    BTFSC 0x00,1
    GOTO plus1
    GOTO plus2
    plus1:
    MOVLW 0x01
    ADDWF 0x10
    GOTO gg
    plus2:
    MOVLW 0x02
    ADDWF 0x10
    GOTO gg
    m1:
    MOVLW 0x01
    SUBWF 0x10
    gg:
    
    RRNCF 0x00
    MOVLW 0xCC
    CPFSEQ 0x00
    GOTO start

end








