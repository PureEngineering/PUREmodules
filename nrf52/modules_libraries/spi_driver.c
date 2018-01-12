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
#include "nrf_drv_spi.h"



uint8_t spiRead(nrf_drv_spi_t spi,uint8_t addr)
{
	uint8_t address = addr & 0x7F;
	uint8_t buffer[3];
	int length =2;
	buffer[0] = address;
	//spi_xfer_done = false;
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, buffer, length, buffer, length));

	//while (!spi_xfer_done)
	//{
	//	__WFE();
	//}

	return buffer[1];
}

void spiWrite(nrf_drv_spi_t spi,uint8_t addr, uint8_t data)
{
	uint8_t address = addr | 0x80;
	uint8_t buffer[3];
	buffer[0] = address;
	buffer[1] = data;
	//spi_xfer_done = false;
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, buffer, 2, buffer, 2));

	//while (!spi_xfer_done)
	//{
	//	__WFE();
	//}

	//read back check
/*	if(addr != REG_FIFO)
	{
		if(addr != REG_OP_MODE)
		{
			if(addr != REG_IRQ_FLAGS)
			{
				buffer[1] = spiRead(addr);
				if(buffer[1] != data)
				{
					DEBUG_PRINTF("spiWrite ERROR A:%x D:%x != %x\n\r",addr,buffer[1],data);
				}
			}
		}
	}*/
}