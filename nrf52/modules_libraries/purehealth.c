#include "supersensor.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include "nrf_delay.h"

#include "i2c_driver.h"
#include "lis2de.h"
#include "si1153.h"
#include "veml6075.h"
#include "bme280.h"
#include "tmp007.h"
#include "ads1114.h"
#include "fdc2214.h"


void purehealth_init(nrf_drv_twi_t twi_master){
	NRF_LOG_RAW_INFO("test_purehealth_init starting \r\n");
	
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

	NRF_LOG_RAW_INFO("tmp007_init starting \r\n");
	NRF_LOG_FLUSH();
	tmp007_begin(twi_master); 

	NRF_LOG_RAW_INFO("ads1114_init starting \r\n");
	NRF_LOG_FLUSH();
	ads1114_begin(); 

	NRF_LOG_RAW_INFO("fdc2214_init starting \r\n");
	NRF_LOG_FLUSH();
	fdc2214_init(twi_master);

}

void purehealth_powerdown(nrf_drv_twi_t twi_master){
	lis2de_powerdown(twi_master);
	bme280_powerdown(twi_master);
	veml6075_powerdown(twi_master);
	//si1153 powerdown 
	fdc2214_reset(twi_master);
	//tmp007 powerdown
	//ads1114 powerdown
}

bool test_individual_sensors(nrf_drv_twi_t twi_master){

	bool pass = true;

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

	NRF_LOG_RAW_INFO("tmp007_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!tmp007_pass(twi_master)){
		pass = false;
	}

	//Find how to test the ADS 
	//NRF_LOG_RAW_INFO("ads1114_pass starting \r\n");
	//NRF_LOG_FLUSH();
	//if(!ads1114_pass(twi_master)){
	//	pass = false;
	//}	

	NRF_LOG_RAW_INFO("fdc2214_pass starting \r\n");
	NRF_LOG_FLUSH();
	if(!fdc2214_pass(twi_master)){
		pass = false;
	}

	return pass;
}




void test_purehealth_run(nrf_drv_twi_t twi_master)
{
    
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

    //placeholder for tmp
    //uint8_t tmp007_ID = 
    //run_tmp007(twi_master); 
    //NRF_LOG_FLUSH();

    //placeholder for ads1114
    //uint8_t ads1114_ID = 
    //run_ads1114(twi_master); 
    //NRF_LOG_FLUSH();    

    //placeholder for fdc2214
    //uint8_t fdc2214_ID = 
    //run_fdc2214(twi_master); 
    //NRF_LOG_FLUSH();
}
