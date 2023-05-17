/*
 * test_station.c
 *
 * Created: 28/04/2023 13:39:08
 *  Author: xstejs30
 */ 

#include "test_station.h"
#include "include/edbg.h"
#include "include/ads7830.h"
#include "include/at24c.h"
#include "include/pca9557.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// In future get from desktop app
const char* test_point_names[ADS7830_N_CHNL] = {
	"VCC", "Air", "+5V", "+4V", "+3V3", "VDD", "DO0", "DO1"
};
const char* cfg_interfaces[6] = {
	"USART", "I2C", "SPI", "CAN", "RS232", "RS485"
};

test_station_t test_station;
test_result_t test_result;

pcb_config_t pcb_cfg_app; // for testing purposes, will be obtained from desktop app
pcb_config_t pcb_cfg_eeprom;

test_result_t test_station_main_test_script(void){
	if (!_read_pcb_config()) {
		test_result.return_code = ERROR_NOT_INITIALIZED;
	}
	else {
		if (!_test_serial_com()) {
			_measure_selected_adc_channels();
			_generate_final_report();
		}
		else {
			_activate_digital_outputs();
			_measure_all_adc_channels();
			_activate_digital_inputs();
			_check_digital_input_states();
			_generate_final_report();
		}
	}
	return test_result;
}

bool _read_pcb_config(void){
	at24c_acivate();
	test_result.return_code = at24c_sequential_read(PCB_CFG_DPS_NAME_ADDR,    (uint8_t *) pcb_cfg_eeprom.dps_name,        PCB_CFG_DPS_NAME_SIZE);
	test_result.return_code = at24c_sequential_read(PCB_CFG_AUTHOR_ADDR,      (uint8_t *) pcb_cfg_eeprom.author,          PCB_CFG_AUTHOR_SIZE);
	test_result.return_code = at24c_sequential_read(PCB_CFG_DPS_VERSION_ADDR, (uint8_t *) pcb_cfg_eeprom.dps_version,     PCB_CFG_DPS_VERSION_SIZE);
	test_result.return_code = at24c_sequential_read(PCB_CFG_DATE_ADDR,        (uint8_t *) pcb_cfg_eeprom.cfg_date,        PCB_CFG_DATE_SIZE);
	test_result.return_code = at24c_sequential_read(PCB_CFG_DPS_DESC_ADDR,    (uint8_t *) pcb_cfg_eeprom.dps_description, PCB_CFG_DPS_DESC_SIZE);
	test_result.return_code = at24c_sequential_read(PCB_CFG_ADDR,             (uint8_t *) pcb_cfg_eeprom.dps_cfg,         PCB_CFG_SIZE);
  
	// Implement check mechanism for each config value?
	
	if (test_result.return_code == ERROR_NONE){ return true; }
	else{ return false; }
}

bool _test_serial_com(void){
	if (false) return false; // remove after serial communication implemented
	
	return true;
}

void _measure_selected_adc_channels(void){
	uint8_t adc_buffer_raw[ADS7830_N_CHNL];
	
	ads7830_activate();
	uint8_t adc_power_supply_channels = ADC_PWR_SUPPLY_CHNLS;
	for (uint8_t chnl = 0; chnl < ADS7830_N_CHNL; ++chnl){
		if (adc_power_supply_channels & 1) {
			test_result.return_code = ads7830_measure_single_ended(chnl, PDIROFF_ADCON, adc_buffer_raw);
			if (test_result.return_code == ERROR_NONE){
				test_station.adc_buffer_volt[chnl] = _adc_raw_to_voltage(adc_buffer_raw[chnl], chnl);
				test_result.adc_channel_passed[chnl] = _verify_test_criteria_voltage(TEST_CRIT_VOLT_RANGE_PERC,
				chnl, test_station.adc_buffer_volt[chnl]);
			}
			else { io_write(edbg_io, (uint8_t *)"Voltage measurement error!\n", 27); }
		}
		else {
			test_station.adc_buffer_volt[chnl] = 0.000f;
			test_result.adc_channel_passed[chnl] = false;
		}
		adc_power_supply_channels = (adc_power_supply_channels >> 1);
	}
}

void _activate_digital_outputs(void){
	// Send RS-485 command for activating digital outputs of MCU on tested PCB
	
	// TODO: implement serial communication
}

