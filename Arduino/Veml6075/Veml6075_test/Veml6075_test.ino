#include "Veml6075.h"

Veml6075 veml6075;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

  //0 =50ms, 1 = 100ms, 2 = 200ms, 3 = 400ms, 4 = 800ms.  
  veml6075.settings.UV_IT = 0;

  //0 = normal dynamic setting, 1 = high dynamic setting
  veml6075.settings.HD = 0;

  //0 = no active force mode trigger, 1 = trigger one measurement
  veml6075.settings.UV_TRIG = 0;

  //0 = active force mode disable (normal mode), 1 = active force mode enable  
  veml6075.settings.UV_AF = 0;

  //0 = power on, 1 = shut down
  veml6075.settings.Veml6075_SD = 0;

  veml6075.begin();
}

void loop() {
  Serial.println("Start Program");
  // put your main code here, to run repeatedly:
  Serial.print("Veml6075 ID: ");
  Serial.println(veml6075.read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::ID),HEX);
  Serial.print("Veml6075 CTRL: ");
  Serial.println(veml6075.read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::UV_CONF),HEX);
  Serial.println();
  Serial.print("Veml6075 UVA: ");
  Serial.println(veml6075.read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::UVA_DATA),HEX);
  Serial.print("Veml6075 UVB: ");
  Serial.println(veml6075.read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::UVB_DATA),HEX);
  Serial.println();
  delay(1000);
}
