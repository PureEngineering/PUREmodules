#ifndef LIS3MDL_H
#define LIS3MDL_H

#include <Arduino.h>
#include <Wire.h>

class Lis3mdl
{
	public:


		typedef enum {
			
			DEVICE_ADDRESS = 0x1E	

		} UnitAddress;


		typedef enum {
		
		WHO_AM_I = 0x0F,
		CTRL_REG1 = 0x20,
		CTRL_REG2 = 0x21,
		CTRL_REG3 = 0x22,
		CTRL_REG4 = 0x23,
		CTRL_REG5 = 0x24,
		STATUS_REG = 0x27,
		OUT_X_L = 0x28,
		OUT_X_H = 0x29,
		OUT_Y_L = 0x2A,
		OUT_Y_H = 0x2B,
		OUT_Z_L = 0x2C,
		OUT_Z_H = 0x2D,
		TEMP_OUT_L = 0x2E,
		TEMP_OUT_R = 0x2F,
		INT_CFG = 0x30,
		INT_SRC = 0x31,
		INT_THS_L = 0x32,
		INT_THS_R = 0x33

		} registerAddress;

		struct SensorSettings
		{
			public:

				//CTRL1
				uint8_t TEMP_EN  = 0x00;
				uint8_t OM       = 0x00;
				uint8_t DO       = 0x04;
				uint8_t FAST_ODR = 0x00;
				uint8_t ST       = 0x00;
				//CTRL2
				uint8_t FS       = 0x00;
				uint8_t REBOOT   = 0x00;
				uint8_t SOFT_RST = 0x00;
				//CTRL3
				uint8_t LP       = 0x00;
				uint8_t SIM      = 0x00;
				uint8_t MD       = 0x03;
				//CTRL4
				uint8_t OMZ      = 0x00;
				uint8_t BLE      = 0x00;
				//CTRL5
				uint8_t FAST_READ= 0x00;
				uint8_t BDU      = 0x00;
		};



		Lis3mdl();
		SensorSettings settings;
		void write_byte(uint8_t addr, uint8_t subAddress, uint8_t data);
		int read_byte(uint8_t addr, uint8_t subAddress);
    	int read_2bytes(uint8_t addr, uint8_t subAddress);
    	uint8_t begin(void);

	};
#endif
