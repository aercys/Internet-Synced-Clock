//
//  rtc.c
//  TemperatureServer
//
//  Created by Atakan ERCIYAS on 03/04/2017.
//  Copyright © 2017 aercys. All rights reserved.
//

#include "rtc.h"
#include "espwifi.h"
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

int second = 0, minute = 0, hour = 0;
int day = 0, month = 0, year = 0;

void sync_time() {
    char *response;
    char *timevar = malloc(sizeof(char) * 22);
    int offset;
    
    connect_wifi(SSID_NAME, SSID_PASSWORD);
    response = make_time_request();
    
    offset = strstr(response, "<<") - response;
    
    
    strcpy(timevar, &response[offset]);
    sscanf(timevar, "<<%d:%d:%d&%d/%d/%d>>", &hour, &minute, &second, &day, &month, &year);
}


char *make_time_request() {
    
    printf(CIP_START,"TCP", "erciyas.biz", 80);
    uart_getall("OK");
    printf(CIP_SEND, strlen(GET_REQUEST_STR) + strlen("http://erciyas.biz/time.php") - 2);
    uart_getall("OK");
    printf(GET_REQUEST_STR, "http://erciyas.biz/time.php");
    return uart_getall("CLOSED");;
}

void init_timer() {
    
    sync_time();
    
    ASSR |= (1 << AS2);
    TCCR2B |= (1 << CS22) | (1 << CS20);
    TCNT2 = 0x00;
    
    TIFR2 |= (1 << TOV2);
    TIMSK2 |= (1 << TOIE2);
    
}

void update_rtc() {
    if (++second == 60) {
        minute++;
        second = 0;
    }
    
    if (minute == 60) {
        hour++;
        minute = 0;
    }
    
    if (hour == 24) {
        day++;
        hour = 0;
    }
    
    if (day == 32) {
        month++;
        day = 1;
    } else if ((day == 31) && (month == 4 || month == 6 || month == 9 || month == 11)) {
        month++;
        day = 1;
    } else if ((month == 2) && (day == 29) && !is_leap_year(year)) {
        month++;
        day = 1;
    } else if ((month == 2) && (day == 30)) {
        month++;
        day = 1;
    }
    
    if (month == 13) {
        year++;
        month = 1;
    }
    
}

uint8_t is_leap_year(int year) {
    if (year % 100)
        return (year % 400) ? 1 : 0;
    else
        return (year % 4) ? 1 : 0;
}

int get_second() {
    return second;
}

int get_minute() {
    return minute;
}

int get_hour() {
    return hour;
}

int get_day() {
    return day;
}

int get_month() {
    return month;
}

int get_year() {
    return year;
}
