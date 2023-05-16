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
#include "include/error_codes.h"

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

// Specify which channels measure power supply voltages
#define ADC_PWR_SUPPLY_CHNLS  0b00111101 // [ch7:ch0]


/************************************************************************/
/* Testing Metrics/Criteria                                             */
/************************************************************************/
#define TEST_CRIT_VOLT_RANGE_PERC 10.0f

#define ADC_CH0_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH1_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH2_REQUIRED_VOLTAGE  5.000f // Volts
#define ADC_CH3_REQUIRED_VOLTAGE  4.000f // Volts
#define ADC_CH4_REQUIRED_VOLTAGE  3.300f // Volts
#define ADC_CH5_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH6_REQUIRED_VOLTAGE 24.000f // Volts
#define ADC_CH7_REQUIRED_VOLTAGE 24.000f // Volts


/************************************************************************/
/* Parameter Definitions Related to PCB Configuration                   */
/************************************************************************/
// Sizes in bytes
#define PCB_CFG_DPS_NAME_SIZE      64U
#define PCB_CFG_AUTHOR_SIZE        32U
#define PCB_CFG_DPS_VERSION_SIZE    4U
#define PCB_CFG_DATE_SIZE          10U
#define PCB_CFG_DPS_DESC_SIZE     128U
#define PCB_CFG_SIZE                6U

#define PCB_CFG_DPS_NAME_ADDR     0x0000
#define PCB_CFG_AUTHOR_ADDR       0x0040
#define PCB_CFG_DPS_VERSION_ADDR  0x0060
#define PCB_CFG_DATE_ADDR         0x0064
#define PCB_CFG_DPS_DESC_ADDR     0x006E
#define PCB_CFG_ADDR              0x00EE

#define CFG_ADC_GPIO_CHNLS_OFFSET 0x00
#define CFG_DI_DO_COUNT_OFFSET    0x01
#define CFG_INTERFACES_OFFSET     0x02

#define PCB_CFG_INTERFACE_COUNT   6U


/************************************************************************/
/* Data Types Definitions                                               */
/************************************************************************/
typedef struct {
	float adc_buffer_volt[ADS7830_N_CHNL];
} test_station_t;

typedef struct {
	bool is_eeprom_load_ok;// = false;
	bool is_serial_com_ok;// = false;
	
	bool adc_channel_passed[ADS7830_N_CHNL];// = {0};
	
	ERROR_t return_code;// = ERROR_NONE;
} test_result_t;

typedef struct {
	char dps_name[PCB_CFG_DPS_NAME_SIZE];
	char author[PCB_CFG_AUTHOR_SIZE];
	char dps_version[PCB_CFG_DPS_VERSION_SIZE];
	char cfg_date[PCB_CFG_DATE_SIZE];
	char dps_description[PCB_CFG_DPS_DESC_SIZE];
	uint8_t dps_cfg[PCB_CFG_SIZE];
} pcb_config_t;

extern test_station_t test_station;
extern test_result_t test_result;

/************************************************************************/
/* Test Point Names of Specific PCB                                     */
/************************************************************************/
extern const char* test_point_names[];


/************************************************************************/
/* Variables Declarations                                               */
/************************************************************************/




/************************************************************************/
/* Function Declarations                                                */
/************************************************************************/
test_result_t test_station_main_test_script(void);
bool _read_pcb_config(void);
bool _test_serial_com(void);
void _measure_selected_adc_channels(void);
void _activate_digital_outputs(void);
void _measure_all_adc_channels(void);
void _activate_digital_inputs(void);
void _check_digital_input_states(void);

void _generate_final_report(void);

float _adc_raw_to_voltage(uint8_t adc_raw_data, uint8_t channel);
bool _verify_test_criteria_voltage(float percentage_voltage_range, uint8_t channel, float measured_voltage);


/************************************************************************/
/* Declarations of functions only for testing purposses                 */
/************************************************************************/
void test_station_write_pcb_config(void);

#ifdef __cplusplus
}
#endif

#endif /* TEST_STATION_H_ */