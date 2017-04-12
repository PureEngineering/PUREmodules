#include "apds9250.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"


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
	uint8_t data;

	data = read_byte(twi_master,APDS9250_DEVICE_ADDRESS,APDS9250_PART_ID);
	NRF_LOG_RAW_INFO("APDS9250_PART_ID: %x (0XB2) \r\n", data);
}

void apds9250_init(nrf_drv_twi_t twi_master)
{

}
