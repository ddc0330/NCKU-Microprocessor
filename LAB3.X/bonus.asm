List p=18f4520
    #include<p18f4520.inc>
        CONFIG OSC = INTIO67
        CONFIG WDT = OFF
        org 0x00
MOVLW 0xFF
MOVWF 0x00
MOVLW 0xF1
MOVWF 0x01
;first loop
MOVLW 0xF ;n
MOVWF 0x10
MOVLW 0x08 ;i
MOVWF 0x11
firL:
BTFSC 0x00,7
GOTO findone
RLNCF 0x00,F
BCF 0x00,0
DECF 0x10
DECFSZ 0x11
GOTO firL
GOTO secL
findone:
MOVFF 0x10,0x02
;loop for check interger,if not plus 1
MOVLW 0x07
MOVWF 0x20
lop:
BTFSC 0x00,0
GOTO plus1
RRNCF 0x00,F
DECFSZ 0x20
GOTO lop    
GOTO ENNND
    
;second loop
secL:
MOVLW 0x7 ;n
MOVWF 0x10
MOVLW 0x08 ;i
MOVWF 0x11
secLL:
BTFSC 0x01,7
GOTO findone2
RLNCF 0x01,F
BCF 0x01,0
DECF 0x10
DECFSZ 0x11
GOTO secLL
GOTO ENNND ;all zeros
findone2:
MOVFF 0x10,0x02
;loop for check interger,if not plus 1
MOVLW 0x07
MOVWF 0x20
lo0p:
BTFSC 0x01,0
GOTO plus1
RRNCF 0x01,F
DECFSZ 0x20
GOTO lo0p    
GOTO ENNND
plus1:
INCF 0x02
ENNND:
end

