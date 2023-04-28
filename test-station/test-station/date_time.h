/*
 * date_time.h
 *
 * Created: 28/04/2023 20:56:59
 *  Author: xstejs30
 */ 


#ifndef DATE_TIME_H_
#define DATE_TIME_H_

/************************************************************************/
/* Struct containing: year, month, day, hour, minute, second            */
/* - year: offset from year 2000 (range: 0--99)                         */
/* - month: Month number (1--12)                                        */
/* - day: Day of the month (1--31)                                      */
/* - hour, min, sec: Hour (0--23), minute (0--59) and second (0--59)    */
/************************************************************************/
typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} date_time_t;

#endif /* DATE_TIME_H_ */