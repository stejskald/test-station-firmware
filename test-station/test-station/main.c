/**
 * \file
 *
 * \brief IO1 Xplained demo
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "io1_xplained_demo_config.h"
#include "temperature_sensor_main.h"
#include "include/ads7830.h"

#include <hal_delay.h>
#include <stdio.h>
#include <string.h>

#define STR_SIZE 24

uint16_t                     temp_result;
static struct io_descriptor *terminal_io; // EDBG_COM

void UART_EDBG_init()
{
	usart_sync_get_io_descriptor(&EDBG_COM, &terminal_io);
	usart_sync_enable(&EDBG_COM);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	UART_EDBG_init();
	ads7830_init_ext();
	
	uint8_t adc_buffer[8];
	char terminal_string[STR_SIZE];
	
	
/************************************************************************/
/* Default                                                              */
/************************************************************************/
// 	uint8_t light_sensor_result_buf[2];
// 	char    str[STR_SIZE];

// 	atmel_start_init();

// 	temperature_sensors_init();
// 	UART_EDBG_init();

// 	adc_sync_enable_channel(&IO1_LIGHT_SENS, CONF_ADC_CHANNEL);
	

	io_write(terminal_io, (uint8_t *)"Well, terminal works!\n", 22);
	while (1) {
// 		for (uint8_t chnl = 0; chnl < 8; ++chnl) {
// 			adc_buffer[chnl] = ads7830_measure_single_ended(SDMODE_SINGLE, chnl, PDIROFF_ADCON);
// 		
// 			if(0 > sprintf(terminal_string, "Channel %u: %u\n", chnl, adc_buffer[chnl])) return -666;
// 		
// 			io_write(terminal_io, (uint8_t *)terminal_string, 13);
// 		}
		
		adc_buffer[0] = ads7830_measure_single_ended(SDMODE_SINGLE, 2, PDIROFF_ADCON);
		if(0 > snprintf(terminal_string, STR_SIZE, "Channel %u, value: 0x%x\r\n", 2, adc_buffer[0])) return -666;
		
		io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
		
		delay_ms(150);
		
/************************************************************************/
/* Default                                                              */
/************************************************************************/
// 		delay_ms(1000);
// 		temp_result = (uint16_t)temperature_sensor_read(AT30TSE75X);
// 
// 		adc_sync_read_channel(&IO1_LIGHT_SENS, CONF_ADC_CHANNEL, light_sensor_result_buf, 2);
// 
// 		uint16_t light_sensor_result = *((uint16_t *)light_sensor_result_buf);
// 
// 		snprintf(str, STR_SIZE, "Temperature: %d Celsius, light sensor: 0x%x\r\n", temp_result, light_sensor_result);
// 
// 		io_write(terminal_io, (uint8_t *)str, strlen(str));
	}
}