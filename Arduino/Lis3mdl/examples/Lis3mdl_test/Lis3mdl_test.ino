//Magnetometer
#include "Lis3mdl.h"

Lis3mdl lis3mdl;

void setup(){
  Wire.begin();
  Serial.begin(115200);

  //Turn on Temp Sensor, Set X and Y mode to Ultra-high-Performance, 10 Hz data rate
  lis3mdl.write_byte(Lis3mdl::DEVICE_ADDRESS, Lis3mdl::CTRL_REG1,0xf0);

  //Set communication mode to I2C and to Continuous-conversion Mode
  lis3mdl.write_byte(Lis3mdl::DEVICE_ADDRESS, Lis3mdl::CTRL_REG3,0x00);

  //Set Z mode to Ultra-high-Performance. 
  lis3mdl.write_byte(Lis3mdl::DEVICE_ADDRESS, Lis3mdl::CTRL_REG4,0x0E);

  //Turn on FAST_READ and set up Block data update
  lis3mdl.write_byte(Lis3mdl::DEVICE_ADDRESS, Lis3mdl::CTRL_REG5,0xc0);  
  
}

void loop(){
  int whoAmI_response;
  int16_t output_X;
  int16_t output_Y;
  int16_t output_Z;
  int output_temp;
  
  Serial.print("Lis3mdl WHO_AM_I (Should be 0x3D) ");
  whoAmI_response = lis3mdl.read_byte(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::WHO_AM_I);
  Serial.println(whoAmI_response,HEX);
  
  Serial.print("X direction output: ");
  output_X = lis3mdl.read_2bytes(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_X_L);
  Serial.println(output_X);

  Serial.print("Y direction output: ");
  output_Y = lis3mdl.read_2bytes(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_Y_L);
  Serial.println(output_Y);

  Serial.print("Z direction output: ");
  output_Z = lis3mdl.read_2bytes(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::OUT_Z_L);
  Serial.println(output_Z);

  Serial.print("Temperature Output: ");
  output_temp = lis3mdl.read_2bytes(Lis3mdl::DEVICE_ADDRESS,Lis3mdl::TEMP_OUT_L);
  Serial.println(output_temp);
  
  Serial.println();
  delay(1000);

}
 

