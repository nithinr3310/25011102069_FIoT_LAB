#include <reg51.h>
sbit pulse = P1^0;
unsigned char tick = 0;
void timer0() interrupt 1 {
    TH0=0xFC; TL0=0x66;
    tick++;
    if(tick >= 10) tick=0;
    pulse = (tick == 0);
}
void main() {
    TMOD=0x01; TH0=0xFC; TL0=0x66; ET0=1; EA=1; TR0=1;
    while(1);
}
