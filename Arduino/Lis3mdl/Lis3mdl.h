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

		Lis3mdl();
		void write_byte(uint8_t addr, uint8_t subAddress, uint8_t data);
		int read_byte(uint8_t addr, uint8_t subAddress);
    	int read_2bytes(uint8_t addr, uint8_t subAddress);

	};
#endif
