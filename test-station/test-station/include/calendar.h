/*
 * calendar.h
 *
 * Created: 01/05/2023 14:39:48
 *  Author: xstejs30
 */ 


#ifndef CALENDAR_H_
#define CALENDAR_H_

#include "../driver_init.h"

#include <stdio.h>

/************************************************************************/
/* Struct containing: year, month, day, hour, minute, second            */
/* - year: offset from year 2000 (range: 0--99)                         */
/* - month: Month number (1--12)                                        */
/* - day: Day of the month (1--31)                                      */
/* - hour, min, sec: Hour (0--23), minute (0--59) and second (0--59)    */
/************************************************************************/
typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} date_time_t;

void calendar_initialization(date_time_t *p_date_time);

// Callback function for Calendar alarm
void alarm_cb(struct calendar_descriptor *const descr);

uint8_t conv_str_to_2digits(const char *str_two_digits);
void conv_str_to_date_time(date_time_t *date_time, const char *date, const char *time);

#endif /* CALENDAR_H_ */