#include <xc.h>
extern unsigned int gcd(unsigned int a , unsigned int b);

void main(void) {
    volatile unsigned int a = 33333;
    volatile unsigned int b = 22222;
    volatile unsigned int result = gcd(a,b);
    while(1);
    return;
}
