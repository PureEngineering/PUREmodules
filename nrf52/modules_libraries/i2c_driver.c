#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
//#include "config.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "nrf.h"
#include "bsp.h"
#include "app_util_platform.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "i2c_driver.h"


void write_byte(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress, uint8_t data){
	ret_code_t ret;
	uint8_t buffer[2]; 
	buffer[0] = subAddress;
	buffer[1] = data;
	ret = nrf_drv_twi_tx(&twi_master, addr, buffer, 2, false);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when Writing\r\n");
		return;
	}     
}

//Writes LSB first and then MSB
void write_2bytes(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress, uint8_t data_LSB, uint8_t data_MSB){
	ret_code_t ret;
	uint8_t buffer[3]; 
	buffer[0] = subAddress;
	buffer[1] = data_LSB;
	buffer[2] = data_MSB;
	ret = nrf_drv_twi_tx(&twi_master, addr, buffer, 3, false);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when Writing\r\n");
		return;
	}     
}

uint8_t read_byte(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress){
	ret_code_t ret;
	uint8_t data;
	uint8_t buffer[1];
	buffer[0] = subAddress;


	ret = nrf_drv_twi_tx(&twi_master, addr, buffer, 1, false);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when asking for byte\r\n");
		return (uint8_t)ret;
	}
	ret = nrf_drv_twi_rx(&twi_master, addr, &data, 1);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when reading byte back\r\n");
		return (uint8_t)ret;
	}
	//NRF_LOG_RAW_INFO("Read Byte: ");
	//NRF_LOG_RAW_INFO("%.2x: \r\n", data);
	return data;

}

uint16_t read_2bytes(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress){
	ret_code_t ret;
	uint8_t data_MSB;
	uint8_t data_LSB;
	uint16_t full_data;
	uint8_t buffer[1];
	buffer[0] = subAddress;

	ret = nrf_drv_twi_tx(&twi_master, addr, buffer, 1, false);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when asking to read\r\n");
		return (uint8_t)ret;
	}
	ret = nrf_drv_twi_rx(&twi_master, addr, &data_MSB, 1);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when reading first byte\r\n");
		return (uint8_t)ret;
	}
	ret = nrf_drv_twi_rx(&twi_master, addr, &data_LSB, 1);
	if (NRF_SUCCESS != ret){
		NRF_LOG_WARNING("Communication error when reading second byte\r\n");
		return (uint8_t)ret;
	}
	NRF_LOG_WARNING("Read 2 bytes: ");
	full_data = (data_MSB<<8) | data_LSB;
	NRF_LOG_RAW_INFO("%.4x: \r\n", full_data);
	return full_data;

}
