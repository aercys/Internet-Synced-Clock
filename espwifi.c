//
//  espwifi.c
//  TemperatureServer
//
//  Created by Atakan E on 29/03/2017.
//  Copyright © 2017 aercys. All rights reserved.
//


#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "espwifi.h"
#include "uart.h"


short init_wifi_module(void) {
    
    uart_init(9600);
    _delay_ms(10000);
    
    
    printf(TEST_MODULE);
    uart_getall("OK");
    
    return 1;
}

short connect_wifi(unsigned char *ssid_name, unsigned char *ssid_password) {
    
    
    printf(CW_MODE, 1);
    uart_getall("OK");
    
    
    printf(CIP_MODE, 0);
    uart_getall("OK");
    
    
    printf(CIP_MUX, 0);
    uart_getall("OK");
    
    
    printf(WIFI_CONNECT, ssid_name, ssid_password);
    uart_getall("OK");
    
    
    return 1;
}




