#include "bme280.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"


void BME280_setup( void )
{
	BME280_settings.runMode = 0;
	BME280_settings.tempOverSample = 0;
	BME280_settings.pressOverSample = 0;
	BME280_settings.humidOverSample = 0;

}


uint8_t BME280_begin(nrf_drv_twi_t twi_master)
{
	//Check the settings structure values to determine how to setup the device
	uint8_t dataToWrite = 0;  //Temporary variable

	//Reading all compensation data, range 0x88:A1, 0xE1:E7
	
	calibration.dig_T1 = ((uint16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T1_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T1_LSB_REG)));
	calibration.dig_T2 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T2_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T2_LSB_REG)));
	calibration.dig_T3 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T3_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_T3_LSB_REG)));

	calibration.dig_P1 = ((uint16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P1_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P1_LSB_REG)));
	calibration.dig_P2 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P2_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P2_LSB_REG)));
	calibration.dig_P3 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P3_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P3_LSB_REG)));
	calibration.dig_P4 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P4_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P4_LSB_REG)));
	calibration.dig_P5 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P5_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P5_LSB_REG)));
	calibration.dig_P6 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P6_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P6_LSB_REG)));
	calibration.dig_P7 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P7_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P7_LSB_REG)));
	calibration.dig_P8 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P8_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P8_LSB_REG)));
	calibration.dig_P9 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P9_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_P9_LSB_REG)));

	calibration.dig_H1 = ((uint8_t)(readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H1_REG)));
	calibration.dig_H2 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H2_MSB_REG) << 8) + readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H2_LSB_REG)));
	calibration.dig_H3 = ((uint8_t)(readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H3_REG)));
	calibration.dig_H4 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H4_MSB_REG) << 4) + (readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H4_LSB_REG) & 0x0F)));
	calibration.dig_H5 = ((int16_t)((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H5_MSB_REG) << 4) + ((readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H4_LSB_REG) >> 4) & 0x0F)));
	calibration.dig_H6 = ((uint8_t)readRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_DIG_H6_REG));

	//Set the oversampling control words.
	//config will only be writeable in sleep mode, so first insure that.
	writeRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_CTRL_MEAS_REG, 0x00);
	
	//Set the config word
	dataToWrite = (BME280_settings.tStandby << 0x5) & 0xE0;
	dataToWrite |= (BME280_settings.filter << 0x02) & 0x1C;
	writeRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_CONFIG_REG, dataToWrite);
	
	//Set ctrl_hum first, then ctrl_meas to activate ctrl_hum
	dataToWrite = BME280_settings.humidOverSample & 0x07; //all other bits can be ignored
	writeRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_CTRL_HUMIDITY_REG, dataToWrite);
	
	//set ctrl_meas
	//First, set temp oversampling
	dataToWrite = (BME280_settings.tempOverSample << 0x5) & 0xE0;
	//Next, pressure oversampling
	dataToWrite |= (BME280_settings.pressOverSample << 0x02) & 0x1C;
	//Last, set mode
	dataToWrite |= (BME280_settings.runMode) & 0x03;
	//Load the byte
	writeRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_CTRL_MEAS_REG, dataToWrite);
	
	return readRegister(twi_master,BME280_DEVICE_ADDRESS,0xD0);
}

//Strictly resets.  Run .begin() afterwards
void BME280_reset(nrf_drv_twi_t twi_master)
{
	writeRegister(twi_master,BME280_DEVICE_ADDRESS,BME280_RST_REG, 0xB6);
	
}


uint8_t BME280_init(nrf_drv_twi_t twi_master){
	uint8_t who_am_i;
	BME280_setup();
	who_am_i = BME280_begin(twi_master);
    if(who_am_i==0x60){
        NRF_LOG_RAW_INFO("BME280 Initialized %x \r\n", who_am_i);
    }
    else{
        NRF_LOG_RAW_INFO("BME280 Initialization Failed %x \r\n", who_am_i);
    }
	return who_am_i;
}




