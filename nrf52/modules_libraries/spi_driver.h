#ifndef SPI_INTERRUPT_H
#define SPI_INTERRUPT_H

uint8_t spiRead(nrf_drv_spi_t spi,uint8_t addr);
void spiWrite(nrf_drv_spi_t spi,uint8_t addr, uint8_t data);
	


#endif 
