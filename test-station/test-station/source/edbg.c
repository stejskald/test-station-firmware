/*
 * edbg.c
 *
 * Created: 01/05/2023 15:45:09
 *  Author: xstejs30
 */ 

#include "../include/edbg.h"
#include "../driver_init.h"

void UART_EDBG_init(void){
	usart_sync_get_io_descriptor(&EDBG_COM, &edbg_io);
	usart_sync_enable(&EDBG_COM);
}