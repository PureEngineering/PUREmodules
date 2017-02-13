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
    NRF_LOG_RAW_INFO(
            "Started Super Sensor\r\n"
    );

    NRF_LOG_FLUSH();
    lis2de_init(m_twi_master);
    /* Main loop */
    while (1)
    {

        run_lis2de(m_twi_master);
        nrf_delay_ms(100);

        /*uint8_t c = NRF_LOG_GETCHAR();
        switch ((char)c)
        {
        case '\n':
        case '\r':
            break;
        case 'q':
            lis3mdl_init(m_twi_master);
            //bsp_board_led_invert(0);
            break;
        case 'w':
            run_lis3mdl(m_twi_master);
            //bsp_board_led_invert(0);
            break;
        case 'e':
            lis2de_init(m_twi_master);
            run_lis2de(m_twi_master);
            //bsp_board_led_invert(0);
            break;
        case 'r':
            vl53l0_init(m_twi_master);
            run_vl53l0(m_twi_master);
            bsp_board_led_invert(0);
            break;         
        case 't':
            si1153_init(m_twi_master);
            bsp_board_led_invert(0);
            break;
        case 'y':
            run_si1153(m_twi_master);
            bsp_board_led_invert(0);
            break;
        case 'u':
            bsp_board_led_invert(0);
            break;
        default:
            NRF_LOG_RAW_INFO("You selected %c. Unknown command\r\n", (char)c); 
            break;
        }
        NRF_LOG_FLUSH();*/
    }
}

/** @} */ /* End of group twi_master_with_twis_slave_example */
