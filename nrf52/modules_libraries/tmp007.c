#include "tmp007.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include <math.h>

#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"


uint8_t tmp007_whoami(nrf_drv_twi_t twi_master){
  return read_2bytes(twi_master,TMP007_DEVICE_ADDR,TMP007_DEVID);
}


uint8_t tmp007_begin(nrf_drv_twi_t twi_master) {

  uint16_t samplerate = TMP007_CFG_16SAMPLE;

  uint16_t config_word = TMP007_CFG_MODEON | TMP007_CFG_ALERTEN | 
    TMP007_CFG_TRANSC | samplerate;
  uint8_t config_word_lsb = config_word & 0xFF;
  uint8_t config_word_msb = config_word>>8;
  write_2bytes(twi_master,TMP007_DEVICE_ADDR,TMP007_CONFIG,config_word_lsb,config_word_msb);
 



  uint16_t statmask_word = TMP007_STAT_ALERTEN |TMP007_STAT_CRTEN;
  uint8_t statmask_word_lsb = statmask_word & 0xFF;
  uint8_t statmask_word_msb = statmask_word>>8;  


  write_2bytes(twi_master,TMP007_DEVICE_ADDR,TMP007_STATMASK,statmask_word_lsb,statmask_word_msb);

  // enable conversion ready alert

  return tmp007_whoami(twi_master);
}




bool tmp007_pass(nrf_drv_twi_t twi_master){
  uint8_t who_am_i = tmp007_whoami(twi_master);
  if(who_am_i==0x78){
      NRF_LOG_RAW_INFO("\nTMP007: Pass %x == 0x78 \r\n", who_am_i);
      //printf("BME280: Pass %x == 0x60 \r\n", who_am_i);
      return true;
  }
  else{
      NRF_LOG_RAW_INFO("\nTMP007: Fail %x != 0x78 \r\n", who_am_i);
      //printf("BME280: Fail %x != 0x60 \r\n", who_am_i);
      return false;
  }
}


//////////////////////////////////////////////////////

double tmp007_readDieTempC(nrf_drv_twi_t twi_master) {
  double tdie = tmp007_readRawDieTemperature(twi_master);
  tdie *= 0.03125; // convert to celsius
  if(TMP007_DEBUG){
    NRF_LOG_RAW_INFO("\nTdie =  " NRF_LOG_FLOAT_MARKER "C \r\n", NRF_LOG_FLOAT(tdie));
  }
  return tdie;
}

double tmp007_readObjTempC(nrf_drv_twi_t twi_master) {
  int16_t raw = (int16_t)(read_2bytes(twi_master,TMP007_DEVICE_ADDR,TMP007_TOBJ));
  // invalid
  //if (raw & 0x1) return NAN;
  raw >>=2;

  double tobj = raw;
  tobj *= 0.03125; // convert to celsius
  if(TMP007_DEBUG){
    NRF_LOG_RAW_INFO("\nTobj =" NRF_LOG_FLOAT_MARKER "C \r\n", NRF_LOG_FLOAT(tobj));
  }
  return tobj;
}



int16_t tmp007_readRawDieTemperature(nrf_drv_twi_t twi_master) {
  int16_t raw = (int16_t)(read_2bytes(twi_master,TMP007_DEVICE_ADDR,TMP007_TDIE));

  //require NRF_LOG_MARKER inorder to print float in NRF_LOG
  if(TMP007_DEBUG){
    NRF_LOG_RAW_INFO("\nRaw Tambient: 0x%x ", raw);NRF_LOG_FLUSH();
    float v = raw/4;
    v *= 0.03125;
    NRF_LOG_RAW_INFO("(" NRF_LOG_FLOAT_MARKER "*C) \r\n",NRF_LOG_FLOAT(v)); NRF_LOG_FLUSH();
  }
  raw >>= 2;
  return raw;
}

int16_t tmp007_readRawVoltage(nrf_drv_twi_t twi_master) {

  int16_t raw = (int16_t)(read_2bytes(twi_master,TMP007_DEVICE_ADDR,TMP007_VOBJ));

  if(TMP007_DEBUG){
    NRF_LOG_RAW_INFO("\nRaw voltage: 0x%x ",raw);
    float v = raw;
    v *= 156.25;
    v /= 1000;
    NRF_LOG_RAW_INFO("\n(" NRF_LOG_FLOAT_MARKER "uV) \r\n",NRF_LOG_FLOAT(v)); NRF_LOG_FLUSH();
  }
  return raw; 
}



uint8_t run_tmp007(nrf_drv_twi_t twi_master){
  uint8_t who_am_i = tmp007_whoami(twi_master);
  NRF_LOG_RAW_INFO("\nTempSensor WhoamI: %x. \r\n",who_am_i);

  double dietemp = tmp007_readDieTempC(twi_master);
  NRF_LOG_RAW_INFO("\nTempSensor Die: " NRF_LOG_FLOAT_MARKER ".\r\n", NRF_LOG_FLOAT(dietemp));

  double objtemp = tmp007_readObjTempC(twi_master);
  NRF_LOG_RAW_INFO("\nTempSensor Die: " NRF_LOG_FLOAT_MARKER ".\r\n", NRF_LOG_FLOAT(objtemp));

  return who_am_i;

}

uint8_t run_tmp007_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
  uint8_t length = 15;
  uint8_t *ble_string[length];


  uint8_t who_am_i = tmp007_whoami(twi_master);
  sprintf((char *)ble_string, "tmp007id: %x \r\n",who_am_i);
  send_ble_data(m_nus,(uint8_t *)ble_string,length);

  double dietemp = tmp007_readDieTempC(twi_master);
  sprintf((char *)ble_string, "tmp007obj: %f \r\n",dietemp);
  send_ble_data(m_nus,(uint8_t *)ble_string,length);

  double objtemp = tmp007_readObjTempC(twi_master);
  sprintf((char *)ble_string, "tmp007die: %f \r\n",objtemp);
  send_ble_data(m_nus,(uint8_t *)ble_string,length);

  return who_am_i;
}
