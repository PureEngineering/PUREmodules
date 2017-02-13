#ifndef LIS3MDL_H
#define LIS3MDL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define Lis3mdl_DEVICE_ADDRESS           0x1E
#define Lis3mdl_WHO_AM_I                 0x0F
#define Lis3mdl_CTRL_REG1                0x20
#define Lis3mdl_CTRL_REG2	             0x21
#define Lis3mdl_CTRL_REG3                0x22
#define Lis3mdl_CTRL_REG4                0x23
#define Lis3mdl_CTRL_REG5                0x24
#define Lis3mdl_STATUS_REG               0x27
#define Lis3mdl_OUT_X_L                  0x28
#define Lis3mdl_OUT_X_H                  0x29
#define Lis3mdl_OUT_Y_L                  0x2A
#define Lis3mdl_OUT_Y_H                  0x2B
#define Lis3mdl_OUT_Z_L                  0x2C
#define Lis3mdl_OUT_Z_H                  0x2D
#define Lis3mdl_TEMP_OUT_L               0x2E
#define Lis3mdl_TEMP_OUT_H               0x2F
#define Lis3mdl_INT_CFG                  0x30
#define Lis3mdl_INT_SRC                  0x31
#define Lis3mdl_INT_THS_L                0x32
#define Lis3mdl_INT_THS_R                0x33

/** @} */    


static void run_lis3mdl(nrf_drv_twi_t twi_master){
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

}

static void lis3mdl_init(nrf_drv_twi_t twi_master){
    uint8_t CTRL1_Word = 0xF0;
    uint8_t CTRL3_Word = 0x00; 
    uint8_t CTRL4_Word = 0x0E;
    uint8_t CTRL5_Word = 0xC0;


    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG1,CTRL1_Word);
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG3,CTRL3_Word);
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG4,CTRL4_Word);
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG5,CTRL5_Word);

    int who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
    NRF_LOG_RAW_INFO("Magnetometer Initialized %x \r\n", who_am_i);
}

#ifdef __cplusplus
}
#endif

#endif // LIS3MDL_H