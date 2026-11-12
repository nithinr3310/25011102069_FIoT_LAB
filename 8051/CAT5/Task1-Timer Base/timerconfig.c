#include <reg52.h>

sbit LED = P1^0; // LED connected to P1.0

// Timer 0 Interrupt Service Routine (triggered every 1 ms)
// Preload values for 1ms using 11.0592 MHz crystal: TH0 = 0xFC, TL0 = 0x66[cite: 9]
void Timer0_ISR(void) interrupt 1 {
    static unsigned int count = 0;
    
    // Reload Timer 0
    TH0 = 0xFC;
    TL0 = 0x66;
    
    count++;
    if (count >= 1000) { // 1000 ticks * 1 ms = 1 second
        LED = ~LED;      // Toggle LED every 1 second
        count = 0;
    }
}

void main(void) {
    LED = 0;
    
    // Configure Timer 0 in Mode 1 (16-bit timer)
    TMOD &= 0xF0; // Clear Timer 0 control bits
    TMOD |= 0x01; // Set Timer 0 to Mode 1
    
    // Initial preload for 1 ms interrupt
    TH0 = 0xFC;
    TL0 = 0x66;
    
    // Enable Timer 0 Interrupt and start Timer 0
    ET0 = 1;      // Enable Timer 0 Interrupt
    EA  = 1;      // Enable Global Interrupts
    TR0 = 1;      // Start Timer 0
    
    while(1) {
        // Main loop remains completely non-blocking; timing handled entirely by ISR
    }
}