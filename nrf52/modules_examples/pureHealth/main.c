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
#include "ble_nus.h"


#include "i2c_driver.h"
#include "ble_driver.h"
#include "lis2de.h"
#include "si1153.h"
#include "veml6075.h"
#include "bme280.h"
#include "tmp007.h"
#include "ads1114.h"
#include "fdc2214.h"

#include "nrf_drv_timer.h"


const nrf_drv_timer_t TIMER_DATA = NRF_DRV_TIMER_INSTANCE(0);

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

/**
 * @brief Handler for timer events.
 */
void timer_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    static uint32_t i;
    uint32_t led_to_invert = ((i++) % LEDS_NUMBER);

    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            bsp_board_led_invert(led_to_invert);
            NRF_LOG_RAW_INFO("\r\nTimer Hit %d \r\n",i);
            //run_lis2de(m_twi_master);
            break;

        default:
            //Do nothing.
            NRF_LOG_RAW_INFO("\r\nTimer Hit inside %d \r\n",i);

            break;
    }
}


/**
 *  The begin of the journey
 */
int si1153_test(void)
{
	int i = 0;
	int si1153_data;
	int si1153_R;
	int si1153_IR1;
	int si1153_IR2;
	ret_code_t err_code;
	/* Initialization of UART */
	bsp_board_leds_init();

	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

	/* Initializing TWI master interface for EEPROM */
	err_code = twi_master_init();
	APP_ERROR_CHECK(err_code);
	NRF_LOG_RAW_INFO("si1153 test\n\r");
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
	param_set(m_twi_master, Si1153_ADCSENS_0, 0x01);

	param_set(m_twi_master, Si1153_ADCCONFIG_1, 0x62);
	param_set(m_twi_master, Si1153_MEASCONFIG_1, 0x02);
	param_set(m_twi_master, Si1153_ADCSENS_1, 0x01);

	param_set(m_twi_master, Si1153_ADCCONFIG_2, 0x62);
	param_set(m_twi_master, Si1153_MEASCONFIG_2, 0x04);
	param_set(m_twi_master, Si1153_ADCSENS_2, 0x01);

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

		si1153_IR1 = si1153_data = si1153_get_channel_data(m_twi_master,0);
		si1153_IR2 = si1153_get_channel_data(m_twi_master,1);
		si1153_R = si1153_get_channel_data(m_twi_master,2);
		NRF_LOG_RAW_INFO("%06d,%06d,%06d\n\r",si1153_IR1,si1153_IR2,si1153_R);
		send_command(m_twi_master,Si1153_FORCE);

		NRF_LOG_FLUSH();
	}
}

/**
 *  The begin of the journey
 */
int main(void)
{
	bool purehealth_allpass = true;

	bool passed_test = tmp007_pass(m_twi_master);
	if(passed_test){
		NRF_LOG_RAW_INFO("Tmp007 pass");
	}
	else{
		NRF_LOG_RAW_INFO("Tmp007 failed");
		purehealth_allpass = false;
	}
	
	passed_test = fdc2214_pass(m_twi_master);
	if(passed_test){
		NRF_LOG_RAW_INFO("fdc2214 pass");
	}
	else{
		NRF_LOG_RAW_INFO("fdc2214 failed");
		purehealth_allpass = false;
	}
	passed_test = bme280_pass(m_twi_master);
	if(passed_test){
		NRF_LOG_RAW_INFO("bme280 pass");
	}
	else{
		NRF_LOG_RAW_INFO("bme280 failed");
		purehealth_allpass = false;
	}
	passed_test = veml6075_pass(m_twi_master);
	if(passed_test){
		NRF_LOG_RAW_INFO("vemo6075 pass");
	}
	else{
		NRF_LOG_RAW_INFO("veml6075 failed");
		purehealth_allpass = false;
	}
	passed_test = lis2de_pass(m_twi_master);
	if(passed_test){
		NRF_LOG_RAW_INFO("lis2de pass");
	}
	else{
		NRF_LOG_RAW_INFO("lis2de failed");
		purehealth_allpass = false;
	}
	passed_test = si1153_pass(m_twi_master);
	if(passed_test){
		NRF_LOG_RAW_INFO("si1153 pass");
	}
	else{
		NRF_LOG_RAW_INFO("si1153 failed");
		purehealth_allpass = false;
	}

	if(purehealth_allpass){
		NRF_LOG_RAW_INFO("----All PureHealth Sensors Passed----");

	}
    //si1153_test();
}
