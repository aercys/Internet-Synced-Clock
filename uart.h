//
//  uart.h
//  TemperatureServer
//
//  Created by Atakan ERCIYAS on 29/03/2017.
//  Copyright © 2017 aercys. All rights reserved.
//

#ifndef uart_h
#define uart_h

#include <stdio.h>
#include <string.h>
#include <avr/io.h>

extern void uart_init(int baudrate);
extern void send_uart_char(unsigned char serial_data, FILE *stream);

extern char receive_uart_char(void);
extern int uart_receive(char *buffer);
extern char *uart_getall(char *delimeter);

#endif /* uart_h */
