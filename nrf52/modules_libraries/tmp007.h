#ifndef TMP007_H
#define TMP007_H


#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


// uncomment for debugging!
#define TMP007_DEBUG 1
                                     //DEFAULTS
#define TMP007_VOBJ       0x00       //0x0000 
#define TMP007_TDIE       0x01       //0x0000
#define TMP007_CONFIG     0x02       //0x1440
#define TMP007_TOBJ       0x03       //0x0000
#define TMP007_STATUS     0x04       //0x0000
#define TMP007_STATMASK   0x05       //0x0000
#define TMP007_TOBJ_HIGHLIM_REG 0x06 //0x7FC0
#define TMP007_TOBJ_LOWLIM_REG  0x07 //0X8000
#define TMP007_TDIE_HIGHLIM_REG 0X08 //0x7FC0
#define TMP007_TDIE_LOWLIM_REG  0X09 //0x8000
#define TMP007_S0               0X0A //0x260E
#define TMP007_A1               0x0B //0x0106
#define TMP007_A2               0x0C //0xFF9B
#define TMP007_B0               0x0D //0xFF3A
#define TMP007_B1               0x0E //0xFF71
#define TMP007_B2               0x0F //0x0553
#define TMP007_C2               0x10 //0x0000
#define TMP007_TC0              0x11 //0x0034
#define TMP007_TC1              0x12 //0x0000

#define TMP007_CFG_RESET    0x8000
#define TMP007_CFG_MODEON   0x1000
#define TMP007_CFG_1SAMPLE  0x0000
#define TMP007_CFG_2SAMPLE  0x0200
#define TMP007_CFG_4SAMPLE  0x0400
#define TMP007_CFG_8SAMPLE  0x0600
#define TMP007_CFG_16SAMPLE 0x0800
#define TMP007_CFG_ALERTEN  0x0100
#define TMP007_CFG_ALERTF   0x0080
#define TMP007_CFG_TRANSC   0x0040

#define TMP007_STAT_ALERTEN 0x8000
#define TMP007_STAT_CRTEN   0x4000

#define TMP007_DEVICE_ADDR 0x40
#define TMP007_MANUFACT_ID 0x1E     //0x5449
#define TMP007_DEVID 0x1F           //0x0078



  uint8_t tmp007_begin(nrf_drv_twi_t twi_master);  // by default go highres
  bool    tmp007_pass(nrf_drv_twi_t twi_master);
  uint8_t tmp007_whoami(nrf_drv_twi_t twi_master);

  int16_t tmp007_readRawDieTemperature(nrf_drv_twi_t twi_master);
  int16_t tmp007_readRawVoltage(nrf_drv_twi_t twi_master);
  double  tmp007_readObjTempC(nrf_drv_twi_t twi_master);
  double  tmp007_readDieTempC(nrf_drv_twi_t twi_master);


#endif