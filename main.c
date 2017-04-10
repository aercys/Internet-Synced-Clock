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

char *lcd_buffer;

ISR(TIMER2_OVF_vect) {
    update_rtc();
    TCNT2 = 0x00;
}


void update_display() {
    
    calculate();
    
    lcd_gotoxy(0, 0);
    sprintf(lcd_buffer, "%.2f %cC",  (float) get_temperature(), (char) 223);
    lcd_puts(lcd_buffer);
    
    sprintf(lcd_buffer, "%02d:%02d", get_hour(), get_minute());
    lcd_gotoxy(11, 0);
    lcd_puts(lcd_buffer);
    
    sprintf(lcd_buffer, "%02d/%02d/%02d", get_day(), get_month(), get_year());
    lcd_gotoxy(0, 1);
    lcd_puts(lcd_buffer);
    
    _delay_ms(1000);
}

void system_init() {
    
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    
    lcd_gotoxy(0, 0);
    lcd_puts("Connecting...");
    
    lcd_gotoxy(0, 1);
    lcd_puts("Please wait");
    
    if (!init_wifi_module()) {
        lcd_clrscr();
        lcd_gotoxy(0, 0);
        lcd_puts("Wifi Error");
    }
    
    init_sensor(bmp180_mode_0);
    init_timer();
    
    sei();
    
    lcd_clrscr();
    lcd_gotoxy(0, 0);
    lcd_puts("System Init");
    lcd_gotoxy(0, 1);
    lcd_puts("Succesful");
    
    _delay_ms(100);
    lcd_clrscr();
    
}



int main(void) {
    
    stdout = &uart_output;
    
    lcd_buffer = malloc(sizeof(char *) * 20);
    
    system_init();
    
    while (1) {
        update_display();
    }
    
	return 0; // never reached
}
