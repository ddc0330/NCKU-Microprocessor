List p=18f4520
#include <p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF
org 0x00 ; PC = 0x00
    


initial:
MOVLW 0X00
MOVWF 0X00
 
MOVLW 0X01
MOVWF 0X01
  
LFSR 0 ,0X00   
LFSR 1,0X01
LFSR 2 ,0X02
    
MOVLW 0X06
MOVWF 0X20
MOVLW 0X01
MOVWF 0X21

    rcall fib
    goto exit

fib:

loop:
    MOVF POSTINC0,WREG
    ADDWF POSTINC1,W
    MOVFF WREG,INDF1
    DECF 0X20
    MOVF 0X21,W
    CPFSEQ 0X20
    rcall fib
finish:

    RETURN

exit:    

    end
