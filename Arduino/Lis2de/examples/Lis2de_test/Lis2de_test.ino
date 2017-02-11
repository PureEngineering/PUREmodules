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
  Serial.println("LIS2DE Register's");

  Serial.print("LIS2DE_REG_WHOAMI (0x33): ");
  Serial.println(lis2de.whoami(),HEX);
  
  Serial.print("Lis2de Status Reg: ");
  Serial.println(lis2de.readStatus(),HEX);
  
  Serial.print("LIS2DE_REG_OUT_X: ");
  Serial.println(lis2de.readOUT_X());

  Serial.print("LIS2DE_REG_OUT_Y: ");
  Serial.println(lis2de.readOUT_Y());

  Serial.print("LIS2DE_REG_OUT_Z: ");
  Serial.println(lis2de.readOUT_Z());

  Serial.print("LIS2DE_REG_TEMP_L: ");
  Serial.println(lis2de.readTEMP_L());

  delay(1000); 
}
