#include <reg52.h>  // Include 8051 register definitions

// Define the LED pin connected to Port 1, Pin 0
sbit LED = P1^0;

// Simple software delay function
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++) {
            // Inner loop for delay calibration
        }
    }
}

void main(void) {
    while (1) {
        LED = 0;            // Turn LED ON (assuming active-low hardware connection)
        delay_ms(500);      // Wait ~500ms
        LED = 1;            // Turn LED OFF
        delay_ms(500);      // Wait ~500ms
    }
}