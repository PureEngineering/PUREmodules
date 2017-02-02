#ifndef BME280CODE_H
#define BME280CODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "SparkFunBME280.h"


static void bme280_init(nrf_drv_twi_t twi_master){



}

static void run_bme280(nrf_drv_twi_t twi_master){

}

#ifdef __cplusplus
}
#endif

#endif // LIS3MDL_H