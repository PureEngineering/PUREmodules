#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "config.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "nrf.h"
#include "bsp.h"
#include "app_util_platform.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_delay.h"
#include "ble_nus.h"
#include "nrf_drv_spi.h"



#include "i2c_driver.h"
#include "ble_driver.h"
#include "lis2de.h"
#include "nrf_drv_timer.h"




const nrf_drv_timer_t TIMER_DATA = NRF_DRV_TIMER_INSTANCE(0);

/**
 * @brief TWI master instance
 *
 * Instance of TWI master driver that would be used for communication with simulated
 * eeprom memory.
 */
static const nrf_drv_twi_t m_twi_master = NRF_DRV_TWI_INSTANCE(MASTER_TWI_INST);

/**
 * @brief Initialize the master TWI
 *
 * Function used to initialize master TWI interface that would communicate with simulated EEPROM.
 *
 * @return NRF_SUCCESS or the reason of failure
 */
static ret_code_t twi_master_init(void)
{
    ret_code_t ret;
    const nrf_drv_twi_config_t config =
    {
       .scl                = TWI_SCL_M,  
       .sda                = TWI_SDA_M,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    ret = nrf_drv_twi_init(&m_twi_master, &config, NULL, NULL);

    if (NRF_SUCCESS == ret)
    {
        nrf_drv_twi_enable(&m_twi_master);
    }

    return ret;
}



#define SPI_INSTANCE  2 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

#define TEST_STRING "Nordic"
//static uint8_t       m_tx_buf[] = {0x08,0x00};           /**< TX buffer. */
//static uint8_t       m_rx_buf[sizeof(TEST_STRING) + 1];    /**< RX buffer. */
//static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */
uint8_t step = 1;
    uint8_t       Cmd_tx_buf[] = {0x15,0x00};           /**< TX buffer. */
    uint8_t       Cmd_rx_buf[2];    /**< RX buffer. */
    uint8_t       Cmd_length = 2;       /**< Transfer  Length */
    uint8_t       m_tx_buf[50];
    uint8_t       m_rx_buf[50];
    //uint8_t       m_length;


/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
    spi_xfer_done = true;
    if(step ==1){
        NRF_LOG_INFO("TransferSend completed.\r\n");
    //if (m_rx_buf[0] != 0)
    //{
        NRF_LOG_INFO(" Received: \r\n");
        NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
    //}
    }

    if(step==2){
        NRF_LOG_INFO(" Received: \r\n");
        NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf)); 
    }


}


/**
 * @brief Handler for timer events.
 */
void timer_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    static uint32_t i;
    uint32_t led_to_invert = ((i++) % LEDS_NUMBER);

    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            bsp_board_led_invert(led_to_invert);
            NRF_LOG_RAW_INFO("\r\nTimer Hit %d \r\n",i);
            //run_lis2de(m_twi_master);
            break;

        default:
            //Do nothing.
            NRF_LOG_RAW_INFO("\r\nTimer Hit inside %d \r\n",i);

            break;
    }
}

/**
 *  The begin of the journey
 */
int main(void)
{

    ret_code_t err_code;
    /* Initialization of UART */
    bsp_board_leds_init();

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));

    /* Initializing TWI master interface for EEPROM */
    err_code = twi_master_init();
    APP_ERROR_CHECK(err_code);


    /* Welcome message */
    NRF_LOG_RAW_INFO("\r\nSpi example\r\n");
    NRF_LOG_FLUSH();

    lis2de_pass(m_twi_master);


    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = SPI_SS_PIN;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin  = SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler));



/*
//  Timer Code if needed

    uint32_t time_ms = 5000; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_DATA, &timer_cfg, timer_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_DATA, time_ms);

    nrf_drv_timer_extended_compare(
         &TIMER_DATA, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER_DATA);*/

    //NRF_LOG_FLUSH();



    while (1){
        

        step =1;

        memset(Cmd_rx_buf, 0xff, 32);
        spi_xfer_done = false;

        NRF_LOG_RAW_INFO("CmdTX: ");
        for (int i = 0; i < Cmd_length; i++)
            {
                NRF_LOG_RAW_INFO("%x",Cmd_tx_buf[i]);
            }
        NRF_LOG_RAW_INFO("\r\n ");
        NRF_LOG_FLUSH();

 

        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, Cmd_tx_buf, Cmd_length, Cmd_rx_buf, 25));

        while (!spi_xfer_done)
        {
            __WFE();

        }

        NRF_LOG_FLUSH();

        NRF_LOG_RAW_INFO("CmdRX: ");
        for (int i = 0; i < 32; i++)
            {
                NRF_LOG_RAW_INFO("%x",Cmd_rx_buf[i]);
            }
        NRF_LOG_RAW_INFO("\r\n ");
        NRF_LOG_FLUSH();


        uint8_t read_len = 50;
        memset(m_rx_buf,0xff,read_len);
        spi_xfer_done = false;
        memset(m_tx_buf, 0xff,read_len);

        step = 2;


        //while(m_rx_buf[0]==0x00){
        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, m_tx_buf, read_len, m_rx_buf, read_len));
        //}

        while (!spi_xfer_done)
        {
            __WFE();

        }

        if(spi_xfer_done){
            for (int i = 0; i < read_len; i++)
            {
                NRF_LOG_RAW_INFO("%x",m_rx_buf[i]);
            }
            NRF_LOG_RAW_INFO("\r\n ");
            NRF_LOG_FLUSH();
        }

        nrf_delay_ms(1000);



    /*
        //__WFI();
        uint8_t c = NRF_LOG_GETCHAR();
        switch ((char)c)
        {
        case '\n':
        case '\r':
            NRF_LOG_RAW_INFO("You select a command\r\n"); 
            break;
        
        case '1':
            lis2de_init(m_twi_master);
            run_lis2de(m_twi_master);
            break;

        case '2':
            lis2de_powerdown(m_twi_master);
            break;

        case '3':
            //lis3mdl_init(m_twi_master);
            //run_lis3mdl(m_twi_master);    
            break;

        case '4':
            //lis3mdl_powerdown(m_twi_master);
            break;

        default:
            break;

        }
        NRF_LOG_FLUSH();*/
    }
}
