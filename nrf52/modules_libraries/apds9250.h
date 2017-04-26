#ifndef APDS9250_H
#define APDS9250_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"

#define APDS9250_DEVICE_ADDRESS		0x52

#define APDS9250_REG_MAIN_CTRL        0x00 /* (t)(rw) LS operation mode control, SW reset */
#define APDS9250_REG_LS_MEAS_RATE     0x04 /* (t)(rw) LS measurement rate and resolution in active mode */
#define APDS9250_REG_LS_GAIN          0x05 /* (t)(rw) LS analog gain range */
#define APDS9250_PART_ID          0x06 /*    (ro) Part number ID and revision ID */
#define APDS9250_REG_MAIN_STATUS      0x07 /*    (ro) Power-on status, interrupt status, data status */
#define APDS9250_REG_LS_DATA_IR_0     0x0A /*    (ro) IR    ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_IR_1     0x0B /*    (ro) IR    ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_IR_2     0x0C /*    (ro) IR    ADC measurement data - MSB */
#define APDS9250_REG_LS_DATA_GREEN_0  0x0D /*    (ro) Green ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_GREEN_1  0x0E /*    (ro) Green ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_GREEN_2  0x0F /*    (ro) Green ADC measurement data - MSB */
#define APDS9250_REG_LS_DATA_BLUE_0   0x10 /*    (ro) Blue  ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_BLUE_1   0x11 /*    (ro) Blue  ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_BLUE_2   0x12 /*    (ro) Blue  ADC measurement data - MSB */
#define APDS9250_REG_LS_DATA_RED_0    0x13 /*    (ro) Red   ADC measurement data - LSB */
#define APDS9250_REG_LS_DATA_RED_1    0x14 /*    (ro) Red   ADC measurement data - ISB */
#define APDS9250_REG_LS_DATA_RED_2    0x15 /*    (ro) Red   ADC measurement data - MSB */
#define APDS9250_REG_INT_CFG          0x19 /*    (rw) Interrupt configuration */
#define APDS9250_REG_INT_PERSISTENCE  0x1A /*    (rw) Interrupt persist setting */
#define APDS9250_REG_LS_THRES_UP_0    0x21 /*    (rw) LS interrupt upper threshold - LSB */
#define APDS9250_REG_LS_THRES_UP_1    0x22 /*    (rw) LS interrupt upper threshold - ISB */
#define APDS9250_REG_LS_THRES_UP_2    0x23 /*    (rw) LS interrupt upper threshold - MSB */
#define APDS9250_REG_LS_THRES_LOW_0   0x24 /*    (rw) LS interrupt lower threshold - LSB */
#define APDS9250_REG_LS_THRES_LOW_1   0x25 /*    (rw) LS interrupt lower threshold - ISB */
#define APDS9250_REG_LS_THRES_LOW_2   0x26 /*    (rw) LS interrupt lower threshold - MSB */
#define APDS9250_REG_LS_THRES_VAR     0x27 /*    (rw) LS interrupt variance threshold */

#define APDS9250_CTRL_SW_RESET    (1 << 4) /* Trigger software reset */
#define APDS9250_CTRL_CS_MODE_ALS (0 << 2) /* Channel Select 0 - ALS & IR mode (default) */
#define APDS9250_CTRL_CS_MODE_RGB (1 << 2) /* Channel Select 1 - RGB & IR mode */
#define APDS9250_CTRL_CS_MASK     (1 << 2) /* Channel Select mask */
#define APDS9250_CTRL_LS_EN       (1 << 1) /* Light Sensor enabled */

/* Sensor resolution, with minimum integration time */
#define APDS9250_RESOLUTION_20BIT (0 << 4) /* 20 bit resolution, 400ms integration time */
#define APDS9250_RESOLUTION_19BIT (1 << 4) /* 19 bit resolution, 200ms integration time */
#define APDS9250_RESOLUTION_18BIT (2 << 4) /* 18 bit resolution, 100ms integration time (default) */
#define APDS9250_RESOLUTION_17BIT (3 << 4) /* 17 bit resolution, 50ms integration time */
#define APDS9250_RESOLUTION_16BIT (4 << 4) /* 16 bit resolution, 25ms integration time */
#define APDS9250_RESOLUTION_13BIT (5 << 4) /* 13 bit resolution, 3.125ms integration time */
#define APDS9250_RESOLUTION_MASK  (7 << 4) /* Mask for resolution bits */

/* Sensor measurement rate -- I think it overrides the integration window above? */
#define APDS9250_MEAS_RATE_25MS   (0 << 0) /* 25ms integration time */
#define APDS9250_MEAS_RATE_50MS   (1 << 0) /* 50ms integration time */
#define APDS9250_MEAS_RATE_100MS  (2 << 0) /* 100ms integration time */
#define APDS9250_MEAS_RATE_200MS  (3 << 0) /* 200ms integration time */
#define APDS9250_MEAS_RATE_500MS  (4 << 0) /* 500ms integration time */
#define APDS9250_MEAS_RATE_1000MS (5 << 0) /* 1000ms integration time */
#define APDS9250_MEAS_RATE_2000MS (6 << 0) /* 2000ms integration time */
#define APDS9250_MEAS_RATE_DUP    (7 << 0) /* 2000ms integration time (duplicate) */
#define APDS9250_MEAS_RATE_MASK   (7 << 0) /* Mask for resolution bits */

