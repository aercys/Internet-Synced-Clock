#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "uart.h"
#include "lcd.h"
#include "bmp180.h"
#include "rtc.h"
#include "espwifi.h"


FILE uart_output = FDEV_SETUP_STREAM(send_uart_char, NULL, _FDEV_SETUP_WRITE);

volatile second_ticks = 0;

ISR(TIMER2_OVF_vect) {
    update_rtc();
    if (second_ticks++ == 5) {
        second_ticks = 0;
    }
}


void lcd_msg(unsigned char *fmt, uint8_t line, ...) {
    va_list aptr;
    unsigned char *lcd_buffer = malloc(sizeof(unsigned char *) * 20);
    
    unsigned char padding[15] = {0x20, 0x20, 0x20, 0x20, 0x20,
                                0x20, 0x20, 0x20, 0x20, 0x20,
                                0x20, 0x20, 0x20, 0x20, '\0'};
    
    if (line < 1) line = 1;
    
    va_start(aptr, fmt);
    vsprintf(lcd_buffer, fmt, aptr);
    lcd_gotoxy(0, line - 1);
    lcd_puts((unsigned char *) padding);
    lcd_gotoxy(0, line - 1);
    lcd_puts(lcd_buffer);
    free(lcd_buffer);
}

void update_display() {
    calculate();
    lcd_clrscr();
    lcd_msg("%.2f %cC %02d:%02d", 1, (float) get_temperature(), (char) 223, get_hour(), get_minute());
    lcd_msg("%02d/%02d/%02d", 2, get_day(), get_month(), get_year());
    _delay_ms(1000);
}

void system_init() {
    
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    lcd_puts("Init System!\n");
    lcd_msg("Wifi: %s\n", 2, "[Init]");
    if (init_wifi_module())
    lcd_msg("Wifi: %s", 2, "[Ok]");
    else
    lcd_msg("Wifi: %s", 2, "[Fail]");
    lcd_msg("Sensor: %s", 2, "[Busy]");
    init_sensor(bmp180_mode_0);
    lcd_msg("Sensor: %s", 2, "[Ok]");
    lcd_msg("Timer: %s", 2, "[Busy]");
    init_timer();
    sei();
    lcd_msg("Timer: %s", 2, "[Ok]");
    lcd_clrscr();
    lcd_msg("System started!", 1);
}



int main(void) {
    
    stdout = &uart_output;
    
    system_init();
    
    while (1) {
        update_display();
        _delay_ms(500);
    }
    
	return 0; // never reached
}
