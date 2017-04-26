#ifndef SUPERSENSOR_H
#define SUPERSENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define LIS2DE_ON_MESSAGE   '1'
#define LIS2DE_OFF_MESSAGE  '2'
#define LIS3MDL_ON_MESSAGE  '3'
#define LIS3MDL_OFF_MESSAGE '4'
#define BME280_ON_MESSAGE   '5'
#define BME280_OFF_MESSAGE  '6'
#define VEML6075_ON_MESSAGE '7'
#define VEML6075_OFF_MESSAGE '8'
#define SI1153_ON_MESSAGE   '9'
#define SI1153_OFF_MESSAGE  'a'
#define APDS9250_ON_MESSAGE 'b'
#define APDS9250_OFF_MESSAGE 'c'
#define VL53L0_ON_MESSAGE    'd'
#define VL53L0_OFF_MESSAGE   'e'
#define P1234701CT_ON_MESSAGE 'f'
#define P1234701CT_OFF_MESSAGE 'g'


typedef enum ss_response {
    SENSOR_SUCCESS,
    LIS3MDL_INIT_FAIL,
    LIS3MDL_DATA_FAIL,
    LIS2DE_INIT_FAIL,
    LIS2DE_DATA_FAIL,
    VEML6075_INIT_FAIL,
    VEML6075_DATA_FAIL,
    PROXIMITY_INIT_FAIL,
    PROXIMITY_DATA_FAIL,
    TOF_INIT_FAIL,
    TOF_DATA_FAIL,
    SS_SUCCESSFUL
} ss_response;


void test_supersensor_init(nrf_drv_twi_t twi_master);
void test_supersensor_run(nrf_drv_twi_t twi_master);
void test_supersensor(nrf_drv_twi_t twi_master);
bool test_individual_sensors(nrf_drv_twi_t twi_master);
void supersensor_powerdown(nrf_drv_twi_t twi_master);




#ifdef __cplusplus
}
#endif

#endif // SUPERSENSOR_H