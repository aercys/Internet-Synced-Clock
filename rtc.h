//
//  rtc.h
//  TemperatureServer
//
//  Created by Atakan ERCIYAS on 03/04/2017.
//  Copyright © 2017 aercys. All rights reserved.
//

#ifndef rtc_h
#define rtc_h

#include <stdio.h>

extern void init_timer(void);
extern void update_rtc(void);
extern int get_second(void);
extern int get_minute(void);
extern int get_hour(void);
extern int get_day(void);
extern int get_month(void);
extern int get_year(void);
extern void sync_time(void);
extern char *make_time_request(void);
extern uint8_t is_leap_year(int year);

#endif /* rtc_h */