void _measure_all_adc_channels(void){
	uint8_t adc_buffer_raw[ADS7830_N_CHNL];
	
	ads7830_activate();
	test_result.return_code = ads7830_measure_all_channels_SE(PDIROFF_ADCON, adc_buffer_raw);
	if (test_result.return_code == ERROR_NONE){
		for (uint8_t chnl = 0; chnl < ADS7830_N_CHNL; ++chnl){
			test_station.adc_buffer_volt[chnl] = _adc_raw_to_voltage(adc_buffer_raw[chnl], chnl);
			test_result.adc_channel_passed[chnl] = _verify_test_criteria_voltage(TEST_CRIT_VOLT_RANGE_PERC,
			chnl, test_station.adc_buffer_volt[chnl]);
		}
	}
	else { io_write(edbg_io, (uint8_t *)"Voltage measurement error!\n", 27); }
}

float _adc_raw_to_voltage(uint8_t adc_raw_data, uint8_t channel){
	if (channel < 0 || channel > 7) {
		return 0;
	}
	
	float divider_ratio = 1.0000f;
	// Set voltage divider ratio
	switch (channel) {
		case (0):
		divider_ratio = ADC_CH0_DIVIDER_RATIO;
		break;
		case (1):
		divider_ratio = ADC_CH1_DIVIDER_RATIO;
		break;
		case (2):
		divider_ratio = ADC_CH2_DIVIDER_RATIO;
		break;
		case (3):
		divider_ratio = ADC_CH3_DIVIDER_RATIO;
		break;
		case (4):
		divider_ratio = ADC_CH4_DIVIDER_RATIO;
		break;
		case (5):
		divider_ratio = ADC_CH5_DIVIDER_RATIO;
		break;
		case (6):
		divider_ratio = ADC_CH6_DIVIDER_RATIO;
		break;
		case (7):
		divider_ratio = ADC_CH7_DIVIDER_RATIO;
		break;
	}
	
	// Unipolar conversion, U=<0, Uref>, raw=<0, (2^N)-1>
	if (adc_raw_data == 0){ return 0.000f; }
	float koef = ADC_VOLTAGE_REFERENCE / (pow(2U,ADS7830_RESOLUTION_BITS) - 1);
	
	return koef * adc_raw_data * (1/divider_ratio);
}

bool _verify_test_criteria_voltage(float percentage_voltage_range, uint8_t channel, float measured_voltage){
	if (!(channel >= 0 && channel < ADS7830_N_CHNL)) {
		return false;
	}

	uint16_t meas_volt = (uint16_t)(measured_voltage * 1000U);
	uint16_t req_voltage;
	
	switch (channel) {
		case (0):
		req_voltage = ADC_CH0_REQUIRED_VOLTAGE * 1000U;
		break;
		case (1):
		req_voltage = ADC_CH1_REQUIRED_VOLTAGE * 1000U;
		break;
		case (2):
		req_voltage = ADC_CH2_REQUIRED_VOLTAGE * 1000U;
		break;
		case (3):
		req_voltage = ADC_CH3_REQUIRED_VOLTAGE * 1000U;
		break;
		case (4):
		req_voltage = ADC_CH4_REQUIRED_VOLTAGE * 1000U;
		break;
		case (5):
		req_voltage = ADC_CH5_REQUIRED_VOLTAGE * 1000U;
		break;
		case (6):
		req_voltage = ADC_CH6_REQUIRED_VOLTAGE * 1000U;
		break;
		case (7):
		req_voltage = ADC_CH7_REQUIRED_VOLTAGE * 1000U;
		break;
	}

	uint16_t perc_volt = (uint16_t)(percentage_voltage_range * 10U);
	uint16_t meas_req_ratio = (uint16_t)((meas_volt / (float)req_voltage) * 1000U);
	
	return (meas_req_ratio >= (1000U - perc_volt)) && (meas_req_ratio <= (1000U + perc_volt));
}

void _activate_digital_inputs(void){
	/* Activate digital inputs via GPIO expander */
	// Set GPIO Expander Slave Address
	pca9557_activate();
	
	// Set IO0 and IO1 as inputs, others as outputs
	pca9557_set_mode_all(IO_OUTPUT);
	pca9557_set_pin_mode(IO0, IO_INPUT);
	pca9557_set_pin_mode(IO1, IO_INPUT);
	
	// Set all pins to High (no effect on pins defined as inputs)
	pca9557_set_state_all(IO_HIGH);
}

