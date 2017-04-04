//
//  uart.c
//  TemperatureServer
//
//  Created by Atakan ERCIYAS on 29/03/2017.
//  Copyright © 2017 aercys. All rights reserved.
//

#include "uart.h"
#include <util/delay.h>


void uart_init(int baudrate) {
    
    uint16_t UBRR = F_CPU / 16 / baudrate - 1;
    
    UBRR0H = (uint8_t) (UBRR >> 8);
    UBRR0L = (uint8_t) UBRR;
    
    UCSR0B |= (1<<TXEN0) | (1<<RXEN0);
    UCSR0C |= (0<<USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
    
}

void send_uart_char(unsigned char serial_data, FILE *stream) {
    
    if (serial_data == '\0') { // Send each byte until reached \n character
        send_uart_char('\r', stream);
        send_uart_char('\n', stream);
        return;
    }
    
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = serial_data;
}

char receive_uart_char(void) {
    
    while (!(UCSR0A & (1<<RXC0)));
    
    return UDR0;
}


int uart_receive(char *buffer) {
    
    char received;
    
    int i = 0;
    
    while (1) {
        if (received == '\n' || received == '\r') break;
        received = receive_uart_char();
        buffer[i++] = received;
    }
    
    return i;
    
}

char *uart_getall(char *delimeter) {
    
    char resp[512] = {0};
    char buffer[32] = {0};
    int i = 0, total = 0;
    
    while (i = uart_receive(buffer)) {
        if (strstr(buffer, delimeter) > 0)
            break;
        sprintf(resp + total, buffer);
        memset(buffer, 0, sizeof(uint8_t) * 32);
        total += i;
    }
    
    resp[i+1] = '\0';
    
    return resp;
}






