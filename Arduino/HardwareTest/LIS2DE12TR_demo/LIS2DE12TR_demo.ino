
#include <Wire.h>

#define I2C_DEVICE_ADDRESS (0x18) //ADDRESS PULLED LOW (default)
//#define I2C_DEVICE_ADDRESS (0x19) //ADDRESS PULLED HIGH 


#define LIS2DH_REG_STATUS1       0x07
#define LIS2DH_REG_OUTADC1_L     0x08
#define LIS2DH_REG_OUTADC1_H     0x09
#define LIS2DH_REG_OUTADC2_L     0x0A
#define LIS2DH_REG_OUTADC2_H     0x0B
#define LIS2DH_REG_OUTADC3_L     0x0C
#define LIS2DH_REG_OUTADC3_H     0x0D
#define LIS2DH_REG_INTCOUNT      0x0E
#define LIS2DH_REG_WHOAMI        0x0F
#define LIS2DH_REG_TEMPCFG       0x1F
#define LIS2DH_REG_CTRL1         0x20
#define LIS2DH_REG_CTRL2         0x21
#define LIS2DH_REG_CTRL3         0x22
#define LIS2DH_REG_CTRL4         0x23
#define LIS2DH_REG_CTRL5         0x24
#define LIS2DH_REG_CTRL6         0x25
#define LIS2DH_REG_REFERENCE     0x26
#define LIS2DH_REG_STATUS2       0x27

#define LIS2DH_REG_OUT_X       0x29

#define LIS2DH_REG_OUT_Y       0x2B

#define LIS2DH_REG_OUT_Z       0x2D
#define LIS2DH_REG_FIFOCTRL      0x2E
#define LIS2DH_REG_FIFOSRC       0x2F
#define LIS2DH_REG_INT1CFG       0x30
#define LIS2DH_REG_INT1SRC       0x31
#define LIS2DH_REG_INT1THS       0x32
#define LIS2DH_REG_INT1DUR       0x33
#define LIS2DH_REG_CLICKCFG      0x38
#define LIS2DH_REG_CLICKSRC      0x39
#define LIS2DH_REG_CLICKTHS      0x3A
#define LIS2DH_REG_TIMELIMIT     0x3B
#define LIS2DH_REG_TIMELATENCY   0x3C
#define LIS2DH_REG_TIMEWINDOW    0x3D
#define LIS2DH_REG_ACTTHS        0x3E
#define LIS2DH_REG_ACTDUR        0x3F

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

write_i2c(I2C_DEVICE_ADDRESS,LIS2DH_REG_CTRL1,0x97); //power on all axis, max sample rate(5.376 kHz)

}


void loop() {
	int i;

	Serial.println("___________________________________________________________");
	Serial.println("LIS3MDL Register's");

	Serial.print("LIS2DH_REG_WHOAMI (0x33): ");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,LIS2DH_REG_WHOAMI),HEX);


	Serial.print("LIS2DH_REG_OUT_X_L: ");
	Serial.println((signed char)read_i2c(I2C_DEVICE_ADDRESS,LIS2DH_REG_OUT_X));

	Serial.print("LIS2DH_REG_OUT_Y_L: ");
	Serial.println((signed char)read_i2c(I2C_DEVICE_ADDRESS,LIS2DH_REG_OUT_Y));

	Serial.print("LIS2DH_REG_OUT_Z_L: ");
	Serial.println((signed char)read_i2c(I2C_DEVICE_ADDRESS,LIS2DH_REG_OUT_Z));


	delay(1000); 
}
