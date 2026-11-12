#include <reg51.h>
unsigned char step = 0;
unsigned char seq[4] = {0x08,0x04,0x02,0x01};
unsigned int ticks = 0;
void timer0() interrupt 1 {
    TH0=0xFC; TL0=0x66; ticks++;
    if(ticks>=50) { ticks=0; P1=seq[step]; step=(step+1)&3; }
}
void main() {
    TMOD=0x01; TH0=0xFC; TL0=0x66; ET0=1; EA=1; TR0=1;
    while(1);
}
