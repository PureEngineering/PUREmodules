#include "lsm6dsm.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include "i2c_driver.h"
#include "ble_nus.h"
#include "nrf_drv_twi.h"


uint8_t defaut_lsm6ds3_address = LSM6DS3_DEVICE_ADDRESS;

uint8_t lsm6ds3_whoami(nrf_drv_twi_t twi_master)
{
	uint8_t who_am_i = read_byte(twi_master,defaut_lsm6ds3_address,LSM6DS3_ACC_GYRO_WHO_AM_I_REG);
	return who_am_i;
}

uint8_t lsm6ds3_init(nrf_drv_twi_t twi_master)
{

	uint8_t who_am_i =  lsm6ds3_whoami(twi_master);
	if(who_am_i != 0x6a)
	{
		NRF_LOG_RAW_INFO("ERROR:lsm6ds3 who_am_i 0x%x != 0x6a \r\n", who_am_i);
	}
	else
	{
		NRF_LOG_RAW_INFO("lsm6ds3 who_am_i 0x%x == 0x6a \r\n", who_am_i);
	}

	return who_am_i;
}


