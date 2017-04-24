#include "lis2de.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"

uint8_t defaut_lis2de_address = Lis2de_DEVICE_ADDRESS;

void override_defaut_lis2de_address(uint8_t new_address)
{
	defaut_lis2de_address = new_address;
}

uint8_t lis2de_whoami(nrf_drv_twi_t twi_master){
	uint8_t who_am_i = read_byte(twi_master,defaut_lis2de_address,Lis2de_WHO_AM_I);
	return who_am_i;
}

uint8_t lis2de_readStatus(nrf_drv_twi_t twi_master){
	uint8_t status = read_byte(twi_master,defaut_lis2de_address,Lis2de_STATUS);
	return status;
}

int8_t lis2de_readOUT_X(nrf_drv_twi_t twi_master){
	int8_t OUT_X = read_byte(twi_master,defaut_lis2de_address,Lis2de_OUT_X);
	return OUT_X;
}

int8_t lis2de_readOUT_Y(nrf_drv_twi_t twi_master){
	int8_t OUT_Y = read_byte(twi_master,defaut_lis2de_address,Lis2de_OUT_Y);
	return OUT_Y;
}

int8_t lis2de_readOUT_Z(nrf_drv_twi_t twi_master){
	int8_t OUT_Z = read_byte(twi_master,defaut_lis2de_address,Lis2de_OUT_Z);
	return OUT_Z;
}

//This code toggles LED0 when the Accelerometer is flipped vertically
void lis2de_toggleLED_when_Flipped(int8_t OUT_Y){
	if(OUT_Y==1){
		//Returns 1 if no communication so this will toggle
		bsp_board_led_invert(0);
	}
	else{
		if(OUT_Y>0){
			bsp_board_led_off(0);
		}
		else{
			bsp_board_led_on(0);
		}
	}
}

int8_t lis2de_readTEMP_L(nrf_drv_twi_t twi_master){
	int8_t temp = read_byte(twi_master,defaut_lis2de_address,Lis2de_TEMP_L);
	return temp;
}

int8_t lis2de_readTEMP_H(nrf_drv_twi_t twi_master){
	int8_t temp = read_byte(twi_master,defaut_lis2de_address,Lis2de_TEMP_H);
	return temp;
}

uint8_t run_lis2de(nrf_drv_twi_t twi_master){
	uint8_t who_am_i = lis2de_whoami(twi_master);
	NRF_LOG_RAW_INFO("Accelerometer WhoamI: %x.\r\n", who_am_i);

	uint8_t status = lis2de_readStatus(twi_master);
	NRF_LOG_RAW_INFO("Accelerometer Status: %x.\r\n", status);

	int8_t OUT_X = lis2de_readOUT_X(twi_master);
	NRF_LOG_RAW_INFO("Accelerometer OUTX: %d.\r\n", OUT_X);
	int8_t OUT_Y = lis2de_readOUT_Y(twi_master);
	NRF_LOG_RAW_INFO("Accelerometer OUTY: %d.\r\n", OUT_Y);

	//lis2de_toggleLED_when_Flipped(OUT_Y);

	int8_t OUT_Z = lis2de_readOUT_Z(twi_master);
	NRF_LOG_RAW_INFO("Accelerometer OUTZ: %d.\r\n", OUT_Z);

	int8_t temp = lis2de_readTEMP_L(twi_master);
	NRF_LOG_RAW_INFO("Accelerometer Temp: %x.\r\n", temp);
	NRF_LOG_RAW_INFO("\r\n");

	return who_am_i;

}



uint8_t run_lis2de_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
	uint8_t length = 13;
	uint8_t *ble_string[length];


	uint8_t who_am_i = lis2de_whoami(twi_master);
    sprintf((char *)ble_string, "lis2deid: %x \r\n",who_am_i);
    //puts(ble_string);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);


	int8_t OUT_X = lis2de_readOUT_X(twi_master);
    sprintf((char *)ble_string, "lis2dex: %d \r\n",OUT_X);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	int8_t OUT_Y = lis2de_readOUT_Y(twi_master);
    sprintf((char *)ble_string, "lis2dey: %d \r\n",OUT_Y);
	send_ble_data(m_nus,(uint8_t *)ble_string,length);

	int8_t OUT_Z = lis2de_readOUT_Z(twi_master);
    sprintf((char *)ble_string, "lis2dez: %d \r\n",OUT_Z);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);
	return who_am_i;

}

