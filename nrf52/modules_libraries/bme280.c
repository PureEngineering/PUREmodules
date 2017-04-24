#include "bme280.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include <math.h>

#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"


void bme280_setup( void )
{
	//Mode Settings
	//0 = Sleep Mode, 1 or 2 Forced Mode, 3 = Normal Mode
	BME280_settings.runMode = 3;

	//tstandby Settings
	//0 = 0.5ms, 1 = 62.5ms, 2 = 125ms, 3 = 250ms
	//4 = 500ms, 5 = 1000ms, 6 = 10ms, 7 = 20ms
	BME280_settings.tStandby = 0;

	//Filter Coefficient Settings
	//0 = Filter off, 1 = 2, 2 = 4, 3 = 8, 4 = 16
	BME280_settings.filter = 0;

	//Temp Oversampling
	//0 = Skipped, 1 = x1 , 2 = x2, 3 = x4, 4 = x8, 5 = x16
	BME280_settings.tempOverSample = 1;

	//Pressure Oversampling
	//0 = Skipped, 1 = x1 , 2 = x2, 3 = x4, 4 = x8, 5 = x16
	BME280_settings.pressOverSample = 1;

	//Humidity Oversampling
	//0 = Skipped, 1 = x1 , 2 = x2, 3 = x4, 4 = x8, 5 = x16
	BME280_settings.humidOverSample = 1;

}

uint8_t bme280_whoami(nrf_drv_twi_t twi_master){
	return read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_CHIP_ID_REG);	
}


uint8_t bme280_begin(nrf_drv_twi_t twi_master)
{
	//Check the settings structure values to determine how to setup the device
	uint8_t dataToWrite = 0;  //Temporary variable

	//Reading all compensation data, range 0x88:A1, 0xE1:E7
	
	calibration.dig_T1 = ((uint16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T1_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T1_LSB_REG)));
	calibration.dig_T2 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T2_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T2_LSB_REG)));
	calibration.dig_T3 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T3_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T3_LSB_REG)));

	calibration.dig_P1 = ((uint16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P1_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P1_LSB_REG)));
	calibration.dig_P2 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P2_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P2_LSB_REG)));
	calibration.dig_P3 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P3_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P3_LSB_REG)));
	calibration.dig_P4 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P4_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P4_LSB_REG)));
	calibration.dig_P5 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P5_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P5_LSB_REG)));
	calibration.dig_P6 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P6_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P6_LSB_REG)));
	calibration.dig_P7 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P7_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P7_LSB_REG)));
	calibration.dig_P8 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P8_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P8_LSB_REG)));
	calibration.dig_P9 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P9_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P9_LSB_REG)));

	calibration.dig_H1 = ((uint8_t)(read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H1_REG)));
	calibration.dig_H2 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H2_MSB_REG) << 8) + read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H2_LSB_REG)));
	calibration.dig_H3 = ((uint8_t)(read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H3_REG)));
	calibration.dig_H4 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H4_MSB_REG) << 4) + (read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H4_LSB_REG) & 0x0F)));
	calibration.dig_H5 = ((int16_t)((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H5_MSB_REG) << 4) + ((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H4_LSB_REG) >> 4) & 0x0F)));
	calibration.dig_H6 = ((uint8_t)read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H6_REG));

	//Set the oversampling control words.
	//config will only be writeable in sleep mode, so first insure that.
	write_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_CTRL_MEAS_REG, 0x00);
	
	//Set the config word
	dataToWrite = (BME280_settings.tStandby << 0x5) & 0xE0;
	dataToWrite |= (BME280_settings.filter << 0x02) & 0x1C;
	write_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_CONFIG_REG, dataToWrite);
	
	//Set ctrl_hum first, then ctrl_meas to activate ctrl_hum
	dataToWrite = BME280_settings.humidOverSample & 0x07; //all other bits can be ignored
	write_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_CTRL_HUMIDITY_REG, dataToWrite);
	
	//set ctrl_meas
	//First, set temp oversampling
	dataToWrite = (BME280_settings.tempOverSample << 0x5) & 0xE0;
	//Next, pressure oversampling
	dataToWrite |= (BME280_settings.pressOverSample << 0x02) & 0x1C;
	//Last, set mode
	dataToWrite |= (BME280_settings.runMode) & 0x03;
	//Load the byte
	write_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_CTRL_MEAS_REG, dataToWrite);
	
	return bme280_whoami(twi_master);	
}

