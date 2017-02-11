#include <Arduino.h>
#include "Lis3mdl.h"

Lis3mdl::Lis3mdl() {
	//empty constructor
}

/**
 * Writes data over i2c
 */
void Lis3mdl::write_byte(uint8_t addr, uint8_t subAddress, uint8_t data){

    Wire.beginTransmission(addr);       //Send Start Signal and Address over i2c
    Wire.write(subAddress);             //Subaddress is the register to write to.
    Wire.write(data);                   //Send Data
    Wire.endTransmission();             //End I2c communication
}

/**
 * Read one byte from I2C
 */
int Lis3mdl::read_byte(uint8_t addr, uint8_t subAddress){

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

 int Lis3mdl::read_2bytes(uint8_t addr, uint8_t subAddress){
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


uint8_t Lis3mdl::begin(){

    uint8_t CTRL1_WORD = 0x00;
    CTRL1_WORD  = (settings.TEMP_EN  <<7)  & 0xF0;        
    CTRL1_WORD |= (settings.OM       <<5)  & 0x60;
    CTRL1_WORD |= (settings.DO       <<2)  & 0x1C;
    CTRL1_WORD |= (settings.FAST_ODR <<1)  & 0x02;
    CTRL1_WORD |= (settings.ST          )  & 0x01;
    write_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::CTRL_REG1,CTRL1_WORD);


    uint8_t CTRL2_WORD = 0x00;
    CTRL2_WORD  = (settings.FS       <<5)  & 0x60;        
    CTRL2_WORD |= (settings.REBOOT   <<3)  & 0x08;
    CTRL2_WORD |= (settings.SOFT_RST <<2)  & 0x04;
    write_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::CTRL_REG2,CTRL2_WORD);

    uint8_t CTRL3_WORD = 0x00;
    CTRL3_WORD  = (settings.LP       <<5)  & 0x20;        
    CTRL3_WORD |= (settings.SIM      <<2)  & 0x04;
    CTRL3_WORD |= (settings.MD          )  & 0x03;
    write_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::CTRL_REG3,CTRL3_WORD);


    uint8_t CTRL4_WORD = 0x00;
    CTRL4_WORD  = (settings.OMZ      <<2)  & 0x0C;        
    CTRL4_WORD |= (settings.BLE      <<1)  & 0x02;
    write_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::CTRL_REG4,CTRL4_WORD);


    uint8_t CTRL5_WORD = 0x00;
    CTRL5_WORD  = (settings.FAST_READ  <<7)  & 0x80;        
    CTRL5_WORD |= (settings.BDU        <<6)  & 0x40;
    write_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::CTRL_REG5,CTRL5_WORD);

}



uint8_t Lis3mdl::whoami(){
      uint8_t who_am_i = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::WHO_AM_I);
      return who_am_i;
}

int8_t Lis3mdl::readOUT_X_L(){
      int8_t OUT_X_L = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_X_L);
      return OUT_X_L;
}
int8_t Lis3mdl::readOUT_X_H(){
      int8_t OUT_X_H = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_X_H);
      return OUT_X_H;
}
int8_t Lis3mdl::readOUT_Y_L(){
      int8_t OUT_Y_L = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_Y_L);
      return OUT_Y_L;
}
int8_t Lis3mdl::readOUT_Y_H(){
      int8_t OUT_Y_H = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_Y_H);
      return OUT_Y_H;
}
int8_t Lis3mdl::readOUT_Z_L(){
      int8_t OUT_Z_L = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_Z_L);
      return OUT_Z_L;
}
int8_t Lis3mdl::readOUT_Z_H(){
      int8_t OUT_Z_H = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_Z_H);
      return OUT_Z_H;
}
int8_t Lis3mdl::readTEMP_L(){
      int8_t temp_L = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::TEMP_OUT_L);
      return temp_L;
}
int8_t Lis3mdl::readTEMP_H(){
      int8_t temp_H = read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::TEMP_OUT_H);
      return temp_H;
}