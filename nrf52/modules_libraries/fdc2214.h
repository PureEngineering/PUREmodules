#ifndef FDC2214_H
#define FDC2214_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


#define FDC2214_DEVICE_ADDRESS 0x2B

#define FDC2214_DATA_MSW_CH0 0x00
#define FDC2214_DATA_LSW_CH0 0x01
#define FDC2214_DATA_MSW_CH1 0x02
#define FDC2214_DATA_LSW_CH1 0x03
#define FDC2214_DATA_MSW_CH2 0x04
#define FDC2214_DATA_LSW_CH2 0x05
#define FDC2214_DATA_MSW_CH3 0x06
#define FDC2214_DATA_LSW_CH3 0x07
#define FDC2214_RCOUNT_CH0 0x08
#define FDC2214_RCOUNT_CH1 0x09
#define FDC2214_RCOUNT_CH2 0x0a
#define FDC2214_RCOUNT_CH3 0x0b
#define FDC2214_OFFSET_CH0 0x0c
#define FDC2214_OFFSET_CH1 0x0d
#define FDC2214_OFFSET_CH2 0x0e
#define FDC2214_OFFSET_CH3 0x0f
#define FDC2214_SETTLECOUNT_CH0 0x10
#define FDC2214_SETTLECOUNT_CH1 0x11
#define FDC2214_SETTLECOUNT_CH2 0x12
#define FDC2214_SETTLECOUNT_CH3 0x13
#define FDC2214_CLOCK_DIVIDERS_CH0 0x14
#define FDC2214_CLOCK_DIVIDERS_CH1 0x15
#define FDC2214_CLOCK_DIVIDERS_CH2 0x16
#define FDC2214_CLOCK_DIVIDERS_CH3 0x17
#define FDC2214_STATUS 0x18
#define FDC2214_ERROR_CONFIG 0x19
#define FDC2214_CONFIG 0x1a
#define FDC2214_MUX_CONFIG 0x1b
#define FDC2214_RESET_DEV 0x1c
#define FDC2214_DRIVE_CURRENT_CH0 0x1e
#define FDC2214_DRIVE_CURRENT_CH1 0x1f
#define FDC2214_DRIVE_CURRENT_CH2 0x20
#define FDC2214_DRIVE_CURRENT_CH3 0x21
#define FDC2214_MANUFACTURER_ID 0x7E
#define FDC2214_DEVICE_ID 0x7F
#define FDC2214_FIN 0xff


uint16_t fdc2214_init(nrf_drv_twi_t twi_master);
void fdc2214_reset(nrf_drv_twi_t twi_master);
uint32_t fdc2214_readchannel(nrf_drv_twi_t twi_master, uint8_t channelNo);
uint16_t fdc2214_whoami(nrf_drv_twi_t twi_master);

bool fdc2214_pass(nrf_drv_twi_t twi_master);



#ifdef __cplusplus
}
#endif

#endif // ADS1114

