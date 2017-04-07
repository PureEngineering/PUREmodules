#ifndef BLE__DRIVER_H
#define BLE__DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif


#define LIS2DE_ON_MESSAGE	'1'
#define LIS2DE_OFF_MESSAGE	'2'
#define LIS3MDL_ON_MESSAGE	'3'
#define LIS3MDL_OFF_MESSAGE '4'
#define BME280_ON_MESSAGE	'5'
#define BME280_OFF_MESSAGE	'6'
#define VEML6075_ON_MESSAGE	'7'
#define VEML6075_OFF_MESSAGE '8'
#define SI1153_ON_MESSAGE	'9'
#define SI1153_OFF_MESSAGE	'a'



void send_ble_data(ble_nus_t m_nus,uint8_t * data, uint8_t length);




#ifdef __cplusplus
}
#endif

#endif // LIS2DE_H