#include "p1234701ct.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"


void read_sensor_data(nrf_drv_twi_t twi_master,unsigned short *red,unsigned short *green,unsigned short *blue){
	ret_code_t ret;
	uint8_t buffer[6]; 
	unsigned short red_temp, blue_temp, green_temp;

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

	red_temp = (unsigned short)(buffer[0]<<8 | buffer[1]);
	red = &red_temp;
	green_temp =  (unsigned short)(buffer[2]<<8 | buffer[3]);
	green = &green_temp;
	blue_temp = (unsigned short)(buffer[4]<<8 | buffer[5]);
	blue = &blue_temp;
}

uint8_t p1234701ct_whoami(nrf_drv_twi_t twi_master){
	uint8_t default_reg = read_byte(twi_master,P12347_DEVICE_ADDRESS,P12347_PROX_SENSOR_CONTROL_1);
	return default_reg;
}

bool p1234701ct_pass(nrf_drv_twi_t twi_master){

	uint8_t who_am_i = p1234701ct_whoami(twi_master);
	
	if(who_am_i==0xd4)
	{
		NRF_LOG_RAW_INFO("p1234701ct PASS %x (0xd4) \r\n", who_am_i);
		return true;
	}
	else
	{
		NRF_LOG_RAW_INFO("p1234701ct FAIL %x (0xd4) \r\n", who_am_i);
		return false;
	}
}


uint8_t run_p1234701ct(nrf_drv_twi_t twi_master)
{

	unsigned short red = 0;
	unsigned short green = 0;
	unsigned short blue = 0;

	uint8_t who_am_i = p1234701ct_whoami(twi_master);
	NRF_LOG_RAW_INFO("p1234701ct_whoami: %x (0Xd4) \r\n", who_am_i);

	read_sensor_data(twi_master, &red,&green, &blue );
	NRF_LOG_RAW_INFO("RED %d GREEN %d BLUE %d \r\n", red, green, blue);

	return who_am_i;
}

void p1234701ct_init(nrf_drv_twi_t twi_master)
{
	write_byte(twi_master,P12347_DEVICE_ADDRESS,P12347_RGB_CONTROL,0x84);
	write_byte(twi_master,P12347_DEVICE_ADDRESS,P12347_RGB_CONTROL,0x04);
}

 void p1234701ct_powerdown(nrf_drv_twi_t twi_master){
	write_byte(twi_master,P12347_DEVICE_ADDRESS,P12347_RGB_CONTROL,0x44);
 }


uint8_t run_p1234701ct_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus)
{
	uint8_t length = 22;
	uint8_t *ble_string[length];

	unsigned short red = 0;
	unsigned short green = 0;
	unsigned short blue = 0;

	read_sensor_data(twi_master, &red, &green, &blue);


	uint8_t who_am_i = p1234701ct_whoami(twi_master);
    sprintf((char *)ble_string, "p1234701ctWHOAMI:    %x\r\n",who_am_i);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

    sprintf((char *)ble_string, "p1234701ctred:    %hu\r\n", red);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

    sprintf((char *)ble_string, "p1234701ctgreen:    %hu\r\n",green);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

    sprintf((char *)ble_string, "p1234701ctblue:    %hu\r\n",blue);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);


	return true;
}
