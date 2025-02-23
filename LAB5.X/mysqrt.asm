#include "xc.inc"
GLOBAL _mysqrt
PSECT mytext, local, class=CODE, reloc=2

_mysqrt:
MOVFF 0x02, LATA //n
MOVLW 0x0F
MOVWF LATB //16
//for
ag:
MOVFF LATB ,WREG
MULWF LATB //^2
MOVFF PRODL,LATC
//>,return current LATB
MOVFF LATC ,WREG
CPFSGT LATA
GOTO lessone
CPFSLT LATA
GOTO ggez
GOTO lessone
ggez:
MOVFF LATB,WREG
INCF WREG
RETURN //return wreg to 0x01(due to 1 bite)
lessone:
DECFSZ LATB
GOTO ag