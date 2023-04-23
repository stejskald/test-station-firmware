/*
 * ads7830.h
 *
 * Created: 18/04/2023 14:24:22
 *  Author: xstejs30
 *
 * Service library for ADS7830IPWR 8-Ch ADC over I2C
 *
 */ 


#ifndef ADS7830_H_
#define ADS7830_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../driver_init.h"

#define ADS7830_I2C_ADDR             (0x48)
// #define ADS7830_I2C_ADDR_W ((ADS7830_I2C_ADDR << 1) | 0x00)
// #define ADS7830_I2C_ADDR_R ((ADS7830_I2C_ADDR << 1) | 0x01)

// Conversion Delay (in ms)
#define ADS7830_CONVERSION_DELAY     (100)

// Command Byte Definitions
#define ADS7830_CMD_SD_MASK          (0x80) // Single-Ended/Differential Inputs
#define ADS7830_CMD_SD_DIFF          (0x00) // Bit = 0, Differential Inputs
#define ADS7830_CMD_SD_SINGLE        (0x80) // Bit = 1, Single-Ended Inputs
								     
#define ADS7830_CMD_CH_MASK          (0x70)
#define ADS7830_CMD_CHNL_SE0         (0x00) // Single-Ended P = CH0, N = COM
#define ADS7830_CMD_CHNL_SE2         (0x10) // Single-Ended P = CH2, N = COM
#define ADS7830_CMD_CHNL_SE4         (0x20) // Single-Ended P = CH4, N = COM
#define ADS7830_CMD_CHNL_SE6         (0x30) // Single-Ended P = CH6, N = COM
#define ADS7830_CMD_CHNL_SE1         (0x40) // Single-Ended P = CH1, N = COM
#define ADS7830_CMD_CHNL_SE3         (0x50) // Single-Ended P = CH3, N = COM
#define ADS7830_CMD_CHNL_SE5         (0x60) // Single-Ended P = CH5, N = COM
#define ADS7830_CMD_CHNL_SE7         (0x70) // Single-Ended P = CH7, N = COM
#define ADS7830_CMD_CHNL_DIFF0_1     (0x00) // Differential P = CH0, N = CH1
#define ADS7830_CMD_CHNL_DIFF2_3     (0x10) // Differential P = CH2, N = CH3
#define ADS7830_CMD_CHNL_DIFF4_5     (0x20) // Differential P = CH4, N = CH5
#define ADS7830_CMD_CHNL_DIFF6_7     (0x30) // Differential P = CH6, N = CH7
#define ADS7830_CMD_CHNL_DIFF1_0     (0x40) // Differential P = CH1, N = CH0
#define ADS7830_CMD_CHNL_DIFF3_2     (0x50) // Differential P = CH3, N = CH2
#define ADS7830_CMD_CHNL_DIFF5_4     (0x60) // Differential P = CH5, N = CH4
#define ADS7830_CMD_CHNL_DIFF7_6     (0x70) // Differential P = CH7, N = CH6
									 
#define ADS7830_CMD_PD_MASK          (0x0C) // Power-Down Selection
#define ADS7830_CMD_PD_PD_BTWN_CONVS (0x00) // Power Down Between A/D Converter Conversions
#define ADS7830_CMD_PD_IROFF_ADCON   (0x04) // Internal Reference OFF and A/D Converter ON
#define ADS7830_CMD_PD_IRON_ADCOFF   (0x08) // Internal Reference ON and A/D Converter OFF
#define ADS7830_CMD_PD_IRON_ADCON    (0x0C) // Internal Reference ON and A/D Converter ON

typedef enum {
	SDMODE_DIFF   = ADS7830_CMD_SD_DIFF,
	SDMODE_SINGLE = ADS7830_CMD_SD_SINGLE
} ads7830_sd_mode_t;

typedef enum {
	PDADC_BTWN_CONV = ADS7830_CMD_PD_PD_BTWN_CONVS,
	PDIROFF_ADCON   = ADS7830_CMD_PD_IROFF_ADCON,
	PDIRON_ADCOFF   = ADS7830_CMD_PD_IRON_ADCOFF,
	PDIRON_ADCON    = ADS7830_CMD_PD_IRON_ADCON
} ads7830_pd_mode_t;

typedef struct {
	ads7830_sd_mode_t sd_mode;
	ads7830_pd_mode_t pd_mode;
	uint8_t cmd_byte;
	uint8_t i2c_address;
} ADS7830_t;


// extern struct i2c_m_sync_desc ADS7830;

extern ADS7830_t i2c_adc;

extern struct io_descriptor *ads7830_io;

void ads7830_init_ext(void);

void set_ads7830_i2c_address(uint8_t i2c_address);
uint8_t get_ads7830_i2c_address(void);


void _set_ads7830_i2c_cmd_sd_mode(ads7830_sd_mode_t sd_mode);
void _set_ads7830_i2c_cmd_pd_mode(ads7830_pd_mode_t pd_mode);
bool _set_ads7830_i2c_cmd_byte(ads7830_sd_mode_t sd_mode, uint8_t channel, ads7830_pd_mode_t pd_mode);

uint8_t ads7830_measure_single_ended(ads7830_sd_mode_t sd_mode, uint8_t channel, ads7830_pd_mode_t pd_mode);

#ifdef __cplusplus
}
#endif

#endif /* ADS7830_H_ */