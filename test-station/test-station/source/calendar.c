/*
 * calendar.c
 *
 * Created: 01/05/2023 14:40:30
 *  Author: xstejs30
 */ 

#include "../include/calendar.h"
#include "../driver_init.h"
#include "../include/edbg.h"

#include <stdlib.h>
#include <string.h>

void calendar_initialization(date_time_t *p_date_time){
	static struct calendar_alarm alarm1;//, alarm2;
	
	struct calendar_date date;
	struct calendar_time time;
	
	calendar_enable(&CALENDAR);
	
	date.year  = (uint16_t)2000 + p_date_time->year;
	date.month = p_date_time->month;
	date.day   = p_date_time->day;

	time.hour = p_date_time->hour;
	time.min  = p_date_time->min;
	time.sec  = p_date_time->sec;
	
	calendar_set_date(&CALENDAR, &date);
	calendar_set_time(&CALENDAR, &time);
	
	// this is set to alarm every sec, but it alarms every minute...why? <--------------------
	alarm1.cal_alarm.datetime.time.sec = 1;
	alarm1.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm1.cal_alarm.mode              = REPEAT;  // Alarm every sec
	
	// 	alarm2.cal_alarm.datetime.time.sec = 5;
	// 	alarm2.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	// 	alarm2.cal_alarm.mode              = ONESHOT; // alarm once after 5 secs

	calendar_set_alarm(&CALENDAR, &alarm1, alarm_cb);
	// 	calendar_set_alarm(&CALENDAR, &alarm2, alarm_cb);
}

void alarm_cb(struct calendar_descriptor *const descr){
	struct calendar_date_time dateTime;           // create calendar_date_time struct
	calendar_get_date_time(&CALENDAR, &dateTime); // get actual date and time
	
	char *alarm_msg = malloc(32);
	sprintf(alarm_msg, "Alarm at %d-%02d-%02d  %02d:%02d:%02d\n",
	dateTime.date.year, dateTime.date.month, dateTime.date.day,
	dateTime.time.hour, dateTime.time.min, dateTime.time.sec);
	
// 	io_write(edbg_io, (uint8_t *)alarm_msg, strlen(alarm_msg));
	free(alarm_msg);
}

/************************************************************************/
/* Converts a string containing two digits to uint8_t                   */
/*                                                                      */
/* e.g. "09" returns 9                                                  */
/************************************************************************/
uint8_t conv_str_to_2digits(const char *str_two_digits) {
	uint8_t tmp = 0;
	if ('0' <= *str_two_digits && *str_two_digits <= '9')
	tmp = *str_two_digits - '0';
	return (10 * tmp + *++str_two_digits - '0');
}

/************************************************************************/
/* Converts parameters date and time (strings in the format generated   */
/* by the compiler's preprocessor macros `__DATE__` and `__TIME__`) to  */
/* date_time_t struct.                                                  */
/************************************************************************/
void conv_str_to_date_time(date_time_t *date_time, const char *date, const char *time) {
	date_time->year = conv_str_to_2digits(date + 9);
	
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	switch (date[0]) {
		case 'J':
		date_time->month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7);
		break;
		case 'F':
		date_time->month = 2;
		break;
		case 'A':
		date_time->month = date[2] == 'r' ? 4 : 8;
		break;
		case 'M':
		date_time->month = date[2] == 'r' ? 3 : 5;
		break;
		case 'S':
		date_time->month = 9;
		break;
		case 'O':
		date_time->month = 10;
		break;
		case 'N':
		date_time->month = 11;
		break;
		case 'D':
		date_time->month = 12;
		break;
	}
	
	date_time->day =  conv_str_to_2digits(date + 4);
	date_time->hour = conv_str_to_2digits(time);
	date_time->min =  conv_str_to_2digits(time + 3);
	date_time->sec =  conv_str_to_2digits(time + 6);
}