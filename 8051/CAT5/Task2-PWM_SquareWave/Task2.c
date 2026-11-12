#include <reg52.h>

sbit PWM_LED = P1^1;  // LED for Software PWM fading
sbit SQ_WAVE  = P1^2;  // Pin for Square-Wave Generation

unsigned char pwm_counter = 0;
unsigned char duty_cycle = 50;   // Duty cycle percentage (0-100)
unsigned char fade_direction = 1; // 1 for increasing, 0 for decreasing
unsigned int fade_delay_count = 0;

unsigned int sq_wave_counter = 0;
unsigned int sq_wave_reload = 10; // Controls frequency

// Timer 0 Interrupt Service Routine (1 ms time base)[cite: 9]
void Timer0_ISR(void) interrupt 1 {
    TH0 = 0xFC;
    TL0 = 0x66;
    
    // --- Task 2A: Software PWM Generation ---
    pwm_counter++;
    if (pwm_counter >= 100) {
        pwm_counter = 0;
    }
    
    if (pwm_counter < duty_cycle) {
        PWM_LED = 1;
    } else {
        PWM_LED = 0;
    }
    
    // Gradually update duty cycle every 50ms for smooth fading
    fade_delay_count++;
    if (fade_delay_count >= 50) {
        fade_delay_count = 0;
        if (fade_direction) {
            duty_cycle += 10;
            if (duty_cycle >= 100) fade_direction = 0;
        } else {
            duty_cycle -= 10;
            if (duty_cycle <= 0) fade_direction = 1;
        }
    }
    
    // --- Task 2B: Variable-Frequency Square-Wave Generation ---
    sq_wave_counter++;
    if (sq_wave_counter >= sq_wave_reload) {
        SQ_WAVE = ~SQ_WAVE;
        sq_wave_counter = 0;
    }
}

void main(void) {
    PWM_LED = 0;
    SQ_WAVE = 0;
    
    // Configure Timer 0 in Mode 1
    TMOD &= 0xF0;
    TMOD |= 0x01;
    
    TH0 = 0xFC;
    TL0 = 0x66;
    
    ET0 = 1;
    EA  = 1;
    TR0 = 1;
    
    while(1) {
        // Non-blocking architecture driven entirely by hardware timer interrupts[cite: 8]
    }
}