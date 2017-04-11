#ifndef APDS9250_H
#define APDS9250_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define APDS9250_DEVICE_ADDRESS		0x52

#define APDS9250_MAIN_CTRL		0x00
#define APDS9250_LS_MEAS_RATE		0x04
#define APDS9250_LS_GAIN		0x05
#define APDS9250_PART_ID		0x06


/** @} */

 bool apds9250_pass(nrf_drv_twi_t twi_master);
 void run_apds9250(nrf_drv_twi_t twi_master);
 void apds9250_init(nrf_drv_twi_t twi_master);


#ifdef __cplusplus
}
#endif

#endif // Vl53l0_H
