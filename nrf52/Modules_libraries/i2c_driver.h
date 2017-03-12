#ifndef I2C_INTERRUPT_H
#define I2C_INTERRUPT_H

 void write_byte(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress, uint8_t data);
 void write_2bytes(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress, uint8_t data_LSB, uint8_t data_MSB);
 uint8_t read_byte(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress);
 uint16_t read_2bytes(nrf_drv_twi_t twi_master,uint8_t addr, uint8_t subAddress);
#endif 
