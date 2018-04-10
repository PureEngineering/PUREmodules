#ifndef DW1000SPI_H
#define DW1000SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_spi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


#define TRANSFER_OK                     0
#define TRANSFER_ERR                    1

#define SPI_INSTANCE  2 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */
#define SPI_MAX_LENGTH                  256	




//#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */
//static ble_nus_t                        m_nus;                                      /**< Structure to identify the Nordic UART Service. */
//static uint16_t                         m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current connection. */
//static ble_uuid_t                       m_adv_uuids[] = {{BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}};  /**< Universally unique service identifier. */



void spi_event_handler(nrf_drv_spi_evt_t const * p_event);
uint8_t spiRead(uint8_t* rx_data,uint8_t* length);
uint8_t spiWrite(ble_nus_t m_nus,uint8_t* tx_data,uint8_t* length);
int SPI_waitforRX(ble_nus_t m_nus,uint8_t* data, uint16_t* length, uint16_t exp_length);
int SPI_Init(void);



#ifdef __cplusplus
}
#endif

#endif // DW1000_SPI
