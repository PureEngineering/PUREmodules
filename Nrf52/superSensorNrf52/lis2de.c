#include "lis2de.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"




static uint8_t run_lis2de(nrf_drv_twi_t twi_master){
    uint8_t who_am_i = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_WHO_AM_I);
    NRF_LOG_RAW_INFO("Accelerometer WhoamI: %x.\r\n", who_am_i);
    uint8_t status = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_STATUS);
    NRF_LOG_RAW_INFO("Accelerometer Status: %x.\r\n", status);
    
    int8_t OUT_X = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_OUT_X);
    NRF_LOG_RAW_INFO("Accelerometer OUTX: %d.\r\n", OUT_X);
    int8_t OUT_Y = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_OUT_Y);
    NRF_LOG_RAW_INFO("Accelerometer OUTY: %d.\r\n", OUT_Y);

    if(OUT_Y==1){
        bsp_board_led_invert(0);
    }
    else{
        if(OUT_Y>0){
            bsp_board_led_off(0);
        }
        else{
            bsp_board_led_on(0);
        }
    }

    int8_t OUT_Z = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_OUT_Z);
    NRF_LOG_RAW_INFO("Accelerometer OUTZ: %d.\r\n", OUT_Z);

    int8_t temp = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_TEMP_L);
    NRF_LOG_RAW_INFO("Accelerometer Temp: %x.\r\n", temp);
    NRF_LOG_RAW_INFO("\r\n");

    return who_am_i;

}

static uint8_t lis2de_init(nrf_drv_twi_t twi_master){
    uint8_t CTRL1_Word =   0x17;
    uint8_t TEMPCFG_Word = 0xC0; 
    uint8_t CTRL4_Word =   0x00;


    write_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_CTRL1,CTRL1_Word);
    write_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_TEMPCFG,TEMPCFG_Word);
    write_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_CTRL4,CTRL4_Word);

    uint8_t who_am_i = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_WHO_AM_I);
    
    if(who_am_i==0x33){
        NRF_LOG_RAW_INFO("Accelerometer Initialized %x \r\n", who_am_i);
    }
    else{
        NRF_LOG_RAW_INFO("Accelerometer Initialization Failed %x \r\n", who_am_i);
    }

    return who_am_i;
}