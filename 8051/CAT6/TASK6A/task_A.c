#include <reg52.h>

// Function to initialize UART for 9600 baud, 8-bit data, 1 stop bit, no parity
void UART_Init(void) {
    TMOD &= 0x0F;     // Clear Timer 1 mode bits
    TMOD |= 0x20;     // Put Timer 1 in Mode 2 (8-bit auto-reload)
    TH1 = 0xFD;       // 9600 baud rate reload value for 11.0592 MHz crystal
    TL1 = 0xFD;
    TR1 = 1;          // Start Timer 1
    
    SCON = 0x50;      // Mode 1 (8-bit UART), REN=1 (enable receiver)
    TI = 1;           // Set TI to allow first print/transmission
}

// Function to transmit a single character
void UART_SendChar(char c) {
    SBUF = c;         // Load character into Serial Buffer
    while (TI == 0);  // Wait until transmission is complete
    TI = 0;           // Clear Transmit Interrupt flag
}

// Function to transmit a string continuously
void UART_SendString(char *s) {
    while (*s) {
        UART_SendChar(*s++);
    }
}

// Millisecond delay loop for 11.0592 MHz crystal
void delay(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++);
    }
}

void main(void) {
    UART_Init();      // Initialize serial communication at 9600 baud[cite: 11]
    
    while(1) {
        UART_SendString("Hello from 8051\r\n");
        delay(1000);  // Delay 1 second between transmissions
    }
}