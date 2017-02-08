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


static uint8_t run_lis3mdl(nrf_drv_twi_t twi_master);
static uint8_t lis3mdl_init(nrf_drv_twi_t twi_master);


#ifdef __cplusplus
}
#endif

#endif // LIS3MDL_H