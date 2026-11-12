#include <reg51.h>
sbit led = P1^0;
unsigned char tick = 0;
unsigned char duty = 0;
unsigned char up = 1;
unsigned int ms = 0;
void timer0() interrupt 1 {
    TH0 = 0xFC; TL0 = 0x66;
    tick++;
    if(tick >= 10) { tick=0; ms++; if(ms>=20) { ms=0; if(up) duty++; else duty--; if(duty>=100) up=0; if(duty==0) up=1; } }
    led = (tick < duty/10);
}
void main() {
    TMOD = 0x01; TH0=0xFC; TL0=0x66; ET0=1; EA=1; TR0=1;
    while(1);
}
