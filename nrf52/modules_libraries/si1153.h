#ifndef SI1153_H
#define SI1153_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


#define Si1153_DEVICE_ADDRESS       0x53
#define Si1153_RESET_CMD_CTR        0x00
#define Si1153_RESET_SW             0x01
#define Si1153_FORCE	            0x11
#define Si1153_PAUSE                0x12
#define Si1153_START                0x13
#define Si1153_PART_ID              0x00
#define Si1153_REV_ID               0x01
#define Si1153_MFR_ID               0x02
#define Si1153_INFO_0               0x03
#define Si1153_INFO_1               0x04
#define Si1153_HOSTIN3              0x07
#define Si1153_HOSTIN2              0x08
#define Si1153_HOSTIN1              0x09
#define Si1153_HOSTIN0              0x0A
#define Si1153_COMMAND              0x0B
#define Si1153_IRQ_ENABLE           0x0F
#define Si1153_RESPONSE0            0x11
#define Si1153_RESPONSE1            0x10
#define Si1153_IRQ_STATUS           0x12
#define Si1153_HOSTOUT0             0x13
#define Si1153_HOSTOUT1             0x14
#define Si1153_HOSTOUT2             0x15
#define Si1153_HOSTOUT3             0x16
#define Si1153_HOSTOUT4             0x17
#define Si1153_HOSTOUT5             0x18
#define Si1153_HOSTOUT6             0x19
#define Si1153_HOSTOUT7             0x1A
#define Si1153_HOSTOUT8             0x1B
#define Si1153_HOSTOUT9             0x1C
#define Si1153_HOSTOUT10            0x1D
#define Si1153_HOSTOUT11            0x1E
#define Si1153_HOSTOUT12            0x1F
#define Si1153_HOSTOUT13            0x20
#define Si1153_HOSTOUT14            0x21
#define Si1153_HOSTOUT15            0x22
#define Si1153_HOSTOUT16            0x23
#define Si1153_HOSTOUT17            0x24
#define Si1153_HOSTOUT18            0x25
#define Si1153_HOSTOUT19            0x26
#define Si1153_HOSTOUT20            0x27
#define Si1153_HOSTOUT21            0x28
#define Si1153_HOSTOUT22            0x29
#define Si1153_HOSTOUT23            0x2A
#define Si1153_HOSTOUT24            0x2B
#define Si1153_HOSTOUT25            0x2C

#define Si1153_I2C_ADDR                    0x00  
#define Si1153_CHAN_LIST                   0x01  
#define Si1153_ADCCONFIG_0                 0x02  
#define Si1153_ADCSENS_0                   0x03  
#define Si1153_ADCPOST_0                   0x04  
#define Si1153_MEASCONFIG_0                0x05            
#define Si1153_ADCCONFIG_1                 0x06  
#define Si1153_ADCSENS_1                   0x07  
#define Si1153_ADCPOST_1                   0x08  
#define Si1153_MEASCONFIG_1                0x09  
#define Si1153_ADCCONFIG_2                 0x0A  
#define Si1153_ADCSENS_2                   0x0B  
#define Si1153_ADCPOST_2                   0x0C  
#define Si1153_MEASCONFIG_2                0x0D  

#define Si1153_ADCCONFIG_3                 0x0E  
#define Si1153_ADCSENS_3                   0x0F  
#define Si1153_ADCPOST_3                   0x10  
#define Si1153_MEASCONFIG_3                0x11  

#define Si1153_ADCCONFIG_4                 0x12  
#define Si1153_ADCSENS_4                   0x13  
#define Si1153_ADCPOST_4                   0x14  
#define Si1153_MEASCONFIG_4                0x15  

#define Si1153_ADCCONFIG_5                 0x16  
#define Si1153_ADCSENS_5                   0x17  
#define Si1153_ADCPOST_5                   0x18  
#define Si1153_MEASCONFIG_5                0x19  

#define Si1153_MEASRATE_H                  0x1A  
#define Si1153_MEASRATE_L                  0x1B  
#define Si1153_MEASCOUNT_0                 0x1C  
#define Si1153_MEASCOUNT_1                 0x1D  
#define Si1153_MEASCOUNT_2                 0x1E  

#define Si1153_LED1_A                      0x1F  
#define Si1153_LED1_B                      0x20  
#define Si1153_LED2_A                      0x21  
#define Si1153_LED2_B                      0x22  
#define Si1153_LED3_A                      0x23  
#define Si1153_LED3_B                      0x24  

#define Si1153_THRESHOLD0_H                0x25  
#define Si1153_THRESHOLD0_L                0x26  
#define Si1153_THRESHOLD1_H                0x27  
#define Si1153_THRESHOLD1_L                0x28  
#define Si1153_THRESHOLD2_H                0x29  
#define Si1153_THRESHOLD2_L                0x2A  

#define Si1153_BURST                       0x2B

	int param_set(nrf_drv_twi_t twi_master,uint8_t loc, uint8_t val);
	int param_query(nrf_drv_twi_t twi_master,uint8_t loc);
	void config_channel(nrf_drv_twi_t twi_master,uint8_t index, uint8_t *conf);
	int send_command(nrf_drv_twi_t twi_master,uint8_t code);
	int bytes_to_int(uint8_t *data, size_t len);
	uint8_t run_si1153(nrf_drv_twi_t twi_master);
	uint8_t run_si1153_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus);
	int si1153_get_data(nrf_drv_twi_t twi_master);
	uint8_t si1153_init(nrf_drv_twi_t twi_master);
	uint8_t si1153_whoami(nrf_drv_twi_t twi_master);
	bool si1153_pass(nrf_drv_twi_t twi_master);




#ifdef __cplusplus
}
#endif

#endif // Si1153_H
