/*
 * pca9557.h
 *
 * Created: 01/05/2023 18:02:18
 *  Author: xstejs30
 *
 * PCA9557 Driver (8-Channel GPIO I2C Expander)
 */ 


#ifndef PCA9557_H_
#define PCA9557_H_

#include "error_codes.h"

#include <stdio.h>

/************************************************************************/
/* PCA9557 Definitions                                                  */
/************************************************************************/
#define PCA9557_I2C_ADDR         0x18  
#define PCA9557_N_CHNL           8     // Number of channels (IOs)
// #define PCA9557_NUM_BYTES        0x01 
#define PCA9557_ALL_INPUT        0xff
#define PCA9557_ALL_OUTPUT       0x00
#define PCA9557_ALL_LOW          0x00
#define PCA9557_ALL_NON_INVERTED 0x00
#define PCA9557_ALL_HIGH         0xff
#define PCA9557_ALL_INVERTED     0xff
#define PCA9557_COM_SUCCESS      0x00 


/************************************************************************/
/* PCA9557 REGISTERS                                                    */
/*                                                                      */
/* REGISTER 0: INPUT REGISTER - READ ONLY (0 = LOW / 1 = HIGH)          */  
/* DEFAULT: 'HIGH' (EACH IO PIN HAS A WEAK PULL-UP RESISTOR)            */    
/*                                                                      */     
/* REGISTER 1: OUTPUT REGISTER - R/W (0 = LOW / 1 = HIGH)               */
/* DEFAULT: 'HIGH'                                                      */
/*                                                                      */
/* REGISTER 2: POLARITY REGISTER - R/W (0 = NON-INV. / 1 = INV.)	    */
/* DEFAULT: 'LOW'                                                       */
/*                                                                      */ 
/* REGISTER 3: CONFIGURATION - R/W (0 = OUTPUT / 1 = INPUT)             */   
/* POWER-UP DEFAULT: ALL PINS ARE SET AS 'INPUT' (1)                    */
/*                                                                      */
/************************************************************************/
typedef enum {
	PCA9557_REG_INPUT    = 0, // Input Port Register (Default)  (R)
	PCA9557_REG_OUTPUT   = 1, // Output Port Register          (R/W)
	PCA9557_REG_POLARITY = 2, // Polarity Inversion Register   (R/W)
	PCA9557_REG_CONFIG   = 3  // Configuration Register        (R/W)
} pca9557_reg_addr_t;


/************************************************************************/
/* Helpful data types                                                   */
/************************************************************************/
typedef enum {
	IO0 = 0,
	IO1 = 1,
	IO2 = 2,
	IO3 = 3,
	IO4 = 4,
	IO5 = 5,
	IO6 = 6,
	IO7 = 7
} pca9557_pin_t;

typedef enum {
	IO_OUTPUT = 0,
	IO_INPUT  = 1
} pca9557_mode_t;

typedef enum {
	IO_LOW  = 0,
	IO_HIGH = 1
} pca9557_state_t;

typedef enum {
	IO_NON_INVERTED = 0,
	IO_INVERTED     = 1
} pca9557_polarity_t;


extern struct io_descriptor *pca9557_io;


/************************************************************************/
/* Function definitions                                                 */
/************************************************************************/
// private functions:
uint8_t _pca9557_get_reg(pca9557_reg_addr_t reg_addr);
uint8_t _pca9557_get_pin(pca9557_pin_t pin, pca9557_reg_addr_t reg);
ERROR_t _pca9557_set_reg(pca9557_reg_addr_t reg, uint8_t reg_setting);
ERROR_t _pca9557_set_pin(pca9557_pin_t pin, pca9557_reg_addr_t reg, pca9557_state_t new_pin_val);

// public functions:
void pca9557_init(void);
void pca9557_reset(void);
void pca9557_activate(void);
uint8_t pca9557_get_pin_mode(pca9557_pin_t pin);
uint8_t pca9557_get_pin_state(pca9557_pin_t pin);
uint8_t pca9557_get_pin_polarity(pca9557_pin_t pin);
ERROR_t pca9557_set_pin_mode(pca9557_pin_t pin, pca9557_mode_t set_mode);
ERROR_t pca9557_set_mode_all(pca9557_mode_t set_mode);
ERROR_t pca9557_set_pin_state(pca9557_pin_t pin, pca9557_state_t new_state);
ERROR_t pca9557_set_state_all(pca9557_state_t new_state);
ERROR_t pca9557_set_pin_polarity(pca9557_pin_t pin, pca9557_polarity_t new_polarity);
ERROR_t pca9557_set_polarity_all(pca9557_polarity_t new_polarity);

#endif /* PCA9557_H_ */