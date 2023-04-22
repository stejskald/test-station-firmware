/*
 * ads7830.c
 *
 * Created: 18/04/2023 14:56:34
 *  Author: xstejs30
 *
 * Service library for ADS7830IPWR 8-Ch ADC over I2C
 *
 */ 

#include "../driver_init.h"
#include "../include/ads7830.h"

// Initialize the I2C descriptor
// struct i2c_m_sync_desc ADS7830; // done in drive_init.c

// Initialize the IO descriptor
struct io_descriptor *ads7830_io;

// Initialize the I2C N-channel ADC instance
ADS7830_t i2c_adc;

void ads7830_init_ext(void){
	
	// Initialize the I2C I/O Communication
// 	i2c_m_sync_init(&ADS7830, SERCOM2); // done in drive_init.c
	i2c_m_sync_get_io_descriptor(&ADS7830, &ads7830_io);
	i2c_m_sync_enable(&ADS7830);
	set_ads7830_i2c_address(ADS7830_I2C_ADDR); 
	i2c_m_sync_set_slaveaddr(&ADS7830, i2c_adc.i2c_address, I2C_M_SEVEN);
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
	if (channel > 7) { 
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

uint8_t ads7830_measure_single_ended(ads7830_sd_mode_t sd_mode, uint8_t channel, ads7830_pd_mode_t pd_mode){
	if (!_set_ads7830_i2c_cmd_byte(sd_mode, channel, pd_mode)) {
		return 0;
	}
	
	// Send the Command Byte via I2C
	io_write(ads7830_io, &i2c_adc.cmd_byte, 1);
	delay_ms(100);
	
	uint8_t buffer = 0;
	io_read(ads7830_io, &buffer, 1);
	delay_ms(100); 
	
	return buffer;
}