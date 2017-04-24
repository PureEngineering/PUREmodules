#include "apds9250.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"



bool apds9250_pass(nrf_drv_twi_t twi_master){


	uint8_t who_am_i = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_PART_ID);
	
	if(who_am_i==0XB2)
	{
		NRF_LOG_RAW_INFO("Apds9250: Pass %x == 0xB2 \r\n", who_am_i);
		printf("Apds9250: Pass %x == 0xB2 \r\n", who_am_i);

		return true;
	}
	else
	{
		NRF_LOG_RAW_INFO("Apds9250 FAIL %x != 0xB2 \r\n", who_am_i);
		printf("Apds9250: Fail %x != 0xB2 \r\n", who_am_i);
		return false;
	}
}


void run_apds9250(nrf_drv_twi_t twi_master)
{
	uint8_t who_am_i = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_PART_ID);
	NRF_LOG_RAW_INFO("APDS9250_PART_ID: %x (0XB2) \r\n", who_am_i);
}

uint8_t run_apds9250_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
	uint8_t who_am_i= read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_PART_ID);
	
	NRF_LOG_RAW_INFO("APDS9250_PART_ID: %x (0XB2) \r\n", who_am_i);
	return who_am_i;

}


void apds9250_setup(void){

	APDS9250_settings.mode = APDS9250_CHAN_ALS;
	APDS9250_settings.res  = APDS9250_RES_18BIT;
	APDS9250_settings.meas_rate = APDS9250_RATE_100MS;
	APDS9250_settings.gain = APDS9250_GAIN_3X;
	APDS9250_settings.raw_r = 0;
	APDS9250_settings.raw_g = 0;
	APDS9250_settings.raw_b = 0;
	APDS9250_settings.raw_als = 0;
	APDS9250_settings.raw_ir = 0;
}


void apds9250_init(nrf_drv_twi_t twi_master)
{
	write_byte(twi_master, APDS9250_DEVICE_ADDRESS, APDS9250_REG_MAIN_CTRL, APDS9250_CTRL_LS_EN);
}


void apds9250_reset(nrf_drv_twi_t twi_master){
	 write_byte(twi_master, APDS9250_DEVICE_ADDRESS,APDS9250_REG_MAIN_CTRL, APDS9250_CTRL_SW_RESET);
}


apds9250_chan_t apds9250_getmode(nrf_drv_twi_t twi_master){

	uint8_t data = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_MAIN_CTRL);
	return apds9250_modefromreg(data);
}


apds9250_chan_t apds9250_setmode(nrf_drv_twi_t twi_master,apds9250_chan_t newMode) {

	uint8_t temp = APDS9250_CTRL_LS_EN;

	temp |= apds9250_modetoreg(newMode);

	write_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_MAIN_CTRL,temp);

	return apds9250_getmode(twi_master);

}


void apds9250_setmodeals(nrf_drv_twi_t twi_master) {
  apds9250_setmode(twi_master, APDS9250_CHAN_ALS);
}

void apds9250_setmodergb(nrf_drv_twi_t twi_master) {
  apds9250_setmode(twi_master, APDS9250_CHAN_RGB);
}


apds9250_res_t apds9250_getresolution(nrf_drv_twi_t twi_master){
	apds9250_getmeasureratereg(twi_master);

	return APDS9250_settings.res;

}

/*
 * Get LS_MEAS_RATE register values.
 */
void apds9250_getmeasureratereg(nrf_drv_twi_t twi_master){
	uint8_t data = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_MEAS_RATE);
	
	APDS9250_settings.res = apds9250_resfromreg(data);

	APDS9250_settings.meas_rate = apds9250_measratefromreg(data);
}



/*
 * Convert register data to enum type for mode.
 */
apds9250_chan_t apds9250_modefromreg(uint8_t reg_value) {
  switch (reg_value & APDS9250_CTRL_CS_MASK) {
    case APDS9250_CTRL_CS_MODE_RGB:
      return APDS9250_CHAN_RGB;
    case APDS9250_CTRL_CS_MODE_ALS:
      return APDS9250_CHAN_ALS;
    default:
      return APDS9250_CHAN_ALS;
  }
}


/*
 * Convert register data to enum type for resolution.
 */
apds9250_res_t apds9250_resfromreg(uint8_t reg_value) {
  switch (reg_value & APDS9250_RESOLUTION_MASK) {
    case APDS9250_RESOLUTION_20BIT:
      return APDS9250_RES_20BIT;
    case APDS9250_RESOLUTION_19BIT:
      return APDS9250_RES_19BIT;
    case APDS9250_RESOLUTION_18BIT:
      return APDS9250_RES_18BIT;
    case APDS9250_RESOLUTION_17BIT:
      return APDS9250_RES_17BIT;
    case APDS9250_RESOLUTION_16BIT:
      return APDS9250_RES_16BIT;
    case APDS9250_RESOLUTION_13BIT:
      return APDS9250_RES_13BIT;
    default:
      return APDS9250_RES_18BIT;
  }
}

