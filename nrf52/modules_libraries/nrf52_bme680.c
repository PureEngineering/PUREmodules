#include "bme680.h"
#include "nrf52_bme680.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include <math.h>

#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"
#include "nrf_delay.h"


bool bme680_begin(){

	gas_sensor.dev_id = BME680_DEFAULT_ADDRESS;
    gas_sensor.intf = BME680_I2C_INTF;
    gas_sensor.read = &i2c_read;
    gas_sensor.write = &i2c_write;
    gas_sensor.delay_ms = nrf_delay_ms;


    int8_t rslt = BME680_OK;
    rslt = bme680_init(&gas_sensor);
  	if (rslt != BME680_OK) 
    	return false;


  bme680_setTemperatureOversampling(BME680_OS_8X);
  bme680_setHumidityOversampling(BME680_OS_2X);
  bme680_setPressureOversampling(BME680_OS_4X);
  bme680_setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme680_setGasHeater(320, 150); // 320*C for 150 ms
  
  // don't do anything till we request a reading
  gas_sensor.power_mode = BME680_FORCED_MODE;

  return true;

}


 float bme680_readTemperature(void){
 	bme680_performReading();
 	return BME680_data.temperature;
 }


 float bme680_readPressure(void){
 	bme680_performReading();
 	return BME680_data.pressure;
 }

 uint32_t bme680_readGas(void){
 	bme680_performReading();
 	return BME680_data.gas_resistance;
 }


 float bme680_readAltitude(float seaLevel){
 	float atmospheric = bme680_readPressure() / 100.0F;
    return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
 }


 bool bme680_performReading(void){
 	uint8_t set_required_settings = 0;
    struct bme680_field_data data;
 	int8_t rslt;

  	/* Select the power mode */
  	/* Must be set before writing the sensor configuration */
  	gas_sensor.power_mode = BME680_FORCED_MODE;

  /* Set the required sensor settings needed */
  if (BME680_settings.tempEnabled)
    set_required_settings |= BME680_OST_SEL;
  if (BME680_settings.humEnabled)
    set_required_settings |= BME680_OSH_SEL;
  if (BME680_settings.presEnabled)
    set_required_settings |= BME680_OSP_SEL;
  if (BME680_settings.filterEnabled)
    set_required_settings |= BME680_FILTER_SEL;
  if (BME680_settings.gasEnabled) 
    set_required_settings |= BME680_GAS_SENSOR_SEL;


  /* Set the desired sensor configuration */
  //Serial.println("Setting sensor settings");
  rslt = bme680_set_sensor_settings(set_required_settings, &gas_sensor);
  if (rslt != BME680_OK) 
    return false;
  
  /* Set the power mode */
  //Serial.println("Setting power mode");
  rslt = bme680_set_sensor_mode(&gas_sensor);
  if (rslt != BME680_OK) 
    return false;


  /* Get the total measurement duration so as to sleep or wait till the
   * measurement is complete */
  uint16_t meas_period;
  bme680_get_profile_dur(&meas_period, &gas_sensor);
  //Serial.print("Waiting (ms) "); Serial.println(meas_period);
  nrf_delay_ms(meas_period * 2); /* Delay till the measurement is ready */
  


  rslt = bme680_get_sensor_data(&data, &gas_sensor);
  if (rslt != BME680_OK) 
    return false;


  rslt = bme680_get_sensor_data(&data, &gas_sensor);
  if (rslt != BME680_OK) 
    return false;

  if (BME680_settings.tempEnabled) {
    BME680_data.temperature = data.temperature / 100.0;
    //NRF_LOG_RAW_INFO("BME680 Temp(C): %f \r\n",BME680_data.temperature);
  } else {
    BME680_data.temperature = NAN;
  }

  if (BME680_settings.humEnabled) {
    BME680_data.humidity = data.humidity / 1000.0;
    //NRF_LOG_RAW_INFO("BME680 Hum(RH%): %f \r\n",BME680_data.humidity);
  } else {
    BME680_data.humidity = NAN;
  }

  if (BME680_settings.presEnabled) {
    BME680_data.pressure = data.pressure;
    //NRF_LOG_RAW_INFO("BME680 Press(Pa): %f \r\n",BME680_data.pressure);
  } else {
    BME680_data.pressure = NAN;
  }

  /* Avoid using measurements from an unstable heating setup */
  if (BME680_settings.gasEnabled) {
    if (data.status & BME680_HEAT_STAB_MSK) {
      BME680_data.gas_resistance = data.gas_resistance;
      //NRF_LOG_RAW_INFO("BME680 Gas Res(ohms): %f \r\n",BME680_data.gas_resistance);
    } else {
      BME680_data.gas_resistance = 0;
      NRF_LOG_RAW_INFO("BME680 Gas reading unstable");
    }
  }

  return true;


 }

bool bme680_setGasHeater(uint16_t heaterTemp, uint16_t heaterTime){
  gas_sensor.gas_sett.heatr_temp = heaterTemp;
  gas_sensor.gas_sett.heatr_dur = heaterTime;

  if ( (heaterTemp == 0) || (heaterTime == 0) ) {
    // disabled!
    gas_sensor.gas_sett.run_gas = BME680_DISABLE_GAS_MEAS;
    BME680_settings.gasEnabled = false;
  } else {
    gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    BME680_settings.gasEnabled = true;
  }
  return true;

}

bool bme680_setTemperatureOversampling(uint8_t oversample){
  if (oversample > BME680_OS_16X) return false;

  gas_sensor.tph_sett.os_temp = oversample;

  if (oversample == BME680_OS_NONE)
    BME680_settings.tempEnabled = false;
  else
    BME680_settings.tempEnabled = true;

  return true;

}
   


bool bme680_setHumidityOversampling(uint8_t oversample){
  if (oversample > BME680_OS_16X) return false;

  gas_sensor.tph_sett.os_hum = oversample;

  if (oversample == BME680_OS_NONE)
    BME680_settings.humEnabled = false;
  else
    BME680_settings.humEnabled = true;

  return true;

}


bool bme680_setPressureOversampling(uint8_t oversample){

  if (oversample > BME680_OS_16X) return false;

  gas_sensor.tph_sett.os_pres = oversample;

  if (oversample == BME680_OS_NONE)
    BME680_settings.presEnabled = false;
  else
    BME680_settings.presEnabled = true;

  return true;


}

bool bme680_setIIRFilterSize(uint8_t filtersize){
  
  if (filtersize > BME680_FILTER_SIZE_127) return false;

  gas_sensor.tph_sett.filter = filtersize;

  if (filtersize == BME680_FILTER_SIZE_0)
    BME680_settings.filterEnabled = false;
  else
    BME680_settings.filterEnabled = true;

  return true;
}






