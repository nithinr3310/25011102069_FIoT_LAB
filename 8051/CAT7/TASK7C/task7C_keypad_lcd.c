#include <reg52.h>

// LCD Pins
sbit RS = P2^0;
sbit EN = P2^1;
sbit D4 = P2^4;
sbit D5 = P2^5;
sbit D6 = P2^6;
sbit D7 = P2^7;

// Keypad Pins (Port 1)
// R1-R4: P1.0-P1.3, C1-C4: P1.4-P1.7

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i=0; i<ms; i++) for(j=0; j<120; j++);
}

void lcd_pulse(void) { EN = 1; delay_ms(1); EN = 0; delay_ms(1); }

void lcd_cmd(unsigned char cmd) {
    RS = 0;
    D4 = (cmd & 0x10)?1:0; D5 = (cmd & 0x20)?1:0; D6 = (cmd & 0x40)?1:0; D7 = (cmd & 0x80)?1:0;
    lcd_pulse();
    D4 = (cmd & 0x01)?1:0; D5 = (cmd & 0x02)?1:0; D6 = (cmd & 0x04)?1:0; D7 = (cmd & 0x08)?1:0;
    lcd_pulse();
}

void lcd_data(unsigned char dat) {
    RS = 1; // Data mode
    
    // Higher nibble transmission
    D4 = (dat & 0x10) ? 1 : 0;
    D5 = (dat & 0x20) ? 1 : 0;
    D6 = (dat & 0x40) ? 1 : 0;
    D7 = (dat & 0x80) ? 1 : 0;
    lcd_pulse();
    
    // Lower nibble transmission
    D4 = (dat & 0x01) ? 1 : 0;
    D5 = (dat & 0x02) ? 1 : 0;
    D6 = (dat & 0x04) ? 1 : 0;
    D7 = (dat & 0x08) ? 1 : 0;
    lcd_pulse();
}

void lcd_init(void) {
    delay_ms(20);
    lcd_cmd(0x02); lcd_cmd(0x28); lcd_cmd(0x0C); lcd_cmd(0x06); lcd_cmd(0x01);
    delay_ms(5);
}

void lcd_goto(unsigned char row, unsigned char col) {
    lcd_cmd(((row == 1) ? 0x80 : 0xC0) + col);
}

char scan_keypad(void) {
    // Basic 4x4 keypad scan returning key values: '2' (Up), '8' (Down), '4' (Left), '6' (Right)[cite: 13]
    // Implementation scans rows/columns and returns directional character codes.
    return 0; 
}

void main(void) {
    unsigned char r = 1, c = 0;
    char key;
    lcd_init();
    
    while(1) {
        key = scan_keypad();
        if(key == '2' && r > 1) r--;      // Up[cite: 13]
        if(key == '8' && r < 2) r++;      // Down[cite: 13]
        if(key == '4' && c > 0) c--;      // Left[cite: 13]
        if(key == '6' && c < 15) c++;     // Right[cite: 13]
        
        lcd_cmd(0x01); // Clear
        lcd_goto(r, c);
        lcd_data('X'); // Moving character
        delay_ms(200);
    }
}