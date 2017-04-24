#include "p1234701ct.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"


void read_sensor_data(nrf_drv_twi_t twi_master,unsigned short *red,unsigned short green,unsigned short blue){
	ret_code_t ret;
	uint8_t buffer[6]; 
	buffer[0] = P12347_RED_CHANNEL_DATA_HIGH;
	ret = nrf_drv_twi_tx(&twi_master, P12347_DEVICE_ADDRESS, buffer, 1, false);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when Writing\r\n");
		return;
	}     

	ret = nrf_drv_twi_rx(&twi_master, P12347_DEVICE_ADDRESS, buffer, 6);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when Writing\r\n");
		return;
	}  

	red = (unsigned short) (buffer[0]<<8 | buffer[1]);
	green =  (unsigned short)(buffer[2]<<8 | buffer[3]);
	blue = (unsigned short) (buffer[4]<<8 | buffer[5]);
}


bool p1234701ct_pass(nrf_drv_twi_t twi_master){

	uint8_t default_reg = read_byte(twi_master,P12347_DEVICE_ADDRESS,P12347_PROX_SENSOR_CONTROL_1);
	
	if(default_reg==0xd4)
	{
		NRF_LOG_RAW_INFO("p1234701ct PASS %x \r\n", default_reg);
		return true;
	}
	else
	{
		NRF_LOG_RAW_INFO("p1234701ct FAIL %x \r\n", default_reg);
		return false;
	}
}


void run_p1234701ct(nrf_drv_twi_t twi_master)
{

	unsigned short red;
	unsigned short green;
	unsigned short blue;

	read_sensor_data(twi_master, &red,&green, &blue );
	NRF_LOG_RAW_INFO("RED %d GREEN %d BLUE %d \r\n", red, green, blue);
}

void p1234701ct_init(nrf_drv_twi_t twi_master)
{
	write_byte(twi_master,P12347_DEVICE_ADDRESS,P12347_RGB_CONTROL,0x84);
	write_byte(twi_master,P12347_DEVICE_ADDRESS,P12347_RGB_CONTROL,0x04);
}


uint8_t run_p1234701ct_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus)
{

	return p1234701ct_pass(twi_master);
}
