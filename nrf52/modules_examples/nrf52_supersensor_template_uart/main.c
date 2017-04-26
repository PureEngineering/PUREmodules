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
#include "lis3mdl.h"
#include "lis2de.h"
#include "vl53l0.h"
#include "si1153.h"
#include "veml6075.h"
#include "bme280.h"
#include "apds9250.h"
#include "supersensor.h"
#include "p1234701ct.h"
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
int main(void)
{

    ret_code_t err_code;
    /* Initialization of UART */
    bsp_board_leds_init();

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

    /* Initializing TWI master interface for EEPROM */
    err_code = twi_master_init();
    APP_ERROR_CHECK(err_code);


    /* Welcome message */
    NRF_LOG_RAW_INFO("\r\nStarted Super Sensor\r\n");
    NRF_LOG_FLUSH();

    test_supersensor(m_twi_master); 
/*
//  Timer Code if needed

    uint32_t time_ms = 5000; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_DATA, &timer_cfg, timer_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_DATA, time_ms);

    nrf_drv_timer_extended_compare(
         &TIMER_DATA, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER_DATA);*/

    NRF_LOG_FLUSH();

    while (1){
    
        //__WFI();
        uint8_t c = NRF_LOG_GETCHAR();
        switch ((char)c)
        {
        case '\n':
        case '\r':
            NRF_LOG_RAW_INFO("You select a command\r\n"); 
            break;
        
        case LIS2DE_ON_MESSAGE:
            lis2de_init(m_twi_master);
            run_lis2de(m_twi_master);
            break;

        case LIS2DE_OFF_MESSAGE:
            lis2de_powerdown(m_twi_master);
            break;

        case LIS3MDL_ON_MESSAGE:
            lis3mdl_init(m_twi_master);
            run_lis3mdl(m_twi_master);    
            break;

        case LIS3MDL_OFF_MESSAGE:
            lis3mdl_powerdown(m_twi_master);
            break;

        case BME280_ON_MESSAGE:
            bme280_init(m_twi_master);
            run_bme280(m_twi_master);
            break;

        case BME280_OFF_MESSAGE:
            bme280_powerdown(m_twi_master);
            break;

        case VEML6075_ON_MESSAGE:
            veml6075_init(m_twi_master);
            run_veml6075(m_twi_master);
            break;

        case VEML6075_OFF_MESSAGE:
            veml6075_powerdown(m_twi_master);
            break;

        case SI1153_ON_MESSAGE:
            si1153_init(m_twi_master);
            run_si1153(m_twi_master);
            break;

        case SI1153_OFF_MESSAGE:
            break;

        case APDS9250_ON_MESSAGE:
            apds9250_init(m_twi_master);
            run_apds9250(m_twi_master);
            break;

        case APDS9250_OFF_MESSAGE:
            apds9250_powerdown(m_twi_master);
            break;

        case VL53L0_ON_MESSAGE:
            vl53l0_init(m_twi_master);
            run_vl53l0(m_twi_master);
            break;

        case VL53L0_OFF_MESSAGE:
            //vl53l0_powerdown(m_twi_master);
            break;

        case P1234701CT_ON_MESSAGE:
            p1234701ct_init(m_twi_master);
            run_p1234701ct(m_twi_master);
            break;

        case P1234701CT_OFF_MESSAGE:
            p1234701ct_powerdown(m_twi_master);
            break;
	   


       case 's':
	    test_supersensor(m_twi_master); 

            break;
        }
        NRF_LOG_FLUSH();
    }
}
