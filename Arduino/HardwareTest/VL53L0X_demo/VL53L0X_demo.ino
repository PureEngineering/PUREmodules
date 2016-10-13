
#include <Wire.h>

#define I2C_DEVICE_ADDRESS (0x29)

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);  
	Wire.begin();  
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

void loop() {
	int i;

	Serial.println("___________________________________________________________");
	Serial.println("VL53L0X Register's");

	Serial.print("VL53L0X Address 0xC0: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,0xC0),HEX);

	Serial.print("VL53L0X Address 0xC1: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,0xC1),HEX);

	Serial.print("VL53L0X Address 0xC2: ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,0xC2),HEX);

	Serial.print("VL53L0X Address 0x51: ");
	Serial.println(read_i2c_16bit(I2C_DEVICE_ADDRESS,0x51),HEX);

	Serial.print("VL53L0X Address 0x61: ");
	Serial.println(read_i2c_16bit(I2C_DEVICE_ADDRESS,0x61),HEX);


	delay(3000); 
}
