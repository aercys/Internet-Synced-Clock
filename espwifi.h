//
//  espwifi.h
//  TemperatureServer
//
//  Created by Atakan E on 29/03/2017.
//  Copyright © 2017 aercys. All rights reserved.
//

#ifndef espwifi_h
#define espwifi_h

#include <stdio.h>

#define ESP_BAUDRATE        9600

#define TEST_MODULE         "AT\r\n"
#define AT_ECHO             "ATE%d\r\n"

#define CW_MODE             "AT+CWMODE=%d\r\n"
#define CIP_MODE            "AT+CIPMODE=%d\r\n"
#define CIP_MUX             "AT+CIPMUX=%d\r\n"
#define CIP_START_MUX       "AT+CIPSTART=%d,\"%s\",\"%s\",%d\r\n"
#define CIP_START           "AT+CIPSTART=\"%s\",\"%s\",%d\r\n"
#define CIP_SEND_MUX        "AT+CIPSEND=%d,%d\r\n"
#define CIP_SEND            "AT+CIPSEND=%d\r\n"

#define WIFI_CONNECT        "AT+CWJAP=\"%s\",\"%s\"\r\n"
#define SHOW_LOCAL_IP       "AT+CIFSR\r\n"

#define SSID_NAME           "SSID"
#define SSID_PASSWORD       "PASS"

#define GET_REQUEST_STR     "GET %s HTTP/1.0\r\n\r\n"
#define POST_REQUEST        "POST %s HTTP/1.1\r\n" \
                            "Host: %s\r\n" \
                            "Connection: close\r\n" \
                            "Content-Type: application/x-www-form-urlencoded\r\n" \
                            "Content-Length: %lu\r\n\r\n" \
                            "%s"


extern short init_wifi_module(void);
extern short connect_wifi(unsigned char *ssid_name, unsigned char *ssid_password);

#endif /* espwifi_h */
