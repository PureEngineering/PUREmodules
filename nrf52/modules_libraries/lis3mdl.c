#include "lis3mdl.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"

#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"




uint8_t lis3mdl_whoami(nrf_drv_twi_t twi_master){
	uint8_t who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
	return who_am_i;
}

int8_t lis3mdl_readOUT_X_L(nrf_drv_twi_t twi_master){
	int8_t OUT_X_L = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_X_L);
	return OUT_X_L;
}
int8_t lis3mdl_readOUT_Y_L(nrf_drv_twi_t twi_master){
	int8_t OUT_Y_L = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Y_L);
	return OUT_Y_L;
}
int8_t lis3mdl_readOUT_Z_L(nrf_drv_twi_t twi_master){
	int8_t OUT_Z_L = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Z_L);
	return OUT_Z_L;
}

int8_t lis3mdl_readOUT_X_H(nrf_drv_twi_t twi_master){
	int8_t OUT_X_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_X_H);
	return OUT_X_H;
}
int8_t lis3mdl_readOUT_Y_H(nrf_drv_twi_t twi_master){
	int8_t OUT_Y_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Y_H);
	return OUT_Y_H;
}
int8_t lis3mdl_readOUT_Z_H(nrf_drv_twi_t twi_master){
	int8_t OUT_Z_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_OUT_Z_H);
	return OUT_Z_H;
}
int8_t lis3mdl_readTEMP_L(nrf_drv_twi_t twi_master){
	int8_t OUT_Y_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_TEMP_OUT_L);
	return OUT_Y_H;
}
int8_t lis3mdl_readTEMP_H(nrf_drv_twi_t twi_master){
	int8_t OUT_Z_H = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_TEMP_OUT_H);
	return OUT_Z_H;
}


uint8_t run_lis3mdl(nrf_drv_twi_t twi_master){
	uint8_t who_am_i = lis3mdl_whoami(twi_master);
	NRF_LOG_RAW_INFO("Magnetometer WhoamI: %x.\r\n", who_am_i);

	int8_t OUT_X = lis3mdl_readOUT_X_L(twi_master);
	NRF_LOG_RAW_INFO("Magnetometer OUT_X: %x.\r\n", OUT_X);
	int8_t OUT_Y = lis3mdl_readOUT_Y_L(twi_master);
	NRF_LOG_RAW_INFO("Magnetometer OUT_Y: %x.\r\n", OUT_Y);
	int8_t OUT_Z = lis3mdl_readOUT_Z_L(twi_master);
	NRF_LOG_RAW_INFO("Magnetometer OUT_Z: %x.\r\n", OUT_Z);

	int8_t temp = lis3mdl_readTEMP_L(twi_master);
	NRF_LOG_RAW_INFO("Magnetometer TEMP: %x.\r\n", temp);
	NRF_LOG_RAW_INFO("\r\n");

	return who_am_i;

}

uint8_t run_lis3mdl_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus){
	uint8_t length = 14;
	uint8_t *ble_string[length];

	uint8_t who_am_i = lis3mdl_whoami(twi_master);
    sprintf((char *)ble_string, "lis3mdlid: %x \r\n",who_am_i);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	int8_t OUT_X = lis3mdl_readOUT_X_L(twi_master);
    sprintf((char *)ble_string, "lis3mdlx: %d \r\n",OUT_X);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);

	int8_t OUT_Y = lis3mdl_readOUT_Y_L(twi_master);
    sprintf((char *)ble_string, "lis3mdly: %d \r\n",OUT_Y);
	send_ble_data(m_nus,(uint8_t *)ble_string,length);

	int8_t OUT_Z = lis3mdl_readOUT_Z_L(twi_master);
    sprintf((char *)ble_string, "lis3mdlz: %d \r\n",OUT_Z);
    send_ble_data(m_nus,(uint8_t *)ble_string,length);
	return who_am_i;

}

uint8_t lis3mdl_init(nrf_drv_twi_t twi_master){

	Lis3mdl_begin(twi_master);

	int who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);

	return who_am_i;

}

bool lis3mdl_pass(nrf_drv_twi_t twi_master){

	int who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
    
    if(who_am_i==0x3d){
        NRF_LOG_RAW_INFO("Lis3mdl: Pass %x == 0x3D \r\n", who_am_i);
        printf("Lis3mdl: Pass %x == 0x3D\r\n", who_am_i);
        return true;
    }
    else{
        NRF_LOG_RAW_INFO("Lis3mdl: Fail %x != 0x3D \r\n", who_am_i);
        printf("Lis3mdl: Fail %x != 0x3D \r\n", who_am_i);
        return false;
    }

}

