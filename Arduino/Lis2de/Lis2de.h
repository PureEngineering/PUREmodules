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


		struct SensorSettings
		{
			public:
				uint8_t TEMP_ENABLE = 0x00;
				//CTRL1
				uint8_t ODR = 0x00;
				uint8_t LPEN= 0x00;
				uint8_t ZEN = 0x01;
				uint8_t YEN = 0x01;
				uint8_t XEN = 0x01;
				//CTRL2
				uint8_t HPM = 0x00;
				uint8_t HPCF = 0x00;
				uint8_t FDS = 0x00;
				uint8_t HPCLICK = 0x00;
				uint8_t HPIS2 = 0x00;
				uint8_t HPIS1 = 0x00;
				//CTRL3
				uint8_t INT1_CLICK = 0x00;
				uint8_t INT1_IG1 = 0x00;
				uint8_t INT1_IG2 = 0x00;
				uint8_t INT1_DRDY1 = 0x00;
				uint8_t INT1_DRDY2 = 0x00;
				uint8_t INT1_WTM = 0x00;
				uint8_t INT1_OVERRUN = 0x00;
				//CTRL4
				uint8_t BDU = 0x00;
				uint8_t FS  = 0x00;
				uint8_t ST  = 0x00;
				uint8_t SIM = 0x00;
				//CTRL5
				uint8_t BOOT = 0x00;
				uint8_t FIFO_EN = 0x00;
				uint8_t LIR_IG1 = 0x00;
				uint8_t D4D_IG1 = 0x00;
				uint8_t LIR_IG2 = 0x00;
				uint8_t D4D_IG2 = 0x00;
				//CTRL6
				uint8_t INT2_CLICK = 0x00;
				uint8_t INT2_IG1 = 0x00;
				uint8_t INT2_IG2 = 0x00;
				uint8_t INT2_BOOT = 0x00;
				uint8_t INT2_ACT = 0x00;
				uint8_t H_LACTIVE = 0x00;
		};


		Lis2de();
		SensorSettings settings;
		void write_byte(uint8_t addr, uint8_t subAddress, uint8_t data);
		int read_byte(uint8_t addr, uint8_t subAddress);
    	int read_2bytes(uint8_t addr, uint8_t subAddress);
    	uint8_t begin(void);

	};
#endif
