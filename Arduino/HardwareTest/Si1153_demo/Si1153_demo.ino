
#include <Wire.h>

#define I2C_DEVICE_ADDRESS (0x53) //(default)

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

 #define SI1153_LED1_A (0x1F)
  #define SI1153_LED1_B (0x20)
   #define SI1153_LED2_A (0x21)
    #define SI1153_LED2_B (0x22)
     #define SI1153_LED3_A (0x23)
      #define SI1153_LED3_B (0x24)
 
 #define SI1153_THRESHOLD0_H (0x25)
 #define SI1153_THRESHOLD0_L (0x26)
 #define SI1153_THRESHOLD1_H (0x27)
 #define SI1153_THRESHOLD1_L (0x28)
 #define SI1153_THRESHOLD2_H (0x29)
 #define SI1153_THRESHOLD2_L (0x2A)
 #define SI1153_BURST (0x2B)


 #define SI1153_RESET_CMD_CTR (0x00)
 #define SI1153_RESET_SW (0x01)
 #define SI1153_FORCE (0x11)
 #define SI1153_PAUSE (0x12)
 #define SI1153_START (0x13)


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
  int response_start;
   int response;


response_start = read_i2c(i2c_address,SI1153_RESPONSE0);
  
  
  Wire.beginTransmission(i2c_address); 
  //send a bit and ask for register zero
  Wire.write(i2c_address);

  Wire.write(SI1153_HOSTIN0);
  Wire.write(data);
    Wire.write(command);

  //end transmission
  Wire.endTransmission();

response = read_i2c(i2c_address,SI1153_RESPONSE0);
if(response & (1<<4) )
{
  Serial.println("ERROR " );

  Serial.print(response,HEX);
  Serial.print("start " );
  Serial.println(response_start,HEX);

    write_i2c(I2C_DEVICE_ADDRESS,SI1153_COMMAND,SI1153_RESET_CMD_CTR);
}
  
  return response;

  /*
  
    Wire.requestFrom(i2c_address, 1);
  //wait for response
  while(Wire.available() == 0);
  //put the temperature in variable c
  int d = Wire.read();   
  return d;
  */
  
}


int read_prameter(int i2c_address,unsigned char command)
{
  read_i2c(i2c_address,SI1153_RESPONSE0);
  write_i2c(i2c_address,SI1153_COMMAND,command);
  read_i2c(i2c_address,SI1153_RESPONSE0);
  return read_i2c(i2c_address,SI1153_RESPONSE1);
  /*
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
  */
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  
  Wire.begin(); 

/*
    write_i2c(I2C_DEVICE_ADDRESS,SI1153_COMMAND,SI1153_START);

  write_prameter(I2C_DEVICE_ADDRESS,SI1153_CHAN_LIST,0x7);
  
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_LED1_A,0x3f);
  
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_LED2_A,0x3f);
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_LED3_A,0x3f);
  
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_ADCCONFIG0,0x62);
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_MEASCONFIG0,0x01);
  
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_ADCCONFIG1,0x62);
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_MEASCONFIG1,0x02);
  
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_ADCCONFIG2,0x62);
  write_prameter(I2C_DEVICE_ADDRESS,SI1153_MEASCONFIG2,0x04);
            
  write_i2c(I2C_DEVICE_ADDRESS,SI1153_COMMAND,SI1153_FORCE);

Serial.print("SI1153 CH1_PS: ");
  Serial.println(read_i2c_16bit(I2C_DEVICE_ADDRESS, SI1153_HOSTOUT(0)));

Serial.print("SI1153 CH2_PS: ");
  Serial.println(read_i2c_16bit(I2C_DEVICE_ADDRESS, SI1153_HOSTOUT(2)));

  Serial.print("SI1153 CH3_PS: ");
  Serial.println(read_i2c_16bit(I2C_DEVICE_ADDRESS, SI1153_HOSTOUT(4)));
*/
}



void loop() {
	int i;

	Serial.println("___________________________________________________________");
	Serial.println("Si1153 Register's");

	Serial.print("SI1153_PART_ID:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_PART_ID),HEX);

	Serial.print("SI1153_HW_ID:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_HW_ID),HEX);

	Serial.print("SI1153_REV_ID:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_REV_ID),HEX);

	Serial.print("SI1153_HOSTIN0:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_HOSTIN0),HEX);

	Serial.print("SI1153_COMMAND:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_COMMAND),HEX);

	Serial.print("SI1153_IRQENABLE:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_IRQENABLE),HEX);

	Serial.print("SI1153_RESPONSE1:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_RESPONSE1),HEX);

	Serial.print("SI1153_RESPONSE0:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_RESPONSE0),HEX);

	Serial.print("SI1153_IRQ_STATUS:");
	Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_IRQ_STATUS),HEX);

	for(i=0;i<25;i++)
	{
		Serial.print("SI1153_HOSTOUT");
		Serial.print("i:");
		Serial.println(read_i2c(I2C_DEVICE_ADDRESS,SI1153_HOSTOUT(i)),HEX);
	}

 /*
    Serial.println("read_prameter dump:");
  for(i=0;i<SI1153_BURST;i++)
  {

    Serial.print("i:");
    Serial.println(read_prameter(I2C_DEVICE_ADDRESS,i),HEX);
    delay(100);
  }
  */

	delay(10000); 
}
