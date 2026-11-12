#include <reg52.h>

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
    EN = 1; delay_ms(1); EN = 0; delay_ms(1);
}

void lcd_cmd(unsigned char cmd) {
    RS = 0;
    D4 = (cmd & 0x10)?1:0; D5 = (cmd & 0x20)?1:0; D6 = (cmd & 0x40)?1:0; D7 = (cmd & 0x80)?1:0;
    lcd_pulse();
    D4 = (cmd & 0x01)?1:0; D5 = (cmd & 0x02)?1:0; D6 = (cmd & 0x04)?1:0; D7 = (cmd & 0x08)?1:0;
    lcd_pulse();
}

void lcd_data(unsigned char dat) {
    RS = 1;
    D4 = (dat & 0x10)?1:0; D5 = (dat & 0x20)?1:0; D6 = (dat & 0x40)?1:0; D7 = (dat & 0x80)?1:0;
    lcd_pulse();
    D4 = (dat & 0x01)?1:0; D5 = (dat & 0x02)?1:0; D6 = (dat & 0x04)?1:0; D7 = (dat & 0x08)?1:0;
    lcd_pulse();
}

void lcd_init(void) {
    delay_ms(20);
    lcd_cmd(0x02); lcd_cmd(0x28); lcd_cmd(0x0C); lcd_cmd(0x06); lcd_cmd(0x01);
    delay_ms(5);
}

// lcd_goto implementation for rows (1 or 2) and columns (0 to 15)
void lcd_goto(unsigned char row, unsigned char col) {
    unsigned char address = (row == 1) ? 0x80 : 0xC0;
    address += col;
    lcd_cmd(address);
}

void lcd_puts(char *s) {
    while(*s) lcd_data(*s++);
}

void lcd_print_number(unsigned int value) {
    char buf[6];
    int i = 0;
    if (value == 0) {
        lcd_data('0');
        return;
    }
    while (value > 0) {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }
    while (i > 0) {
        lcd_data(buf[--i]);
    }
}

void main(void) {
    unsigned int counter = 0;
    lcd_init();
    
    lcd_goto(1, 0);
    lcd_puts("Dynamic Value:");
    
    while(1) {
        lcd_goto(2, 0);
        lcd_puts("Count: ");
        lcd_print_number(counter++);
        delay_ms(500);
    }
}