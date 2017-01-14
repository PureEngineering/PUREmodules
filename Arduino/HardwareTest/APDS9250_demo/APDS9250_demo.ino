/*
 * test.ino
 *
 * Simple sketch to test & demo the chip
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 *install library from here https://github.com/schizobovine/APDS9250
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include <APDS9250.h>

APDS9250 apds9250 = APDS9250();
bool found = false;

typedef struct sensor_data {
  uint32_t r = 0;
  uint32_t g = 0;
  uint32_t b = 0;
  uint32_t ir = 0;
  uint32_t als = 0;
} sensor_data_t;

sensor_data_t curr, last;

void setup() {


  pinMode(LED_BUILTIN, OUTPUT);

  delay(200);
  while (!Serial)
    ;
  Serial.begin(115200);

  Wire.begin();
  if (apds9250.begin()) {
    found = true;
    Serial.println("APDS9250 found!");
  } else {
    Serial.println("APDS9250 not found! Check wiring?");
  }

}

void loop() {

  if (found) {

    Serial.print(F("Resolution ")); Serial.println((uint8_t)apds9250.getResolution(), BIN);
    Serial.print(F("Meas Rate ")); Serial.println((uint8_t)apds9250.getMeasRate(), BIN);
    Serial.print(F("Gain ")); Serial.println((uint8_t)apds9250.getGain(), BIN);

    apds9250.setModeALS();
    delay(500);
    curr.als = apds9250.getRawALSData();
    curr.ir = apds9250.getRawIRData();
    Serial.print(F("ALS   ")); Serial.println(curr.als, HEX);
    Serial.print(F("IR    ")); Serial.println(curr.ir, HEX);

    apds9250.setModeRGB();
    delay(500);
    curr.r = apds9250.getRawRedData();
    curr.g = apds9250.getRawGreenData();
    curr.b = apds9250.getRawBlueData();
    Serial.print(F("Red   ")); Serial.println(curr.r, HEX);
    Serial.print(F("Green ")); Serial.println(curr.g, HEX);
    Serial.print(F("Blue  ")); Serial.println(curr.b, HEX);

  }

  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);

}