/*
 * Convert register data to enum type for measurement rate.
 */
apds9250_rate_t apds9250_measratefromreg(uint8_t reg_value) {
  switch (reg_value & APDS9250_MEAS_RATE_MASK) {
    case APDS9250_MEAS_RATE_25MS:
      return APDS9250_RATE_25MS;
    case APDS9250_MEAS_RATE_50MS:
      return APDS9250_RATE_50MS;
    case APDS9250_MEAS_RATE_100MS:
      return APDS9250_RATE_100MS;
    case APDS9250_MEAS_RATE_200MS:
      return APDS9250_RATE_200MS;
    case APDS9250_MEAS_RATE_500MS:
      return APDS9250_RATE_500MS;
    case APDS9250_MEAS_RATE_1000MS:
      return APDS9250_RATE_1000MS;
    case APDS9250_MEAS_RATE_2000MS:
      return APDS9250_RATE_2000MS;
    case APDS9250_MEAS_RATE_DUP:
      return APDS9250_RATE_2000MS;
    default:
      return APDS9250_RATE_100MS;
  }
}

/*
 * Convert register data to enum type for gain.
 */
apds9250_gain_t apds9250_gainfromreg(uint8_t reg_value) {
  switch (reg_value & APDS9250_LS_GAIN_MASK) {
    case APDS9250_LS_GAIN_1X:
      return APDS9250_GAIN_1X;
    case APDS9250_LS_GAIN_3X:
      return APDS9250_GAIN_3X;
    case APDS9250_LS_GAIN_6X:
      return APDS9250_GAIN_6X;
    case APDS9250_LS_GAIN_9X:
      return APDS9250_GAIN_9X;
    case APDS9250_LS_GAIN_18X:
      return APDS9250_GAIN_18X;
    default:
      return APDS9250_GAIN_3X;
  }
}

uint8_t apds9250_restoreg(apds9250_res_t newRes) {
  switch (newRes) {
    case APDS9250_RES_20BIT:
      return APDS9250_RESOLUTION_20BIT;
    case APDS9250_RES_19BIT:
      return APDS9250_RESOLUTION_19BIT;
    case APDS9250_RES_18BIT:
      return APDS9250_RESOLUTION_18BIT;
    case APDS9250_RES_17BIT:
      return APDS9250_RESOLUTION_17BIT;
    case APDS9250_RES_16BIT:
      return APDS9250_RESOLUTION_16BIT;
    case APDS9250_RES_13BIT:
      return APDS9250_RESOLUTION_13BIT;
    default:
      return APDS9250_RESOLUTION_18BIT;
  }
}

uint8_t apds9250_measratetoreg(apds9250_rate_t newMeasRate) {
  switch (newMeasRate) {
    case APDS9250_RATE_25MS:
      return APDS9250_MEAS_RATE_25MS;
    case APDS9250_RATE_50MS:
      return APDS9250_MEAS_RATE_50MS;
    case APDS9250_RATE_100MS:
      return APDS9250_MEAS_RATE_100MS;
    case APDS9250_RATE_200MS:
      return APDS9250_MEAS_RATE_200MS;
    case APDS9250_RATE_500MS:
      return APDS9250_MEAS_RATE_500MS;
    case APDS9250_RATE_1000MS:
      return APDS9250_MEAS_RATE_1000MS;
    case APDS9250_RATE_2000MS:
      return APDS9250_MEAS_RATE_2000MS;
    default:
      return APDS9250_MEAS_RATE_100MS;
  }
}

uint8_t apds9250_modetoreg(apds9250_chan_t newMode) {
  switch (newMode) {
    case APDS9250_CHAN_ALS:
      return APDS9250_CTRL_CS_MODE_ALS;
    case APDS9250_CHAN_RGB:
      return APDS9250_CTRL_CS_MODE_RGB;
    default:
      return APDS9250_CTRL_CS_MODE_ALS;
  }
}

uint8_t apds9250_gaintoreg(apds9250_gain_t newGain) {
  switch (newGain) {
    case APDS9250_GAIN_1X:
      return APDS9250_LS_GAIN_1X;
    case APDS9250_GAIN_3X:
      return APDS9250_LS_GAIN_3X;
    case APDS9250_GAIN_6X:
      return APDS9250_LS_GAIN_6X;
    case APDS9250_GAIN_9X:
      return APDS9250_LS_GAIN_9X;
    case APDS9250_GAIN_18X:
      return APDS9250_LS_GAIN_18X;
    default:
      return APDS9250_LS_GAIN_3X;
  }
}

