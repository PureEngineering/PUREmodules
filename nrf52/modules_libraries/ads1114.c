#include "ads1114.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_twi.h"
#include "bsp.h"
#include "i2c_driver.h"
#include "ble_driver.h"
#include "ble_nus.h"
#include "nrf_delay.h"




void ads1114_setup(){

  Ads1114_settings.i2cAddress = ADS1114_DEVICE_ADDRESS;
  Ads1114_settings.conversionDelay = ADS1114_CONVERSIONDELAY;
  Ads1114_settings.bitShift = 0;
  Ads1114_settings.gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}

void ads1114_begin() {
  ads1114_setup();
}

void ads1114_setGain(adsGain_t gain)
{
  Ads1114_settings.gain = gain;
}


adsGain_t ads1114_getGain()
{
  return Ads1114_settings.gain;
}

uint16_t ads1114_readADC_SingleEnded(nrf_drv_twi_t twi_master,uint8_t channel) {
  if (channel > 3)
  {
    return 0;
  }
  
  // Start with default values
  uint16_t config = ADS1114_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1114_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1114_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1114_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1114_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1114_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= Ads1114_settings.gain;

  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1114_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  write_byte(twi_master, Ads1114_settings.i2cAddress,ADS1114_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  nrf_delay_ms(Ads1114_settings.conversionDelay);

  // Read the conversion results
  return read_byte(twi_master, Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_CONVERT) >> Ads1114_settings.bitShift;  
}

int16_t ads1114_readADC_Differential_0_1(nrf_drv_twi_t twi_master) {
  // Start with default values
  uint16_t config = ADS1114_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1114_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1114_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1114_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1114_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1114_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= Ads1114_settings.gain;
                    
  // Set channels
  config |= ADS1114_REG_CONFIG_MUX_DIFF_0_1;          // AIN0 = P, AIN1 = N

  // Set 'start single-conversion' bit
  config |= ADS1114_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  write_byte(twi_master, Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  nrf_delay_ms(Ads1114_settings.conversionDelay);

  // Read the conversion results
  uint16_t res = read_byte(twi_master,Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_CONVERT) >> Ads1114_settings.bitShift;
  if (Ads1114_settings.bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1114,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}

int16_t ads1114_readADC_Differential_2_3(nrf_drv_twi_t twi_master) {
  // Start with default values
  uint16_t config = ADS1114_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1114_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1114_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1114_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1114_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1114_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= Ads1114_settings.gain;

  // Set channels
  config |= ADS1114_REG_CONFIG_MUX_DIFF_2_3;          // AIN2 = P, AIN3 = N

  // Set 'start single-conversion' bit
  config |= ADS1114_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  write_byte(twi_master,Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  nrf_delay_ms(Ads1114_settings.conversionDelay);

  // Read the conversion results
  uint16_t res = read_byte(twi_master,Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_CONVERT) >> Ads1114_settings.bitShift;
  if (Ads1114_settings.bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1114,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}

void ads1114_startComparator_SingleEnded(nrf_drv_twi_t twi_master,uint8_t channel, int16_t threshold)
{
  // Start with default values
  uint16_t config = ADS1114_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
                    ADS1114_REG_CONFIG_CLAT_LATCH   | // Latching mode
                    ADS1114_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1114_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1114_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1114_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
                    ADS1114_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

  // Set PGA/voltage range
  config |= Ads1114_settings.gain;
                    
  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1114_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set the high threshold register
  // Shift 12-bit results left 4 bits for the ADS1114
  write_byte(twi_master,Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_HITHRESH, threshold << Ads1114_settings.bitShift);

  // Write config register to the ADC
  write_byte(twi_master, Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_CONFIG, config);
}


int16_t ads1114_getLastConversionResults(nrf_drv_twi_t twi_master)
{
  // Wait for the conversion to complete
  nrf_delay_ms(Ads1114_settings.conversionDelay);

  // Read the conversion results
  uint16_t res = read_byte(twi_master, Ads1114_settings.i2cAddress, ADS1114_REG_POINTER_CONVERT) >> Ads1114_settings.bitShift;
  if (Ads1114_settings.bitShift == 0)
  {
    return (int16_t)res;
  }
  else
  {
    // Shift 12-bit results right 4 bits for the ADS1114,
    // making sure we keep the sign bit intact
    if (res > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}

