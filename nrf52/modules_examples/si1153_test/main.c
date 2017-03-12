#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "config.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "nrf.h"
#include "bsp.h"
#include "app_util_platform.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_delay.h"

#include "i2c_driver.h"
#include "lis3mdl.h"
#include "lis2de.h"
#include "vl53l0.h"
#include "si1153.h"
#include "veml6075.h"
#include "bme280.h"

/**
 * @brief TWI master instance
 *
 * Instance of TWI master driver that would be used for communication with simulated
 * eeprom memory.
 */
static const nrf_drv_twi_t m_twi_master = NRF_DRV_TWI_INSTANCE(MASTER_TWI_INST);

/**
 * @brief Initialize the master TWI
 *
 * Function used to initialize master TWI interface that would communicate with simulated EEPROM.
 *
 * @return NRF_SUCCESS or the reason of failure
 */
static ret_code_t twi_master_init(void)
{
	ret_code_t ret;
	const nrf_drv_twi_config_t config =
	{
		.scl                = TWI_SCL_M,
		.sda                = TWI_SDA_M,
		.frequency          = NRF_TWI_FREQ_400K,
		.interrupt_priority = APP_IRQ_PRIORITY_HIGH,
		.clear_bus_init     = false
	};

	ret = nrf_drv_twi_init(&m_twi_master, &config, NULL, NULL);

	if (NRF_SUCCESS == ret)
	{
		nrf_drv_twi_enable(&m_twi_master);
	}

	return ret;
}

typedef enum ss_response {
	SENSOR_SUCCESS,
	LIS3MDL_INIT_FAIL,
	LIS3MDL_DATA_FAIL,
	LIS2DE_INIT_FAIL,
	LIS2DE_DATA_FAIL,
	VEML6075_INIT_FAIL,
	VEML6075_DATA_FAIL,
	PROXIMITY_INIT_FAIL,
	PROXIMITY_DATA_FAIL,
	TOF_INIT_FAIL,
	TOF_DATA_FAIL,
	SS_SUCCESSFUL
} ss_response;


ss_response test_SuperSensor_init(nrf_drv_twi_t twi_master){
	//uint8_t magnet_ID = 
	lis3mdl_init(twi_master);
	//uint8_t accel_ID = 
	lis2de_init(twi_master);
	//uint8_t uva_ID = 
	veml6075_init(twi_master);
	//uint8_t prox_ID = 
	si1153_init(twi_master);
	//uint8_t bme280_id = 
	BME280_init(m_twi_master); 


	return SS_SUCCESSFUL;

}

ss_response test_SuperSensor_run(nrf_drv_twi_t twi_master){
	//uint8_t magnet_ID = 
	run_lis3mdl(twi_master);
	//uint8_t accel_ID = 
	run_lis2de(twi_master);
	//uint8_t uva_ID = 
	run_veml6075(twi_master);
	//uint8_t prox_ID = 
	run_si1153(twi_master);
	//uint8_t bme_ID = 
	run_BME280(m_twi_master); 

	return SS_SUCCESSFUL;

}


/**
 *  The begin of the journey
 */
int main(void)
{
	int i = 0;
	int si1153_data;
	ret_code_t err_code;
	/* Initialization of UART */
	bsp_board_leds_init();

	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

	/* Initializing TWI master interface for EEPROM */
	err_code = twi_master_init();
	APP_ERROR_CHECK(err_code);

	override_defaut_lis2de_address(0x19);  ////0x18 for Super Sensor Accel. 0x19 for Nrf52 onboard Accel.

	/* Welcome message */
	NRF_LOG_RAW_INFO("\r\nStarted Super Sensor\r\n");
	NRF_LOG_FLUSH();   
	//test_SuperSensor_init(m_twi_master); 
	NRF_LOG_FLUSH();   

	send_command(m_twi_master,Si1153_RESET_SW);
	nrf_delay_ms(10);
	//si1153_init(m_twi_master);


	param_set(m_twi_master, Si1153_CHAN_LIST, 0x07);

	param_set(m_twi_master, Si1153_LED1_A, 0x3F);
	param_set(m_twi_master, Si1153_LED2_A, 0x3F);
	param_set(m_twi_master, Si1153_LED3_A, 0x3F);

	param_set(m_twi_master, Si1153_ADCCONFIG_0, 0x62);
	param_set(m_twi_master, Si1153_MEASCONFIG_0, 0x01);

	param_set(m_twi_master, Si1153_ADCCONFIG_1, 0x62);
	param_set(m_twi_master, Si1153_MEASCONFIG_1, 0x02);

	param_set(m_twi_master, Si1153_ADCCONFIG_2, 0x62);
	param_set(m_twi_master, Si1153_MEASCONFIG_2, 0x04);

	//send_command(m_twi_master,Si1153_START);
	send_command(m_twi_master,Si1153_FORCE);

	NRF_LOG_FLUSH();   

	while (1)
	{

		NRF_LOG_FLUSH();   

		if(0==1)
		{
			si1153_data = si1153_get_data(m_twi_master);
			NRF_LOG_RAW_INFO("%06d:",si1153_data );
			for(i=0;i<((si1153_data/4)%70);i++)
			{
				NRF_LOG_RAW_INFO("-");
				NRF_LOG_FLUSH();   
			}
			NRF_LOG_RAW_INFO("*\n\r");
			NRF_LOG_FLUSH();   
		}

		//NRF_LOG_RAW_INFO("%d------------------------\r\n",i++);
		nrf_delay_ms(10);

		//run_si1153(m_twi_master);
		bsp_board_led_invert(0);


		si1153_data = si1153_get_channel_data(m_twi_master,0);
		send_command(m_twi_master,Si1153_FORCE);
		NRF_LOG_RAW_INFO("%06d ",si1153_data );

		for(i=0;i<((si1153_data/4)%70);i++)
		{
			NRF_LOG_RAW_INFO("-");
			NRF_LOG_FLUSH();   
		}
		NRF_LOG_RAW_INFO("*\n\r");
		NRF_LOG_FLUSH();   

		si1153_data = si1153_get_channel_data(m_twi_master,1);
		//NRF_LOG_RAW_INFO("%06d ",si1153_data );
		si1153_data = si1153_get_channel_data(m_twi_master,2);
		//NRF_LOG_RAW_INFO("%06d \n\r",si1153_data );



		NRF_LOG_FLUSH();
	}
}I2C_INTERRUPT_H
