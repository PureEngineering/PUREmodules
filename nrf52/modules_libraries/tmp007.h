#ifndef TMP007_H
#define TMP007_H


#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


// uncomment for debugging!
#define TMP007_DEBUG 1

#define TMP007_VOBJ       0x00
#define TMP007_TDIE       0x01
#define TMP007_CONFIG     0x02
#define TMP007_TOBJ       0x03
#define TMP007_STATUS     0x04
#define TMP007_STATMASK   0x05

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
#define TMP007_DEVID 0x1F



  //Adafruit_TMP007(uint8_t addr = TMP007_I2CADDR);
  //uint16_t samplerate = TMP007_CFG_16SAMPLE;

  uint8_t tmp007_begin(nrf_drv_twi_t twi_master,uint16_t samplerate);  // by default go highres
  bool tmp007_pass(nrf_drv_twi_t twi_master);
  uint8_t tmp007_whoami(nrf_drv_twi_t twi_master);

  int16_t tmp007_readRawDieTemperature(nrf_drv_twi_t twi_master);
  int16_t tmp007_readRawVoltage(nrf_drv_twi_t twi_master);
  double tmp007_readObjTempC(nrf_drv_twi_t twi_master);
  double tmp007_readDieTempC(nrf_drv_twi_t twi_master);


#endif