void lis3mdl_setup(){
	//Temp Sensor
	//0 = off, 1 = on
	Lis3mdl_settings.TEMP_EN = 0;
	//X and Y Power Mode
	//0 = Low-power Mode, 1= Medium Performance Mode, 2=High Perf Mode, 3 = Ultra high Perf Mode
	Lis3mdl_settings.OM = 3;
	//0 = 0.625Hz, 1 = 1.25Hz, 2 = 2.5Hz, 3 = 5Hz
	//4 = 10Hz, 5 = 20Hz, 6 = 40Hz, 7 = 80Hz
	Lis3mdl_settings.DO = 4;

	//FAST_ODR enables data rates higher than 80Hz
	//0 = FAST_ODR disabled, 1 = FAST_ODR enabled
	Lis3mdl_settings.FAST_ODR = 0;

	//Self-test enable
	//0 = disabled, 1 = enabled
	Lis3mdl_settings.ST = 0;

	//Full-scale selection
	//0 = +-4 gauss, 1 = +-8 gauss,
	//2 = +-12 gauss, 3 = +- 16 gauss
	Lis3mdl_settings.FS =0;

	//Reboot memory content
	//0 = normal mode, 1 = reboot memory content
	Lis3mdl_settings.REBOOT = 0;

	//Configuration regsiters and user register reset function
	//0 = Default, 1 = Reset Operation
	Lis3mdl_settings.SOFT_RST = 0;

	//Low power mode configuration
	//0 = DOp2:0] used to decide frequency
	//1 = DO[2:0] ignored and frequency set to 0.625 Hz.
	Lis3mdl_settings.LP = 0;

	//SPI serial interface mode selection
	//0 = 4 wire interface, 1 = 3 wire interface
	Lis3mdl_settings.SIM = 0;


	//Operating Mode Selection
	// 0 = Continuous Conversion, 1 = Single Conversion, 2 or 3 = Power-down
	Lis3mdl_settings.MD = 0;


	//Z-axis Power mode
	//0 = Low-power Mode, 1= Medium Performance Mode, 2=High Perf Mode, 3 = Ultra high Perf Mode
	Lis3mdl_settings.OMZ = 3;
	//Big/Little Endian
	//0 for LSB at lower address, 1 for MSB at lower Address
	Lis3mdl_settings.BLE = 1;


	//FAST_READ allows reading the high part of DATA OUT only in order to increase reading efficiency
	//0 = Disabled, 1 = Enabled
	Lis3mdl_settings.FAST_READ = 1;
	//Block data Update
	//0 = continuous update, 1 = output not updated until MSB and LSB have been read
	Lis3mdl_settings.BDU = 1;

}

void Lis3mdl_begin(nrf_drv_twi_t twi_master){

	uint8_t CTRL1_WORD = 0x00;
	CTRL1_WORD  = (Lis3mdl_settings.TEMP_EN  <<7)  & 0x80;        
	CTRL1_WORD |= (Lis3mdl_settings.OM       <<5)  & 0x60;
	CTRL1_WORD |= (Lis3mdl_settings.DO       <<2)  & 0x1C;
	CTRL1_WORD |= (Lis3mdl_settings.FAST_ODR <<1)  & 0x02;
	CTRL1_WORD |= (Lis3mdl_settings.ST          )  & 0x01;
	write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG1,CTRL1_WORD);


	uint8_t CTRL2_WORD = 0x00;
	CTRL2_WORD  = (Lis3mdl_settings.FS       <<5)  & 0x60;        
	CTRL2_WORD |= (Lis3mdl_settings.REBOOT   <<3)  & 0x08;
	CTRL2_WORD |= (Lis3mdl_settings.SOFT_RST <<2)  & 0x04;
	write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG2,CTRL2_WORD);

	uint8_t CTRL3_WORD = 0x00;
	CTRL3_WORD  = (Lis3mdl_settings.LP       <<5)  & 0x20;        
	CTRL3_WORD |= (Lis3mdl_settings.SIM      <<2)  & 0x04;
	CTRL3_WORD |= (Lis3mdl_settings.MD          )  & 0x03;
	write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG3,CTRL3_WORD);


	uint8_t CTRL4_WORD = 0x00;
	CTRL4_WORD  = (Lis3mdl_settings.OMZ      <<2)  & 0x0C;        
	CTRL4_WORD |= (Lis3mdl_settings.BLE      <<1)  & 0x02;
	write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG4,CTRL4_WORD);


	uint8_t CTRL5_WORD = 0x00;
	CTRL5_WORD  = (Lis3mdl_settings.FAST_READ  <<7)  & 0x80;        
	CTRL5_WORD |= (Lis3mdl_settings.BDU        <<6)  & 0x40;
	write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG5,CTRL5_WORD);

}


 uint8_t lis3mdl_powerdown(nrf_drv_twi_t twi_master){
	
	int who_am_i = read_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_WHO_AM_I);
	uint8_t CTRL3_WORD = 0x00;
	write_byte(twi_master,Lis3mdl_DEVICE_ADDRESS,Lis3mdl_CTRL_REG3,CTRL3_WORD);
	return who_am_i;
 }
