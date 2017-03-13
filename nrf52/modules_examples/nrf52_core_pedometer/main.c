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
#include "supersensor.h"


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



uint8_t ORIENTATION_X = 0;
uint8_t ORIENTATION_Y = 1;
uint8_t ORIENTATION_Z = 2;

bool did_sign_change(int value, int prev_value){
    if (value >= 0){
        return prev_value < 0;
    }
    else{
        return prev_value >= 0;
    }
}


uint8_t check_orientation(int x_cycle_count, int y_cycle_count, int z_cycle_count){
    if(x_cycle_count>=y_cycle_count){
        if(x_cycle_count>=z_cycle_count){
            return ORIENTATION_X;
        }
        else{
            return ORIENTATION_Z;
        }
    }
    else{
        if(y_cycle_count>=z_cycle_count){
            return ORIENTATION_Y;
        }
        else{
            return ORIENTATION_Z;
        }
    }
}

int step_incrementer(int step_count, uint8_t current_orientation, bool x_changed, bool y_changed,bool z_changed){

    if(current_orientation==ORIENTATION_X){
        if(x_changed){
            bsp_board_led_invert(0);
            return step_count + 1;
        }
        else{
            return step_count;
        }
    }
    else if(current_orientation==ORIENTATION_Y){
        if(y_changed){
            bsp_board_led_invert(0);
            return step_count + 1;
        }
        else{
            return step_count;
        }
    }
    else{
        if(z_changed){
            bsp_board_led_invert(0);
            return step_count + 1;
        }
        else{
            return step_count;
        }
    }
}



void lis2de_stepcounter(){
    int x_cycle_count = 0;
    int y_cycle_count = 0;
    int z_cycle_count = 0;

    int8_t prev_outx = 0;
    int8_t prev_outy = 0;
    int8_t prev_outz = 0;

    bool x_changed = 0; 
    bool y_changed = 0;
    bool z_changed = 0;

    uint8_t current_orientation = 0;

    int step_count = 0;

    while(1){


    int8_t outx =  lis2de_readOUT_X(m_twi_master);
    int8_t outy =  lis2de_readOUT_Y(m_twi_master);
    int8_t outz =  lis2de_readOUT_Z(m_twi_master);

    current_orientation = check_orientation(x_cycle_count,y_cycle_count,z_cycle_count);

    if(did_sign_change(outx, prev_outx)){
        x_changed = 1;
        x_cycle_count++;
    }
    else{
        x_changed = 0;
    }
    if(did_sign_change(outy, prev_outy)){
        y_changed = 1;
        y_cycle_count++;
    }
    else{
        y_changed = 0;
    }
    if(did_sign_change(outz, prev_outz)){
        z_changed = 1;
        z_cycle_count++;
    }
    else{
        z_changed = 0;
    }

    step_count = step_incrementer(step_count, current_orientation, x_changed, y_changed, z_changed);



    prev_outx = outx;
    prev_outy = outy;
    prev_outz = outz;

    //NRF_LOG_RAW_INFO("-----------------------");
    NRF_LOG_RAW_INFO("STEP Count : %x.\r\n\r\n", step_count);
    //NRF_LOG_RAW_INFO("Orientation: %x.\r\n", current_orientation);
    //NRF_LOG_RAW_INFO("X cycle    : %x.\r\n", x_cycle_count);
    //NRF_LOG_RAW_INFO("Y cycle    : %x.\r\n", y_cycle_count);
    //NRF_LOG_RAW_INFO("Z cycle    : %x.\r\n", z_cycle_count);

    nrf_delay_ms(100);
    NRF_LOG_FLUSH();
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
    lis2de_init(m_twi_master);

    NRF_LOG_FLUSH();
    while (1)
    {
        lis2de_stepcounter();
        NRF_LOG_FLUSH();
    }
}
