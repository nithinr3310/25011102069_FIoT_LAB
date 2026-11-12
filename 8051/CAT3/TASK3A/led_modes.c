#include <reg52.h>

// Pin definitions
#define LED_PORT P1          // P1.0 to P1.3 connected to the 4 LEDs
sbit BUTTON = P3^2;          // Push-button connected to P3.2

// Function prototypes
void delay(unsigned int ms);
void handle_button(void);
void run_mode_0(void);
void run_mode_1(void);
void run_mode_2(void);
void run_mode_3(void);
void run_mode_4(void);

unsigned char mode = 0;      // 0: Initial, 1: Running, 2: Binary, 3: Alternating, 4: Flash

// Approximate millisecond delay loop for 11.0592 MHz crystal
void delay(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 120; j++) {
            // Internal loop ticks
        }
    }
}

// Button polling with debounce and release check (1 press = 1 transition)
void handle_button(void) {
    if (BUTTON == 0) {       // Button pressed (Active Low)
        delay(20);           // Software debounce delay
        if (BUTTON == 0) {
            mode++;
            if (mode > 4) {
                mode = 1;    // Cycle back to Mode 1 after Mode 4
            }
            // Wait until the user releases the button
            while (BUTTON == 0) {
                delay(10);
            }
        }
    }
}

// Initial State: All LEDs OFF (0000)
void run_mode_0(void) {
    LED_PORT = 0x00;
    handle_button();
}

// Mode 1: Running LED Pattern (LED1 -> LED2 -> LED3 -> LED4)
void run_mode_1(void) {
    unsigned char patterns[] = {0x01, 0x02, 0x04, 0x08};
    unsigned char i, d;
    for(i = 0; i < 4; i++) {
        LED_PORT = patterns[i];
        for(d = 0; d < 20; d++) {
            delay(10);
            handle_button();
            if (mode != 1) return; // Exit immediately on state change
        }
    }
}

// Mode 2: Binary Counter (0000 to 1111)
void run_mode_2(void) {
    unsigned char count, d;
    for(count = 0; count < 16; count++) {
        LED_PORT = count & 0x0F;
        for(d = 0; d < 30; d++) {
            delay(10);
            handle_button();
            if (mode != 2) return;
        }
    }
}

// Mode 3: Alternating Pattern (1010 <-> 0101)
void run_mode_3(void) {
    unsigned char d;
    LED_PORT = 0x0A; // 1010
    for(d = 0; d < 30; d++) {
        delay(10);
        handle_button();
        if (mode != 3) return;
    }
    LED_PORT = 0x05; // 0101
    for(d = 0; d < 30; d++) {
        delay(10);
        handle_button();
        if (mode != 3) return;
    }
}

// Mode 4: All-LED Flash (1111 <-> 0000)
void run_mode_4(void) {
    unsigned char d;
    LED_PORT = 0x0F; // All ON
    for(d = 0; d < 25; d++) {
        delay(10);
        handle_button();
        if (mode != 4) return;
    }
    LED_PORT = 0x00; // All OFF
    for(d = 0; d < 25; d++) {
        delay(10);
        handle_button();
        if (mode != 4) return;
    }
}

void main(void) {
    BUTTON = 1;              // Set P3.2 as input high (pull-up)
    LED_PORT = 0x00;
    mode = 0;

    while(1) {
        switch(mode) {
            case 0: run_mode_0(); break;
            case 1: run_mode_1(); break;
            case 2: run_mode_2(); break;
            case 3: run_mode_3(); break;
            case 4: run_mode_4(); break;
            default: mode = 0; break;
        }
    }
}