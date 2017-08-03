#ifndef ADS1114_H
#define ADS1114_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"


#define ADS1114_DEVICE_ADDRESS          (0x49)    //(ADDR = VCC)

#define ADS1114_CONVERSIONDELAY         (8)       // in mS

#define ADS1114_REG_POINTER_MASK        (0x03)
#define ADS1114_REG_POINTER_CONVERT     (0x00)
#define ADS1114_REG_POINTER_CONFIG      (0x01)
#define ADS1114_REG_POINTER_LOWTHRESH   (0x02)
#define ADS1114_REG_POINTER_HITHRESH    (0x03)

#define ADS1114_REG_CONFIG_OS_MASK      (0x8000)
#define ADS1114_REG_CONFIG_OS_SINGLE    (0x8000)  // Write: Set to start a single-conversion
#define ADS1114_REG_CONFIG_OS_BUSY      (0x0000)  // Read: Bit = 0 when conversion is in progress
#define ADS1114_REG_CONFIG_OS_NOTBUSY   (0x8000)  // Read: Bit = 1 when device is not performing a conversion

#define ADS1114_REG_CONFIG_MUX_MASK     (0x7000)
#define ADS1114_REG_CONFIG_MUX_DIFF_0_1 (0x0000)  // Differential P = AIN0, N = AIN1 (default)
#define ADS1114_REG_CONFIG_MUX_DIFF_0_3 (0x1000)  // Differential P = AIN0, N = AIN3
#define ADS1114_REG_CONFIG_MUX_DIFF_1_3 (0x2000)  // Differential P = AIN1, N = AIN3
#define ADS1114_REG_CONFIG_MUX_DIFF_2_3 (0x3000)  // Differential P = AIN2, N = AIN3
#define ADS1114_REG_CONFIG_MUX_SINGLE_0 (0x4000)  // Single-ended AIN0
#define ADS1114_REG_CONFIG_MUX_SINGLE_1 (0x5000)  // Single-ended AIN1
#define ADS1114_REG_CONFIG_MUX_SINGLE_2 (0x6000)  // Single-ended AIN2
#define ADS1114_REG_CONFIG_MUX_SINGLE_3 (0x7000)  // Single-ended AIN3

#define ADS1114_REG_CONFIG_PGA_MASK     (0x0E00)
#define ADS1114_REG_CONFIG_PGA_6_144V   (0x0000)  // +/-6.144V range = Gain 2/3
#define ADS1114_REG_CONFIG_PGA_4_096V   (0x0200)  // +/-4.096V range = Gain 1
#define ADS1114_REG_CONFIG_PGA_2_048V   (0x0400)  // +/-2.048V range = Gain 2 (default)
#define ADS1114_REG_CONFIG_PGA_1_024V   (0x0600)  // +/-1.024V range = Gain 4
#define ADS1114_REG_CONFIG_PGA_0_512V   (0x0800)  // +/-0.512V range = Gain 8
#define ADS1114_REG_CONFIG_PGA_0_256V   (0x0A00)  // +/-0.256V range = Gain 16

#define ADS1114_REG_CONFIG_MODE_MASK    (0x0100)
#define ADS1114_REG_CONFIG_MODE_CONTIN  (0x0000)  // Continuous conversion mode
#define ADS1114_REG_CONFIG_MODE_SINGLE  (0x0100)  // Power-down single-shot mode (default)

#define ADS1114_REG_CONFIG_DR_MASK      (0x00E0)  
#define ADS1114_REG_CONFIG_DR_128SPS    (0x0000)  // 128 samples per second
#define ADS1114_REG_CONFIG_DR_250SPS    (0x0020)  // 250 samples per second
#define ADS1114_REG_CONFIG_DR_490SPS    (0x0040)  // 490 samples per second
#define ADS1114_REG_CONFIG_DR_920SPS    (0x0060)  // 920 samples per second
#define ADS1114_REG_CONFIG_DR_1600SPS   (0x0080)  // 1600 samples per second (default)
#define ADS1114_REG_CONFIG_DR_2400SPS   (0x00A0)  // 2400 samples per second
#define ADS1114_REG_CONFIG_DR_3300SPS   (0x00C0)  // 3300 samples per second

#define ADS1114_REG_CONFIG_CMODE_MASK   (0x0010)
#define ADS1114_REG_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
#define ADS1114_REG_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator

#define ADS1114_REG_CONFIG_CPOL_MASK    (0x0008)
#define ADS1114_REG_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
#define ADS1114_REG_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active

#define ADS1114_REG_CONFIG_CLAT_MASK    (0x0004)  // Determines if ALERT/RDY pin latches once asserted
#define ADS1114_REG_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
#define ADS1114_REG_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator

#define ADS1114_REG_CONFIG_CQUE_MASK    (0x0003)
#define ADS1114_REG_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
#define ADS1114_REG_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
#define ADS1114_REG_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
#define ADS1114_REG_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)

typedef enum
{
  GAIN_TWOTHIRDS    = ADS1114_REG_CONFIG_PGA_6_144V,
  GAIN_ONE          = ADS1114_REG_CONFIG_PGA_4_096V,
  GAIN_TWO          = ADS1114_REG_CONFIG_PGA_2_048V,
  GAIN_FOUR         = ADS1114_REG_CONFIG_PGA_1_024V,
  GAIN_EIGHT        = ADS1114_REG_CONFIG_PGA_0_512V,
  GAIN_SIXTEEN      = ADS1114_REG_CONFIG_PGA_0_256V
} adsGain_t;



struct Ads1114_SensorSettings
{
   uint8_t   i2cAddress;
   uint8_t   conversionDelay;
   uint8_t   bitShift;
   adsGain_t gain;
} Ads1114_settings;


void ads1114_begin(void);
void ads1114_setup(void);

void      ads1114_setGain(adsGain_t gain);
adsGain_t ads1114_getGain(void);

uint16_t  ads1114_readADC_SingleEnded(nrf_drv_twi_t twi_master,uint8_t channel);
int16_t   ads1114_readADC_Differential_0_1(nrf_drv_twi_t twi_master);
int16_t   ads1114_readADC_Differential_2_3(nrf_drv_twi_t twi_master);
void      ads1114_startComparator_SingleEnded(nrf_drv_twi_t twi_master,uint8_t channel, int16_t threshold);
int16_t   ads1114_getLastConversionResults(nrf_drv_twi_t twi_master);



void run_ads1114(nrf_drv_twi_t twi_master);
void run_ads1114_ble(nrf_drv_twi_t twi_master,ble_nus_t m_nus);


#ifdef __cplusplus
}
#endif

#endif // ADS1114

