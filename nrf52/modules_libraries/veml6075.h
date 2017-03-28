#ifndef VEML6075_H
#define VEML6075_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"

#define Veml6075_DEVICE_ADDRESS  0x10
#define Veml6075_UV_CONF         0x00
#define Veml6075_UVA_DATA        0x07
#define Veml6075_UVB_DATA        0x09
#define Veml6075_UVCOMP1_DATA    0X0A
#define Veml6075_UVCOMP2_DATA    0X0B
#define Veml6075_ID              0X0C

struct Veml6075_SensorSettings
{    
  //Main Interface and mode settings
    uint8_t UV_IT;
    uint8_t HD;
    uint8_t UV_TRIG;
    uint8_t UV_AF;
    uint8_t Veml6075_SD;
} Veml6075_settings;



 uint16_t veml6075_read_2bytes(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress);
 void veml6075_begin(nrf_drv_twi_t twi_master);
 uint16_t veml6075_init(nrf_drv_twi_t twi_master);
 uint16_t run_veml6075(nrf_drv_twi_t twi_master);
 uint8_t run_veml6075_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus);


 uint16_t veml6075_whoami(nrf_drv_twi_t twi_master);
 uint16_t veml6075_readUVA(nrf_drv_twi_t twi_master);
 uint16_t veml6075_readUVB(nrf_drv_twi_t twi_master);


#ifdef __cplusplus
}
#endif

#endif // VEML6075_H
