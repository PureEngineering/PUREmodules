#ifndef PUREHEALTH_H
#define PUREHEALTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#define LIS2DE_ON_MESSAGE   '1'
#define LIS2DE_OFF_MESSAGE  '2'
#define BME280_ON_MESSAGE   '5'
#define BME280_OFF_MESSAGE  '6'
#define VEML6075_ON_MESSAGE '7'
#define VEML6075_OFF_MESSAGE '8'
#define SI1153_ON_MESSAGE   '9'
#define SI1153_OFF_MESSAGE  'a'
#define TMP007_ON_MESSAGE 'h'
#define TMP007_OFF_MESSAGE 'i'
#define ADS1114_ON_MESSAGE 'j'
#define ADS1114_OFF_MESSAGE 'k'
#define FDC2214_ON_MESSAGE 'l'
#define FDC2214_OFF_MESSAGE 'm'



typedef enum purehealth_response {
    SENSOR_SUCCESS,
    LIS2DE_INIT_FAIL,
    LIS2DE_DATA_FAIL,
    VEML6075_INIT_FAIL,
    VEML6075_DATA_FAIL,
    PROXIMITY_INIT_FAIL,
    PROXIMITY_DATA_FAIL,
    ENVIRON_INIT_FAIL,
    ENVIRON_DATA_FAIL
} purehealth_response;


void purehealth_init(nrf_drv_twi_t twi_master);
void test_purehealth_run(nrf_drv_twi_t twi_master);
bool test_individual_sensors(nrf_drv_twi_t twi_master);
void purehealth_powerdown(nrf_drv_twi_t twi_master);




#ifdef __cplusplus
}
#endif

#endif // PUREHEALTH_H