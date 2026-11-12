#include <reg52.h>

// Function to initialize UART for 9600 baud, 8-bit data, 1 stop bit, no parity
void UART_Init(void) {
    TMOD &= 0x0F;     // Clear Timer 1 mode bits
    TMOD |= 0x20;     // Put Timer 1 in Mode 2 (8-bit auto-reload)
    TH1 = 0xFD;       // 9600 baud rate reload value for 11.0592 MHz crystal
    TL1 = 0xFD;
    TR1 = 1;          // Start Timer 1
    
    SCON = 0x50;      // Mode 1 (8-bit UART), REN=1 (enable receiver)
}

// Function to receive a character (polling method)
char UART_ReceiveChar(void) {
    while (RI == 0);  // Wait until character is received
    RI = 0;           // Clear Receive Interrupt flag
    return SBUF;      // Read received data from SBUF
}

// Function to transmit a character
void UART_SendChar(char c) {
    SBUF = c;         // Load character into Serial Buffer
    while (TI == 0);  // Wait until transmission is complete
    TI = 0;           // Clear Transmit Interrupt flag
}

void main(void) {
    char received_char;
    
    UART_Init();      // Initialize UART communication[cite: 11]
    
    while(1) {
        received_char = UART_ReceiveChar(); // Wait for character from PuTTY[cite: 13]
        UART_SendChar(received_char);       // Echo exact character back to terminal[cite: 12]
    }
}