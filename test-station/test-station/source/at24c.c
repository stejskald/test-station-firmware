/*
 * at24c.c
 *
 * Created: 22/04/2023 13:41:20
 *  Author: xstejs30
 *
 * Service library for AT24C I2C EEPROM
 *
 */ 

#include "../driver_init.h"
#include "../include/at24c.h"

// Initialize the IO descriptor
struct io_descriptor *at24c_io;

AT24C_t i2c_eeprom;


/************************************************************************/
/* EEPROM Initialization                                                */
/************************************************************************/
void at24c_init(void){
	set_at24c_params(&i2c_eeprom);
	i2c_m_sync_get_io_descriptor(&I2C_SERCOM2, &at24c_io);
	i2c_m_sync_set_slaveaddr(&I2C_SERCOM2, i2c_eeprom.slave_address, I2C_M_SEVEN);
}

void set_at24c_params(AT24C_t *i2c_eeprom){
// 	i2c_eeprom->i2c = NULL; // Will implementation be needed?
	i2c_eeprom->slave_address = AT24C_EEPROM_I2C_ADDR;
	i2c_eeprom->page_size = AT24C_EEPROM_PAGE_SIZE_BYTES;
	i2c_eeprom->flash_size = AT24C_EEPROM_TOTAL_SIZE_BYTES;
}


/************************************************************************/
/* Write Operations                                                     */
/************************************************************************/
ERROR_t at24c_byte_write(uint16_t byte_address, uint8_t *byte_buffer){
	if (byte_buffer == NULL){
		return ERROR_INVALID_ARG;
	}
	
	// Write 1 byte at the address
	uint8_t whole_cmd[3] = {(byte_address >> 8), (byte_address & (0x00ff)), *byte_buffer};
	int32_t test = io_write(at24c_io, (uint8_t *)whole_cmd, 3);
	if (test != 3){
		return ERROR_I2C_WRITE;
	}
	
	return ERROR_NONE;
}


/************************************************************************/
/* Read Operations                                                      */
/************************************************************************/
ERROR_t at24c_current_addr_read(uint8_t *byte_buffer){
	if (byte_buffer == NULL){
		return ERROR_INVALID_ARG;
	}
	
	// Read 1 byte at the current address
	int32_t test = io_read(at24c_io, byte_buffer, 1);
	if (test != 1){
		return ERROR_I2C_READ;
	}

	return ERROR_NONE;
}






// AT24C_I2C_ERROR at24c_current_addr_read(uint8_t *data_buffer, size_t length){
// 	if (data_buffer == NULL || sizeof(data_buffer) < length){
// 		return AT24C_I2C_ERROR_FUNC_PARAM;
// 	}
// 	// ...
// 	
// 	return AT24C_I2C_ERROR_NONE;
// }