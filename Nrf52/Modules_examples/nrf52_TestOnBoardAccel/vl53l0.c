#ifndef Vl53l0_H
#define Vl53l0_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define Vl53l0_DEVICE_ADDRESS       0x29
#define Vl53l0_REF1                 0xC0
#define Vl53l0_REF2                 0xC1
#define Vl53l0_REF3	                0xC2
#define Vl53l0_REF4                 0x51
#define Vl53l0_REF5                 0x61


/** @} */


static void run_vl53l0(nrf_drv_twi_t twi_master){
    read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF1);
    read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF2);
    read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF3);
    read_2bytes(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF4);
    read_2bytes(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF5);

}

static void vl53l0_init(nrf_drv_twi_t twi_master){

    NRF_LOG_WARNING("Time of Flight Initialized\r\n");
}

#ifdef __cplusplus
}
#endif

#endif // Vl53l0_H