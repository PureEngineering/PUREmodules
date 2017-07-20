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


uint32_t fdc2214_read_ch0(nrf_drv_twi twi_master){

  uint16_t data_MSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_MSW_CH0);
  uint16_t data_LSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_LSW_CH0);

  uint32_t data = (data_MSW << 16 | data_LSW);

  return data

}

uint32_t fdc2214_read_ch1(nrf_drv_twi twi_master){

  uint16_t data_MSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_MSW_CH1);
  uint16_t data_LSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_LSW_CH1);

  uint32_t data = (data_MSW << 16 | data_LSW);

  return data


}

uint32_t fdc2214_read_ch2(nrf_drv_twi twi_master){

  uint16_t data_MSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_MSW_CH2);
  uint16_t data_LSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_LSW_CH2);

  uint32_t data = (data_MSW << 16 | data_LSW);

  return data


}
uint32_t fdc2214_read_ch3(nrf_drv_twi twi_master){

  uint16_t data_MSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_MSW_CH3);
  uint16_t data_LSW = read_2bytes(nrf_drv_twi_t twi_master,FDC2214_DEVICE_ADDRESS, FDC2214_DATA_LSW_CH3);

  uint32_t data = (data_MSW << 16 | data_LSW);

  return data


}

