#ifndef LIS2DE_H
#define LIS2DE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

//0x18 for Super Sensor Accel. 0x19 for Nrf52 onboard Accel.
#define Lis2de_DEVICE_ADDRESS    0x18
#define Lis2de_AUX               0x07
#define Lis2de_TEMP_L            0x0C
#define Lis2de_TEMP_H            0x0D
#define Lis2de_WHO_AM_I          0x0F
#define Lis2de_TEMPCFG           0x1F
#define Lis2de_CTRL1             0x20
#define Lis2de_CTRL2	         0x21
#define Lis2de_CTRL3             0x22
#define Lis2de_CTRL4             0x23
#define Lis2de_CTRL5             0x24
#define Lis2de_CTRL6             0x25
#define Lis2de_REFERENCE         0x26
#define Lis2de_STATUS            0x27
#define Lis2de_OUT_X             0x29
#define Lis2de_OUT_Y             0x2B
#define Lis2de_OUT_Z             0x2D
#define Lis2de_FIFO_CTRL         0x2E
#define Lis2de_FIFO_SRC          0x2F
#define Lis2de_INT1CFG           0x30
#define Lis2de_INT1SRC           0x31
#define Lis2de_INT1THS           0x32
#define Lis2de_INT1DUR           0x33
#define Lis2de_INT2CFG           0x34
#define Lis2de_INT2SRC           0x35
#define Lis2de_INT2THS           0x36
#define Lis2de_INT2DUR           0x37
#define Lis2de_CLICKCFG          0x38
#define Lis2de_CLICKSRC          0x39
#define Lis2de_CLICKTHS          0x3A
#define Lis2de_TIMELIMIT         0x3B
#define Lis2de_TIMELATENCY       0x3C
#define Lis2de_TIMEWINDOW        0x3D
#define Lis2de_ACTTHS            0x3E
#define Lis2de_ACTDUR            0x3F

/** @} */

static uint8_t run_lis2de(nrf_drv_twi_t twi_master);
static uint8_t lis2de_init(nrf_drv_twi_t twi_master);



#ifdef __cplusplus
}
#endif

#endif // LIS3MDL_H