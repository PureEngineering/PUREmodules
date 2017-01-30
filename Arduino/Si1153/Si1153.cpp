#include <Arduino.h>
#include "Si1153.h"

Si1153::Si1153() {
	//empty constructor
}

/**
 * Configures a channel at a given index
 */
void Si1153::config_channel(uint8_t index, uint8_t *conf){
	int len = sizeof(conf);

	if(len != 4 || index < 0 || index > 5){

		return;

	}

	int inc = index * len;

	param_set(Si1153::ADCCONFIG_0 + inc, conf[0]);
	param_set(Si1153::ADCSENS_0 + inc, conf[1]);
	param_set(Si1153::ADCPOST_0 + inc, conf[2]);
	param_set(Si1153::MEASCONFIG_0 + inc, conf[3]);
}

/**
 * Writes data over i2c
 */
void Si1153::write_data(uint8_t addr, uint8_t *data, size_t len){
	Wire.beginTransmission(addr);
	Wire.write(data, len);
	Wire.endTransmission();
}

/**
 * Reads data from a register over i2c
 */
int Si1153::read_register(uint8_t addr, uint8_t reg, int bytesOfData){
	int val = -1;

	Si1153::write_data(addr, &reg, sizeof(reg));
	Wire.requestFrom(addr, bytesOfData);

	if(Wire.available()){

		val = Wire.read();

	}

	return val;
}

/**
 * param set as shown in the datasheet
 */
void Si1153::param_set(uint8_t loc, uint8_t val){
	uint8_t packet[2];

	while(1){
		int cmmnd_ctr = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE_0, 1);

		packet[0] = Si1153::HOSTIN_0;
		packet[1] = val;
		Si1153::write_data(Si1153::DEVICE_ADDRESS, packet, sizeof(packet));

		packet[0] = Si1153::COMMAND;
		packet[1] = loc | (0B10 << 6);
		Si1153::write_data(Si1153::DEVICE_ADDRESS, packet, sizeof(packet));

		int r = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE_0, 1);

		if(r > cmmnd_ctr){
			break;
		}
	} 
}

/**
 * param query as shown in the datasheet
 */
int Si1153::param_query(uint8_t loc){
	int result = -1;
	uint8_t packet[2];

	while(1){
		int cmmnd_ctr = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE_0, 1);

		packet[0] = Si1153::COMMAND;
		packet[1] = loc | (0B01 << 6);
		Si1153::write_data(Si1153::DEVICE_ADDRESS, packet, sizeof(packet));

		int r = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE_0, 1);

		if(r > cmmnd_ctr){
			result = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE_1, 1);
			break;
		}
	}

	return result;
}

/**
 * Sends command to the command register
 */
void Si1153::send_command(uint8_t code){
	uint8_t packet[2];

	while(1){
		int cmmnd_ctr = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE_0, 1);

		packet[0] = Si1153::COMMAND;
		packet[1] = code;
		Si1153::write_data(Si1153::DEVICE_ADDRESS, packet, sizeof(packet));

		int r = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE_0, 1);

		if(r > cmmnd_ctr){
			break;
		}
	}
}

/**
 * Returns int given a byte array
 */
int Si1153::get_int_from_bytes(uint8_t *data, size_t len){
	int result = 0;
	int shift = 8 * len;

	for(int i = 0; i < len; i++){
		shift -= 8;
		result |= ((data[i] << shift) & (0xFF << shift));
	}

	return result;
}

