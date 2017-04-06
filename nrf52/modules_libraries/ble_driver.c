
#include "ble_nus.h"



void send_ble_data(ble_nus_t m_nus,uint8_t *data, uint8_t length){
    //static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    //static uint8_t index = 0;

    ble_nus_string_send(&m_nus, data, length);

    //err_code = ble_nus_string_send(&m_nus, data_array, index);

    //nrf_delay_ms(5000);
}