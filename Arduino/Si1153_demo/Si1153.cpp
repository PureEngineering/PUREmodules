#include <Arduino.h>
#include "Si1153.h"

Si1153::Si1153() {
	
	//empty constructor
}

/**
 * Writes data over i2c
 */
void Si1153::write_data(uint8_t addr, uint8_t reg, uint8_t data, int len){

    Wire.beginTransmission(addr);            //Send I2C Start signal and Address to write to
    Wire.write(reg);                         //Send which Register to write to
    Wire.write(data);                        //Send data to write
    Wire.endTransmission();                  //End I2C communication
}


/**
 * Reads data from a register 'reg' over i2c
 */
int Si1153::read_register(uint8_t addr, uint8_t reg){
  
    int val = -1;
    Wire.beginTransmission(addr);             //Send I2C Start signal and Address to write to
    Wire.write(reg);                          //Send which Register to read from
    Wire.endTransmission();                   //End I2C Transmission

    Wire.requestFrom(addr,1);                 //Request one byte from I2C bus
  
    if(Wire.available()){                     //Read data from the register
      val = Wire.read();
    }
  
    return val;                               //Return value
  
}

/**
 * Code used to Setup the parameters 
 */

void Si1153::param_set(uint8_t loc, uint8_t val){
	
	while(1){

        //Check the current state of Command_CTR register. This will be used to see if write was successful.
		int CMMND_CTR = Si1153::read_register(Si1153::DEVICE_ADDRESS,Si1153::RESPONSE0); 

        //Write data to INPUT0 register
		Si1153::write_data(Si1153::DEVICE_ADDRESS,Si1153::HOSTIN0,val,1);

        //Write location (location of param to set) to COMMAND Register
        uint8_t location = loc | (0B10<<6);
		Si1153::write_data(Si1153::DEVICE_ADDRESS,Si1153::COMMAND,location,1);

        //If response has incremented then Write was Successful
		int response = Si1153::read_register(Si1153::DEVICE_ADDRESS,Si1153::RESPONSE0);
		if (response > CMMND_CTR){
			break;
		}
	}
}

/**
 *   Code used to read the current Parameters values.
 */

int Si1153::param_query(uint8_t loc){

	int result = -1;

	while(1){

        //Check the current state of Command_CTR register. This will be used to see if write was successful.
		int CMMND_CTR = Si1153::read_register(Si1153::DEVICE_ADDRESS,Si1153::RESPONSE0);

        //Write location (location of param to set) to COMMAND Register
        uint8_t location = loc | (0B01<<6);
		Si1153::write_data(Si1153::DEVICE_ADDRESS,Si1153::COMMAND,location,1);

        //If response has incremented then Write was Successful
		int response = Si1153::read_register(Si1153::DEVICE_ADDRESS,Si1153::RESPONSE0);
		if(response>CMMND_CTR){
            //Read the param value from the Response1 Register
			result = Si1153::read_register(Si1153::DEVICE_ADDRESS,Si1153::RESPONSE1);
			break;
		}
	}	
	return result;
}

/**
 * Configures a channel at a given index
 */
void Si1153::config_channel(uint8_t index, uint8_t *conf){

    //Need four configuration values to Configure Channel.
    int len = sizeof(conf);   

    //There are 6 available channels. (Indices 0 through 5).
    if(len != 4 || index < 0 || index > 5){  
        return;
    }

    //Write the Configuration values to the correct locations
    int inc = index * len;
    param_set(Si1153::ADCCONFIG_0 + inc, conf[0]);
    param_set(Si1153::ADCSENS_0 + inc, conf[1]);
    param_set(Si1153::ADCPOST_0 + inc, conf[2]);
    param_set(Si1153::MEASCONFIG_0 + inc, conf[3]);
    
}

/**
 * Sends command to the command register
 */
void Si1153::send_command(uint8_t code){

    while(1){

        //Check the current state of Command_CTR register. This will be used to see if write was successful.
        int CMMD_CTR = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE0);
      
        //Write the Command to the Command Register
        Si1153::write_data(Si1153::DEVICE_ADDRESS, Si1153::COMMAND,code,1);
      
        //If response has incremented then Write was Successful
        int response = Si1153::read_register(Si1153::DEVICE_ADDRESS, Si1153::RESPONSE0);
        if(response > CMMD_CTR){
            break; 
        }   
    }   
}

/**
 * Converts the return data into an int for easy Serial output
 */

int Si1153::bytes_to_int(uint8_t *data, size_t len){
    int result = 0;
    int shift = 8 * len;   

    //Data is sorted MSB-> LSB
    for(int i = 0; i < len; i++){
        shift -= 8;
        result |= ((data[i] << shift) & (0xFF << shift));
    }
    return result;
}
