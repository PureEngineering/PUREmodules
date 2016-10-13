
#include <Wire.h>

#define SI1153_PART_ID (0x00)
#define SI1153_HW_ID (0x01)
#define SI1153_REV_ID (0x02)
#define SI1153_HOSTIN0 (0x0A)
#define SI1153_COMMAND (0x0B)
#define SI1153_IRQENABLE (0x0F)
#define SI1153_RESPONSE1 (0x10)
#define SI1153_RESPONSE0 (0x11)
#define SI1153_IRQ_STATUS (0x12)
#define SI1153_HOSTOUT(x) (0x13+x)

 #define SI1153_I2C_ADDR (0x00)
 #define SI1153_CHAN_LIST (0x01)
 #define SI1153_ADCCONFIG0 (0x02)
 #define SI1153_ADCSENS0 (0x03)
 #define SI1153_ADCPOST0 (0x04)
 #define SI1153_MEASCONFIG0 (0x05)
 #define SI1153_ADCCONFIG1 (0x06)
 #define SI1153_ADCSENS1 (0x07)
 #define SI1153_ADCPOST1 (0x08)
 #define SI1153_MEASCONFIG1 (0x09)
 #define SI1153_ADCCONFIG2 (0x0A)
 #define SI1153_ADCSENS2 (0x0B)
 #define SI1153_ADCPOST2 (0x0C)
 #define SI1153_MEASCONFIG2 (0x0D)
 #define SI1153_ADCCONFIG3 (0x0E)
 #define SI1153_ADCSENS3 (0x0F)
 #define SI1153_ADCPOST3 (0x10)
 #define SI1153_MEASCONFIG3 (0x11)
 #define SI1153_ADCCONFIG4 (0x12)
 #define SI1153_ADCSENS4 (0x13)
 #define SI1153_ADCPOST4 (0x14)
 #define SI1153_MEASCONFIG4 (0x15)
 #define SI1153_ADCCONFIG5 (0x16)
 #define SI1153_ADCSENS5 (0x17)
 #define SI1153_ADCPOST5 (0x18)
 #define SI1153_MEASCONFIG5 (0x19)
 #define SI1153_MEASRATE_H (0x1A)
 #define SI1153_MEASRATE_L (0x1B)
 #define SI1153_MEASCOUNT0 (0x1C)
 #define SI1153_MEASCOUNT1 (0x1D)
 #define SI1153_MEASCOUNT2 (0x1E)
 #define SI1153_THRESHOLD0_H (0x25)
 #define SI1153_THRESHOLD0_L (0x26)
 #define SI1153_THRESHOLD1_H (0x27)
 #define SI1153_THRESHOLD1_L (0x28)
 #define SI1153_THRESHOLD2_H (0x29)
 #define SI1153_THRESHOLD2_L (0x2A)
 #define SI1153_BURST BURST (0x2B)


void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);  
	Wire.begin();  
}


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

int write_prameter(int i2c_address,unsigned char command, unsigned char data)
{
  Wire.beginTransmission(i2c_address); 
  //send a bit and ask for register zero
  Wire.write(i2c_address);

  Wire.write(SI1153_HOSTIN0);
  Wire.write(data);
    Wire.write(command);

  //end transmission
  Wire.endTransmission();
  
    Wire.requestFrom(i2c_address, 1);
  //wait for response
  while(Wire.available() == 0);
  //put the temperature in variable c
  int d = Wire.read();   
  return d;
  
}


int read_prameter(int i2c_address,unsigned char command)
{
  Wire.beginTransmission(i2c_address); 
  //send a bit and ask for register zero
  Wire.write(i2c_address);

  Wire.write(SI1153_COMMAND);
    Wire.write(command);

  //end transmission
  Wire.endTransmission();
  
    Wire.requestFrom(i2c_address, 1);
  //wait for response
  while(Wire.available() == 0);
  //put the temperature in variable c
  int d = Wire.read();   
  return d;
  
}

void loop() {
	int i;

	Serial.println("___________________________________________________________");
	Serial.println("Si1153 Register's");


#define SI1153_PART_ID (0x00)
#define SI1153_HW_ID (0x01)
#define SI1153_REV_ID (0x02)
#define SI1153_HOSTIN0 (0x0A)
#define SI1153_COMMAND (0x0B)
#define SI1153_IRQENABLE (0x0F)
#define SI1153_RESPONSE1 (0x10)
#define SI1153_RESPONSE0 (0x11)
#define SI1153_IRQ_STATUS (0x12)
#define SI1153_HOSTOUT(x) (0x13+x)

	Serial.print("SI1153_PART_ID:");
	Serial.println(read_i2c(0x53,SI1153_PART_ID),HEX);

	Serial.print("SI1153_HW_ID:");
	Serial.println(read_i2c(0x53,SI1153_HW_ID),HEX);

	Serial.print("SI1153_REV_ID:");
	Serial.println(read_i2c(0x53,SI1153_REV_ID),HEX);

	Serial.print("SI1153_HOSTIN0:");
	Serial.println(read_i2c(0x53,SI1153_HOSTIN0),HEX);

	Serial.print("SI1153_COMMAND:");
	Serial.println(read_i2c(0x53,SI1153_COMMAND),HEX);

	Serial.print("SI1153_IRQENABLE:");
	Serial.println(read_i2c(0x53,SI1153_IRQENABLE),HEX);

	Serial.print("SI1153_RESPONSE1:");
	Serial.println(read_i2c(0x53,SI1153_RESPONSE1),HEX);

	Serial.print("SI1153_RESPONSE0:");
	Serial.println(read_i2c(0x53,SI1153_RESPONSE0),HEX);

	Serial.print("SI1153_IRQ_STATUS:");
	Serial.println(read_i2c(0x53,SI1153_IRQ_STATUS),HEX);

	for(i=0;i<25;i++)
	{
		Serial.print("SI1153_HOSTOUT");
		Serial.print("i:");
		Serial.println(read_i2c(0x53,SI1153_HOSTOUT(i)),HEX);
	}

	delay(3000); 
}
