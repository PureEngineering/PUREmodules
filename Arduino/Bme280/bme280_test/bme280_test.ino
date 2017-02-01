#include <stdint.h>
#include "SparkFunBME280.h"
#include "Wire.h"

BME280 BME280;

void setup()
{
  BME280.settings.commInterface = I2C_MODE;
  BME280.settings.I2CAddress = 0x77;


  //runMode- 0 = Sleep mode, 1 and 2 = Forced Mode, 3 = Normal Mode
  BME280.settings.runMode = 3; //Normal mode
  
  //tStandby can be:
  //  0 = 0.5ms, 1 = 62.5ms, 2 = 125ms, 3 = 250ms, 4 = 500ms, 5 = 1000ms, 6 = 10ms, 7 = 20ms
  BME280.settings.tStandby = 0;
  
  //filter can be off or number of FIR coefficients to use:
  //  0 = filter off, 1 = coefficients = 2, 2 = coefficients = 4, 3 = coefficients = 8, 4 = coefficients = 16
  BME280.settings.filter = 0;
  
  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  BME280.settings.tempOverSample = 1;

  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    BME280.settings.pressOverSample = 1;
  
  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  BME280.settings.humidOverSample = 1;
  
  Serial.begin(115200);
  Serial.print("Program Started\n");
  Serial.print("Starting BME280... result of .begin(): 0x");
  
  //Calling .begin() causes the settings to be loaded
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  Serial.println(BME280.begin(), HEX);

  Serial.print("Displaying ID, reset and ctrl regs\n");
  
  Serial.print("ID(0xD0): 0x");
  Serial.println(BME280.readRegister(BME280_CHIP_ID_REG), HEX);
  Serial.print("Reset register(0xE0): 0x");
  Serial.println(BME280.readRegister(BME280_RST_REG), HEX);
  Serial.print("ctrl_meas(0xF4): 0x");
  Serial.println(BME280.readRegister(BME280_CTRL_MEAS_REG), HEX);
  Serial.print("ctrl_hum(0xF2): 0x");
  Serial.println(BME280.readRegister(BME280_CTRL_HUMIDITY_REG), HEX);

}

void loop()
{

  Serial.print("Temperature: ");
  Serial.print(BME280.readTempC(), 2);
  Serial.println(" degrees C");

  Serial.print("Temperature: ");
  Serial.print(BME280.readTempF(), 2);
  Serial.println(" degrees F");

  Serial.print("Pressure: ");
  Serial.print(BME280.readFloatPressure(), 2);
  Serial.println(" Pa");

  Serial.print("Altitude: ");
  Serial.print(BME280.readFloatAltitudeMeters(), 2);
  Serial.println("m");

  Serial.print("Altitude: ");
  Serial.print(BME280.readFloatAltitudeFeet(), 2);
  Serial.println("ft"); 

  Serial.print("%RH: ");
  Serial.print(BME280.readFloatHumidity(), 2);
  Serial.println(" %");
  
  Serial.println();
  
  delay(1000);

}
