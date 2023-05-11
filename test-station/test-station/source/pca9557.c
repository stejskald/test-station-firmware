/*
 * pca9557.c
 *
 * Created: 02/05/2023 06:28:42
 *  Author: xstejs30
 */ 

#include "../include/pca9557.h"
#include "../driver_init.h"



// Initialize the IO descriptor
struct io_descriptor *pca9557_io;

void pca9557_init(void){
	// Initialize the I2C I/O Communication
	i2c_m_sync_enable(&I2C_SERCOM0);
	i2c_m_sync_get_io_descriptor(&I2C_SERCOM0, &pca9557_io);
	i2c_m_sync_set_slaveaddr(&I2C_SERCOM0, PCA9557_I2C_ADDR, I2C_M_SEVEN);
}

uint8_t pca9557_get_reg(pca9557_reg_addr_t reg_addr){
	uint8_t reg_data = 0;
	
	if (io_write(pca9557_io, &reg_addr, 1) != 1) return ERROR_I2C_WRITE;
	
	if (io_read(pca9557_io, &reg_data, 1) != 1) return ERROR_I2C_READ;

	return reg_data;
}

uint8_t pca9557_get_pin(pca9557_pin_t pin, pca9557_reg_addr_t reg_addr){
	if (pin < 0 || pin >= PCA9557_N_CHNL) return ERROR_INVALID_ARG;
	
	return (pca9557_get_reg(reg_addr) & (pin+1)) >> pin;
}

ERROR_t pca9557_set_reg(pca9557_reg_addr_t reg_addr, uint8_t reg_setting){
	if (reg_addr == PCA9557_REG_INPUT) return ERROR_INVALID_ARG; // Read only
	
	uint8_t command[2] = {reg_addr, reg_setting};
	
	int test = io_write(pca9557_io, (uint8_t *)command, 2);
	if (test != 2) return ERROR_I2C_WRITE;
	
	return ERROR_NONE;
}

ERROR_t pca9557_set_pin(pca9557_pin_t pin, pca9557_reg_addr_t reg_addr, pca9557_state_t new_pin_val){
	if (pin < 0 || pin >= PCA9557_N_CHNL) return ERROR_INVALID_ARG;
	
	uint8_t act_reg_data, new_reg_data;
	
	act_reg_data = pca9557_get_reg(reg_addr);
	// bit at position pin in act_reg_data is firstly cleared and then ORed with new_pin_val
	new_reg_data = (act_reg_data & ~(1 << pin)) | (new_pin_val << pin);
	if (pca9557_set_reg(reg_addr, new_reg_data) != ERROR_NONE) return ERROR_I2C_WRITE;
	
	return ERROR_NONE;
}