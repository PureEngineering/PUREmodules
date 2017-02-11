#ifndef VEML6075_H
#define VEML6075_H

#include <Arduino.h>
#include <Wire.h>

class Veml6075
{
	public:

		typedef enum {
			
			DEVICE_ADDRESS = 0x10

		} UnitAddress;


		typedef enum {

			UV_CONF	=  		0x00,
			UVA_DATA   = 	0x07,
			UVB_DATA   = 	0x09,
			UVCOMP1_DATA=	0X0A,
			UVCOMP2_DATA=	0X0B,
			ID 				=   0X0C


		} registerAddress;


		struct SensorSettings
		{
		  public:
			
		  //Main Interface and mode settings
		    uint8_t UV_IT;
		    uint8_t HD;
		    uint8_t UV_TRIG;
			uint8_t UV_AF;
			uint8_t Veml6075_SD;
		};

		Veml6075();
		SensorSettings settings;
		void write_2bytes(uint8_t addr, uint8_t subAddress, uint8_t data_LSB,uint8_t data_MSB);
    	int read_2bytes(uint8_t addr, uint8_t subAddress);
    	uint8_t begin(void);

    	uint16_t whoami(void);
		uint16_t readUV_CONF(void);
		uint16_t readUVA_DATA(void);
		uint16_t readUVB_DATA(void);

	};
#endif
