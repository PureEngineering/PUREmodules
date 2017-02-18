
#include <Wire.h>

#define I2C_DEVICE_ADDRESS (0x1E) //ADDRESS PULLED HIGH (default)
//#define I2C_DEVICE_ADDRESS (0x1C) //ADDRESS PULLED LOW 

#define LIS3MDL_WHO_AM_I (0x0f)
#define LIS3MDL_CTRL_REG1 (0x20)
#define LIS3MDL_CTRL_REG2 (0x21)
#define LIS3MDL_CTRL_REG3 (0x22)
#define LIS3MDL_CTRL_REG4 (0x23)
#define LIS3MDL_CTRL_REG5 (0x24)
#define LIS3MDL_STATUS_REG (0x27)
#define LIS3MDL_OUT_X_L (0x28)
#define LIS3MDL_OUT_X_H (0x29)
#define LIS3MDL_OUT_Y_L (0x2a)
#define LIS3MDL_OUT_Y_H (0x2b)
#define LIS3MDL_OUT_Z_L (0x2c)
#define LIS3MDL_OUT_Z_H (0x2d)
#define LIS3MDL_TEMP_OUT_L (0x2e)
#define LIS3MDL_TEMP_OUT_H (0x2f)
#define LIS3MDL_INT_CFG (0x30)
#define LIS3MDL_INT_SRC (0x31)
#define LIS3MDL_INT_THS_L (0x32)
#define LIS3MDL_INT_THS_H (0x33)




void write_i2c(int address,int subaddress, int data) {
	//start the communication with IC with the address xx
	Wire.beginTransmission(address); 
	//send a bit and ask for register zero
	Wire.write(subaddress);

	Wire.write(data);

	//end transmission
	Wire.endTransmission();
}

int read_i2c(int address,int subaddress) {
	//start the communication with IC with the address xx
	Wire.beginTransmission(address); 
	//send a bit and ask for register zero
	Wire.write(subaddress);
	//end transmission
	Wire.endTransmission();
	//request 1 byte from address xx
	Wire.requestFrom(address, 1);
	//wait for response
	while(Wire.available() == 0);
	//put the temperature in variable c
	int d = Wire.read();   
	return d;
}

int read_i2c_16bit(int address,int subaddress) {
	//start the communication with IC with the address xx
	Wire.beginTransmission(address); 
	//send a bit and ask for register zero
	Wire.write(subaddress);
	//end transmission
	Wire.endTransmission();
	//request 2 byte from address xx
	Wire.requestFrom(address, 2);
	//wait for response
	while(Wire.available() == 0);
	//put the temperature in variable c
	int d = Wire.read()<<8;   

	while(Wire.available() == 0);
	//put the temperature in variable c
	d |= Wire.read();   

	return d;
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);  
	Wire.begin();  

	write_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG1,0xf0); //enable temp sensor,  (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
	write_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG3,0x00); //continuous-conversion mode
	write_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG4,0x0E); //Ultra-high-performance mode
	write_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG5,0xC0); //fast read and bdu enable
}


void loop() {
	int i;

	Serial.println("___________________________________________________________");
	Serial.println("LIS3MDL Register's");

	Serial.print("LIS3MDL_WHO_AM_I: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_WHO_AM_I),HEX);

	Serial.print("LIS3MDL_CTRL_REG1: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG1),HEX);

	Serial.print("LIS3MDL_CTRL_REG2: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG2),HEX);

	Serial.print("LIS3MDL_CTRL_REG3: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG3),HEX);

	Serial.print("LIS3MDL_CTRL_REG4: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG4),HEX);

	Serial.print("LIS3MDL_CTRL_REG5: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,LIS3MDL_CTRL_REG5),HEX);


	Serial.print("LIS3MDL_OUT_X: ");
	Serial.println((signed short)read_i2c_16bit(I2C_DEVICE_ADDRESS,LIS3MDL_OUT_X_L));

	Serial.print("LIS3MDL_OUT_Y: ");
	Serial.println((signed short)read_i2c_16bit(I2C_DEVICE_ADDRESS,LIS3MDL_OUT_Y_L));

	Serial.print("LIS3MDL_OUT_Z: ");
	Serial.println((signed short)read_i2c_16bit(I2C_DEVICE_ADDRESS,LIS3MDL_OUT_Z_L));

	Serial.print("LIS3MDL_TEMP_OUT: ");
	Serial.println(((signed short)read_i2c_16bit(I2C_DEVICE_ADDRESS,LIS3MDL_TEMP_OUT_L))/8 + 25);


	delay(1000); 
}
