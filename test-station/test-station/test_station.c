/*
 * test_station.c
 *
 * Created: 28/04/2023 13:39:08
 *  Author: xstejs30
 */ 

#include "test_station.h"
#include "include/ads7830.h"
#include "include/edbg.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

const char* testPointNames[] = {
	"VCC", "Air", "+5V", "+4V", "+3V3", "VDD", "DO0", "DO1"
};

float adc_raw_to_voltage(uint8_t adc_raw_data, uint8_t channel){
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
	if (adc_raw_data == 0){ return 0.0f; }
	float koef = ADC_VOLTAGE_REFERENCE / (pow(2U,ADS7830_RESOLUTION_BITS) - 1);
	
	return koef * adc_raw_data * (1/divider_ratio);
}

bool verify_test_criteria_voltage_range(float percentage_voltage_range, uint8_t channel, float measured_voltage){
	if (!(channel >= 0 && channel < ADS7830_N_CHNL)) {
		return false;
	}

	uint16_t meas_volt = (uint16_t)(measured_voltage * 1000U);
	uint16_t req_voltage;
	
	switch (channel) {
		case (0):
		req_voltage = ADC_CH0_REQUIRED_VOLTAGE;
		break;
		case (1):
		req_voltage = ADC_CH1_REQUIRED_VOLTAGE;
		break;
		case (2):
		req_voltage = ADC_CH2_REQUIRED_VOLTAGE;
		break;
		case (3):
		req_voltage = ADC_CH3_REQUIRED_VOLTAGE;
		break;
		case (4):
		req_voltage = ADC_CH4_REQUIRED_VOLTAGE;
		break;
		case (5):
		req_voltage = ADC_CH5_REQUIRED_VOLTAGE;
		break;
		case (6):
		req_voltage = ADC_CH6_REQUIRED_VOLTAGE;
		break;
		case (7):
		req_voltage = ADC_CH7_REQUIRED_VOLTAGE;
		break;
	}
	req_voltage *= 1000U;

	uint16_t perc_volt = (uint16_t)(percentage_voltage_range * 10U);
	uint16_t meas_req_ratio = (uint16_t)((meas_volt / (float)req_voltage) * 1000U);
	
	return (meas_req_ratio >= (1000U - perc_volt)) && (meas_req_ratio <= (1000U + perc_volt));
}

void test_station_send_report(void){
	io_write(edbg_io, (uint8_t *)"-----------------------------------------------------\n", 54);
	io_write(edbg_io, (uint8_t *)"===========  Automatic Test Final Report  ===========\n", 54);
	io_write(edbg_io, (uint8_t *)"-----------------------------------------------------\n", 54);
	
	struct calendar_date_time dateTime;
	calendar_get_date_time(&CALENDAR, &dateTime); // get actual date and time

	sprintf(edbg_msg, "Date: %d-%02d-%02d\n", dateTime.date.year, dateTime.date.month, dateTime.date.day);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	sprintf(edbg_msg, "Time: %02d:%02d:%02d\n\n", dateTime.time.hour, dateTime.time.min, dateTime.time.sec);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	
	io_write(edbg_io, (uint8_t *)"Test Configuration:\n", 20);
	io_write(edbg_io, (uint8_t *)"- Measured DPS: <Name and Version>\n\n", 36);
	io_write(edbg_io, (uint8_t *)"Test Results:\n", 14);
	io_write(edbg_io, (uint8_t *)"- Test Points:\n", 15);
	
	for (uint8_t chnl = 0; chnl < ADS7830_N_CHNL; ++chnl){
		sprintf(edbg_msg, "  - %s: \t %6.3f V -- %s\n", testPointNames[chnl], adc_volt_buffer[chnl],
		meas_volt_passed[chnl] == true ? "PASS" : "FAIL");
		io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	}
	
	io_write(edbg_io, (uint8_t *)"\nPass/Fail Criteria:\n", 21);
	sprintf(edbg_msg, "- Voltage in range �%4.2f %%\n\n", TEST_CRIT_VOLT_RANGE_PERCENTAGE);
	io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
	
	io_write(edbg_io, (uint8_t *)"Conclusion:\n", 12);
	
	// Check if array meas_volt_passed contains all elements equal to true
	uint8_t sum = 0;
	for (uint8_t chnl=0; chnl < ADS7830_N_CHNL; ++chnl) sum += meas_volt_passed[chnl];
	if (sum == ADS7830_N_CHNL)
	io_write(edbg_io, (uint8_t *)"All measured voltages are within acceptable range.\n\n", 51);
	else {
		io_write(edbg_io, (uint8_t *)"The following TPs are not within acceptable range:\n", 51);
		for (uint8_t chnl=0; chnl < ADS7830_N_CHNL; ++chnl){
			if (!meas_volt_passed[chnl]) {
				sprintf(edbg_msg, " - %s\n", testPointNames[chnl]);
				io_write(edbg_io, (uint8_t *)edbg_msg, strlen(edbg_msg));
			}
		}
	}
	
	io_write(edbg_io, (uint8_t *)"\n-----------------------------------------------------\n\n", 56);
}