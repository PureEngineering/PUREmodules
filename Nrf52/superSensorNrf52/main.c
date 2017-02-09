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

#include "i2c_driver.c"
#include "lis3mdl.c"
#include "lis2de.c"
#include "vl53l0.c"
#include "si1153.c"
#include "veml6075.c"
#include "bme280.c"


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


static ss_response test_SuperSensor_init(nrf_drv_twi_t twi_master){
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

static ss_response test_SuperSensor_run(nrf_drv_twi_t twi_master){
    //uint8_t magnet_ID = 
    run_lis3mdl(twi_master);
    //uint8_t accel_ID = 
    run_lis2de(twi_master);
    //uint8_t uva_ID = 
    run_veml6075(twi_master);
    //uint8_t prox_ID = 
    run_si1153(twi_master);

    return SS_SUCCESSFUL;

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
    while (1)
    {

        uint8_t c = NRF_LOG_GETCHAR();
        switch ((char)c)
        {
        case '\n':
        case '\r':
            break;
        case 'q':
            lis3mdl_init(m_twi_master);
            break;
        case 'w':
            run_lis3mdl(m_twi_master);
            break;
        case 'e':
            lis2de_init(m_twi_master);
            run_lis2de(m_twi_master);
            break;
        case 'r':
            veml6075_init(m_twi_master);
            break;
        case 't':
            run_veml6075(m_twi_master);
            break;    
        case 'y':
            bsp_board_led_invert(0);
            break;
        case 'u':
            si1153_init(m_twi_master);
            bsp_board_led_invert(0);
            break;
        case 'i':
            run_si1153(m_twi_master);
            bsp_board_led_invert(0);
            break;
        case 'o':
            vl53l0_init(m_twi_master);
            run_vl53l0(m_twi_master);
            bsp_board_led_invert(0);
            break; 
        case 'a':
            NRF_LOG_RAW_INFO("\r\nStart Initializing all Sensors: \r\n"); 
            test_SuperSensor_init(m_twi_master); 
            break;
        case 's':
            NRF_LOG_RAW_INFO("\r\nStart Running all Sensors\r\n"); 
            test_SuperSensor_run(m_twi_master); 
            break;
        case 'd':
            BME280_init(m_twi_master); 
            break;
        case 'f':
            run_BME280(m_twi_master); 
            break;
        default:
            NRF_LOG_RAW_INFO("You selected %c. Unknown command\r\n", (char)c); 
            break;
        }
        NRF_LOG_FLUSH();
    }
}
