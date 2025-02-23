List p=18f4520
    #include<p18f4520.inc>
        CONFIG OSC = INTIO67
        CONFIG WDT = OFF
	org 0x00
Sub_Mul macro xh,xl,yh,yl 
    MOVLW xh
    MOVWF 0x04  ; xh
    MOVLW xl
    MOVWF 0x05  ; xl
    MOVLW yh
    MOVWF 0x06  ; yh
    MOVLW yl
    MOVWF 0x07  ; yl
    
    MOVLW xl
    CPFSGT 0x07 ;0x07>0x05
    GOTO dontdo
    MOVLW yh
    SUBWF 0x04,WREG
    MOVWF 0x00
    DECF 0x00
    MOVLW 0xFF
    MOVWF 0x09
    MOVLW xl
    SUBWF 0x07,WREG
    SUBWF 0x09
    INCF 0x09
    MOVFF 0x09,0x01
    GOTO endsub
    dontdo:
    MOVLW yl
    SUBWF 0x05,WREG
    MOVWF 0x01
    MOVLW yh
    SUBWF 0x04,WREG
    MOVWF 0x00
    endsub:
    MOVFF 0x01,WREG
    MULWF 0x00
    MOVFF PRODL,0x10
    MOVFF PRODH,0x11
    endm
Sub_Mul 0xFF, 0x07, 0x0A, 0x05
end