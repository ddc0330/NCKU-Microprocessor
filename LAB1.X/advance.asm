List p=18f4520

    #include<p18f4520.inc>
        CONFIG OSC = INTIO67
        CONFIG WDT = OFF
        org 0x00 
MOVLW 0xF7
MOVWF 0x00 
MOVLW 0x9F
MOVWF 0x01
MOVLW b'11110000' 
ANDWF 0x00
MOVLW b'00001111'
ANDWF 0x01
CLRF WREG
ADDWF 0x00,W
ADDWF 0x01,W
MOVWF 0x02

MOVLW 0x00
MOVWF 0x03
	
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02
BTFSS 0x02, 0
INCF 0x03
RRNCF 0x02	
end





