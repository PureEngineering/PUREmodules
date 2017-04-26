#ifndef P1234701CT_H
#define P1234701CT_H  

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


#define P12347_DEVICE_ADDRESS 		0x15 //0x54 //0x2A

#define P12347_RGB_CONTROL 		0x00
#define P12347_MANUAL_TIMING_HIGH 	0x01
#define P12347_MANUAL_TIMING_LOW 	0x02
#define P12347_RED_CHANNEL_DATA_HIGH 	0x03
#define P12347_RED_CHANNEL_DATA_LOW 	0x04


#define P12347_PROX_SENSOR_CONTROL_1 	0x0B
#define P12347_PROX_SENSOR_CONTROL_2 	0x0C
#define P12347_PROX_SENSOR_THRESHOLD 	0x0D
#define P12347_3_COLOR_DRIVE_CONTROL_1 	0x0E
#define P12347_3_COLOR_DRIVE_CONTROL_2 	0x0F


/** @} */

 bool p1234701ct_pass(nrf_drv_twi_t twi_master);
 uint8_t run_p1234701ct(nrf_drv_twi_t twi_master);
 void p1234701ct_init(nrf_drv_twi_t twi_master);
 uint8_t run_p1234701ct_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus);
 uint8_t p1234701ct_whoami(nrf_drv_twi_t twi_master);


#ifdef __cplusplus
}
#endif

#endif // Vl53l0_H
