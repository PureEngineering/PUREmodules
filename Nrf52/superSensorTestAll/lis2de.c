#ifndef LIS2DE_H
#define LIS2DE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define Lis2de_DEVICE_ADDRESS    0x19
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


static void run_lis2de(nrf_drv_twi_t twi_master){
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

}

static void lis2de_init(nrf_drv_twi_t twi_master){
    uint8_t CTRL1_Word =   0x17;
    uint8_t TEMPCFG_Word = 0xC0; 
    uint8_t CTRL4_Word =   0x00;


    write_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_CTRL1,CTRL1_Word);
    write_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_TEMPCFG,TEMPCFG_Word);
    write_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_CTRL4,CTRL4_Word);

    uint8_t who_am_i = read_byte(twi_master,Lis2de_DEVICE_ADDRESS,Lis2de_WHO_AM_I);
    NRF_LOG_RAW_INFO("Accelerometer Initialized: %x.\r\n", who_am_i);
}

#ifdef __cplusplus
}
#endif

#endif // LIS3MDL_H