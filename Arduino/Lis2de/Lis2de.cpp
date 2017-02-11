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


uint8_t Lis2de::begin(){

    uint8_t TEMPCFG_WORD = (settings.TEMP_ENABLE<<6);
    write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::TEMPCFG,TEMPCFG_WORD);

    uint8_t CTRL1_WORD = 0x00;
    CTRL1_WORD  = (settings.ODR     <<4)  & 0xF0;        
    CTRL1_WORD |= (settings.LPEN    <<3)  & 0x08;
    CTRL1_WORD |= (settings.ZEN     <<2)  & 0x04;
    CTRL1_WORD |= (settings.YEN     <<1)  & 0x02;
    CTRL1_WORD |= (settings.XEN        )  & 0x01;
    write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL1,CTRL1_WORD);


    uint8_t CTRL2_WORD = 0x00;
    CTRL2_WORD  = (settings.HPM     <<6)  & 0xC0;        
    CTRL2_WORD |= (settings.HPCF    <<4)  & 0x30;
    CTRL2_WORD |= (settings.FDS     <<3)  & 0x08;
    CTRL2_WORD |= (settings.HPCLICK <<2)  & 0x04;
    CTRL2_WORD |= (settings.HPIS2   <<1)  & 0x02;
    CTRL2_WORD |= (settings.HPIS1      )  & 0x01;
    write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL2,CTRL2_WORD);

    uint8_t CTRL3_WORD = 0x00;
    CTRL3_WORD  = (settings.INT1_CLICK  <<7)  & 0x80;        
    CTRL3_WORD |= (settings.INT1_IG1    <<6)  & 0x40;
    CTRL3_WORD |= (settings.INT1_IG2    <<5)  & 0x20;
    CTRL3_WORD |= (settings.INT1_DRDY1  <<4)  & 0x10;
    CTRL3_WORD |= (settings.INT1_DRDY2  <<3)  & 0x08;
    CTRL3_WORD |= (settings.INT1_WTM    <<2)  & 0x04;
    CTRL3_WORD |= (settings.INT1_OVERRUN<<1)  & 0x02;
    write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL3,CTRL3_WORD);


    uint8_t CTRL4_WORD = 0x00;
    CTRL4_WORD  = (settings.BDU     <<7)  & 0x80;        
    CTRL4_WORD |= (settings.FS      <<4)  & 0x30;
    CTRL4_WORD |= (settings.ST      <<1)  & 0x06;
    CTRL4_WORD |= (settings.SIM        )  & 0x01;
    write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL4,CTRL4_WORD);


    uint8_t CTRL5_WORD = 0x00;
    CTRL5_WORD  = (settings.BOOT       <<7)  & 0x80;        
    CTRL5_WORD |= (settings.FIFO_EN    <<6)  & 0x40;
    CTRL5_WORD |= (settings.LIR_IG1    <<3)  & 0x08;
    CTRL5_WORD |= (settings.D4D_IG1    <<2)  & 0x04;
    CTRL5_WORD |= (settings.LIR_IG2    <<1)  & 0x02;
    CTRL5_WORD |= (settings.D4D_IG2       )  & 0x01;
    write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL5,CTRL5_WORD);

    uint8_t CTRL6_WORD = 0x00;
    CTRL6_WORD  = (settings.INT2_CLICK  <<7)  & 0x80;        
    CTRL6_WORD |= (settings.INT2_IG1    <<6)  & 0x40;
    CTRL6_WORD |= (settings.INT2_IG2    <<5)  & 0x20;
    CTRL6_WORD |= (settings.INT2_BOOT   <<4)  & 0x10;
    CTRL6_WORD |= (settings.INT2_ACT    <<3)  & 0x08;
    CTRL6_WORD |= (settings.H_LACTIVE   <<1)  & 0x02;
    write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL6,CTRL6_WORD);

}

uint8_t Lis2de::whoami(){
    uint8_t who_am_i = read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::WHOAMI);
    return who_am_i;
}

uint8_t Lis2de::readStatus(){
    uint8_t status = read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::STATUS);
    return status;
}

int8_t Lis2de::readOUT_X(){
    int8_t OUT_X = read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::OUT_X);
    return OUT_X;
}

int8_t Lis2de::readOUT_Y(){
    int8_t OUT_Y = read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::OUT_Y);
    return OUT_Y;
}

int8_t Lis2de::readOUT_Z(){
    int8_t OUT_Z = read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::OUT_Z);
    return OUT_Z;
}

/*
//This code toggles LED0 when the Accelerometer is flipped vertically
void Lis2de::toggleLED_when_Flipped(int8_t OUT_Y){
    if(OUT_Y==1){
        //Returns 1 if no communication so this will toggle
        bsp_board_led_invert(0);
    }
    else{
        if(OUT_Y>0){
            bsp_board_led_off(0);
        }
        else{
            bsp_board_led_on(0);
        }
    }
}*/

int8_t Lis2de::readTEMP_L(){
    int8_t temp = read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::TEMP_L);
    return temp;
}

int8_t Lis2de::readTEMP_H(){
    int8_t temp = read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::TEMP_H);
    return temp;
}
