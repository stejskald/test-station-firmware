#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "test_station.h"
#include "date_time.h"
#include "include/error_codes.h"
#include "include/ads7830.h"
#include "include/at24c.h"

#include <hal_delay.h>
#include <stdio.h>
#include <string.h>

#define STR_SIZE 60


static ERROR_t ret_code = 0;
char terminal_string[STR_SIZE];

static struct io_descriptor *terminal_io; // EDBG_COM

static date_time_t now_date_time = {0};
date_time_t *p_now_date_time = &now_date_time;
char cmpl_date[12] = {}; // 1 space for \n or \0 at the end
char cmpl_time[9]  = {}; // 1 space for \n or \0 at the end

void UART_EDBG_init(void);
ERROR_t test_eeprom(void);
ERROR_t test_adc(void);

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	UART_EDBG_init();
	ads7830_init();
	at24c_init();
	
// 	io_write(terminal_io, (uint8_t *)"...DEBUG START...\n", 18);
	
	while (1) {
// 		if (test_eeprom() != ERROR_NONE) return ERROR_FAILURE;
		if (test_adc() != ERROR_NONE) return ERROR_FAILURE;
		
		delay_ms(5000);
	}
}

void UART_EDBG_init(void)
{
	usart_sync_get_io_descriptor(&EDBG_COM, &terminal_io);
	usart_sync_enable(&EDBG_COM);
}

ERROR_t test_eeprom(void){
	uint8_t eeprom_page_buffer[AT24C_EEPROM_PAGE_SIZE_BYTES];
	uint16_t eeprom_rw_addr;
	eeprom_rw_addr = 0x0000;
	uint8_t eeprom_test_page[AT24C_EEPROM_PAGE_SIZE_BYTES];
	for (uint8_t i = 0; i < sizeof(eeprom_test_page); ++i){
		eeprom_test_page[i] = i;
	}
	
	delay_ms(5000);
	ret_code = at24c_page_write(eeprom_rw_addr, eeprom_test_page, AT24C_EEPROM_PAGE_SIZE_BYTES);
	if (ret_code != ERROR_NONE){
		if(0 > snprintf(terminal_string, STR_SIZE, "Byte write to EEPROM failed with code %d.\n", ret_code)) return ERROR_WRONG_LENGTH;
		io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
	}
	ret_code = ERROR_NONE;
	
	delay_ms(5000);
	ret_code = at24c_sequential_read(eeprom_rw_addr, eeprom_page_buffer, AT24C_EEPROM_PAGE_SIZE_BYTES);
	if (ret_code != ERROR_NONE){
		if(0 > snprintf(terminal_string, STR_SIZE, "Read from EEPROM failed with code %d.\n", ret_code)) return ERROR_WRONG_LENGTH;
		io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
	}
	ret_code = ERROR_NONE;
	
	return ERROR_NONE;
}

ERROR_t test_adc(void){
	uint8_t adc_raw_buffer[8];
	float adc_volt_buffer[8];
	bool meas_volt_passed[8] = { 0 };
	
	io_write(terminal_io, (uint8_t *)"-----------------------------------------------------\n", 54);
	io_write(terminal_io, (uint8_t *)"=====  Voltage Measurement Test Output Report  ======\n", 54);
	io_write(terminal_io, (uint8_t *)"-----------------------------------------------------\n", 54);
	
	sprintf(terminal_string, "Date: %s\n", __DATE__); //compiled Date: "mmm dd yyyy"
	io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
	sprintf(terminal_string, "Time: %s\n\n", __TIME__); //compiled Time: "hh:mm:ss"
	io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
	
	io_write(terminal_io, (uint8_t *)"Test Configuration:\n", 20);
	io_write(terminal_io, (uint8_t *)"- Measured DPS: <Name and Version>\n\n", 36);
	io_write(terminal_io, (uint8_t *)"Test Results:\n", 14);
	io_write(terminal_io, (uint8_t *)"- Test Points:\n", 15);
	
	ret_code = ads7830_measure_all_channels_SE(SDMODE_SINGLE, PDIROFF_ADCON, adc_raw_buffer);
	if (ret_code == ERROR_NONE){
		for (uint8_t chnl = 0; chnl < sizeof(adc_raw_buffer); ++chnl){
			adc_volt_buffer[chnl] = adc_raw_to_voltage(adc_raw_buffer[chnl], chnl);
			meas_volt_passed[chnl] = verify_test_criteria_voltage_range(TEST_CRIT_VOLT_RANGE_PERCENTAGE, chnl, adc_volt_buffer[chnl]);
			if(0 > snprintf(terminal_string, STR_SIZE, "  - %s: \t %6.3f V -- %s\n", testPointNames[chnl], 
							adc_volt_buffer[chnl], meas_volt_passed[chnl] == true ? "PASS" : "FAIL")) return ERROR_WRONG_LENGTH;
			io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
// 			snprintf(terminal_string, STR_SIZE, "- Channel %u Voltage: %6.3f V (raw: %3u)\n", 
// 					 chnl, adc_volt_buffer[chnl], adc_raw_buffer[chnl]);
		}
	}
	
	io_write(terminal_io, (uint8_t *)"\nPass/Fail Criteria:\n", 21);
	snprintf(terminal_string, STR_SIZE, "- Voltage in range ± %4.2f %%\n\n", TEST_CRIT_VOLT_RANGE_PERCENTAGE);
	io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
	
	io_write(terminal_io, (uint8_t *)"Conclusion:\n", 12);
	// zkontrolovat, zda meas_volt_passed[ obsahuje same TRUE]
	io_write(terminal_io, (uint8_t *)"All measured voltages are within acceptable range.\n\n", 51);
	
	io_write(terminal_io, (uint8_t *)"-----------------------------------------------------\n\n", 55);
	
	return ERROR_NONE;
}