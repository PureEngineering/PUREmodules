#include "lis3mdl.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"


static uint8_t run_lis3mdl(nrf_drv_twi_t twi_master){
    uint8_t who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
    NRF_LOG_RAW_INFO("Magnetometer WhoamI: %x.\r\n", who_am_i);

    uint8_t OUT_X = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_X_L);
    NRF_LOG_RAW_INFO("Magnetometer OUT_X: %x.\r\n", OUT_X);
    uint8_t OUT_Y = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Y_L);
    NRF_LOG_RAW_INFO("Magnetometer OUT_Y: %x.\r\n", OUT_Y);
    uint8_t OUT_Z = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Z_L);
    NRF_LOG_RAW_INFO("Magnetometer OUT_Z: %x.\r\n", OUT_Z);

    uint8_t temp = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_TEMP_OUT_L);
    NRF_LOG_RAW_INFO("Magnetometer TEMP: %x.\r\n", temp);
    NRF_LOG_RAW_INFO("\r\n");

    return who_am_i;

}

static uint8_t lis3mdl_init(nrf_drv_twi_t twi_master){
    uint8_t CTRL1_Word = 0xF0;
    uint8_t CTRL3_Word = 0x00; 
    uint8_t CTRL4_Word = 0x0E;
    uint8_t CTRL5_Word = 0xC0;

    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG1,CTRL1_Word);
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG3,CTRL3_Word);
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG4,CTRL4_Word);
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG5,CTRL5_Word);

    int who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
    
    if(who_am_i==0x3d){
        NRF_LOG_RAW_INFO("Magnetometer Initialized %x \r\n", who_am_i);
    }
    else{
        NRF_LOG_RAW_INFO("Magnetometer Initialization Failed %x \r\n", who_am_i);
    }

    return who_am_i;

}