uint8_t lis2de_init(nrf_drv_twi_t twi_master){

	Lis2de_setup();
	Lis2de_begin(twi_master);

	uint8_t who_am_i =  lis2de_whoami(twi_master);
	if(who_am_i != 0x33)
	{
		override_defaut_lis2de_address(Lis2de_CORE_DEVICE_ADDRESS);
		NRF_LOG_RAW_INFO("Lis2de address changed to 0x%x \r\n", Lis2de_CORE_DEVICE_ADDRESS);
		who_am_i =  lis2de_whoami(twi_master);
	}

	return who_am_i;
}


bool lis2de_pass(nrf_drv_twi_t twi_master){
	uint8_t who_am_i =  lis2de_whoami(twi_master);

	if(who_am_i==0x33){
		NRF_LOG_RAW_INFO("Lis2de: Pass %x == 0x33 \r\n", who_am_i);
		//printf("Lis2de: Pass %x == 0x33 \r\n", who_am_i);
		return true;
	}
	else{
		NRF_LOG_RAW_INFO("Lis2de: Fail %x != 0x33 \r\n", who_am_i);
		//printf("Lis2de: Fail %x != 0x33 \r\n", who_am_i);
		return false;
	}

}


void Lis2de_setup(void){
	Lis2de_settings.TEMP_ENABLE = 3;
	//CTRL1
	Lis2de_settings.ODR = 1;
	Lis2de_settings.LPEN= 0;
	Lis2de_settings.ZEN = 1;
	Lis2de_settings.YEN = 1;
	Lis2de_settings.XEN = 1;
	//CTRL2
	Lis2de_settings.HPM = 0;
	Lis2de_settings.HPCF = 0;
	Lis2de_settings.FDS = 0;
	Lis2de_settings.HPCLICK = 0;
	Lis2de_settings.HPIS2 = 0;
	Lis2de_settings.HPIS1 = 0;
	//CTRL3
	Lis2de_settings.INT1_CLICK = 0;
	Lis2de_settings.INT1_IG1 = 0;
	Lis2de_settings.INT1_IG2 = 0;
	Lis2de_settings.INT1_DRDY1 = 0;
	Lis2de_settings.INT1_DRDY2 = 0;
	Lis2de_settings.INT1_WTM = 0;
	Lis2de_settings.INT1_OVERRUN = 0;
	//CTRL4
	Lis2de_settings.BDU = 0;
	Lis2de_settings.FS  = 0;
	Lis2de_settings.ST  = 0;
	Lis2de_settings.SIM = 0;
	//CTRL5
	Lis2de_settings.BOOT = 0;
	Lis2de_settings.FIFO_EN = 0;
	Lis2de_settings.LIR_IG1 = 0;
	Lis2de_settings.D4D_IG1 = 0;
	Lis2de_settings.LIR_IG2 = 0;
	Lis2de_settings.D4D_IG2 = 0;
	//CTRL6
	Lis2de_settings.INT2_CLICK = 0;
	Lis2de_settings.INT2_IG1 = 0;
	Lis2de_settings.INT2_IG2 = 0;
	Lis2de_settings.INT2_BOOT = 0;
	Lis2de_settings.INT2_ACT = 0;
	Lis2de_settings.H_LACTIVE = 0;
}


