#ifndef BME680_H
#define BME680_H

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


#define BME680_DEFAULT_ADDRESS                (0x77)
#define SEALEVELPRESSURE_HPA 				(1013.25)

struct BME680_Data
{
    /// Temperature (Celsius) assigned after calling performReading() 
    float temperature;  
    /// Pressure (Pascals) assigned after calling performReading() 
    float pressure;
    /// Humidity (RH %) assigned after calling performReading()
    float humidity;
    /// Gas resistor (ohms) assigned after calling performReading()
    float gas_resistance;

} BME680_data;


 struct bme680_dev gas_sensor;

 struct BME680_Settings
 {
 	bool filterEnabled;
 	bool tempEnabled;
 	bool humEnabled;
 	bool presEnabled;
 	bool gasEnabled;
 } BME680_settings;


    uint8_t i2caddr;
    int32_t sensorID;
    int8_t cs;

//Adafruit_BME680(int8_t cspin = -1);
//Adafruit_BME680(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

    bool  bme680_begin();
    float bme680_readTemperature(void);
    float bme680_readPressure(void);
    float bme680_readHumidity(void);
    uint32_t bme680_readGas(void);
    float bme680_readAltitude(float seaLevel);

    bool bme680_setTemperatureOversampling(uint8_t oversample);
    bool bme680_setPressureOversampling(uint8_t oversample);
    bool bme680_setHumidityOversampling(uint8_t oversample);
    bool bme680_setIIRFilterSize(uint8_t filtersize);
    bool bme680_setGasHeater(uint16_t heaterTemp, uint16_t heaterTime);

    bool bme680_performReading(void);




#endif