/*
float BME280_readFloatPressure( void )
{

	// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
	// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
	int32_t adc_P = ((uint32_t)readRegister(BME280_PRESSURE_MSB_REG) << 12) | ((uint32_t)readRegister(BME280_PRESSURE_LSB_REG) << 4) | ((readRegister(BME280_PRESSURE_XLSB_REG) >> 4) & 0x0F);
	
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

float BME280_readFloatAltitudeMeters( void )
{
	float heightOutput = 0;
	
	heightOutput = ((float)-45846.2)*(pow(((float)readFloatPressure()/(float)101325), 0.190263) - (float)1);
	return heightOutput;
	
}

float BME280_readFloatAltitudeFeet( void )
{
	float heightOutput = 0;
	
	heightOutput = readFloatAltitudeMeters() * 3.28084;
	return heightOutput;
	
}

float BME280_readFloatHumidity( void )
{
	
	// Returns humidity in %RH as unsigned 32 bit integer in Q22. 10 format (22 integer and 10 fractional bits).
	// Output value of “47445” represents 47445/1024 = 46. 333 %RH
	int32_t adc_H = ((uint32_t)readRegister(BME280_HUMIDITY_MSB_REG) << 8) | ((uint32_t)readRegister(BME280_HUMIDITY_LSB_REG));
	
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

float BME280_readTempC( void )
{
	// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
	// t_fine carries fine temperature as global value

	//get the reading (adc_T);
	int32_t adc_T = ((uint32_t)readRegister(BME280_TEMPERATURE_MSB_REG) << 12) | ((uint32_t)readRegister(BME280_TEMPERATURE_LSB_REG) << 4) | ((readRegister(BME280_TEMPERATURE_XLSB_REG) >> 4) & 0x0F);

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

float BME280_readTempF( void )
{
	float output = readTempC();
	output = (output * 9) / 5 + 32;

	return output;
}
*/
//****************************************************************************//
//
//  Utility
//
//****************************************************************************//
/*void readRegisterRegion(uint8_t *outputPointer , uint8_t offset, uint8_t length)
{
	//define pointer that will point to the external space
	uint8_t i = 0;
	char c = 0;

		Wire.beginTransmission(settings.I2CAddress);
		Wire.write(offset);
		Wire.endTransmission();

		// request bytes from slave device
		Wire.requestFrom(settings.I2CAddress, length);
		while ( (Wire.available()) && (i < length))  // slave may send less than requested
		{
			c = Wire.read(); // receive a byte as character
			*outputPointer = c;
			outputPointer++;
			i++;
		}
		

}

uint8_t readRegister(uint8_t offset)
{
	//Return value
	uint8_t result;
	uint8_t numBytes = 1;

	Wire.beginTransmission(settings.I2CAddress);
	Wire.write(offset);
	Wire.endTransmission();

	Wire.requestFrom(settings.I2CAddress, numBytes);
	while ( Wire.available() ) // slave may send less than requested
	{
		result = Wire.read(); // receive a byte as a proper uint8_t
	}
	return result;
}*/


uint8_t readRegister(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress){
    ret_code_t ret;
    uint8_t data;
    uint8_t buffer[1];
    buffer[0] = subAddress;


    ret = nrf_drv_twi_tx(&twi_master, addr, buffer, 1, false);
    if (NRF_SUCCESS != ret){
        NRF_LOG_WARNING("Communication error when asking for byte\r\n");
        return (uint8_t)ret;
    }
    ret = nrf_drv_twi_rx(&twi_master, addr, &data, 1);
    if (NRF_SUCCESS != ret){
        NRF_LOG_WARNING("Communication error when reading byte back\r\n");
        return (uint8_t)ret;
    }
    //NRF_LOG_RAW_INFO("Read Byte: ");
    //NRF_LOG_RAW_INFO("%.2x: \r\n", data);
    return data;	
}


/*
int16_t readRegisterInt16( uint8_t offset )
{
	uint8_t myBuffer[2];
	readRegisterRegion(myBuffer, offset, 2);  //Does memory transfer
	int16_t output = (int16_t)myBuffer[0] | int16_t(myBuffer[1] << 8);
	
	return output;
}*/

/*void writeRegister(uint8_t offset, uint8_t dataToWrite)
{
		Wire.beginTransmission(settings.I2CAddress);
		Wire.write(offset);
		Wire.write(dataToWrite);
		Wire.endTransmission();
}*/

void writeRegister(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress, uint8_t data)
{
    ret_code_t ret;
    uint8_t buffer[2]; 
    buffer[0] = subAddress;
    buffer[1] = data;
    ret = nrf_drv_twi_tx(&twi_master, addr, buffer, 2, false);
    if (NRF_SUCCESS != ret){
        NRF_LOG_WARNING("Communication error when Writing\r\n");
        return;
    }    
}