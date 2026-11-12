#include <reg52.h>

// Pin Definitions for 4-bit HD44780 LCD Interface
sbit RS = P2^0;
sbit EN = P2^1;
sbit D4 = P2^4;
sbit D5 = P2^5;
sbit D6 = P2^6;
sbit D7 = P2^7;

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<120; j++);
}

void lcd_pulse(void) {
    EN = 1;
    delay_ms(1);
    EN = 0;
    delay_ms(1);
}

void lcd_cmd(unsigned char cmd) {
    RS = 0; // Command mode
    // Higher nibble
    D4 = (cmd & 0x10) ? 1 : 0;
    D5 = (cmd & 0x20) ? 1 : 0;
    D6 = (cmd & 0x40) ? 1 : 0;
    D7 = (cmd & 0x80) ? 1 : 0;
    lcd_pulse();
    // Lower nibble
    D4 = (cmd & 0x01) ? 1 : 0;
    D5 = (cmd & 0x02) ? 1 : 0;
    D6 = (cmd & 0x04) ? 1 : 0;
    D7 = (cmd & 0x08) ? 1 : 0;
    lcd_pulse();
}

void lcd_data(unsigned char dat) {
    RS = 1; // Data mode
    // Higher nibble
    D4 = (dat & 0x10) ? 1 : 0;
    D5 = (dat & 0x20) ? 1 : 0;
    D6 = (dat & 0x40) ? 1 : 0;
    D7 = (dat & 0x80) ? 1 : 0;
    lcd_pulse();
    // Lower nibble
    D4 = (dat & 0x01) ? 1 : 0;
    D5 = (dat & 0x02) ? 1 : 0;
    D6 = (dat & 0x04) ? 1 : 0;
    D7 = (dat & 0x08) ? 1 : 0;
    lcd_pulse();
}

void lcd_init(void) {
    delay_ms(20);
    lcd_cmd(0x02); // Initialize in 4-bit mode
    lcd_cmd(0x28); // 2 line, 5x8 matrix
    lcd_cmd(0x0C); // Display ON, cursor OFF
    lcd_cmd(0x06); // Auto increment cursor
    lcd_cmd(0x01); // Clear display
    delay_ms(5);
}

void lcd_puts(char *s) {
    while(*s) {
        lcd_data(*s++);
    }
}

void main(void) {
    lcd_init();
    lcd_puts("Hello SNU Chennai"); // Row 1 string
    lcd_cmd(0xC0);                  // Move to Row 2 (0x80 + 0x40)
    lcd_puts("IoT Lab CAT-7A");     // Row 2 string
    
    while(1);
}