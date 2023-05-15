/*
 * edbg.h
 *
 * Created: 01/05/2023 15:43:09
 *  Author: xstejs30
 */ 


#ifndef EDBG_H_
#define EDBG_H_

#define EDBG_MSG_LEN 128

char edbg_msg[EDBG_MSG_LEN];

struct io_descriptor *edbg_io; // EDBG_COM

void UART_EDBG_init(void);



#endif /* EDBG_H_ */