#include "DW1000Time.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "ble_nus.h"
#include "bsp.h"
#include "i2c_driver.h"
#include "ble_driver.h"
#include "math.h"

#include "DW1000Time.h"



void DW1000Time_init(){
	DW1000Time_value.timestamp = 0;
}

void DW1000Time_setTime(float timeUs) {
	DW1000Time_value.timestamp = (long long int)(timeUs * TIME_RES_INV);
}


void DW1000Time_setTimestamp(uint8_t data[]) {
	DW1000Time_value.timestamp = 0;
	for(int i = 0; i < LEN_STAMP; i++) {
	    DW1000Time_value.timestamp |= ((long long int)data[i] << (i*8));
	}
}

long long int DW1000Time_getTimestamp() {
	return DW1000Time_value.timestamp;
}

float DW1000Time_getAsFloat() {
	return fmod((float)DW1000Time_value.timestamp, TIME_OVERFLOW) * TIME_RES;
}

float DW1000Time_getAsMeters(){
	return fmod((float)DW1000Time_value.timestamp, TIME_OVERFLOW) * DISTANCE_OF_RADIO;
}


//It asks for a DWTIME instead lets pull it before and assign it
void DW1000Time_assign(long long int assign) {
	DW1000Time_value.timestamp = assign;
}

void DW1000Time_add(long long int add) {
	DW1000Time_value.timestamp += add;
}

void DW1000Time_sub(long long int sub) {
	DW1000Time_value.timestamp -= sub;
}

void DW1000Time_mult(float mult) {
	DW1000Time_value.timestamp *= mult;
}

void DW1000Time_div(float factor) {
	DW1000Time_value.timestamp *= (1.0f/factor);
}

bool DW1000Time_cmp(long long int cmp) {
	return DW1000Time_value.timestamp == cmp;
}
