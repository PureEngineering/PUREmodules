#include "vl53l0.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"



 void run_vl53l0(nrf_drv_twi_t twi_master){
    read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF1);
    read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF2);
    read_byte(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF3);
    read_2bytes(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF4);
    read_2bytes(twi_master,Vl53l0_DEVICE_ADDRESS,Vl53l0_REF5);

}

 void vl53l0_init(nrf_drv_twi_t twi_master){

    NRF_LOG_WARNING("Time of Flight Not Implemented \r\n");
}
