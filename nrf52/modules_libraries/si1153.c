#include "si1153.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"
#include "ble_driver.h"


int param_set(nrf_drv_twi_t twi_master,uint8_t loc, uint8_t val){



	//int CMMD_CTR = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);

	write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTIN0,val);

	uint8_t location = loc | (0B10<<6);
	write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_COMMAND,location);

	int response = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);
	return response;
}

int param_query(nrf_drv_twi_t twi_master,uint8_t loc){
	int result = -1;

	int CMMD_CTR = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);

	uint8_t location = loc | (0B01<<6);
	write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_COMMAND,location);

	int response = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);
	if(response > CMMD_CTR){
		result = read_byte(twi_master, Si1153_DEVICE_ADDRESS,Si1153_RESPONSE1);
	}
	return result;
}

void config_channel(nrf_drv_twi_t twi_master,uint8_t index, uint8_t *conf){
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

int send_command(nrf_drv_twi_t twi_master,uint8_t code){
	int CMMD_CTR = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);

	write_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_COMMAND,code);

	int response = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_RESPONSE0);
	if(response > CMMD_CTR){
	}
	return response;
}

int bytes_to_int(uint8_t *data, size_t len){
	int result = 0;
	int shift = 8*len;

	for(int i = 0; i < len; i++){
		shift -=8;
		result |= ((data[i]<<shift) & (0xFF <<shift));
	}
	return result;
}


uint8_t run_si1153(nrf_drv_twi_t twi_master){
	uint8_t data[3];
	//uint8_t channel3_data[3];
	uint8_t who_am_i = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_PART_ID);

	data[0] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT0);
	data[1] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT1);
	data[2] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT2);
	int int_data = bytes_to_int(data, sizeof(data));
	NRF_LOG_RAW_INFO("Si1153: %.4d \r\n",int_data); 
	NRF_LOG_RAW_INFO("Si1153id 0x%x \r\n",who_am_i);

	return who_am_i;
}


uint8_t si1153_whoami(nrf_drv_twi_t twi_master){
		uint8_t who_am_i = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_PART_ID);
		return who_am_i;
}

uint8_t run_si1153_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
	uint8_t length = 13;
	uint8_t *ble_string[length];

	uint8_t who_am_i = si1153_whoami(twi_master);
    sprintf((char *)ble_string, "si1153id: %x\r\n",who_am_i);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);


	uint8_t data[3];
	//uint8_t channel3_data[3];
	data[0] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT0);
	data[1] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT1);
	data[2] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT2);
	int int_data = bytes_to_int(data, sizeof(data));

    sprintf((char *)ble_string, "si1153x:  %.4d \r\n",int_data);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);


	return who_am_i;


}


int si1153_get_data(nrf_drv_twi_t twi_master){
	uint8_t data[3];
	//uint8_t channel3_data[3];
	//uint8_t who_am_i = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_PART_ID);

	data[0] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT0);
	data[1] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT1);
	data[2] = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT2);
	int int_data = bytes_to_int(data, sizeof(data));
	return int_data;
}


int si1153_get_channel_data(nrf_drv_twi_t twi_master, int channel)
{
	int data =0;

	if(channel == 0)
	{
		data = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT1);
		data |= read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT0)*256;
	}
	else if (channel ==1)
	{
		data = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT3);
		data |= read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT2)*256;
	}
	else if (channel ==2)
	{
		data = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT5);
		data |= read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_HOSTOUT4)*256;
	}
	return data;
}

uint8_t si1153_init(nrf_drv_twi_t twi_master){

	uint8_t ADCCONFIGx;
	uint8_t ADCSENSx;
	uint8_t ADCPOSTx;
	uint8_t MEASCONFIGx;
	uint8_t configurations[4];


	uint8_t CHAN_LIST_Word = 0x0A;
	param_set(twi_master,Si1153_CHAN_LIST,CHAN_LIST_Word);


	param_set(twi_master,Si1153_MEASRATE_H,0);
	param_set(twi_master,Si1153_MEASRATE_L,1);

	param_set(twi_master,Si1153_MEASCOUNT_0,5);
	param_set(twi_master,Si1153_MEASCOUNT_1,10);
	param_set(twi_master,Si1153_MEASCOUNT_2,10);


	ADCCONFIGx = 0x00;
	ADCSENSx   = 0x02;
	ADCPOSTx   = 0x40;
	MEASCONFIGx= 0x41;

	configurations[0] = ADCCONFIGx;
	configurations[1] = ADCSENSx;
	configurations[2] = ADCPOSTx;
	configurations[3] = MEASCONFIGx;

	config_channel(twi_master,1,configurations);

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

	uint8_t who_am_i = read_byte(twi_master,Si1153_DEVICE_ADDRESS,Si1153_PART_ID);

	return who_am_i;
}


bool si1153_pass(nrf_drv_twi_t twi_master){
	uint8_t who_am_i = si1153_whoami(twi_master);


	if(who_am_i==0x53){
		NRF_LOG_RAW_INFO("Si1153: Pass %x \r\n", who_am_i); NRF_LOG_FLUSH();   
		//printf("Si1153: Pass %x == 0x53 \r\n", who_am_i);
		return true;
	}
	else{
		NRF_LOG_RAW_INFO("Si1153: Fail %x \r\n", who_am_i); NRF_LOG_FLUSH();   
		//printf("Si1153: Fail %x != 0x53 \r\n", who_am_i);
		return false;
	}

	return who_am_i;	
}
