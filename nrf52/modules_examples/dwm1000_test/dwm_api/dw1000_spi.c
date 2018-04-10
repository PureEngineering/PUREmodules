#include "dw1000_spi.h"

#include "nrf_drv_spi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "bsp.h"
#include "ble_driver.h"
#include "ble_nus.h"


int SPI_Init(void){
    //TODO:
    return -1;
}


void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    spi_xfer_done = true;
}



uint8_t spiRead(uint8_t* rx_data,uint8_t* length)
{
    
    //uint8_t address = addr & 0x7F;
    uint8_t tx_data[SPI_MAX_LENGTH];
    uint8_t rx_length = *length;
    spi_xfer_done = false;

    if(length == 0){
        return TRANSFER_OK;
    }

    if (rx_length > SPI_MAX_LENGTH){
        return TRANSFER_ERR;
    }

    memset(tx_data, 0xff,rx_length);

    spi_xfer_done = false;

    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, tx_data, rx_length, rx_data, rx_length));


    while (!spi_xfer_done)
    {
        __WFE();
    }

    return TRANSFER_OK;
}



int SPI_waitforRX(ble_nus_t m_nus,uint8_t* data, uint16_t* length, uint16_t exp_length){
    uint8_t lengthble = 32;
    uint8_t *ble_string[lengthble];
    uint8_t len_header = 1;
    uint8_t header[len_header];
    memset(header,0,len_header);
    
    sprintf((char *)ble_string, "inside wait \r\n");
    send_ble_data(m_nus,(uint8_t *)ble_string,lengthble);

    while(header[0]==0x00){
      spiRead(header,&len_header);
    }

    *length = 0;
    spiRead(data, header);
    *length += header[0];


    sprintf((char *)ble_string, "length: %d expected: %d \r\n",*length, exp_length);
    send_ble_data2(m_nus,(uint8_t *)ble_string,lengthble);

    return -1;

}


uint8_t spiWrite(ble_nus_t m_nus, uint8_t* tx_data,uint8_t* length)
{
    //uint8_t address = addr | 0x80;
    uint8_t rx_data[SPI_MAX_LENGTH];
    uint8_t tx_length = *length;
    //uint16_t str_len = 0;


    uint8_t lengthble = 15;
    uint8_t *ble_string[lengthble];



    if(tx_length == 0){
        return TRANSFER_OK;
    }

    if(tx_length > SPI_MAX_LENGTH){
        return TRANSFER_ERR;
    }

    spi_xfer_done = false;

    sprintf((char *)ble_string, "before write \r\n");
    send_ble_data(m_nus,(uint8_t *)ble_string,lengthble);


    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, tx_data, tx_length, rx_data, tx_length));


    sprintf((char *)ble_string, "after write %d \r\n",spi_xfer_done);
    send_ble_data(m_nus,(uint8_t *)ble_string,lengthble);

    //while (!spi_xfer_done)
    //{
    //    __WFE();
    //}



    sprintf((char *)ble_string, "return  \r\n");
    send_ble_data(m_nus,(uint8_t *)ble_string,lengthble);
    return TRANSFER_OK;
}

