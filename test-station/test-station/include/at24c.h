/*
 * AT24C.h
 *
 * Created: 22/04/2023 13:41:07
 *  Author: xstejs30
 *
 * Service library for AT24C I2C EEPROM
 *
 */ 


#ifndef AT24C_H_
#define AT24C_H_

#include "../include/error_codes.h"


/************************************************************************/
/* AT24C256C Definitions                                                */
/************************************************************************/
#define AT24C_EEPROM_I2C_ADDR         0x50 //0x50
#define AT24C_EEPROM_PAGE_SIZE_BYTES  64U
#define AT24C_EEPROM_PAGE_COUNT	      512U
#define AT24C_EEPROM_TOTAL_SIZE_BYTES 32768U

#define AT24C_EEPROM_DELAY 50U // ms


/************************************************************************/
/* AT24C Data Types                                                     */
/************************************************************************/
//// typedef void (* DRV_AT24_PLIB_CALLBACK)( uintptr_t );
//
//typedef bool (* AT24C_WRITE_READ)(uint16_t , uint8_t* , uint32_t , uint8_t* , uint32_t);
//
//typedef bool (* AT24C_WRITE)(uint16_t , uint8_t* , uint32_t );
//
//typedef bool (* AT24C_READ)(uint16_t , uint8_t* , uint32_t);
//
//typedef bool (* AT24C_IS_BUSY)(void);
//
//typedef AT24C_I2C_ERROR (* AT24C_ERROR_GET)(void);
//
//// typedef void (* DRV_AT24_PLIB_CALLBACK_REGISTER)(DRV_AT24_PLIB_CALLBACK, uintptr_t);
//
//typedef struct {
	///* AT24C writeRead API */
	//AT24C_WRITE_READ        write_read;
//
	///* AT24C write API */
	//AT24C_WRITE             write;
//
	///* AT24C read API */
	//AT24C_READ              read;
//
	///* AT24C Transfer status API */
	//AT24C_IS_BUSY           is_busy;
//
	///* AT24C Error get API */
	//AT24C_ERROR_GET         error_get;
//
	///* AT24C callback register API */
//// 	AT24C_CALLBACK_REGISTER callback_register;
//} AT24C_INTERFACE_t;

typedef struct {
// 	const AT24C_INTERFACE_t* i2c;
	
    /* EEPROM I2C address */
    uint8_t  slave_address;

    /* EEPROM page size (in Bytes)  */
    uint8_t  page_size;

    /* EEPROM total size (in Bytes) */
    uint16_t flash_size;

    /* Number of clients */
//     size_t   clients_count;

//     uint32_t block_start_address;
} AT24C_t;

extern AT24C_t i2c_eeprom;


/************************************************************************/
/* Function definitions                                                 */
/************************************************************************/
// private functions
void _set_at24c_params(AT24C_t *i2c_eeprom);

// public functions
void at24c_init(void);
void at24c_acivate(void);

ERROR_t at24c_byte_write(uint16_t byte_address, uint8_t *byte_buffer);
ERROR_t at24c_page_write(uint16_t address, uint8_t *page_buffer, uint8_t length);
ERROR_t at24c_page_clear(uint16_t page_number);

ERROR_t at24c_current_addr_read(uint8_t *byte_buffer);
ERROR_t at24c_random_read(uint16_t address, uint8_t *byte_buffer);
ERROR_t at24c_sequential_read(uint16_t address, uint8_t *data_buffer, uint16_t length);

// software_reset ... S 0xff S P

#endif /* AT24C_H_ */