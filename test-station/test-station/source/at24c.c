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

#include <string.h>

// Initialize the IO descriptor
struct io_descriptor *at24c_io;

AT24C_t i2c_eeprom;


/************************************************************************/
/* EEPROM Initialization                                                */
/************************************************************************/
void at24c_init(void){
	_set_at24c_params(&i2c_eeprom);
	i2c_m_sync_enable(&I2C_SERCOM0);
	i2c_m_sync_get_io_descriptor(&I2C_SERCOM0, &at24c_io);
	at24c_acivate();
}

void _set_at24c_params(AT24C_t *i2c_eeprom){
// 	i2c_eeprom->i2c = NULL; // Will implementation be needed?
	i2c_eeprom->slave_address = AT24C_EEPROM_I2C_ADDR;
	i2c_eeprom->page_size = AT24C_EEPROM_PAGE_SIZE_BYTES;
	i2c_eeprom->flash_size = AT24C_EEPROM_TOTAL_SIZE_BYTES;
}

void at24c_acivate(void){
	i2c_m_sync_set_slaveaddr(&I2C_SERCOM0, i2c_eeprom.slave_address, I2C_M_SEVEN);
}


/************************************************************************/
/* Write Operations                                                     */
/************************************************************************/
ERROR_t at24c_byte_write(uint16_t address, uint8_t *byte_buffer){
	if (byte_buffer == NULL){
		return ERROR_INVALID_ARG;
	}
	
	// Write 2 address bytes and 1 data byte at the specific address
	uint8_t command[3] = {(address >> 8), (address & (0x00ff)), *byte_buffer};
	if (io_write(at24c_io, (uint8_t *)command, 3) != 3){
		return ERROR_I2C_WRITE;
	}
	
	return ERROR_NONE;
}

ERROR_t at24c_page_write(uint16_t address, uint8_t *page_buffer, uint8_t length){
	if ((page_buffer == NULL) || !(length <= 64)){
		return ERROR_INVALID_ARG;
	}
	else{
		uint8_t command[2+length];
		memset(command, 0, (2+length)*sizeof(uint8_t));
		command[0] = address >> 8;
		command[1] = address & (0xff);
		for (uint8_t i = 2; i <= 2+length; ++i) {
			command[i] = page_buffer[i-2];
		}
		
		if (io_write(at24c_io, command, 2+length) != 2+length){
			return ERROR_I2C_WRITE;
		}
		
		return ERROR_NONE;
	}
}


/************************************************************************/
/* Read Operations                                                      */
/************************************************************************/
ERROR_t at24c_current_addr_read(uint8_t *byte_buffer){
	if (byte_buffer == NULL){
		return ERROR_INVALID_ARG;
	}
	
	// Read 1 data byte at the current address
	if (io_read(at24c_io, byte_buffer, 1) != 1){
		return ERROR_I2C_READ;
	}

	return ERROR_NONE;
}

ERROR_t at24c_random_read(uint16_t address, uint8_t *byte_buffer){
	if (byte_buffer == NULL){
		return ERROR_INVALID_ARG;
	}
	// Dummy Write: 2 address bytes 
	uint8_t command[2] = {(address >> 8), (address & (0xff))};

	if (io_write(at24c_io, (uint8_t *)command, 2) != 2){
		return ERROR_I2C_WRITE;
	}
	
	// Read 1 byte at the specific address
	if (io_read(at24c_io, byte_buffer, 1) != 1){
		return ERROR_I2C_READ;
	}

	return ERROR_NONE;
}

ERROR_t at24c_sequential_read(uint16_t address, uint8_t *data_buffer, uint16_t length){
	if ((data_buffer == NULL) || (length < 1)){
		return ERROR_INVALID_ARG;
	}
	
	// Dummy Write: 2 address bytes
	uint8_t command[2] = {(address >> 8), (address & (0xff))};

	if (io_write(at24c_io, (uint8_t *)command, 2) != 2){
		return ERROR_I2C_WRITE;
	}
	
	// Read "length" number of bytes from the specific address
	if (io_read(at24c_io, data_buffer, length) != length){
		return ERROR_I2C_READ;
	}
	
	return ERROR_NONE;
}