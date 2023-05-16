/*
 * ads7830.c
 *
 * Created: 18/04/2023 14:56:34
 *  Author: xstejs30
 *
 * Service library for ADS7830IPWR 8-Ch ADC via I2C
 *
 */ 

#include "../driver_init.h"
#include "../include/ads7830.h"
#include "sam.h"

// Initialize the IO descriptor
struct io_descriptor *ads7830_io;

// Initialize the I2C N-channel ADC instance
ADS7830_t i2c_adc;

void ads7830_init(void){
	// Initialize the I2C I/O Communication
	i2c_m_sync_enable(&I2C_SERCOM0);
	i2c_m_sync_get_io_descriptor(&I2C_SERCOM0, &ads7830_io);
	set_ads7830_i2c_address(ADS7830_I2C_ADDR); 
	ads7830_activate();
}

void set_ads7830_i2c_address(uint8_t i2c_address){
	i2c_adc.i2c_address = i2c_address;
}

uint8_t get_ads7830_i2c_address(void){
	return i2c_adc.i2c_address;
}


void _set_ads7830_i2c_cmd_sd_mode(ads7830_sd_mode_t sd_mode){
	i2c_adc.sd_mode = sd_mode;
	i2c_adc.cmd_byte |= sd_mode;
}

void _set_ads7830_i2c_cmd_pd_mode(ads7830_pd_mode_t pd_mode){
	i2c_adc.pd_mode = pd_mode;
	i2c_adc.cmd_byte |= pd_mode;
}

bool _set_ads7830_i2c_cmd_byte(ads7830_sd_mode_t sd_mode, uint8_t channel, ads7830_pd_mode_t pd_mode){
	if (!(channel >= 0 && channel < ADS7830_N_CHNL)) { 
		return false;
	}
	
	i2c_adc.cmd_byte = 0x00; // reset the Command Byte value
	_set_ads7830_i2c_cmd_sd_mode(sd_mode);
	
	// Set single-ended input channel
	switch (channel) {
		case (0):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE0;
			break;
		case (1):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE1;
			break;
		case (2):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE2;
			break;
		case (3):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE3;
			break;
		case (4):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE4;
			break;
		case (5):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE5;
			break;
		case (6):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE6;
			break;
		case (7):
			i2c_adc.cmd_byte |= ADS7830_CMD_CHNL_SE7;
			break;
	}
	
	_set_ads7830_i2c_cmd_pd_mode(pd_mode);
	
	return true;
}

void ads7830_activate(void){
	i2c_m_sync_set_slaveaddr(&I2C_SERCOM0, i2c_adc.i2c_address, I2C_M_SEVEN);
}

ERROR_t ads7830_measure_single_ended(uint8_t channel, ads7830_pd_mode_t pd_mode, uint8_t *adc_data){
	if (adc_data == NULL){
		return ERROR_INVALID_ARG;
	}
	if (!_set_ads7830_i2c_cmd_byte(ADS7830_CMD_SD_SINGLE, channel, pd_mode)) {
		return ERROR_INVALID_DATA;
	}
		
	SERCOM0->I2CM.ADDR.reg = i2c_adc.i2c_address << 1 | 0;   // Send ADC address with write bit
	if (SERCOM0->I2CM.STATUS.bit.BUSERR || SERCOM0->I2CM.STATUS.bit.ARBLOST) {
		return ERROR_I2C_WRITE;                              // Handle error
	}
	while(!SERCOM0->I2CM.INTFLAG.bit.MB);                    // Wait for transmission to complete
	
	SERCOM0->I2CM.DATA.reg = i2c_adc.cmd_byte;               // Send configuration byte
	if (SERCOM0->I2CM.STATUS.bit.BUSERR || SERCOM0->I2CM.STATUS.bit.ARBLOST) {
		return ERROR_I2C_WRITE;                              // Handle error
	}
	while(!SERCOM0->I2CM.INTFLAG.bit.MB);                    // Wait for transmission to complete
	if (SERCOM0->I2CM.STATUS.bit.RXNACK) {
		return ERROR_I2C_NACK;								 // Handle error
	}

	SERCOM0->I2CM.ADDR.reg = i2c_adc.i2c_address << 1 | 1;   // Send ADC address with read bit
	if (SERCOM0->I2CM.STATUS.bit.BUSERR || SERCOM0->I2CM.STATUS.bit.ARBLOST) {
		return ERROR_I2C_READ;                               // Handle error
	}
	while(!SERCOM0->I2CM.INTFLAG.bit.SB);                    // Wait until data is received (stop bit to be sent)
	if (SERCOM0->I2CM.STATUS.bit.RXNACK) {
		return ERROR_I2C_NACK;								 // Handle error
	}
	
	*adc_data = SERCOM0->I2CM.DATA.reg;                      // Read data from slave
	
	// Send NACK after last byte and STOP command
	SERCOM0->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_CMD(0x3); 

	return ERROR_NONE;
}

