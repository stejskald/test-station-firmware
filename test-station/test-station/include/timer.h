/*
 * timer.h
 *
 * Created: 01/05/2023 15:58:46
 *  Author: xstejs30
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "../driver_init.h"

void timer_initialization(void);

// Callback function for Timer task
void TIMER_task1_cb(const struct timer_task *const timer_task);



#endif /* TIMER_H_ */