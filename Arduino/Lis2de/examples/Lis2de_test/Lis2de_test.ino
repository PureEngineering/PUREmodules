//Accelerometer
#include "Lis2de.h"

Lis2de lis2de;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  
  Wire.begin();  

  //1Hz Sampling rate,  Enable X,Y and Z axes. 
  //lis2de.write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL1,0x17);
  //enable temperature Sensor
  //lis2de.write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::TEMPCFG,0xC0);
  //Block data update enable to read out temperature data
  //lis2de.write_byte(Lis2de::DEVICE_ADDRESS,Lis2de::CTRL4,0x00);

  lis2de.settings.TEMP_ENABLE = 3;
  lis2de.settings.ODR = 1;
  lis2de.settings.ZEN = 1;
  lis2de.settings.YEN = 1;
  lis2de.settings.XEN = 1;

  lis2de.begin();



}


void loop() {
  int i;

  Serial.println("___________________________________________________________");
  Serial.println("LIS2DH Register's");

  Serial.print("LIS2DH_REG_WHOAMI (0x33): ");
  Serial.println(lis2de.read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::WHOAMI),HEX);
  
  Serial.print("Lis2dh Status Reg: ");
  Serial.println(lis2de.read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::STATUS));
  
  Serial.print("LIS2DH_REG_OUT_X_L: ");
  Serial.println((signed int8_t)lis2de.read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::OUT_X));

  Serial.print("LIS2DH_REG_OUT_Y_L: ");
  Serial.println((signed int8_t)lis2de.read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::OUT_Y));

  Serial.print("LIS2DH_REG_OUT_Z_L: ");
  Serial.println((signed int8_t)lis2de.read_byte(Lis2de::DEVICE_ADDRESS,Lis2de::OUT_Z));

  Serial.print("LIS2DH_REG_TEMP_L: ");
  Serial.println((signed int8_t)lis2de.read_2bytes(Lis2de::DEVICE_ADDRESS,Lis2de::TEMP_L));

  delay(1000); 
}
