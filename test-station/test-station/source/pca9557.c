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

/************************************************************************/
/* Private functions                                                    */
/************************************************************************/
uint8_t _pca9557_get_reg(pca9557_reg_addr_t reg_addr){
	uint8_t reg_data = 0;
	
	if (io_write(pca9557_io, &reg_addr, 1) != 1) return ERROR_I2C_WRITE;
	
	if (io_read(pca9557_io, &reg_data, 1) != 1) return ERROR_I2C_READ;

	return reg_data;
}

uint8_t _pca9557_get_pin(pca9557_pin_t pin, pca9557_reg_addr_t reg_addr){
	return (_pca9557_get_reg(reg_addr) & (pin+1)) >> pin;
}

ERROR_t _pca9557_set_reg(pca9557_reg_addr_t reg_addr, uint8_t reg_setting){
	if (reg_addr == PCA9557_REG_INPUT) return ERROR_INVALID_ARG; // Read only
	
	uint8_t command[2] = {reg_addr, reg_setting};
	
	int test = io_write(pca9557_io, (uint8_t *)command, 2);
	if (test != 2) return ERROR_I2C_WRITE;
	
	return ERROR_NONE;
}

ERROR_t _pca9557_set_pin(pca9557_pin_t pin, pca9557_reg_addr_t reg_addr, pca9557_state_t new_pin_val){
	uint8_t act_reg_data, new_reg_data;
	
	act_reg_data = _pca9557_get_reg(reg_addr);
	// bit at position pin in act_reg_data is firstly cleared and then ORed with new_pin_val
	new_reg_data = (act_reg_data & ~(1 << pin)) | (new_pin_val << pin);
	if (_pca9557_set_reg(reg_addr, new_reg_data) != ERROR_NONE) return ERROR_I2C_WRITE;
	
	return ERROR_NONE;
}


/************************************************************************/
/* Public functions                                                     */
/************************************************************************/
void pca9557_init(void){
	// Initialize the I2C I/O Communication
	i2c_m_sync_enable(&I2C_SERCOM0);
	i2c_m_sync_get_io_descriptor(&I2C_SERCOM0, &pca9557_io);
	i2c_m_sync_set_slaveaddr(&I2C_SERCOM0, PCA9557_I2C_ADDR, I2C_M_SEVEN);
}

void pca9557_reset(void){
	pca9557_set_mode_all(IO_INPUT);
	pca9557_set_state_all(IO_HIGH);
	pca9557_set_polarity_all(IO_NON_INVERTED);
}

uint8_t pca9557_get_pin_mode(pca9557_pin_t pin){
	return _pca9557_get_pin(pin, PCA9557_REG_CONFIG);
}

uint8_t pca9557_get_pin_state(pca9557_pin_t pin){
	return _pca9557_get_pin(pin, pca9557_get_pin_mode(pin) ? PCA9557_REG_INPUT : PCA9557_REG_OUTPUT);
}

uint8_t pca9557_get_pin_polarity(pca9557_pin_t pin){
	return _pca9557_get_pin(pin, PCA9557_REG_POLARITY);
}

ERROR_t pca9557_set_pin_mode(pca9557_pin_t pin, pca9557_mode_t new_mode){
	return _pca9557_set_pin(pin, PCA9557_REG_CONFIG, new_mode);
}

ERROR_t pca9557_set_mode_all(pca9557_mode_t set_mode){
	return _pca9557_set_reg(PCA9557_REG_CONFIG, set_mode ? PCA9557_ALL_INPUT : PCA9557_ALL_OUTPUT);
}

ERROR_t pca9557_set_pin_state(pca9557_pin_t pin, pca9557_mode_t new_state){
	return _pca9557_set_pin(pin, PCA9557_REG_OUTPUT, new_state);
}

ERROR_t pca9557_set_state_all(pca9557_mode_t new_state){
	return _pca9557_set_reg(PCA9557_REG_OUTPUT, new_state ? PCA9557_ALL_HIGH : PCA9557_ALL_LOW);
}

ERROR_t pca9557_set_pin_polarity(pca9557_pin_t pin, pca9557_polarity_t new_polarity){
	return _pca9557_set_pin(pin, PCA9557_REG_POLARITY, new_polarity);
}

ERROR_t pca9557_set_polarity_all(pca9557_polarity_t new_polarity){
	uint8_t act_polarity, polarity_mask, new_polarity_all;
	
	act_polarity = _pca9557_get_reg(PCA9557_REG_POLARITY);
	polarity_mask = _pca9557_get_reg(PCA9557_REG_CONFIG);
	new_polarity_all  = new_polarity ? PCA9557_ALL_INVERTED : PCA9557_ALL_NON_INVERTED;
	if (_pca9557_set_reg(PCA9557_REG_POLARITY, (act_polarity & ~polarity_mask) | (new_polarity_all & polarity_mask)) != ERROR_NONE) {
		return ERROR_I2C_WRITE;
	}
	
	return ERROR_NONE;
}