void Lis2de_begin(nrf_drv_twi_t twi_master){

	uint8_t TEMPCFG_WORD = (Lis2de_settings.TEMP_ENABLE<<6);
	write_byte(twi_master,defaut_lis2de_address,Lis2de_TEMPCFG,TEMPCFG_WORD);

	uint8_t CTRL1_WORD = 0x00;
	CTRL1_WORD  = (Lis2de_settings.ODR     <<4)  & 0xF0;        
	CTRL1_WORD |= (Lis2de_settings.LPEN    <<3)  & 0x08;
	CTRL1_WORD |= (Lis2de_settings.ZEN     <<2)  & 0x04;
	CTRL1_WORD |= (Lis2de_settings.YEN     <<1)  & 0x02;
	CTRL1_WORD |= (Lis2de_settings.XEN        )  & 0x01;
	write_byte(twi_master,defaut_lis2de_address,Lis2de_CTRL1,CTRL1_WORD);


	uint8_t CTRL2_WORD = 0x00;
	CTRL2_WORD  = (Lis2de_settings.HPM     <<6)  & 0xC0;        
	CTRL2_WORD |= (Lis2de_settings.HPCF    <<4)  & 0x30;
	CTRL2_WORD |= (Lis2de_settings.FDS     <<3)  & 0x08;
	CTRL2_WORD |= (Lis2de_settings.HPCLICK <<2)  & 0x04;
	CTRL2_WORD |= (Lis2de_settings.HPIS2   <<1)  & 0x02;
	CTRL2_WORD |= (Lis2de_settings.HPIS1      )  & 0x01;
	write_byte(twi_master,defaut_lis2de_address,Lis2de_CTRL2,CTRL2_WORD);

	uint8_t CTRL3_WORD = 0x00;
	CTRL3_WORD  = (Lis2de_settings.INT1_CLICK  <<7)  & 0x80;        
	CTRL3_WORD |= (Lis2de_settings.INT1_IG1    <<6)  & 0x40;
	CTRL3_WORD |= (Lis2de_settings.INT1_IG2    <<5)  & 0x20;
	CTRL3_WORD |= (Lis2de_settings.INT1_DRDY1  <<4)  & 0x10;
	CTRL3_WORD |= (Lis2de_settings.INT1_DRDY2  <<3)  & 0x08;
	CTRL3_WORD |= (Lis2de_settings.INT1_WTM    <<2)  & 0x04;
	CTRL3_WORD |= (Lis2de_settings.INT1_OVERRUN<<1)  & 0x02;
	write_byte(twi_master,defaut_lis2de_address,Lis2de_CTRL3,CTRL3_WORD);


	uint8_t CTRL4_WORD = 0x00;
	CTRL4_WORD  = (Lis2de_settings.BDU     <<7)  & 0x80;        
	CTRL4_WORD |= (Lis2de_settings.FS      <<4)  & 0x30;
	CTRL4_WORD |= (Lis2de_settings.ST      <<1)  & 0x06;
	CTRL4_WORD |= (Lis2de_settings.SIM        )  & 0x01;
	write_byte(twi_master,defaut_lis2de_address,Lis2de_CTRL4,CTRL4_WORD);


	uint8_t CTRL5_WORD = 0x00;
	CTRL5_WORD  = (Lis2de_settings.BOOT       <<7)  & 0x80;        
	CTRL5_WORD |= (Lis2de_settings.FIFO_EN    <<6)  & 0x40;
	CTRL5_WORD |= (Lis2de_settings.LIR_IG1    <<3)  & 0x08;
	CTRL5_WORD |= (Lis2de_settings.D4D_IG1    <<2)  & 0x04;
	CTRL5_WORD |= (Lis2de_settings.LIR_IG2    <<1)  & 0x02;
	CTRL5_WORD |= (Lis2de_settings.D4D_IG2       )  & 0x01;
	write_byte(twi_master,defaut_lis2de_address,Lis2de_CTRL5,CTRL5_WORD);

	uint8_t CTRL6_WORD = 0x00;
	CTRL6_WORD  = (Lis2de_settings.INT2_CLICK  <<7)  & 0x80;        
	CTRL6_WORD |= (Lis2de_settings.INT2_IG1    <<6)  & 0x40;
	CTRL6_WORD |= (Lis2de_settings.INT2_IG2    <<5)  & 0x20;
	CTRL6_WORD |= (Lis2de_settings.INT2_BOOT   <<4)  & 0x10;
	CTRL6_WORD |= (Lis2de_settings.INT2_ACT    <<3)  & 0x08;
	CTRL6_WORD |= (Lis2de_settings.H_LACTIVE   <<1)  & 0x02;
	write_byte(twi_master,defaut_lis2de_address,Lis2de_CTRL6,CTRL6_WORD);

}


uint8_t lis2de_powerdown(nrf_drv_twi_t twi_master){
	uint8_t who_am_i =  lis2de_whoami(twi_master);

	uint8_t CTRL1_WORD = 0x00;

	write_byte(twi_master,defaut_lis2de_address,Lis2de_CTRL1,CTRL1_WORD);


	return who_am_i;
}
