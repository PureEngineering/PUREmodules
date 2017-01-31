#include <Arduino.h>
#include "Lis2de.h"

Lis2de::Lis2de() {
	//empty constructor
}

/**
 * Writes data over i2c
 */
void Lis2de::write_byte(uint8_t addr, uint8_t subAddress, uint8_t data){

    Wire.beginTransmission(addr);       //Send Start Signal and Address over i2c
    Wire.write(subAddress);             //Subaddress is the register to write to.
    Wire.write(data);                   //Send Data
    Wire.endTransmission();             //End I2c communication
}

/**
 * Read one byte from I2C
 */
int Lis2de::read_byte(uint8_t addr, uint8_t subAddress){

  	int val = -1;
  	Wire.beginTransmission(addr);         //Send Start Signal and Address over i2c
  	Wire.write(subAddress);               //SubAddress is the register to read from
    Wire.endTransmission();                
  	Wire.requestFrom(addr,1);             //Request the data from Sensor
    
    while(Wire.available()==0);
    val = Wire.read();                    //Read back the data
    
    return val;

}

/**
 * Read two byte from I2C. Used for sensor data.
 */

 int Lis2de::read_2bytes(uint8_t addr, uint8_t subAddress){
    int val = -1;
    Wire.beginTransmission(addr);        //Send Start Signal and Address over i2c
    Wire.write(subAddress);              //SubAddress is the register to read from
    Wire.endTransmission();
    Wire.requestFrom(addr,2);            //Request two bytes of data from Sensor
    
    while(Wire.available()==0);
    val = Wire.read()<<8;                //First byte is MSB
    
    while(Wire.available()==0);
    val |= Wire.read();                  //Second byte is LSB
    
    return val;                          //Return Value
 }

