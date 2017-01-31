#ifndef SI1153_H
#define SI1153_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

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

/** @} */


static void param_set(nrf_drv_twi_t twi_master,uint8_t loc, uint8_t val){
    while(1){
        int CMMD_CTR = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);

        write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTIN0,val);

        uint8_t location = loc | (0B10<<6);
        write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_COMMAND,location);

        int response = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);
        if(response > CMMD_CTR){
            break;
        }
    }
}

static int param_query(nrf_drv_twi_t twi_master,uint8_t loc){
    int result = -1;
    while(1){

        int CMMD_CTR = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);

        uint8_t location = loc | (0B01<<6);
        write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_COMMAND,location);

        int response = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);
        if(response > CMMD_CTR){
            result = read_byte(twi_master, Si1153_DEVICE_ADDRESS,Si1153_RESPONSE1);
            break;
        }
    }
    return result;
}

static void config_channel(nrf_drv_twi_t twi_master,uint8_t index, uint8_t *conf){
    int len = sizeof(conf);

    if(len!=4 || index < 0 || index > 5){
        NRF_LOG_WARNING("Proximity Sensor Improper Channel Config\r\n");
        return;
    }

    int inc = index * len;
    param_set(twi_master,Si1153_ADCCONFIG_0 + inc, conf[0]);
    param_set(twi_master,Si1153_ADCSENS_0   + inc, conf[1]);
    param_set(twi_master,Si1153_ADCPOST_0   + inc, conf[2]);
    param_set(twi_master,Si1153_MEASCONFIG_0+ inc, conf[3]);
}

static void send_command(nrf_drv_twi_t twi_master,uint8_t code){
    while(1){
        int CMMD_CTR = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);

        write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_COMMAND,code);

        int response = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);
        if(response > CMMD_CTR){
            break;
        }
    }
}

static int bytes_to_int(uint8_t *data, size_t len){
    int result = 0;
    int shift = 8*len;

    for(int i = 0; i < len; i++){
        shift -=8;
        result |= ((data[i]<<shift) & (0xFF <<shift));
    }
    return result;
}


static void run_si1153(nrf_drv_twi_t twi_master){
    uint8_t data[3];
    //uint8_t channel3_data[3];

    data[0] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT0);
    data[1] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT1);
    data[2] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT2);

    int int_data = bytes_to_int(data, sizeof(data));
    NRF_LOG_RAW_INFO("%.4d: \r\n",int_data);

}

static void si1153_init(nrf_drv_twi_t twi_master){
    
    NRF_LOG_WARNING("Proximity Sensor Start\r\n");
    uint8_t ADCCONFIGx;
    uint8_t ADCSENSx;
    uint8_t ADCPOSTx;
    uint8_t MEASCONFIGx;
    uint8_t configurations[4];


    uint8_t CHAN_LIST_Word = 0x0A;
    param_set(twi_master,Si1153_CHAN_LIST,CHAN_LIST_Word);


    NRF_LOG_WARNING("Proximity Sensor First Set\r\n");

    param_set(twi_master,Si1153_MEASRATE_H,0);
    param_set(twi_master,Si1153_MEASRATE_L,1);

    param_set(twi_master,Si1153_MEASCOUNT_0,5);
    param_set(twi_master,Si1153_MEASCOUNT_1,10);
    param_set(twi_master,Si1153_MEASCOUNT_2,10);


    NRF_LOG_WARNING("Proximity Sensor Setup\r\n");
    ADCCONFIGx = 0x00;
    ADCSENSx   = 0x02;
    ADCPOSTx   = 0x40;
    MEASCONFIGx= 0x41;

    configurations[0] = ADCCONFIGx;
    configurations[1] = ADCSENSx;
    configurations[2] = ADCPOSTx;
    configurations[3] = MEASCONFIGx;

    config_channel(twi_master,1,configurations);

    NRF_LOG_WARNING("Proximity Sensor Channel1\r\n");
    ADCCONFIGx = 0x00;
    ADCSENSx   = 0x02;
    ADCPOSTx   = 0x40;
    MEASCONFIGx= 0x81;

    configurations[0] = ADCCONFIGx;
    configurations[1] = ADCSENSx;
    configurations[2] = ADCPOSTx;
    configurations[3] = MEASCONFIGx;

    config_channel(twi_master,3,configurations);
    send_command(twi_master,Si1153_START);


    NRF_LOG_WARNING("Proximity Sensor Initialized\r\n");
}

#ifdef __cplusplus
}
#endif

#endif // Si1153_H