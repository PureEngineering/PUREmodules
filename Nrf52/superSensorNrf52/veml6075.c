#ifndef VEML6075_H
#define VEML6075_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define Veml6075_DEVICE_ADDRESS  0x10
#define Veml6075_UV_CONF         0x00
#define Veml6075_UVA_DATA        0x07
#define Veml6075_UVB_DATA        0x09
#define Veml6075_UVCOMP1_DATA    0X0A
#define Veml6075_UVCOMP2_DATA    0X0B
#define Veml6075_ID              0X0C

struct SensorSettings
{    
  //Main Interface and mode settings
    uint8_t UV_IT;
    uint8_t HD;
    uint8_t UV_TRIG;
    uint8_t UV_AF;
    uint8_t Veml6075_SD;
} settings;



//Different read function than the others
 static uint16_t Veml6075_read_2bytes(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress){
    ret_code_t ret;
    uint16_t full_data;
    uint8_t return_buffer[2];
    return_buffer[0] = 0;
    return_buffer[1] = 0;
    uint8_t buffer[1];
    buffer[0] = subAddress;


    //NRF_LOG_RAW_INFO("Inside read 2 bytes \r\n");
    ret = nrf_drv_twi_tx(&twi_master, addr, buffer, 1, true);
    if (NRF_SUCCESS != ret){
        NRF_LOG_WARNING("Communication error when asking to read\r\n");
        return (uint8_t)ret;
    }
    ret = nrf_drv_twi_rx(&twi_master, addr, &return_buffer, 2);
    if (NRF_SUCCESS != ret){
        NRF_LOG_WARNING("Communication error when reading first byte\r\n");
        return (uint8_t)ret;
    }
    full_data = (return_buffer[1]<<8) | return_buffer[0];
    return full_data;

}

static void begin(nrf_drv_twi_t twi_master){
    uint8_t UV_CONF_WORD = 0x00;

    UV_CONF_WORD  = (settings.UV_IT   <<4)  & 0x70;        
    UV_CONF_WORD |= (settings.HD      <<3)  & 0x08;
    UV_CONF_WORD |= (settings.UV_TRIG <<2)  & 0x04;
    UV_CONF_WORD |= (settings.UV_AF   <<1)  & 0x02;
    UV_CONF_WORD |= (settings.Veml6075_SD)  & 0x01;
    NRF_LOG_RAW_INFO("UVA about to write the conf \r\n");

    write_2bytes(twi_master,Veml6075_DEVICE_ADDRESS,Veml6075_UV_CONF,UV_CONF_WORD,0x00);

} 

static void veml6075_init(nrf_drv_twi_t twi_master){
    settings.UV_IT       = 0;
    settings.HD          = 0;
    settings.UV_TRIG     = 0;
    settings.UV_AF       = 0;
    settings.Veml6075_SD = 0;
    begin(twi_master);
    NRF_LOG_RAW_INFO("UVA Sensor Initialized:\r\n");
    uint16_t who_am_i = Veml6075_read_2bytes(twi_master,Veml6075_DEVICE_ADDRESS,Veml6075_ID);
    NRF_LOG_RAW_INFO("UVA Sensor ID: %.4x.\r\n", who_am_i);


}

static void run_veml6075(nrf_drv_twi_t twi_master){
    uint16_t who_am_i = Veml6075_read_2bytes(twi_master,Veml6075_DEVICE_ADDRESS,Veml6075_ID);
    NRF_LOG_RAW_INFO("UVA Sensor ID: %.4x.\r\n", who_am_i);

    uint16_t UVA_data = Veml6075_read_2bytes(twi_master,Veml6075_DEVICE_ADDRESS,Veml6075_UVA_DATA);
    NRF_LOG_RAW_INFO("UVA Sensor UVA: %.4x.\r\n", UVA_data);
    uint16_t UVB_data = Veml6075_read_2bytes(twi_master,Veml6075_DEVICE_ADDRESS,Veml6075_UVB_DATA);
    NRF_LOG_RAW_INFO("UVA Sensor UVB: %.4x.\r\n", UVB_data);
 
}



#ifdef __cplusplus
}
#endif

#endif // VEML6075_H