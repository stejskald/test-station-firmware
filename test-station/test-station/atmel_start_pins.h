/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMC21 has 9 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8

#define PA08 GPIO(GPIO_PORTA, 8)
#define PA09 GPIO(GPIO_PORTA, 9)
#define IO1_TWI_SDA GPIO(GPIO_PORTA, 12)
#define IO1_TWI_SCL GPIO(GPIO_PORTA, 13)
#define IO1_LIGHTSENSOR GPIO(GPIO_PORTB, 9)
#define EDBG_COM_TX GPIO(GPIO_PORTB, 10)
#define EDBG_COM_RX GPIO(GPIO_PORTB, 11)

#endif // ATMEL_START_PINS_H_INCLUDED
