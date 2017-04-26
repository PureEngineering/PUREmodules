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
	uint8_t who_am_i = apds9250_whoami(twi_master);
	
	if(who_am_i==0XB2)
	{
		NRF_LOG_RAW_INFO("Apds9250: Pass %x == 0xB2 \r\n", who_am_i);
		//printf("Apds9250: Pass %x == 0xB2 \r\n", who_am_i);

		return true;
	}
	else
	{
		NRF_LOG_RAW_INFO("Apds9250 FAIL %x != 0xB2 \r\n", who_am_i);
		//printf("Apds9250: Fail %x != 0xB2 \r\n", who_am_i);
		return false;
	}
}


uint8_t run_apds9250(nrf_drv_twi_t twi_master)
{
	uint8_t who_am_i = apds9250_whoami(twi_master);
	NRF_LOG_RAW_INFO("APDS9250_PART_ID: %x (0XB2) \r\n", who_am_i);

	uint32_t red_data = apds9250_getrawreddata(twi_master);
	NRF_LOG_RAW_INFO("APDS9250_RAW_RED: %x \r\n", red_data);

	uint32_t green_data = apds9250_getrawgreendata(twi_master);
	NRF_LOG_RAW_INFO("APDS9250_RAW_RED: %x \r\n", green_data);

	uint32_t blue_data = apds9250_getrawbluedata(twi_master);
	NRF_LOG_RAW_INFO("APDS9250_RAW_RED: %x \r\n", blue_data);

	uint32_t als_data = apds9250_getrawalsdata(twi_master);
	NRF_LOG_RAW_INFO("APDS9250_RAW_RED: %x \r\n", als_data);

	uint32_t ir_data = apds9250_getrawirdata(twi_master);
	NRF_LOG_RAW_INFO("APDS9250_RAW_RED: %x \r\n", ir_data);

	return who_am_i;
}



uint8_t run_apds9250_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
	uint8_t length = 15;
	uint8_t *ble_string[length];

	uint8_t who_am_i = apds9250_whoami(twi_master);
    sprintf((char *)ble_string, "apds9250id:    %x\r\n",who_am_i);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t red_data = apds9250_getrawreddata(twi_master);
    sprintf((char *)ble_string, "apds9250red:    %x\r\n",red_data);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t green_data = apds9250_getrawgreendata(twi_master);
    sprintf((char *)ble_string, "apds9250green:    %x\r\n",green_data);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t blue_data = apds9250_getrawbluedata(twi_master);
    sprintf((char *)ble_string, "apds9250blue:    %x\r\n",blue_data);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t ir_data = apds9250_getrawirdata(twi_master);
    sprintf((char *)ble_string, "apds9250IR:    %x\r\n",ir_data);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t als_data = apds9250_getrawalsdata(twi_master);
    sprintf((char *)ble_string, "apds9250ALS:    %x\r\n",als_data);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

    return who_am_i;

}

uint8_t apds9250_whoami(nrf_drv_twi_t twi_master){
	uint8_t who_am_i = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_PART_ID);
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
	//write_byte(twi_master, APDS9250_DEVICE_ADDRESS, APDS9250_REG_MAIN_CTRL, APDS9250_CTRL_LS_EN);
	apds9250_setmode(twi_master,APDS9250_settings.mode);
	apds9250_setmeasureratereg(twi_master);
	apds9250_setgain(twi_master, APDS9250_settings.gain);

}


void apds9250_reset(nrf_drv_twi_t twi_master){
	 write_byte(twi_master, APDS9250_DEVICE_ADDRESS,APDS9250_REG_MAIN_CTRL, APDS9250_CTRL_SW_RESET);
}


void apds9250_powerdown(nrf_drv_twi_t twi_master){
	
	write_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_MAIN_CTRL,0x00);
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

void apds9250_setmeasureratereg(nrf_drv_twi_t twi_master){
	uint8_t temp = 0; 
	temp |= (apds9250_restoreg(APDS9250_settings.res) <<4);
	temp |= (apds9250_measratetoreg(APDS9250_settings.meas_rate));
	write_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_MEAS_RATE,temp); 
}

apds9250_res_t apds9250_setresolution(nrf_drv_twi_t twi_master, apds9250_res_t newRes){
	APDS9250_settings.res = newRes;
	apds9250_setmeasureratereg(twi_master);
	return apds9250_getresolution(twi_master);
}

/*
 * Get LS_MEAS_RATE register values.
 */
void apds9250_getmeasureratereg(nrf_drv_twi_t twi_master){
	uint8_t data = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_MEAS_RATE);
	
	APDS9250_settings.res = apds9250_resfromreg(data);

	APDS9250_settings.meas_rate = apds9250_measratefromreg(data);
}



apds9250_rate_t apds9250_getmeasrate(nrf_drv_twi_t twi_master) {
	apds9250_getmeasureratereg(twi_master);
	return APDS9250_settings.meas_rate;
}

apds9250_rate_t apds9250_setmeasrate(nrf_drv_twi_t twi_master, apds9250_rate_t newRate){
	APDS9250_settings.meas_rate = newRate;
	apds9250_setmeasureratereg(twi_master);
	return apds9250_getmeasrate(twi_master);
}

apds9250_gain_t apds9250_getgain(nrf_drv_twi_t twi_master) {
	uint8_t temp = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_GAIN);
	APDS9250_settings.gain = apds9250_gainfromreg(temp);
	return APDS9250_settings.gain;
}

apds9250_gain_t apds9250_setgain(nrf_drv_twi_t twi_master,apds9250_gain_t newGain) {
	uint8_t data = apds9250_gaintoreg(newGain);
	write_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_GAIN,data);
	return apds9250_getgain(twi_master);
}


uint32_t apds9250_getrawreddata(nrf_drv_twi_t twi_master){
	APDS9250_settings.raw_r = read_20bits(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_DATA_RED_0);
	return APDS9250_settings.raw_r;
}

uint32_t apds9250_getrawgreendata(nrf_drv_twi_t twi_master){
	APDS9250_settings.raw_g = read_20bits(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_DATA_GREEN_0);
	return APDS9250_settings.raw_g;
}

uint32_t apds9250_getrawbluedata(nrf_drv_twi_t twi_master){
	APDS9250_settings.raw_b = read_20bits(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_DATA_BLUE_0);
	return APDS9250_settings.raw_b;
}

uint32_t apds9250_getrawirdata(nrf_drv_twi_t twi_master){
	APDS9250_settings.raw_ir = read_20bits(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_DATA_IR_0);
	return APDS9250_settings.raw_ir;
}

uint32_t apds9250_getrawalsdata(nrf_drv_twi_t twi_master){
	APDS9250_settings.raw_als = read_20bits(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_REG_LS_DATA_GREEN_0);
	return APDS9250_settings.raw_als;
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

