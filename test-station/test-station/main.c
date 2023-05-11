#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "test_station.h"
#include "include/edbg.h"
#include "include/error_codes.h"
#include "include/calendar.h"
#include "include/timer.h"
#include "include/ads7830.h"
#include "include/at24c.h"
#include "include/pca9557.h"

#include <hal_delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ERROR_t ret_code = 0;

char cmpl_date[12] = {}; // 1 space for \n or \0 at the end
char cmpl_time[9]  = {}; // 1 space for \n or \0 at the end
	
static date_time_t cmpl_date_time = {0};
date_time_t *p_cmpl_date_time = &cmpl_date_time;

ERROR_t test_eeprom(void);
ERROR_t test_adc(void);
ERROR_t test_gpio_expander(void);

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	sprintf(cmpl_date, "%s", __DATE__); //compiled Date: "mmm dd yyyy"
	sprintf(cmpl_time, "%s", __TIME__); //compiled Time: "hh:mm:ss"
	conv_str_to_date_time(p_cmpl_date_time, cmpl_date, cmpl_time);
	
	UART_EDBG_init();
	calendar_initialization(p_cmpl_date_time);
	timer_initialization();
	at24c_init();
	ads7830_init();
	pca9557_init();
	
	while (1) {
		delay_ms(1000);
		
// 		if (test_eeprom() != ERROR_NONE) return ERROR_FAILURE;
// 		if (test_adc() != ERROR_NONE) return ERROR_FAILURE;
// 		test_station_send_report();
		
		if (test_gpio_expander() != ERROR_NONE) return ERROR_FAILURE;
	}
}

ERROR_t test_gpio_expander(void){
// 	uint8_t pca9557_reg_buffer;
// 	uint8_t pca9557_reg_settings = PCA9557_REG_CONFIG;
	uint8_t ret_val = 0;
	
// 	ret_code = pca9557_get_reg(PCA9557_REG_CONFIG, &pca9557_reg_buffer);
// 	if (ret_code == ERROR_NONE){
// 		sprintf(edbg_msg, "Config register has value 0x%x.\n", pca9557_reg_buffer);
// 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
// 	}
// 	ret_code = ERROR_NONE;
// 	
// 	ret_code = pca9557_set_reg(PCA9557_REG_CONFIG, pca9557_reg_settings);
// 	if (ret_code == ERROR_NONE){
// 		sprintf(edbg_msg, "Setting of config reg was successful.\n");
// 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
// 	}
// 	ret_code = ERROR_NONE;
// 	
// 	ret_code = pca9557_get_reg(PCA9557_REG_OUTPUT, &pca9557_reg_buffer);
// 	if (ret_code == ERROR_NONE){
// 		sprintf(edbg_msg, "Output register has value 0x%x.\n", pca9557_reg_buffer);
// 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
// 	}
// 	ret_code = ERROR_NONE;
// 	
// 	ret_code = pca9557_get_reg(PCA9557_REG_INPUT, &pca9557_reg_buffer);
// 	if (ret_code == ERROR_NONE){
// 		sprintf(edbg_msg, "Input register has value 0x%x.\n", pca9557_reg_buffer);
// 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
// 	}
// 	ret_code = ERROR_NONE;

// 	ret_val = pca9557_get_pin(IO1, PCA9557_REG_CONFIG);
// 	if (ret_val >= ERROR_NONE){
// 		sprintf(edbg_msg, "Selected pin has value 0x%x.\n", ret_val);
// 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
// 	}
// 	ret_code = ERROR_NONE;
// 	
// 	ret_val = pca9557_set_pin(IO1, PCA9557_REG_CONFIG, IO_LOW);
// 	if (ret_val >= ERROR_NONE){
// 		sprintf(edbg_msg, "Selected pin was set to 0x%x.\n", ret_val);
// 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
// 	}
// 	ret_code = ERROR_NONE;
// 	
// 	ret_val = pca9557_get_pin(IO1, PCA9557_REG_CONFIG);
// 	if (ret_val >= ERROR_NONE){
// 		sprintf(edbg_msg, "Selected pin has value 0x%x.\n", ret_val);
// 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
// 	}
// 	ret_code = ERROR_NONE;
	
	return ERROR_NONE;
}

ERROR_t test_adc(void){
	uint8_t adc_raw_buffer[ADS7830_N_CHNL];
	
	ret_code = ads7830_measure_all_channels_SE(SDMODE_SINGLE, PDIROFF_ADCON, adc_raw_buffer);
	if (ret_code == ERROR_NONE){
		for (uint8_t chnl = 0; chnl < ADS7830_N_CHNL; ++chnl){
			adc_volt_buffer[chnl] = adc_raw_to_voltage(adc_raw_buffer[chnl], chnl);
			meas_volt_passed[chnl] = verify_test_criteria_voltage_range(TEST_CRIT_VOLT_RANGE_PERCENTAGE,
			chnl, adc_volt_buffer[chnl]);
		}
	}
	return ERROR_NONE;
}

ERROR_t test_eeprom(void){
	uint8_t eeprom_page_buffer[AT24C_EEPROM_PAGE_SIZE_BYTES];
	uint16_t eeprom_rw_addr;
	eeprom_rw_addr = 0x0000;
	uint8_t eeprom_test_page[AT24C_EEPROM_PAGE_SIZE_BYTES];
	for (uint8_t i = 0; i < sizeof(eeprom_test_page); ++i){
		eeprom_test_page[i] = i;
	}
	
	delay_ms(2000);
	ret_code = at24c_page_write(eeprom_rw_addr, eeprom_test_page, AT24C_EEPROM_PAGE_SIZE_BYTES);
	if (ret_code != ERROR_NONE){
		if(0 > snprintf(edbg_msg, EDBG_MSG_LEN, "Byte write to EEPROM failed with code %d.\n", ret_code)) return ERROR_WRONG_LENGTH;
		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	}
	ret_code = ERROR_NONE;
	
	delay_ms(2000);
	ret_code = at24c_sequential_read(eeprom_rw_addr, eeprom_page_buffer, AT24C_EEPROM_PAGE_SIZE_BYTES);
	if (ret_code != ERROR_NONE){
		if(0 > snprintf(edbg_msg, EDBG_MSG_LEN, "Read from EEPROM failed with code %d.\n", ret_code)) return ERROR_WRONG_LENGTH;
		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	}
	ret_code = ERROR_NONE;
	
	return ERROR_NONE;
}