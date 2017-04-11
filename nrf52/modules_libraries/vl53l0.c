#include "vl53l0.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"


bool vl53l0_pass(nrf_drv_twi_t twi_master){

	uint8_t data;

	data = read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF1);
	if(data==0xee)
	{
		NRF_LOG_RAW_INFO("Vl53l0 PASS %x == 0xee \r\n", data);
		return true;
	}
	else
	{
		NRF_LOG_RAW_INFO("Vl53l0 FAIL %x != 0xee \r\n", data);
		return false;
	}
}


void run_vl53l0(nrf_drv_twi_t twi_master)
{
	uint8_t data;

	data = read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF1);
	NRF_LOG_RAW_INFO("Vl53l0_REF1: %x (0xee) \r\n", data);
	data = read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF2);
	//NRF_LOG_RAW_INFO("Vl53l0_REF2: %x (0xAA)\r\n", data);
	data = read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF3);
	//NRF_LOG_RAW_INFO("Vl53l0_REF3: %x (0x10)\r\n", data);
	data = read_2bytes(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF4);
	//NRF_LOG_RAW_INFO("Vl53l0_REF4: %x (0x0099)\r\n", data);
	data = read_2bytes(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF5);
	//NRF_LOG_RAW_INFO("Vl53l0_REF5: %x (0x0000)\r\n", data);


}

void vl53l0_init(nrf_drv_twi_t twi_master)
{
	//NRF_LOG_WARNING("Time of Flight Not Implemented \r\n");
}