void _check_digital_input_states(void){
	// Send RS-485 command for checking the digital inputs of MCU on tested PCB
	
	// TODO: implement serial communication
}

void _generate_final_report(void){
	io_write(edbg_io, (uint8_t *)"---------------------------------------------------------------------\n", 70);
	io_write(edbg_io, (uint8_t *)"===================  Final Automatic Test Report  ===================\n", 70);
	io_write(edbg_io, (uint8_t *)"---------------------------------------------------------------------\n", 70);
	
	struct calendar_date_time dateTime;
	// get actual date and time -- test board does not have a battery, so actual time must be got from PC
	calendar_get_date_time(&CALENDAR, &dateTime);

	sprintf(edbg_msg, "Date: %d-%02d-%02d\n", dateTime.date.year, dateTime.date.month, dateTime.date.day);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	sprintf(edbg_msg, "Time: %02d:%02d:%02d\n\n", dateTime.time.hour, dateTime.time.min, dateTime.time.sec);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	
	io_write(edbg_io, (uint8_t *)"General Information:\n", 21);
	snprintf(edbg_msg, EDBG_MSG_LEN, "- DPS Name: %s\n", pcb_cfg_eeprom.dps_name);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	snprintf(edbg_msg, EDBG_MSG_LEN, "- Author Name: %s\n", pcb_cfg_eeprom.author);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	snprintf(edbg_msg, EDBG_MSG_LEN, "- DPS Version: %s\n", pcb_cfg_eeprom.dps_version);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	sprintf(edbg_msg, "- DPS Configuration Date: %s\n", pcb_cfg_eeprom.cfg_date);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	sprintf(edbg_msg, "- DPS Description: %s\n\n", pcb_cfg_eeprom.dps_description);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	
	io_write(edbg_io, (uint8_t *)"Test Configuration:\n", 20);
	
	uint8_t adc_ch_count      = pcb_cfg_eeprom.dps_cfg[CFG_ADC_GPIO_CHNLS_OFFSET] >> 4;
	uint8_t gpio_exp_ch_count = pcb_cfg_eeprom.dps_cfg[CFG_ADC_GPIO_CHNLS_OFFSET] & 0x0f;
	uint8_t di_count          = pcb_cfg_eeprom.dps_cfg[CFG_DI_DO_COUNT_OFFSET] >> 4;
	uint8_t do_count          = pcb_cfg_eeprom.dps_cfg[CFG_DI_DO_COUNT_OFFSET] & 0x0f;
	
	sprintf(edbg_msg, " - ADC Channels: %u\n", adc_ch_count);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	sprintf(edbg_msg, " - GPIO Channels: %u\n", gpio_exp_ch_count);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	sprintf(edbg_msg, " - Dig. Inputs: %u\n", di_count);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	sprintf(edbg_msg, " - Dig. Outputs: %u\n - ", do_count);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	
	// Go through dps_cfg bit by bit and get each interface set to high:
	uint8_t cfg_interfaces_byte = pcb_cfg_eeprom.dps_cfg[CFG_INTERFACES_OFFSET];
	for (uint8_t i = 0; i < PCB_CFG_INTERFACE_COUNT; ++i) {
		if (cfg_interfaces_byte & 1) {
	 		sprintf(edbg_msg, "%s", cfg_interfaces[i]);
	 		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	 		if (!(i == sizeof(cfg_interfaces) - 1)) io_write(edbg_io, (uint8_t *)"  ", 2);
	 	}
		cfg_interfaces_byte = cfg_interfaces_byte >> 1;
	}
	io_write(edbg_io, (uint8_t *)"\n\n", 2);

	io_write(edbg_io, (uint8_t *)"Test Results:\n", 14);
	io_write(edbg_io, (uint8_t *)"- Test Points:\n", 15);
	
	for (uint8_t chnl = 0; chnl < ADS7830_N_CHNL; ++chnl){
		sprintf(edbg_msg, "  - %s: \t %6.3f V -- %s\n", test_point_names[chnl], test_station.adc_buffer_volt[chnl],
		test_result.adc_channel_passed[chnl] == true ? "PASS" : "FAIL");
		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	}
	
	io_write(edbg_io, (uint8_t *)"\nPass/Fail Criteria:\n", 21);
	sprintf(edbg_msg, "- Voltage in range +/-%4.2f %%\n\n", TEST_CRIT_VOLT_RANGE_PERC);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	
	io_write(edbg_io, (uint8_t *)"Conclusion:\n", 12);
	
	// Check if array meas_volt_passed contains all elements equal to true
	uint8_t sum = 0;
	for (uint8_t chnl=0; chnl < ADS7830_N_CHNL; ++chnl) sum += test_result.adc_channel_passed[chnl];
	if (sum == ADS7830_N_CHNL)
	io_write(edbg_io, (uint8_t *)"All measured voltages are within the acceptable range.\n\n", 55);
	else {
		io_write(edbg_io, (uint8_t *)"The following TPs are not within the acceptable range:\n - ", 59);
		for (uint8_t chnl=0; chnl < ADS7830_N_CHNL; ++chnl){
			if (!test_result.adc_channel_passed[chnl]) {
				sprintf(edbg_msg, "%s", test_point_names[chnl]);
				io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
				if (!(chnl == ADS7830_N_CHNL - 1)) io_write(edbg_io, (uint8_t *)", ", 2);
				else io_write(edbg_io, (uint8_t *)"\n", 1);
			}
		}
	}
	
	io_write(edbg_io, (uint8_t *)"---------------------------------------------------------------------\n\n", 71);
}


