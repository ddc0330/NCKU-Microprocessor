#include <xc.h>
extern unsigned int multi_signed(unsigned char a,unsigned char b);

void main(void) {
    volatile unsigned char a = -4;
    volatile unsigned char b = -20;
    volatile unsigned int res = multi_signed(a,b);
    while(1);
    return;
}
