#include "lis3mdl.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"



static uint8_t lis3mdl_whoami(nrf_drv_twi_t twi_master){
      uint8_t who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
      return who_am_i;
}

static int8_t lis3mdl_readOUT_X_L(nrf_drv_twi_t twi_master){
      int8_t OUT_X_L = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_X_L);
      return OUT_X_L;
}
static int8_t lis3mdl_readOUT_Y_L(nrf_drv_twi_t twi_master){
      int8_t OUT_Y_L = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Y_L);
      return OUT_Y_L;
}
static int8_t lis3mdl_readOUT_Z_L(nrf_drv_twi_t twi_master){
      int8_t OUT_Z_L = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Z_L);
      return OUT_Z_L;
}

static int8_t lis3mdl_readOUT_X_H(nrf_drv_twi_t twi_master){
      int8_t OUT_X_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_X_H);
      return OUT_X_H;
}
static int8_t lis3mdl_readOUT_Y_H(nrf_drv_twi_t twi_master){
      int8_t OUT_Y_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Y_H);
      return OUT_Y_H;
}
static int8_t lis3mdl_readOUT_Z_H(nrf_drv_twi_t twi_master){
      int8_t OUT_Z_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Z_H);
      return OUT_Z_H;
}
static int8_t lis3mdl_readTEMP_L(nrf_drv_twi_t twi_master){
      int8_t OUT_Y_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_TEMP_OUT_L);
      return OUT_Y_H;
}
static int8_t lis3mdl_readTEMP_H(nrf_drv_twi_t twi_master){
      int8_t OUT_Z_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_TEMP_OUT_H);
      return OUT_Z_H;
}


static uint8_t run_lis3mdl(nrf_drv_twi_t twi_master){
    uint8_t who_am_i = lis3mdl_whoami(twi_master);
    NRF_LOG_RAW_INFO("Magnetometer WhoamI: %x.\r\n", who_am_i);

    int8_t OUT_X = lis3mdl_readOUT_X_L(twi_master);
    NRF_LOG_RAW_INFO("Magnetometer OUT_X: %x.\r\n", OUT_X);
    int8_t OUT_Y = lis3mdl_readOUT_Y_L(twi_master);
    NRF_LOG_RAW_INFO("Magnetometer OUT_Y: %x.\r\n", OUT_Y);
    int8_t OUT_Z = lis3mdl_readOUT_Z_L(twi_master);
    NRF_LOG_RAW_INFO("Magnetometer OUT_Z: %x.\r\n", OUT_Z);

    int8_t temp = lis3mdl_readTEMP_L(twi_master);
    NRF_LOG_RAW_INFO("Magnetometer TEMP: %x.\r\n", temp);
    NRF_LOG_RAW_INFO("\r\n");

    return who_am_i;

}

static uint8_t lis3mdl_init(nrf_drv_twi_t twi_master){

    Lis3mdl_begin(twi_master);

    int who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
    
    if(who_am_i==0x3d){
        NRF_LOG_RAW_INFO("Magnetometer Initialized %x \r\n", who_am_i);
    }
    else{
        NRF_LOG_RAW_INFO("Magnetometer Initialization Failed %x \r\n", who_am_i);
    }

    return who_am_i;

}

static void lis3mdl_setup(){
  //Temp Sensor
  //0 = off, 1 = on
  Lis3mdl_settings.TEMP_EN = 0;
  //X and Y Power Mode
  //0 = Low-power Mode, 1= Medium Performance Mode, 2=High Perf Mode, 3 = Ultra high Perf Mode
  Lis3mdl_settings.OM = 3;
  //0 = 0.625Hz, 1 = 1.25Hz, 2 = 2.5Hz, 3 = 5Hz
  //4 = 10Hz, 5 = 20Hz, 6 = 40Hz, 7 = 80Hz
  Lis3mdl_settings.DO = 4;

  Lis3mdl_settings.FAST_ODR = 0;
  Lis3mdl_settings.ST = 0;


  Lis3mdl_settings.FS =0;
  Lis3mdl_settings.REBOOT = 0;
  Lis3mdl_settings.SOFT_RST = 0;

  Lis3mdl_settings.LP = 0;
  Lis3mdl_settings.SIM = 0;


  //Operating Mode Selection
  // 0 = Continuous Conversion, 1 = Single Conversion, 2 or 3 = Power-down
  Lis3mdl_settings.MD = 0;


  //Z-axis Power mode
  //0 = Low-power Mode, 1= Medium Performance Mode, 2=High Perf Mode, 3 = Ultra high Perf Mode
  Lis3mdl_settings.OMZ = 3;
  //Big/Little Endian
  //0 for LSB at lower address, 1 for MSB at lower Address
  Lis3mdl_settings.BLE = 1;


  //FAST_READ allows reading the high part of DATA OUT only in order to increase reading efficiency
  //0 = Disabled, 1 = Enabled
  Lis3mdl_settings.FAST_READ = 1;
  //Block data Update
  //0 = continuous update, 1 = output not updated until MSB and LSB have been read
  Lis3mdl_settings.BDU = 1;

}

static void Lis3mdl_begin(nrf_drv_twi_t twi_master){
    
    uint8_t CTRL1_WORD = 0x00;
    CTRL1_WORD  = (Lis3mdl_settings.TEMP_EN  <<7)  & 0x80;        
    CTRL1_WORD |= (Lis3mdl_settings.OM       <<5)  & 0x60;
    CTRL1_WORD |= (Lis3mdl_settings.DO       <<2)  & 0x1C;
    CTRL1_WORD |= (Lis3mdl_settings.FAST_ODR <<1)  & 0x02;
    CTRL1_WORD |= (Lis3mdl_settings.ST          )  & 0x01;
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG1,CTRL1_WORD);


    uint8_t CTRL2_WORD = 0x00;
    CTRL2_WORD  = (Lis3mdl_settings.FS       <<5)  & 0x60;        
    CTRL2_WORD |= (Lis3mdl_settings.REBOOT   <<3)  & 0x08;
    CTRL2_WORD |= (Lis3mdl_settings.SOFT_RST <<2)  & 0x04;
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG2,CTRL2_WORD);

    uint8_t CTRL3_WORD = 0x00;
    CTRL3_WORD  = (Lis3mdl_settings.LP       <<5)  & 0x20;        
    CTRL3_WORD |= (Lis3mdl_settings.SIM      <<2)  & 0x04;
    CTRL3_WORD |= (Lis3mdl_settings.MD          )  & 0x03;
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG3,CTRL3_WORD);


    uint8_t CTRL4_WORD = 0x00;
    CTRL4_WORD  = (Lis3mdl_settings.OMZ      <<2)  & 0x0C;        
    CTRL4_WORD |= (Lis3mdl_settings.BLE      <<1)  & 0x02;
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG4,CTRL4_WORD);


    uint8_t CTRL5_WORD = 0x00;
    CTRL5_WORD  = (Lis3mdl_settings.FAST_READ  <<7)  & 0x80;        
    CTRL5_WORD |= (Lis3mdl_settings.BDU        <<6)  & 0x40;
    write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG5,CTRL5_WORD);

}