#include <reg52.h>

// Port definitions
#define LED_PORT P1          // P1.0 to P1.3 connected to 4 LEDs
#define SEG_PORT P2          // 7-segment display data lines
#define KEYPAD_PORT P3       // Matrix keypad interface port

// Required Function Prototypes
void keypad_scan(void);
void display_digit(unsigned char value);
void led_control(unsigned char key);
void running_led(void);
void binary_counter(void);
void alternate_led(void);
void blink_all(void);
void delay(unsigned int ms);

// Seven-segment digit encoding (Common Cathode: 0-9)
code unsigned char seg_codes[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Millisecond delay loop for 11.0592 MHz crystal
void delay(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 120; j++);
    }
}

// Display digit on seven-segment display
void display_digit(unsigned char value) {
    if(value <= 9) {
        SEG_PORT = seg_codes[value];
    } else {
        SEG_PORT = 0x00;
    }
}

// Running LED Pattern (Mode 5)
void running_led(void) {
    unsigned char patterns[] = {0x01, 0x02, 0x04, 0x08};
    unsigned char i;
    for(i = 0; i < 4; i++) {
        LED_PORT = patterns[i];
        delay(200);
    }
}

// Binary Counter 0-15 (Mode 7)
void binary_counter(void) {
    unsigned char count;
    for(count = 0; count < 16; count++) {
        LED_PORT = count & 0x0F;
        delay(250);
    }
}

// Alternate LEDs (1010 <-> 0101) (Mode 6)
void alternate_led(void) {
    LED_PORT = 0x0A; // 1010
    delay(250);
    LED_PORT = 0x05; // 0101
    delay(250);
}

// All LEDs Blink (Mode 8)
void blink_all(void) {
    LED_PORT = 0x0F; // All ON
    delay(200);
    LED_PORT = 0x00; // All OFF
    delay(200);
}

// LED Control dispatcher based on key pressed[cite: 5]
void led_control(unsigned char key) {
    switch(key) {
        case 1: LED_PORT = 0x01; break; // LED1 ON[cite: 5]
        case 2: LED_PORT = 0x02; break; // LED2 ON[cite: 5]
        case 3: LED_PORT = 0x04; break; // LED3 ON[cite: 5]
        case 4: LED_PORT = 0x08; break; // LED4 ON[cite: 5]
        case 5: running_led(); break;     // Running LED Pattern[cite: 5]
        case 6: alternate_led(); break;   // Alternate LEDs[cite: 5]
        case 7: binary_counter(); break;  // Binary Counter[cite: 5]
        case 8: blink_all(); break;       // All LEDs Blink[cite: 5]
        case 9: LED_PORT = 0x00; break;   // All LEDs OFF[cite: 5]
        case 0:                            // Reset System[cite: 5]
            LED_PORT = 0x00; 
            SEG_PORT = 0x3F; 
            break;
        default: break;
    }
}

// Keypad scanning function[cite: 5]
void keypad_scan(void) {
    unsigned char pressed_key = 0xFF;

    // Row 1 scan
    KEYPAD_PORT = 0xFE;
    if(KEYPAD_PORT == 0xEE) pressed_key = 1;
    if(KEYPAD_PORT == 0xDE) pressed_key = 2;
    if(KEYPAD_PORT == 0xBE) pressed_key = 3;

    // Row 2 scan
    KEYPAD_PORT = 0xFD;
    if(KEYPAD_PORT == 0xED) pressed_key = 4;
    if(KEYPAD_PORT == 0xDD) pressed_key = 5;
    if(KEYPAD_PORT == 0xBD) pressed_key = 6;

    // Row 3 scan
    KEYPAD_PORT = 0xFB;
    if(KEYPAD_PORT == 0xEB) pressed_key = 7;
    if(KEYPAD_PORT == 0xDB) pressed_key = 8;
    if(KEYPAD_PORT == 0xBB) pressed_key = 9;

    // Row 4 scan
    KEYPAD_PORT = 0xF7;
    if(KEYPAD_PORT == 0xE7) pressed_key = 0;

    if(pressed_key != 0xFF) {
        delay(20); // Software debounce
        display_digit(pressed_key);
        led_control(pressed_key);
    }
}

void main(void) {
    LED_PORT = 0x00;
    SEG_PORT = 0x3F; // Initial display '0'

    while(1) {
        keypad_scan();
        delay(50);
    }
}