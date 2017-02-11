//Magnetometer
#include "Lis3mdl.h"
//Magnetometer
Lis3mdl lis3mdl;

void setup(){
  Serial.begin(115200);
  Wire.begin();

  //Temp Sensor
  //0 = off, 1 = on
  lis3mdl.settings.TEMP_EN = 1;
  //X and Y Power Mode
  //0 = Low-power Mode, 1= Medium Performance Mode, 2=High Perf Mode, 3 = Ultra high Perf Mode
  lis3mdl.settings.OM = 3;
  //0 = 0.625Hz, 1 = 1.25Hz, 2 = 2.5Hz, 3 = 5Hz
  //4 = 10Hz, 5 = 20Hz, 6 = 40Hz, 7 = 80Hz
  lis3mdl.settings.DO = 4;


  //Operating Mode Selection
  // 0 = Continuous Conversion, 1 = Single Conversion, 2 or 3 = Power-down
  lis3mdl.settings.MD = 0;


  //Z-axis Power mode
  //0 = Low-power Mode, 1= Medium Performance Mode, 2=High Perf Mode, 3 = Ultra high Perf Mode
  lis3mdl.settings.OMZ = 3;
  //Big/Little Endian
  //0 for LSB at lower address, 1 for MSB at lower Address
  lis3mdl.settings.BLE = 1;


  //FAST_READ allows reading the high part of DATA OUT only in order to increase reading efficiency
  //0 = Disabled, 1 = Enabled
  lis3mdl.settings.FAST_READ = 1;
  //Block data Update
  //0 = continuous update, 1 = output not updated until MSB and LSB have been read
  lis3mdl.settings.BDU = 1;

  lis3mdl.begin();

}

void loop(){
  
  Serial.print("Lis3mdl WHO_AM_I (Should be 0x3D) ");
  uint8_t whoAmI = lis3mdl.whoami();
  Serial.println(whoAmI,HEX);
  
  Serial.print("X direction output: ");
  int8_t output_X = lis3mdl.readOUT_X_L();
  Serial.println(output_X);

  Serial.print("Y direction output: ");
  int8_t output_Y = lis3mdl.readOUT_Y_L();
  Serial.println(output_Y);

  Serial.print("Z direction output: ");
  int8_t output_Z = lis3mdl.readOUT_Z_L();
  Serial.println(output_Z);

  Serial.print("Temperature Output: ");
  int8_t output_temp = lis3mdl.readTEMP_L();
  Serial.println(output_temp);
  
  Serial.println();
  delay(1000);

}
 

