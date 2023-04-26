/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

void I2C_SERCOM0_example(void)
{
	struct io_descriptor *I2C_SERCOM0_io;

	i2c_m_sync_get_io_descriptor(&I2C_SERCOM0, &I2C_SERCOM0_io);
	i2c_m_sync_enable(&I2C_SERCOM0);
	i2c_m_sync_set_slaveaddr(&I2C_SERCOM0, 0x12, I2C_M_SEVEN);
	io_write(I2C_SERCOM0_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using EDBG_COM to write "Hello World" using the IO abstraction.
 */
void EDBG_COM_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&EDBG_COM, &io);
	usart_sync_enable(&EDBG_COM);

	io_write(io, (uint8_t *)"Hello World!", 12);
}
