#ifndef SUPERSENSOR_H
#define SUPERSENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"


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


ss_response test_SuperSensor_init(nrf_drv_twi_t twi_master);
ss_response test_SuperSensor_run(nrf_drv_twi_t twi_master);
void test_SuperSensor(nrf_drv_twi_t twi_master);
bool test_individual_sensors(nrf_drv_twi_t twi_master);




#ifdef __cplusplus
}
#endif

#endif // SUPERSENSOR_H