//Strictly resets.  Run .begin() afterwards
void bme280_reset(nrf_drv_twi_t twi_master)
{
	write_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_RST_REG, 0xB6);
	
}

void bme280_powerdown(nrf_drv_twi_t twi_master){
	//Put Sensor in sleep mode
	write_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_CTRL_MEAS_REG, 0x00);
}

uint8_t bme280_init(nrf_drv_twi_t twi_master){
	uint8_t who_am_i;
	bme280_setup();
	who_am_i = bme280_whoami(twi_master);

	return who_am_i;
}

bool bme280_pass(nrf_drv_twi_t twi_master){

	uint8_t who_am_i = bme280_whoami(twi_master);
    if(who_am_i==0x60){
        NRF_LOG_RAW_INFO("BME280: Pass %x == 0x60 \r\n", who_am_i);
        //printf("BME280: Pass %x == 0x60 \r\n", who_am_i);

        return true;
    }
    else{
        NRF_LOG_RAW_INFO("BME280: Fail %x != 0x60 \r\n", who_am_i);
        //printf("BME280: Fail %x != 0x60 \r\n", who_am_i);
        return false;
    }
}


uint8_t run_bme280(nrf_drv_twi_t twi_master){
	uint8_t who_am_i = bme280_whoami(twi_master);
    NRF_LOG_RAW_INFO("BME280 ID: %x.\r\n", who_am_i);

	uint8_t tempF = bme280_readTempF(twi_master);
    NRF_LOG_RAW_INFO("BME280 Temp: %d F.\r\n", tempF);

	uint8_t pressure = bme280_readFloatPressure(twi_master);
    NRF_LOG_RAW_INFO("BME280 Pressure: %d(Pa).\r\n", pressure);

	uint8_t altitude = bme280_readFloatAltitudeFeet(twi_master);
    NRF_LOG_RAW_INFO("BME280 Altitude: %d ft.\r\n", altitude);

	uint8_t humidity = bme280_readFloatHumidity(twi_master);
    NRF_LOG_RAW_INFO("BME280 Humidity: %d percent\r\n", humidity);

    return who_am_i;
}


uint8_t run_bme280_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
	uint8_t length = 15;
	uint8_t *ble_string[length];

	uint8_t who_am_i = bme280_whoami(twi_master);
    sprintf((char *)ble_string, "bme280id:    %x\r\n",who_am_i);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t tempF = bme280_readTempF(twi_master);
    sprintf((char *)ble_string, "bme280tempf: %d\r\n",tempF);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);


	uint8_t tempc = bme280_readTempC(twi_master);
    sprintf((char *)ble_string, "bme280tempc: %d\r\n",tempc);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t pressure = bme280_readFloatPressure(twi_master);
    sprintf((char *)ble_string, "bme280press: %d\r\n",pressure);
	send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t altitude = bme280_readFloatAltitudeFeet(twi_master);
    sprintf((char *)ble_string, "bme280alt:   %d\r\n",altitude);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	uint8_t humidity = bme280_readFloatHumidity(twi_master);
    sprintf((char *)ble_string, "bme280hum:   %d\r\n",humidity);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);


	return who_am_i;

}


