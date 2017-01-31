#include "VL53L0.h"

VL53L0 vl53l0;

void setup() {
  Serial.begin(9600);  
  Wire.begin();  
}

void loop() {
  int i;

  Serial.println("___________________________________________________________");
  Serial.println("VL53L0X Register's");

  Serial.print("VL53L0X Address 0xC0: ");
  Serial.println(vl53l0.read_byte(VL53L0::DEVICE_ADDRESS,0xC0),HEX);

  Serial.print("VL53L0X Address 0xC1: ");
  Serial.println(vl53l0.read_byte(VL53L0::DEVICE_ADDRESS,0xC1),HEX);

  Serial.print("VL53L0X Address 0xC2: ");
  Serial.println(vl53l0.read_byte(VL53L0::DEVICE_ADDRESS,0xC2),HEX);

  Serial.print("VL53L0X Address 0x51: ");
  Serial.println(vl53l0.read_2bytes(VL53L0::DEVICE_ADDRESS,0x51),HEX);

  Serial.print("VL53L0X Address 0x61: ");
  Serial.println(vl53l0.read_2bytes(VL53L0::DEVICE_ADDRESS,0x61),HEX);


  delay(3000); 
}
