#include "nrf_drv_spi.h"


 #define SPI_INSTANCE  2 //< SPI instance index. 

 static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  //< SPI instance. 
 static volatile bool spi_xfer_done;  //< Flag used to indicate that SPI instance completed the transfer. 
