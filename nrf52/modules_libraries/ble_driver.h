#ifndef BLE__DRIVER_H
#define BLE__DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif


#include "ble_nus.h"


void send_ble_data(ble_nus_t m_nus,uint8_t * data, uint8_t length);
void send_ble_data2(ble_nus_t m_nus,uint8_t * data, uint16_t length);



#ifdef __cplusplus
}
#endif

#endif // LIS2DE_H