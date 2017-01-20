#ifndef SI1153_H
#define SI1153_H

#include <Arduino.h>
#include <Wire.h>

class Si1153
{
	public:

		typedef enum {
			
			DEVICE_ADDRESS = 0x53			

		} UnitAddress;

		typedef enum {
		
			RESET_CMD_CTR = 0x00,
			RESET_SW = 0x01,
			FORCE = 0x11,
			PAUSE = 0x12,
			START = 0x13

		} CommandCodes;

		typedef enum {
			
			PART_ID = 0x00,
			REV_ID = 0x01,
			MFR_ID = 0x02,
			INFO_0 = 0x03,
			INFO_1 = 0x04,
			HOSTIN3 = 0x07,
			HOSTIN2 = 0x08,
	
			HOSTIN0 = 0x0A,
			COMMAND = 0x0B,
			IRQ_ENABLE = 0x0F,
			RESPONSE0 = 0x11,
			RESPONSE1 = 0x10,
			
			IRQ_STATUS = 0x12,
			HOSTOUT0 = 0x13,
			HOSTOUT1 = 0x14,
			HOSTOUT2 = 0x15,
			HOSTOUT3 = 0x16,
			HOSTOUT4 = 0x17,
			HOSTOUT5 = 0x18,
			HOSTOUT6 = 0x19,
			HOSTOUT7 = 0x1A,
			HOSTOUT8 = 0x1B,
			HOSTOUT9 = 0x1C,
			HOSTOUT10 = 0x1D,
			HOSTOUT11 = 0x1E,
			HOSTOUT12 = 0x1F,
			HOSTOUT13 = 0x20,
			HOSTOUT14 = 0x21,
			HOSTOUT15 = 0x22,
			HOSTOUT16 = 0x23,
			HOSTOUT17 = 0x24,
			HOSTOUT18 = 0x25,
			HOSTOUT19 = 0x26,
			HOSTOUT20 = 0x27,
			HOSTOUT21 = 0x28,
			HOSTOUT22 = 0x29,
			HOSTOUT23 = 0x2A,
			HOSTOUT24 = 0x2B,
			HOSTOUT25 = 0x2C	
	
		} RegisterAddress;

		typedef enum {		
	
			I2C_ADDR = 0x00,
			CHAN_LIST = 0x01,

			ADCCONFIG_0 = 0x02,
			ADCSENS_0 = 0x03,
			ADCPOST_0 = 0x04,
			MEASCONFIG_0 = 0x05,
			
			ADCCONFIG_1 = 0x06,
			ADCSENS_1 = 0x07,
			ADCPOST_1 = 0x08,
			MEASCONFIG_1 = 0x09,

			ADCCONFIG_2 = 0x0A,
			ADCSENS_2 = 0x0B,
			ADCPOST_2 = 0x0C,
			MEASCONFIG_2 = 0x0D,

			ADCCONFIG_3 = 0x0E,
			ADCSENS_3 = 0x0F,
			ADCPOST_3 = 0x10,
			MEASCONFIG_3 = 0x11,

			ADCCONFIG_4 = 0x12,
			ADCSENS_4 = 0x13,
			ADCPOST_4 = 0x14,
			MEASCONFIG_4 = 0x15,

			ADCCONFIG_5 = 0x16,
			ADCSENS_5 = 0x17,
			ADCPOST_5 = 0x18,
			MEASCONFIG_5 = 0x19,
			
			MEASRATE_H = 0x1A,
			MEASRATE_L = 0x1B,
			MEASCOUNT_0 = 0x1C,
			MEASCOUNT_1 = 0x1D,
			MEASCOUNT_2 = 0x1E,
			
			LED1_A = 0x1F,
			LED1_B = 0x20,
			LED2_A = 0x21,
			LED2_B = 0x22,
			LED3_A = 0x23,
			LED3_B = 0x24,

			THRESHOLD0_H = 0x25,
			THRESHOLD0_L = 0x26,
			THRESHOLD1_H = 0x27,
			THRESHOLD1_L = 0x28,
			THRESHOLD2_H = 0x29,
			THRESHOLD2_L = 0x2A,

			BURST = 0x2B
			
		} ParameterAddress;



		Si1153();
		void config_channel(uint8_t index, uint8_t *conf);
		void write_data(uint8_t addr, uint8_t *data, int len);
		int read_register(uint8_t addr, uint8_t reg, int len);
		int param_query(uint8_t loc);
		void param_set(uint8_t loc, uint8_t val);
		void send_command(uint8_t code);
		int bytes_to_int(uint8_t *data, size_t len);





	};
#endif
