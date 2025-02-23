#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#define _XTAL_FREQ 4000000  // ????????????? 4MHz
// using namespace std;

char str[20];
int interrupted = 0;
int cont=0;
void Mode1(){   // Todo : Mode1
    int cur=2;
    while(cur<20){
        strcpy(str, GetString());
        if(str[cur]!='\0'){
            LATA = (int)str[cur++] - 48;
        }
    }
    return ;
}
void Mode2(){   // Todo : Mode2 
    int cur=2;
    while(cur<20)
    {
        strcpy(str, GetString());
        if(str[cur]!='\0'){
            int limit = (int)str[cur++] - 48;
            int i=0;
            while(1){
                strcpy(str, GetString());
                if(str[cur]!='\0') break;
                if(interrupted)
                {
                    break;
                }
                LATA = i++;
                __delay_ms(500);
                if(i == limit+1)
                {
                    i = 0;
                }
            }
            interrupted = 0;
        }
    }
    return ;
}
void main(void) 
{
    
    SYSTEM_Initialize() ;
    
    while(1) {
        strcpy(str, GetString()); // TODO : GetString() in uart.c
        if(str[0]=='m' && str[1]=='1'){ // Mode1
            Mode1();
            ClearBuffer();
        }
        else if(str[0]=='m' && str[1]=='2'){ // Mode2
            Mode2();
            ClearBuffer();
        }
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{
    LATA = 0;
    interrupted = 1;
    INTCONbits.INT0IF = 0;
}