ERROR_t ads7830_measure_all_channels_SE(ads7830_pd_mode_t pd_mode, uint8_t *adc_data){
	if (adc_data == NULL){
		return ERROR_INVALID_ARG;
	}
	
// 	uint32_t start_time = millis(); // Get current time in milliseconds <- timer needs to be initialized first...
// 	uint32_t timeout = 100;         // Set timeout in milliseconds
	
	for (uint8_t chnl = 0; chnl < ADS7830_N_CHNL; ++chnl)
	{
		if (!_set_ads7830_i2c_cmd_byte(ADS7830_CMD_SD_SINGLE, chnl, pd_mode)) {
			return ERROR_INVALID_DATA;
		}
		
		SERCOM0->I2CM.ADDR.reg = (i2c_adc.i2c_address << 1) | 0; // Send ADC address with write bit
		if (SERCOM0->I2CM.STATUS.bit.BUSERR || SERCOM0->I2CM.STATUS.bit.ARBLOST) {
			return ERROR_I2C_WRITE;                              // Handle error
		}
		while(!SERCOM0->I2CM.INTFLAG.bit.MB) {                   // Wait for transmission to complete
// 			if(millis() - start_time > timeout){                 // Check if timeout has occurred - ...copy to all while loops
// 				return ERROR_TIMEOUT;                            // Return error code indicating timeout
// 			}
		}                    
		
		SERCOM0->I2CM.DATA.reg = i2c_adc.cmd_byte;               // Send configuration byte
		if (SERCOM0->I2CM.STATUS.bit.BUSERR || SERCOM0->I2CM.STATUS.bit.ARBLOST) {
			return ERROR_I2C_WRITE;                              // Handle error
		}
		while(!SERCOM0->I2CM.INTFLAG.bit.MB);                    // Wait for transmission to complete
		if (SERCOM0->I2CM.STATUS.bit.RXNACK) {
			return ERROR_I2C_NACK;								 // Handle error
		}
		
		SERCOM0->I2CM.ADDR.reg = (i2c_adc.i2c_address << 1) | 1; // Send ADC address with read bit
		if (SERCOM0->I2CM.STATUS.bit.BUSERR || SERCOM0->I2CM.STATUS.bit.ARBLOST) {
			return ERROR_I2C_READ;                               // Handle error
		}
		while(!SERCOM0->I2CM.INTFLAG.bit.SB);                    // Wait until data is received (stop bit to be sent)
		if (SERCOM0->I2CM.STATUS.bit.RXNACK) {
			return ERROR_I2C_NACK;								 // Handle error
		}

		adc_data[chnl] = SERCOM0->I2CM.DATA.reg;                 // Read data from slave

		SERCOM0->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_ACKACT;      // Send a NACK after data is received
		SERCOM0->I2CM.CTRLB.bit.CMD = 0x2;                       // Send repeated start condition
	}
	
	// Send NACK after last byte and STOP command
	SERCOM0->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_CMD(0x3);

	return ERROR_NONE;
}