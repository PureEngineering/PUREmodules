#include "fdc2214.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"
#include "nrf_delay.h"



void fdc2214_reset(nrf_drv_twi_t twi_master){
  //Write any value to RESET_DEV register to reset it. 
  write_byte(twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_RESET_DEV, 0x01);
}


uint32_t fdc2214_readchannel(nrf_drv_twi_t twi_master, uint8_t channelNo){ 
  uint8_t MSW_REG;
  uint8_t LSW_REG;
  switch (channelNo)
  {
  case 0x00:
    MSW_REG = FDC2214_DATA_MSW_CH0;
    LSW_REG = FDC2214_DATA_LSW_CH0;
    break;
  case 0x01:
    MSW_REG = FDC2214_DATA_MSW_CH1;
    LSW_REG = FDC2214_DATA_LSW_CH1;
    break;
  case 0x02:
    MSW_REG = FDC2214_DATA_MSW_CH2;
    LSW_REG = FDC2214_DATA_LSW_CH2;
    break;
  case 0x03:
    MSW_REG = FDC2214_DATA_MSW_CH3;
    LSW_REG = FDC2214_DATA_LSW_CH3;
    break;
  default:
    NRF_LOG_RAW_INFO("Incorrect Channel Number; Returning Device ID"); 
    MSW_REG = FDC2214_DEVICE_ID;
    LSW_REG = FDC2214_DEVICE_ID;
  }

  uint16_t data_MSW = read_2bytes(twi_master,FDC2214_DEVICE_ADDRESS, MSW_REG);
  uint16_t data_LSW = read_2bytes(twi_master,FDC2214_DEVICE_ADDRESS, LSW_REG);

  uint32_t data = (data_MSW << 16 | data_LSW) &0x3fff;

  return data;

}

bool fdc2214_pass(nrf_drv_twi_t twi_master){

    uint16_t device_id = fdc2214_whoami(twi_master);
    if(device_id == 0x3055){
      return true;
    }
    else{
      return false;
    }

}

uint16_t fdc2214_whoami(nrf_drv_twi_t twi_master){
    uint16_t device_id = read_2bytes(twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DEVICE_ID);
    return device_id;
}

//fdc values are little endian encoding, write_2bytes() are big endian encoding
uint16_t fdc2214_init(nrf_drv_twi_t twi_master){

 uint8_t config_word_lsb;
  uint8_t config_word_msb;


  config_word_lsb = 0xFF;
  config_word_msb = 0xFF;

  //set RCOUNT Registers for each channel
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_RCOUNT_CH0,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_RCOUNT_CH1,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_RCOUNT_CH2,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_RCOUNT_CH3,config_word_lsb,config_word_msb);
 
  //offset
  config_word_lsb = 0x00;
  config_word_msb = 0x00;

  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_OFFSET_CH0,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_OFFSET_CH1,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_OFFSET_CH2,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_OFFSET_CH3,config_word_lsb,config_word_msb);

  //settle count
  config_word_lsb = 0x04;
  config_word_msb = 0x00;

  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_SETTLECOUNT_CH0,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_SETTLECOUNT_CH1,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_SETTLECOUNT_CH2,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_SETTLECOUNT_CH3,config_word_lsb,config_word_msb);

  //clock divider
  config_word_lsb = 0x10;
  config_word_msb = 0x01;

  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_CLOCK_DIVIDERS_CH0,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_CLOCK_DIVIDERS_CH1,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_CLOCK_DIVIDERS_CH2,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_CLOCK_DIVIDERS_CH3,config_word_lsb,config_word_msb);
  
  //set driven current, value 0x7C00
  config_word_lsb = 0x88;
  config_word_msb = 0x00; 
  
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_DRIVE_CURRENT_CH0,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_DRIVE_CURRENT_CH1,config_word_lsb,config_word_msb);
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_DRIVE_CURRENT_CH2,config_word_lsb,config_word_msb);
 
  // error config
  config_word_lsb = 0x00;
  config_word_msb = 0x01;
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_ERROR_CONFIG,config_word_lsb,config_word_msb);

  //mux config
  config_word_lsb = 0xA2;
  config_word_msb = 0x0C;
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_MUX_CONFIG,config_word_lsb,config_word_msb);
  
  //config 0x3E sleep mode, 0x1E wake mode
  config_word_lsb = 0x1E;
  config_word_msb = 0x01;
  write_2bytes(twi_master,FDC2214_DEVICE_ADDRESS,FDC2214_CONFIG,config_word_lsb,config_word_msb);
  
  return fdc2214_whoami(twi_master);
}

uint8_t run_fdc2214(nrf_drv_twi_t twi_master){
  uint8_t who_am_i = fdc2214_whoami(twi_master);
  NRF_LOG_RAW_INFO("FDC2214 WhoamI: %x. \r\n",who_am_i);

  uint8_t channel = 1;
  uint32_t data = fdc2214_readchannel(twi_master, channel); 
  NRF_LOG_RAW_INFO("FDC2214 Ch0: %d.\r\n", data);


  return who_am_i;

}

uint8_t run_fdc2214_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
  uint8_t length = 15;
  uint8_t *ble_string[length];


  uint8_t who_am_i = fdc2214_whoami(twi_master);
  sprintf((char *)ble_string, "fdc2214id: %x \r\n",who_am_i);
  send_ble_data(m_nus,(uint8_t *)ble_string,length);

  uint8_t channel = 1;
  uint8_t data = fdc2214_readchannel(twi_master, channel); 
  sprintf((char *)ble_string, "fdc2214ch0: %x \r\n",data);
  send_ble_data(m_nus,(uint8_t *)ble_string,length);

  return who_am_i;

}




