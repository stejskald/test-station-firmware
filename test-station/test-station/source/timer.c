/*
 * timer.c
 *
 * Created: 01/05/2023 15:59:18
 *  Author: xstejs30
 */ 


#include "../include/timer.h"
#include "../driver_init.h"
#include "../include/edbg.h"

void timer_initialization(void){
	static struct timer_task TIMER_task1;
	
	TIMER_task1.interval = 2; // 2 sec
	TIMER_task1.cb = TIMER_task1_cb;
	TIMER_task1.mode = TIMER_TASK_REPEAT;
	
	timer_add_task(&TIMER, &TIMER_task1);
	timer_start(&TIMER);
}

void TIMER_task1_cb(const struct timer_task *const timer_task){
// 	io_write(edbg_io, (uint8_t *)"Timer_task1_cb function executed!\n", 34);
}