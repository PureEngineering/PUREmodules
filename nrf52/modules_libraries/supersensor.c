#include "supersensor.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"
#include "lis3mdl.h"
#include "lis2de.h"
#include "vl53l0.h"
#include "si1153.h"
#include "veml6075.h"
#include "bme280.h"
#include "apds9250.h"
#include "p1234701ct.h"
#include "nrf_delay.h"


void test_supersensor_init(nrf_drv_twi_t twi_master){
	NRF_LOG_RAW_INFO("test_supersensor_init starting \r\n");
	//uint8_t magnet_ID = 
	NRF_LOG_RAW_INFO("lis3mdl_init starting \r\n");
	NRF_LOG_FLUSH();
	lis3mdl_init(twi_master);
	//uint8_t accel_ID = 
	NRF_LOG_RAW_INFO("lis2de_init starting \r\n");
	NRF_LOG_FLUSH();
	lis2de_init(twi_master);
	//uint8_t uva_ID = 
	NRF_LOG_RAW_INFO("veml6075_init starting \r\n");
	NRF_LOG_FLUSH();
	veml6075_init(twi_master);
	//uint8_t prox_ID = 
	NRF_LOG_RAW_INFO("si1153_init starting \r\n");
	NRF_LOG_FLUSH();
	si1153_init(twi_master);
	//uint8_t bme280_id = 
	NRF_LOG_RAW_INFO("bme280_init starting \r\n");
	NRF_LOG_FLUSH();
	bme280_init(twi_master); 

	NRF_LOG_RAW_INFO("vl53l0_init starting \r\n");
	NRF_LOG_FLUSH();
	vl53l0_init(twi_master); 

	NRF_LOG_RAW_INFO("apds9250_init starting \r\n");
	NRF_LOG_FLUSH();
	apds9250_init(twi_master); 

	NRF_LOG_RAW_INFO("p1234701ct_init starting \r\n");
	NRF_LOG_FLUSH();
	p1234701ct_init(twi_master);

}

void supersensor_powerdown(nrf_drv_twi_t twi_master){
	lis2de_powerdown(twi_master);
	lis3mdl_powerdown(twi_master);
	bme280_powerdown(twi_master);
	veml6075_powerdown(twi_master);
	apds9250_powerdown(twi_master);
	p1234701ct_powerdown(twi_master);
}

bool test_individual_sensors(nrf_drv_twi_t twi_master){

	test_supersensor_init(twi_master);
	NRF_LOG_FLUSH();

	bool pass = true;

	NRF_LOG_RAW_INFO("lis3mdl_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!lis3mdl_pass(twi_master)){
		pass = false;
	}

	NRF_LOG_RAW_INFO("lis2de_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!lis2de_pass(twi_master)){
		pass = false;
	}

	NRF_LOG_RAW_INFO("veml6075_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!veml6075_pass(twi_master)){
		pass = false;
	}

	NRF_LOG_RAW_INFO("bme280_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!bme280_pass(twi_master)){
		pass = false;
	} 

	NRF_LOG_RAW_INFO("si1153_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!si1153_pass(twi_master)){
		pass = false;
	}

	NRF_LOG_RAW_INFO("vl53l0_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!vl53l0_pass(twi_master)){
		pass = false;
	}

	NRF_LOG_RAW_INFO("apds9250_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!apds9250_pass(twi_master)){
		pass = false;
	}

	NRF_LOG_RAW_INFO("p1234701ct_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!p1234701ct_pass(twi_master)){
		pass = false;
	}



	return pass;

}

void test_supersensor(nrf_drv_twi_t twi_master){

	int i;
	NRF_LOG_RAW_INFO("SuperSensor Test starting \r\n");
    NRF_LOG_FLUSH();
    bool supersensor_pass = test_individual_sensors(twi_master);

    //supersensor_powerdown(twi_master);

    if(supersensor_pass)
    {
	    NRF_LOG_RAW_INFO("SuperSensor Pass \r\n");
	    NRF_LOG_FLUSH();
	    for(i=0;i<10;i++)
	    {
		    bsp_board_led_on(0);
		    nrf_delay_ms(50);
		    bsp_board_led_off(0);
		    nrf_delay_ms(50);
	    }

    }
    else
    {
        NRF_LOG_RAW_INFO("SuperSensor Fail \r\n");
	NRF_LOG_FLUSH();
    }
}



void test_supersensor_run(nrf_drv_twi_t twi_master)
{
    
    run_apds9250(twi_master); 
    NRF_LOG_FLUSH();
    
    run_vl53l0(twi_master); 
    NRF_LOG_FLUSH();
    
    //uint8_t magnet_ID = 
    run_lis3mdl(twi_master);
    NRF_LOG_FLUSH();

    //uint8_t accel_ID = 
    run_lis2de(twi_master);
    NRF_LOG_FLUSH();

    //uint8_t uva_ID = 
    run_veml6075(twi_master);
    NRF_LOG_FLUSH();

    //uint8_t prox_ID = 
    run_si1153(twi_master);
    NRF_LOG_FLUSH();

    //uint8_t bme_ID = 
    run_bme280(twi_master); 
    NRF_LOG_FLUSH();

    //uint8_t p123_ID = 
    run_p1234701ct(twi_master); 
    NRF_LOG_FLUSH();
    

}
