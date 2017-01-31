#ifndef LIS2DE_H
#define LIS2DE_H

#include <Arduino.h>
#include <Wire.h>

class Lis2de
{
	public:


		typedef enum {
			
			DEVICE_ADDRESS = 0x18	

		} UnitAddress;


		typedef enum {

    	AUX       =    0x07,
		TEMP_L    =    0x0C,
		TEMP_H    =    0x0D,
		WHOAMI    =    0x0F,
		TEMPCFG   =    0x1F,
		CTRL1     =    0x20,
		CTRL2     =    0x21,
		CTRL3     =    0x22,
		CTRL4     =    0x23,
		CTRL5     =    0x24,
		CTRL6     =    0x25,
		REFERENCE =    0x26,
		STATUS    =    0x27,
		OUT_X     =    0x29,
		OUT_Y     =    0x2B,
		OUT_Z     =    0x2D,
		FIFOCTRL  =    0x2E,
		FIFOSRC   =    0x2F,
		INT1CFG   =    0x30,
		INT1SRC   =    0x31,
		INT1THS   =    0x32,
		INT1DUR   =    0x33,
    	INT2CFG   =    0x34,
    	INT2SRC   =    0x35,
    	INT2THS   =    0x36,
    	INT2DUR   =    0x37,
		CLICKCFG  =    0x38,
		CLICKSRC  =    0x39,
		CLICKTHS  =    0x3A,
		TIMELIMIT =    0x3B,
		TIMELATENCY=   0x3C,
		TIMEWINDOW=    0x3D,
		ACTTHS    =    0x3E,
		ACTDUR    =    0x3F

		} registerAddress;

		Lis2de();
		void write_byte(uint8_t addr, uint8_t subAddress, uint8_t data);
		int read_byte(uint8_t addr, uint8_t subAddress);
    	int read_2bytes(uint8_t addr, uint8_t subAddress);

	};
#endif