float bme280_readFloatPressure(nrf_drv_twi_t twi_master)
{

	// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
	// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
	int32_t adc_P = ((uint32_t)read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_PRESSURE_MSB_REG) << 12) | ((uint32_t)read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_PRESSURE_LSB_REG) << 4) | ((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_PRESSURE_XLSB_REG) >> 4) & 0x0F);
	
	int64_t var1, var2, p_acc;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calibration.dig_P6;
	var2 = var2 + ((var1 * (int64_t)calibration.dig_P5)<<17);
	var2 = var2 + (((int64_t)calibration.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)calibration.dig_P3)>>8) + ((var1 * (int64_t)calibration.dig_P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)calibration.dig_P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p_acc = 1048576 - adc_P;
	p_acc = (((p_acc<<31) - var2)*3125)/var1;
	var1 = (((int64_t)calibration.dig_P9) * (p_acc>>13) * (p_acc>>13)) >> 25;
	var2 = (((int64_t)calibration.dig_P8) * p_acc) >> 19;
	p_acc = ((p_acc + var1 + var2) >> 8) + (((int64_t)calibration.dig_P7)<<4);
	
	return (float)p_acc / 256.0;
	
}

float bme280_readFloatAltitudeMeters(nrf_drv_twi_t twi_master)
{
	float heightOutput = 0;
	
	heightOutput = ((float)-45846.2)*(pow(((float)bme280_readFloatPressure(twi_master)/(float)101325), 0.190263) - (float)1);
	return heightOutput;
	
}

float bme280_readFloatAltitudeFeet(nrf_drv_twi_t twi_master)
{
	float heightOutput = 0;
	
	heightOutput = bme280_readFloatAltitudeMeters(twi_master) * 3.28084;
	return heightOutput;
	
}

float bme280_readFloatHumidity(nrf_drv_twi_t twi_master)
{
	
	// Returns humidity in %RH as unsigned 32 bit integer in Q22. 10 format (22 integer and 10 fractional bits).
	// Output value of “47445” represents 47445/1024 = 46. 333 %RH
	int32_t adc_H = ((uint32_t)read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_HUMIDITY_MSB_REG) << 8) | ((uint32_t)read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_HUMIDITY_LSB_REG));
	
	int32_t var1;
	var1 = (t_fine - ((int32_t)76800));
	var1 = (((((adc_H << 14) - (((int32_t)calibration.dig_H4) << 20) - (((int32_t)calibration.dig_H5) * var1)) +
	((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)calibration.dig_H6)) >> 10) * (((var1 * ((int32_t)calibration.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
	((int32_t)calibration.dig_H2) + 8192) >> 14));
	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)calibration.dig_H1)) >> 4));
	var1 = (var1 < 0 ? 0 : var1);
	var1 = (var1 > 419430400 ? 419430400 : var1);

	return (float)(var1>>12) / 1024.0;

}

float bme280_readTempC(nrf_drv_twi_t twi_master)
{
	// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
	// t_fine carries fine temperature as global value

	//get the reading (adc_T);
	int32_t adc_T = ((uint32_t)read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_TEMPERATURE_MSB_REG) << 12) | ((uint32_t)read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_TEMPERATURE_LSB_REG) << 4) | ((read_byte(twi_master,BME280_DEVICE_ADDRESS,BME280_TEMPERATURE_XLSB_REG) >> 4) & 0x0F);

	//By datasheet, calibrate
	int64_t var1, var2;

	var1 = ((((adc_T>>3) - ((int32_t)calibration.dig_T1<<1))) * ((int32_t)calibration.dig_T2)) >> 11;
	var2 = (((((adc_T>>4) - ((int32_t)calibration.dig_T1)) * ((adc_T>>4) - ((int32_t)calibration.dig_T1))) >> 12) *
	((int32_t)calibration.dig_T3)) >> 14;
	t_fine = var1 + var2;
	float output = (t_fine * 5 + 128) >> 8;

	output = output / 100;
	
	return output;
}

float bme280_readTempF(nrf_drv_twi_t twi_master)
{
	float output = bme280_readTempC(twi_master);
	output = (output * 9) / 5 + 32;

	return output;
}

