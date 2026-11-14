#include <reg51.h>

sbit mode0 = P3^0;
sbit mode1 = P3^1;
sbit pulse = P1^0;
unsigned int ticks = 0;
unsigned int limit = 100;
void timer0() interrupt 1 {
    TH0=0xFC; TL0=0x66;
    ticks++;
    if(ticks >= limit) { ticks=0; pulse = !pulse; }
}
void main() {
    TMOD=0x01; TH0=0xFC; TL0=0x66; ET0=1; EA=1; TR0=1;
    while(1) {
        if(mode0 == 0) limit=10;
        else if(mode1 == 0) limit=2;
        else limit=50;
    }
}