// Data initialized in this function will be obtained from desktop app in future...
// When using snprintf (and other functions) with struct elements, saving to elements doesn't work properly! <--------
void test_station_write_pcb_config(void){
	snprintf(pcb_cfg_app.dps_name,        PCB_CFG_DPS_NAME_SIZE,      "Prizpusobovaci DPS pro Aromatizacni system");
	snprintf(pcb_cfg_app.author,          PCB_CFG_AUTHOR_SIZE,        "David Stejskal");
	snprintf(pcb_cfg_app.dps_version,     PCB_CFG_DPS_VERSION_SIZE+1, "v1.0");
	snprintf(pcb_cfg_app.cfg_date,        PCB_CFG_DATE_SIZE+1,        "08/05/2023");
	snprintf(pcb_cfg_app.dps_description, PCB_CFG_DPS_DESC_SIZE,    "Tato Prizpusobovaci DPS je testovacim podstavcem pro ridici desku Aromatizacniho systemu.");
	static const uint8_t dps_cfg_vals[PCB_CFG_SIZE] =                  {0x88, 0x62, 0x22, 0x00, 0x00, 0x00};
	memcpy(pcb_cfg_app.dps_cfg, dps_cfg_vals, PCB_CFG_SIZE);
	
	uint8_t desc_buffer1[AT24C_EEPROM_PAGE_SIZE_BYTES];
	uint8_t desc_buffer2[AT24C_EEPROM_PAGE_SIZE_BYTES];
	for (uint8_t i = 0, j = AT24C_EEPROM_PAGE_SIZE_BYTES; i < AT24C_EEPROM_PAGE_SIZE_BYTES; ++i, ++j) {
		desc_buffer1[i] = pcb_cfg_app.dps_description[i];
		desc_buffer2[i] = pcb_cfg_app.dps_description[j];
	}
	
	at24c_acivate();
	test_result.return_code = at24c_page_write(PCB_CFG_DPS_NAME_ADDR,    (uint8_t *) pcb_cfg_app.dps_name,    PCB_CFG_DPS_NAME_SIZE);
	test_result.return_code = at24c_page_write(PCB_CFG_AUTHOR_ADDR,      (uint8_t *) pcb_cfg_app.author,      PCB_CFG_AUTHOR_SIZE);
	test_result.return_code = at24c_page_write(PCB_CFG_DPS_VERSION_ADDR, (uint8_t *) pcb_cfg_app.dps_version, PCB_CFG_DPS_VERSION_SIZE);
	test_result.return_code = at24c_page_write(PCB_CFG_DATE_ADDR,        (uint8_t *) pcb_cfg_app.cfg_date,    PCB_CFG_DATE_SIZE);
	test_result.return_code = at24c_page_write(PCB_CFG_DPS_DESC_ADDR,    desc_buffer1, AT24C_EEPROM_PAGE_SIZE_BYTES);
	test_result.return_code = at24c_page_write(PCB_CFG_DPS_DESC_ADDR+AT24C_EEPROM_PAGE_SIZE_BYTES, desc_buffer2, AT24C_EEPROM_PAGE_SIZE_BYTES);
	test_result.return_code = at24c_page_write(PCB_CFG_ADDR,             (uint8_t *) pcb_cfg_app.dps_cfg,     PCB_CFG_SIZE);
}