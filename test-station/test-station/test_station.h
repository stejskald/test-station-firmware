/*
 * test_station.h
 *
 * Created: 28/04/2023 12:48:03
 *  Author: xstejs30
 */ 


#ifndef TEST_STATION_H_
#define TEST_STATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "include/ads7830.h"

/************************************************************************/
/* Parameter definitions related to 8-Channel ADC                       */
/************************************************************************/
#define ADC_VOLTAGE_REFERENCE 4.76f // Volts
#define ADC_CH0_DIVIDER_RATIO 0.2087f
#define ADC_CH1_DIVIDER_RATIO 0.2087f
#define ADC_CH2_DIVIDER_RATIO 1.0000f
#define ADC_CH3_DIVIDER_RATIO 1.0000f
#define ADC_CH4_DIVIDER_RATIO 1.0000f
#define ADC_CH5_DIVIDER_RATIO 0.1254f
#define ADC_CH6_DIVIDER_RATIO 0.1254f
#define ADC_CH7_DIVIDER_RATIO 0.1254f


/************************************************************************/
/* Testing Metrics/Criteria                                             */
/************************************************************************/
#define TEST_CRIT_VOLT_RANGE_PERCENTAGE 10.0f

#define ADC_CH0_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH1_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH2_REQUIRED_VOLTAGE  5.000f // Volts
#define ADC_CH3_REQUIRED_VOLTAGE  4.000f // Volts
#define ADC_CH4_REQUIRED_VOLTAGE  3.300f // Volts
#define ADC_CH5_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH6_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH7_REQUIRED_VOLTAGE 24.000f // Volts


/************************************************************************/
/* Parameter definitions related to PCB CONFIGURATION                   */
/************************************************************************/
#define PCB_CFG_DPS_NAME_SIZE     64U // Bytes
#define PCB_CFG_DPS_AUTHOR_SIZE   32U // Bytes
#define PCB_CFG_DPS_VERSION_SIZE   4U // Bytes
#define PCB_CFG_DATE_SIZE         10U // Bytes
#define PCB_CFG_DESCRIPTION_SIZE 128U // Bytes
#define PCB_CFG_SIZE               6U // Bytes

#define PCB_CFG_DPS_NAME_ADDR    0x0000
#define PCB_CFG_DPS_AUTHOR_ADDR  0x0040
#define PCB_CFG_DPS_VERSION_ADDR 0x0060
#define PCB_CFG_DATE_ADDR        0x0064
#define PCB_CFG_DESCRIPTION_ADDR 0x006E
#define PCB_CFG_ADDR             0x00EE


/************************************************************************/
/* Data Types Definitions                                               */
/************************************************************************/
typedef struct {
	
} test_result_t;

typedef struct {
	char dps_name[PCB_CFG_DPS_NAME_SIZE];
	char author[PCB_CFG_DPS_AUTHOR_SIZE];
	char dps_version[PCB_CFG_DPS_VERSION_SIZE];
	char cfg_date[PCB_CFG_DATE_SIZE];
	char dps_description[PCB_CFG_DESCRIPTION_SIZE];
	uint8_t dps_cfg[PCB_CFG_SIZE];
} pcb_config_t;


/************************************************************************/
/* Test Point names of specific PCB                                     */
/************************************************************************/
extern const char* testPointNames[];


/************************************************************************/
/* Variables declarations                                               */
/************************************************************************/
float adc_volt_buffer[ADS7830_N_CHNL];
bool meas_volt_passed[ADS7830_N_CHNL];


/************************************************************************/
/* Help function declarations                                           */
/************************************************************************/
float adc_raw_to_voltage(uint8_t adc_raw_data, uint8_t channel);
bool verify_test_criteria_voltage_range(float percentage_voltage_range, uint8_t channel, float measured_voltage);

void test_station_send_report(void);

void test_station_write_pcb_config(void);
void test_station_read_pcb_config(void);

#ifdef __cplusplus
}
#endif

#endif /* TEST_STATION_H_ */