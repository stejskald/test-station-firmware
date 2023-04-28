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
/* Test Criteria                                                        */
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
/* Test Point names of specific PCB                                     */
/************************************************************************/
extern const char* testPointNames[];

/************************************************************************/
/* Help function declarations                                           */
/************************************************************************/
float adc_raw_to_voltage(uint8_t adc_raw_data, uint8_t channel);
bool verify_test_criteria_voltage_range(float percentage_voltage_range, uint8_t channel, float measured_voltage);


#ifdef __cplusplus
}
#endif

#endif /* TEST_STATION_H_ */