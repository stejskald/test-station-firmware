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
// #include "io1_xplained_demo_config.h"
// #include "temperature_sensor_main.h"
#include "include/error_codes.h"
#include "include/ads7830.h"
#include "include/at24c.h"

#include <hal_delay.h>
#include <stdio.h>
#include <string.h>

#define STR_SIZE 60

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
// 	ads7830_init_ext();
	at24c_init();
	
	char terminal_string[STR_SIZE];
// 	uint8_t adc_buffer[8];
// 	uint8_t eeprom_buffer[8];
	uint8_t eeprom_page_buffer[AT24C_EEPROM_PAGE_SIZE_BYTES];
	uint16_t eeprom_rw_addr;
	int8_t ret_code = 0;
	
	io_write(terminal_io, (uint8_t *)"Well, terminal works!\n", 22);
	
	eeprom_rw_addr = 0x0000;
	uint8_t eeprom_test_page[AT24C_EEPROM_PAGE_SIZE_BYTES];
	for (uint8_t i = 0; i < sizeof(eeprom_test_page); ++i){
		eeprom_test_page[i] = i;
	}
	
	while (1) {		
		delay_ms(10000);
		ret_code = at24c_page_write(eeprom_rw_addr, eeprom_test_page, AT24C_EEPROM_PAGE_SIZE_BYTES);
		if (ret_code != 0){
			if(0 > snprintf(terminal_string, STR_SIZE, "Byte write to EEPROM failed with code %d.\n", ret_code)) return -666;
			io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
		}
		ret_code = 0;
		
		delay_ms(10000);
		ret_code = at24c_sequential_read(eeprom_rw_addr, eeprom_page_buffer, AT24C_EEPROM_PAGE_SIZE_BYTES);
		if (ret_code != 0){
			if(0 > snprintf(terminal_string, STR_SIZE, "Read from EEPROM failed with code %d.\n", ret_code)) return -666;
			io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
		}
		ret_code = 0;
		
// 		for (uint8_t chnl = 0; chnl < 8; ++chnl) {
// 			adc_buffer[chnl] = ads7830_measure_single_ended(SDMODE_SINGLE, chnl, PDIROFF_ADCON);
// 		
// 			if(0 > sprintf(terminal_string, "Channel %u: %u\n", chnl, adc_buffer[chnl])) return -666;
// 		
// 			io_write(terminal_io, (uint8_t *)terminal_string, 13);
// 		}
		
		
		//adc_buffer[0] = ads7830_measure_single_ended(SDMODE_SINGLE, 2, PDIROFF_ADCON);
		//
		//if(0 > snprintf(terminal_string, STR_SIZE, "Channel %u, value: 0x%x\n", 2, adc_buffer[0])) return -666;
		//io_write(terminal_io, (uint8_t *)terminal_string, strlen(terminal_string));
		
		
		
	}
}