#define APDS9250_LS_GAIN_1X       (0 << 0) /* Gain 1x */
#define APDS9250_LS_GAIN_3X       (1 << 0) /* Gain 3x */
#define APDS9250_LS_GAIN_6X       (2 << 0) /* Gain 6x */
#define APDS9250_LS_GAIN_9X       (3 << 0) /* Gain 9x */
#define APDS9250_LS_GAIN_18X      (4 << 0) /* Gain 18x */
#define APDS9250_LS_GAIN_MASK     (7 << 0) /* Gain mask */


typedef enum apds9250_chan {
  APDS9250_CHAN_ALS = 0,
  APDS9250_CHAN_RGB = 1
} apds9250_chan_t;

typedef enum apds9250_res {
  APDS9250_RES_20BIT = 0,
  APDS9250_RES_19BIT = 1,
  APDS9250_RES_18BIT = 2,
  APDS9250_RES_17BIT = 3,
  APDS9250_RES_16BIT = 4,
  APDS9250_RES_13BIT = 5
} apds9250_res_t;

typedef enum apds9250_rate {
  APDS9250_RATE_25MS = 0,
  APDS9250_RATE_50MS = 1,
  APDS9250_RATE_100MS = 2,
  APDS9250_RATE_200MS = 3,
  APDS9250_RATE_500MS = 4,
  APDS9250_RATE_1000MS = 5,
  APDS9250_RATE_2000MS = 6
} apds9250_rate_t;

typedef enum apds9250_gain {
  APDS9250_GAIN_1X = 0,
  APDS9250_GAIN_3X = 1,
  APDS9250_GAIN_6X = 2,
  APDS9250_GAIN_9X = 3,
  APDS9250_GAIN_18X = 4,
} apds9250_gain_t;


struct APDS9250_SensorSettings
{	
	apds9250_chan_t mode;
	apds9250_res_t res;
	apds9250_rate_t meas_rate;
	apds9250_gain_t gain;
	uint32_t raw_r;
	uint32_t raw_g;
	uint32_t raw_b;
	uint32_t raw_als;
	uint32_t raw_ir;

} APDS9250_settings;



/** @} */

 bool apds9250_pass(nrf_drv_twi_t twi_master);
 void apds9250_init(nrf_drv_twi_t twi_master);
 uint8_t apds9250_whoami(nrf_drv_twi_t twi_master);
 void apds9250_setup(void);
 uint8_t run_apds9250(nrf_drv_twi_t twi_master);
 uint8_t run_apds9250_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus);

 void apds9250_reset(nrf_drv_twi_t twi_master);
void apds9250_powerdown(nrf_drv_twi_t twi_master);

apds9250_chan_t apds9250_getmode(nrf_drv_twi_t twi_master);
apds9250_chan_t apds9250_setmode(nrf_drv_twi_t twi_master,apds9250_chan_t newMode);
void apds9250_setmodeals(nrf_drv_twi_t twi_master);
void apds9250_setmodergb(nrf_drv_twi_t twi_master);


void apds9250_getmeasureratereg(nrf_drv_twi_t twi_master);
apds9250_res_t apds9250_getresolution(nrf_drv_twi_t twi_master);

void apds9250_setmeasureratereg(nrf_drv_twi_t twi_master);
apds9250_res_t apds9250_setresolution(nrf_drv_twi_t twi_master, apds9250_res_t newRes);


apds9250_rate_t apds9250_getmeasrate(nrf_drv_twi_t twi_master);
apds9250_rate_t apds9250_setmeasrate(nrf_drv_twi_t twi_master, apds9250_rate_t newRate);
 

apds9250_gain_t apds9250_getgain(nrf_drv_twi_t twi_master);
apds9250_gain_t apds9250_setgain(nrf_drv_twi_t twi_master,apds9250_gain_t newGain);


uint32_t apds9250_getrawreddata(nrf_drv_twi_t twi_master);
uint32_t apds9250_getrawgreendata(nrf_drv_twi_t twi_master);
uint32_t apds9250_getrawbluedata(nrf_drv_twi_t twi_master);
uint32_t apds9250_getrawirdata(nrf_drv_twi_t twi_master);
uint32_t apds9250_getrawalsdata(nrf_drv_twi_t twi_master);


	apds9250_chan_t apds9250_modefromreg(uint8_t reg_value);
	apds9250_res_t apds9250_resfromreg(uint8_t reg_value);
    apds9250_rate_t apds9250_measratefromreg(uint8_t reg_value);
    apds9250_gain_t apds9250_gainfromreg(uint8_t reg_value);

  uint8_t apds9250_modetoreg(apds9250_chan_t newMode);
  uint8_t apds9250_restoreg(apds9250_res_t newRes);
  uint8_t apds9250_measratetoreg(apds9250_rate_t newMeasRate);
  uint8_t apds9250_gaintoreg(apds9250_gain_t newGain);

#ifdef __cplusplus
}
#endif

#endif // Vl53l0_H
