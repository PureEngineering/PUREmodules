/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net>
 * Decawave DW1000 library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file DW1000.h
 * Arduino driver library (header file) for the Decawave DW1000 UWB transceiver IC.
 * 
 * @todo
 * - impl: later:
 * - TXBOFFS in TX_FCTRL for offset buffer transmit
 * - TR in TX_FCTRL for flagging for ranging messages
 * - CANSFCS in SYS_CTRL to cancel frame check suppression
 * - HSRBP in SYS_CTRL to determine in double buffered mode from which buffer to read
 */

/*
*   All arduino dependencies and code changes for use with nrf52
*
*/


#ifndef DW1000_H
#define DW1000_H

/*
#ifdef __cplusplus
extern "C++" {
#endif*/

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "DW1000Time.h"
#include "nrf_drv_spi.h"


#define LEN_STAMP 5
// enum to determine RX or TX mode of device
#define IDLE_MODE 0x00
#define RX_MODE 0x01
#define TX_MODE 0x02

// used for SPI ready w/o actual writes
#define JUNK 0x00

// no sub-address for register write
#define NO_SUB 0x00

// device id register
#define DEV_ID 0x00
#define LEN_DEV_ID 4

// extended unique identifier register
#define EUI 0x01
#define LEN_EUI 8

// PAN identifier, short address register
#define PANADR 0x03
#define LEN_PANADR 4

// device configuration register
#define SYS_CFG 0x04
#define LEN_SYS_CFG 4
#define FFEN_BIT 0
#define FFBC_BIT 1
#define FFAB_BIT 2
#define FFAD_BIT 3
#define FFAA_BIT 4
#define FFAM_BIT 5
#define FFAR_BIT 6
#define FFEN_BIT 0
#define DIS_DRXB_BIT 12
#define DIS_STXP_BIT 18
#define HIRQ_POL_BIT 9
#define RXAUTR_BIT 29
#define PHR_MODE_SUB 16
#define LEN_PHR_MODE_SUB 2
#define RXM110K_BIT 22

// device control register
#define SYS_CTRL 0x0D
#define LEN_SYS_CTRL 4
#define SFCST_BIT 0
#define TXSTRT_BIT 1
#define TXDLYS_BIT 2
#define TRXOFF_BIT 6
#define WAIT4RESP_BIT 7
#define RXENAB_BIT 8
#define RXDLYS_BIT 9

// system event status register
#define SYS_STATUS 0x0F
#define LEN_SYS_STATUS 5
#define CPLOCK_BIT 1
#define AAT_BIT 3
#define TXFRB_BIT 4
#define TXPRS_BIT 5
#define TXPHS_BIT 6
#define TXFRS_BIT 7
#define LDEDONE_BIT 10
#define RXPHE_BIT 12
#define RXDFR_BIT 13
#define RXFCG_BIT 14
#define RXFCE_BIT 15
#define RXRFSL_BIT 16
#define RXRFTO_BIT 17
#define LDEERR_BIT 18
#define RFPLL_LL_BIT 24
#define CLKPLL_LL_BIT 25

// system event mask register
// NOTE: uses the bit definitions of SYS_STATUS (below 32)
#define SYS_MASK 0x0E
#define LEN_SYS_MASK 4

// system time counter
#define SYS_TIME 0x06
#define LEN_SYS_TIME LEN_STAMP

// RX timestamp register
#define RX_TIME 0x15
#define LEN_RX_TIME 14
#define RX_STAMP_SUB 0x00
#define FP_AMPL1_SUB 0x07
#define LEN_RX_STAMP LEN_STAMP
#define LEN_FP_AMPL1 2

// RX frame quality
#define RX_FQUAL 0x12
#define LEN_RX_FQUAL 8
#define STD_NOISE_SUB 0x00
#define FP_AMPL2_SUB 0x02
#define FP_AMPL3_SUB 0x04
#define CIR_PWR_SUB 0x06
#define LEN_STD_NOISE 2
#define LEN_FP_AMPL2 2
#define LEN_FP_AMPL3 2
#define LEN_CIR_PWR 2

// TX timestamp register
#define TX_TIME 0x17
#define LEN_TX_TIME 10
#define TX_STAMP_SUB 0
#define LEN_TX_STAMP LEN_STAMP

// timing register (for delayed RX/TX)
#define DX_TIME 0x0A
#define LEN_DX_TIME LEN_STAMP

// transmit data buffer
#define TX_BUFFER 0x09
#define LEN_TX_BUFFER 1024
#define LEN_UWB_FRAMES 127
#define LEN_EXT_UWB_FRAMES 1023

// RX frame info
#define RX_FINFO 0x10
#define LEN_RX_FINFO 4

// receive data buffer
#define RX_BUFFER 0x11
#define LEN_RX_BUFFER 1024

// transmit control
#define TX_FCTRL 0x08
#define LEN_TX_FCTRL 5

// channel control
#define CHAN_CTRL 0x1F
#define LEN_CHAN_CTRL 4
#define DWSFD_BIT 17
#define TNSSFD_BIT 20
#define RNSSFD_BIT 21

// user-defined SFD
#define USR_SFD 0x21
#define LEN_USR_SFD 41
#define SFD_LENGTH_SUB 0x00
#define LEN_SFD_LENGTH 1

// OTP control (for LDE micro code loading only)
#define OTP_IF 0x2D
#define OTP_ADDR_SUB 0x04
#define OTP_CTRL_SUB 0x06
#define OTP_RDAT_SUB 0x0A
#define LEN_OTP_ADDR 2
#define LEN_OTP_CTRL 2
#define LEN_OTP_RDAT 4

// AGC_TUNE1/2 (for re-tuning only)
#define AGC_TUNE 0x23
#define AGC_TUNE1_SUB 0x04
#define AGC_TUNE2_SUB 0x0C
#define AGC_TUNE3_SUB 0x12
#define LEN_AGC_TUNE1 2
#define LEN_AGC_TUNE2 4
#define LEN_AGC_TUNE3 2

// DRX_TUNE2 (for re-tuning only)
#define DRX_TUNE 0x27
#define DRX_TUNE0b_SUB 0x02
#define DRX_TUNE1a_SUB 0x04
#define DRX_TUNE1b_SUB 0x06
#define DRX_TUNE2_SUB 0x08
#define DRX_TUNE4H_SUB 0x26
#define LEN_DRX_TUNE0b 2
#define LEN_DRX_TUNE1a 2
#define LEN_DRX_TUNE1b 2
#define LEN_DRX_TUNE2 4
#define LEN_DRX_TUNE4H 2

// LDE_CFG1 (for re-tuning only)
#define LDE_IF 0x2E
#define LDE_CFG1_SUB 0x0806
#define LDE_RXANTD_SUB 0x1804
#define LDE_CFG2_SUB 0x1806
#define LDE_REPC_SUB 0x2804
#define LEN_LDE_CFG1 1
#define LEN_LDE_CFG2 2
#define LEN_LDE_REPC 2
#define LEN_LDE_RXANTD 2

// TX_POWER (for re-tuning only)
#define TX_POWER 0x1E
#define LEN_TX_POWER 4

// RF_CONF (for re-tuning only)
#define RF_CONF 0x28
#define RF_RXCTRLH_SUB 0x0B
#define RF_TXCTRL_SUB 0x0C
#define LEN_RF_RXCTRLH 1
#define LEN_RF_TXCTRL 4

// TX_CAL (for re-tuning only)
#define TX_CAL 0x2A
#define TC_PGDELAY_SUB 0x0B
#define LEN_TC_PGDELAY 1
#define TC_SARC 0x00
#define TC_SARL 0x03

// FS_CTRL (for re-tuning only)
#define FS_CTRL 0x2B
#define FS_PLLCFG_SUB 0x07
#define FS_PLLTUNE_SUB 0x0B
#define FS_XTALT_SUB 0x0E
#define LEN_FS_PLLCFG 4
#define LEN_FS_PLLTUNE 1
#define LEN_FS_XTALT 1

// AON
#define AON 0x2C
#define AON_WCFG_SUB 0x00
#define LEN_AON_WCFG 2
#define ONW_LDC_BIT 6
#define ONW_LDD0_BIT 12
#define AON_CTRL_SUB 0x02
#define LEN_AON_CTRL 1
#define RESTORE_BIT 0
#define SAVE_BIT 1
#define UPL_CFG_BIT 2

#define AON_CFG0_SUB 0x06
#define LEN_AON_CFG0 4
#define SLEEP_EN_BIT 0
#define WAKE_PIN_BIT 1
#define WAKE_SPI_BIT 2
#define WAKE_CNT_BIT 3


// PMSC
#define PMSC 0x36
#define PMSC_CTRL0_SUB 0x00
#define PMSC_CTRL1_SUB 0x04
#define PMSC_LEDC_SUB 0x28
#define LEN_PMSC_CTRL0 4
#define LEN_PMSC_CTRL1 4
#define LEN_PMSC_LEDC 4
#define GPDCE_BIT 18
#define KHZCLKEN_BIT 23
#define BLNKEN 8

#define ATXSLP_BIT 11
#define ARXSLP_BIT 12


// TX_ANTD Antenna delays
#define TX_ANTD 0x18
#define LEN_TX_ANTD 2



class DW1000Class {
public:
	/* TODO impl: later
	 * - TXBOFFS in TX_FCTRL for offset buffer transmit
 	 * - TR in TX_FCTRL for flagging for ranging messages
	 * - CANSFCS in SYS_CTRL to cancel frame check suppression
	 * - HSRBP in SYS_CTRL to determine in double buffered mode from which buffer to read
	 */

	/* ##### Init ################################################################ */
	/** 
	Initiates and starts a sessions with one or more DW1000.

	@param[in] irq The interrupt line/pin that connects the Arduino.
	@param[in] rst The reset line/pin for hard resets of ICs that connect to the Arduino.
	*/
	static void begin(int irq, int rst);
	
	/** 
	Initiates and starts a sessions with one or more DW1000. Soft resets (i.e. command
	triggered) are used and it is assumed that no reset line is wired.

	@param[in] irq The interrupt line/pin that connects the Arduino.
	*/
	static void begin(int irq);

	/** 
	Selects a specific DW1000 chip for communication. In case of a single DW1000 chip in use
	this call only needs to be done once at start up, but is still mandatory. Other than a call
	to `reselect()` this function performs an initial setup of the now-selected chip.

	@param[in] ss The chip select line/pin that connects the to-be-selected chip with the
	Arduino.
	*/
	//static void select(int ss);

	/** 
	(Re-)selects a specific DW1000 chip for communication. In case of a single DW1000 chip in use
	this call is not needed; only a call to `select()` has to be performed once at start up. Other 
	than a call to `select()` this function does not perform an initial setup of the (again-)selected 
	chips and assumes it to have a valid configuration loaded.

	@param[in] ss The chip select line/pin that connects the to-be-selected chip with the
	Arduino.
	*/
	//static void reselect(int ss);

	/** 
	Tells the driver library that no communication to a DW1000 will be required anymore.
	This basically just frees SPI and the previously used pins.
	*/
	//static void end(nrf_drv_spi_t spi);

	/** 
	Resets all connected or the currently selected DW1000 chip. A hard reset of all chips
	is preferred, although a soft reset of the currently selected one is executed if no 
	reset pin has been specified (when using `begin(int)`, instead of `begin(int, int)`).
	*/
	//static void reset();

	/** 
	Resets the currently selected DW1000 chip programmatically (via corresponding commands).
	*/
	static void softReset();

	/* ##### Print device id, address, etc. ###################################### */
	/** 
	Generates a String representation of the device identifier of the chip. That usually 
	are the letters "DECA" plus the	version and revision numbers of the chip.

	@param[out] msgBuffer The String buffer to be filled with printable device information.
		Provide 128 bytes, this should be sufficient.
	*/
	static void getPrintableDeviceIdentifier(char msgBuffer[]);

	/** 
	Generates a String representation of the extended unique identifier (EUI) of the chip.

	@param[out] msgBuffer The String buffer to be filled with printable device information.
		Provide 128 bytes, this should be sufficient.
	*/
	static void getPrintableExtendedUniqueIdentifier(char msgBuffer[]);

	/** 
	Generates a String representation of the short address and network identifier currently
	defined for the respective chip.

	@param[out] msgBuffer The String buffer to be filled with printable device information.
		Provide 128 bytes, this should be sufficient.
	*/
	static void getPrintableNetworkIdAndShortAddress(char msgBuffer[]);

	/** 
	Generates a String representation of the main operational settings of the chip. This
	includes data rate, pulse repetition frequency, preamble and channel settings.

	@param[out] msgBuffer The String buffer to be filled with printable device information.
		Provide 128 bytes, this should be sufficient.
	*/
	static void getPrintableDeviceMode(char msgBuffer[]);

	/* ##### Device address management, filters ################################## */
	/** 
	(Re-)set the network identifier which the selected chip should be associated with. This
	setting is important for certain MAC address filtering rules.

	@param[in] val An arbitrary numeric network identifier.
	*/
	static void setNetworkId(unsigned int val);

	/** 
	(Re-)set the device address (i.e. short address) for the currently selected chip. This
	setting is important for certain MAC address filtering rules.

	@param[in] val An arbitrary numeric device address.
	*/
	static void setDeviceAddress(unsigned int val);
	// TODO EUI, MAC and filters
	
	/* ##### General device configuration ######################################## */
	/** 
	Specifies whether the DW1000 chip should, again, turn on its receiver in case that the
	last reception failed. 

	This setting is enabled as part of `setDefaults()` if the device is
	in idle mode.

	@param[in] val `true` to enable, `false` to disable receiver auto-reenable.
	*/
	static void setReceiverAutoReenable(bool val);
	
	/** 
	Specifies the interrupt polarity of the DW1000 chip. 

	As part of `setDefaults()` if the device is in idle mode, interrupt polarity is set to 
	active high.

	@param[in] val `true` for active high interrupts, `false` for active low interrupts.
	*/
	static void setInterruptPolarity(bool val);

	/** 
	Specifies whether to suppress any frame check measures while sending or receiving messages.
	If suppressed, no 2-byte checksum is appended to the message before sending and this 
	checksum is also not expected at receiver side. Note that when suppressing frame checks, 
	the error event handler	(attached via `attachReceiveErrorHandler()`) will not be triggered 
	if received data is corrupted.

	Frame checks are enabled as part of `setDefaults()` if the device is in idle mode.

	@param[in] val `true` to suppress frame check on sender and receiver side, `false` otherwise.
	*/
	static void suppressFrameCheck(bool val);

	/** 
	Specifies the data transmission rate of the DW1000 chip. One of the values
	- `TRX_RATE_110KBPS` (i.e. 110 kb/s)
	- `TRX_RATE_850KBPS` (i.e. 850 kb/s)
	- `TRX_RATE_6800KBPS` (i.e. 6.8 Mb/s)
	has to be provided.

	See `setDefaults()` and `enableMode()` for additional information on data rate settings.

	@param[in] rate The data transmission rate, encoded by the above defined constants.
	*/
	static void setDataRate(uint8_t rate);

	/** 
	Specifies the pulse repetition frequency (PRF) of data transmissions with the DW1000. Either
	- `TX_PULSE_FREQ_16MHZ` (i.e. 16 MHz)
	- `TX_PULSE_FREQ_64MHZ` (i.e. 64 MHz)
	has to be chosen.

	Note that the 16 MHz setting is more power efficient, while the 64 MHz setting requires more
	power, but also delivers slightly better transmission performance (i.e. on communication range and 
	timestamp accuracy) (see DWM1000 User Manual, section 9.3). 

	See `setDefaults()` and `enableMode()` for additional information on PRF settings.

	@param[in] freq The PRF, encoded by the above defined constants.
	*/
	static void setPulseFrequency(uint8_t freq);
	static void setPreambleLength(uint8_t prealen);
	static void setChannel(uint8_t channel);
	static void setPreambleCode(uint8_t preacode);
	static void useSmartPower(bool smartPower);

	/* transmit and receive configuration. */
	static DW1000Time setDelay(const DW1000Time& delay);
	static void receivePermanently(bool val);

	static void setData(uint8_t data[], uint16_t n);
	static void setData(const std::string& data); //remember to check this
	static void getData(uint8_t data[], uint16_t n);
	static void getData(std::string& data);  //remember to check this

	static unsigned int getDataLength();
	static void getTransmitTimestamp(DW1000Time& time);
	static void getReceiveTimestamp(DW1000Time& time);
	static void getSystemTimestamp(DW1000Time& time);
	static void getTransmitTimestamp(uint8_t data[]);
	static void getReceiveTimestamp(uint8_t data[]);
	static void getSystemTimestamp(uint8_t data[]);

	/* receive quality information. */
	static float getReceivePower();
	static float getFirstPathPower();
	static float getReceiveQuality();

	/* interrupt management. */
	static void interruptOnSent(bool val);
	static void interruptOnReceived(bool val);
	static void interruptOnReceiveFailed(bool val);
	static void interruptOnReceiveTimeout(bool val);
	static void interruptOnReceiveTimestampAvailable(bool val);
	static void interruptOnAutomaticAcknowledgeTrigger(bool val);

	/* callback handler management. */
	static void attachErrorHandler(void (*handleError)(void)) {
		_handleError = handleError;
	}
	static void attachSentHandler(void (*handleSent)(void)) {
		_handleSent = handleSent;
	}
	static void attachReceivedHandler(void (*handleReceived)(void)) {
		_handleReceived = handleReceived;
	}
	static void attachReceiveFailedHandler(void (*handleReceiveFailed)(void)) {
		_handleReceiveFailed = handleReceiveFailed;
	}
	static void attachReceiveTimeoutHandler(void (*handleReceiveTimeout)(void)) {
		_handleReceiveTimeout = handleReceiveTimeout;
	}
	static void attachReceiveTimestampAvailableHandler(
			void (*handleReceiveTimestampAvailable)(void)) {
		_handleReceiveTimestampAvailable = handleReceiveTimestampAvailable;
	}

	/* device state management. */
	// idle state
	static void idle();

	// general configuration state
	static void newConfiguration();
	static void commitConfiguration();

	// reception state
	static void newReceive();
	static void startReceive();

	// transmission state
	static void newTransmit();
	static void startTransmit();




	//Application Functions/////////////////////////
	static void printDeviceData();




	/* ##### Operation mode selection ############################################ */
	/** 
	Specifies the mode of operation for the DW1000. Modes of operation are pre-defined
	combinations of data rate, pulse repetition frequency, preamble and channel settings
	that properly go together. If you simply want the chips to work, choosing a mode is 
	preferred over manual configuration.

	The following modes are pre-configured and one of them needs to be chosen:
	- `MODE_LONGDATA_RANGE_LOWPOWER` (basically this is 110 kb/s data rate, 16 MHz PRF and long preambles)
	- `MODE_SHORTDATA_FAST_LOWPOWER` (basically this is 6.8 Mb/s data rate, 16 MHz PRF and short preambles)
	- `MODE_LONGDATA_FAST_LOWPOWER` (basically this is 6.8 Mb/s data rate, 16 MHz PRF and long preambles)
	- `MODE_SHORTDATA_FAST_ACCURACY` (basically this is 6.8 Mb/s data rate, 64 MHz PRF and short preambles)
	- `MODE_LONGDATA_FAST_ACCURACY` (basically this is 6.8 Mb/s data rate, 64 MHz PRF and long preambles)
	- `MODE_LONGDATA_RANGE_ACCURACY` (basically this is 110 kb/s data rate, 64 MHz PRF and long preambles)

	Note that LOWPOWER and ACCURACY refers to the better power efficiency and improved transmission performance
	of 16 MHZ and 64 MHZ PRF respectively (see `setPulseFrequency()`).

	The default setting that is selected by `setDefaults()` is MODE_LONGDATA_RANGE_LOWPOWER.

	@param[in] mode The mode of operation, encoded by the above defined constants.
	*/
	static void enableMode(const uint8_t mode[]);

	// use RX/TX specific and general default settings
	static void setDefaults();

	/* debug pretty print registers. */
	static void getPrettyBytes(nrf_drv_spi_t m_spi, uint8_t cmd, uint16_t offset, char msgBuffer[], unsigned int n);
	static void getPrettyBytes(nrf_drv_spi_t m_spi, uint8_t data[], char msgBuffer[], unsigned int n);

	// transmission/reception bit rate
	static const uint8_t TRX_RATE_110KBPS = 0x00;
	static const uint8_t TRX_RATE_850KBPS = 0x01;
	static const uint8_t TRX_RATE_6800KBPS = 0x02;

	// transmission pulse frequency
	// 0x00 is 4MHZ, but receiver in DW1000 does not support it (!??)
	static const uint8_t TX_PULSE_FREQ_16MHZ = 0x01; 
	static const uint8_t TX_PULSE_FREQ_64MHZ = 0x02;

	// preamble length (PE + TXPSR bits)
	static const uint8_t TX_PREAMBLE_LEN_64 = 0x01;
	static const uint8_t TX_PREAMBLE_LEN_128 = 0x05;
	static const uint8_t TX_PREAMBLE_LEN_256 = 0x09;
	static const uint8_t TX_PREAMBLE_LEN_512 = 0x0D;
	static const uint8_t TX_PREAMBLE_LEN_1024 = 0x02;
	static const uint8_t TX_PREAMBLE_LEN_1536 = 0x06;
	static const uint8_t TX_PREAMBLE_LEN_2048 = 0x0A;
	static const uint8_t TX_PREAMBLE_LEN_4096 = 0x03;

	// PAC size. */
	static const uint8_t PAC_SIZE_8 = 8;
	static const uint8_t PAC_SIZE_16 = 16;
	static const uint8_t PAC_SIZE_32 = 32;
	static const uint8_t PAC_SIZE_64 = 64;

	/* channel of operation. */
	static const uint8_t CHANNEL_1 = 1;
	static const uint8_t CHANNEL_2 = 2;
	static const uint8_t CHANNEL_3 = 3;
	static const uint8_t CHANNEL_4 = 4;
	static const uint8_t CHANNEL_5 = 5;
	static const uint8_t CHANNEL_7 = 7;

	/* preamble codes. */
	static const uint8_t PREAMBLE_CODE_16MHZ_1 = 1;
	static const uint8_t PREAMBLE_CODE_16MHZ_2 = 2;
	static const uint8_t PREAMBLE_CODE_16MHZ_3 = 3;
	static const uint8_t PREAMBLE_CODE_16MHZ_4 = 4;
	static const uint8_t PREAMBLE_CODE_16MHZ_5 = 5;
	static const uint8_t PREAMBLE_CODE_16MHZ_6 = 6;
	static const uint8_t PREAMBLE_CODE_16MHZ_7 = 7;
	static const uint8_t PREAMBLE_CODE_16MHZ_8 = 8;
	static const uint8_t PREAMBLE_CODE_64MHZ_9 = 9;
	static const uint8_t PREAMBLE_CODE_64MHZ_10 = 10;
	static const uint8_t PREAMBLE_CODE_64MHZ_11 = 11;
	static const uint8_t PREAMBLE_CODE_64MHZ_12 = 12;
	static const uint8_t PREAMBLE_CODE_64MHZ_17 = 17;
	static const uint8_t PREAMBLE_CODE_64MHZ_18 = 18;
	static const uint8_t PREAMBLE_CODE_64MHZ_19 = 19;
	static const uint8_t PREAMBLE_CODE_64MHZ_20 = 20;

	/* frame length settings. */
	static const uint8_t FRAME_LENGTH_NORMAL = 0x00;
	static const uint8_t FRAME_LENGTH_EXTENDED = 0x03;

	/* pre-defined modes of operation (3 bytes for data rate, pulse frequency and 
	preamble length). */
	static const uint8_t MODE_LONGDATA_RANGE_LOWPOWER[];
	static const uint8_t MODE_SHORTDATA_FAST_LOWPOWER[];
	static const uint8_t MODE_LONGDATA_FAST_LOWPOWER[];
	static const uint8_t MODE_SHORTDATA_FAST_ACCURACY[];
	static const uint8_t MODE_LONGDATA_FAST_ACCURACY[];
	static const uint8_t MODE_LONGDATA_RANGE_ACCURACY[];

private:
	/* chip select, reset and interrupt pins. */
	static unsigned int _ss;
	static unsigned int _rst;
	static unsigned int _irq;

	/* callbacks. */
	static void (*_handleError)(void);
	static void (*_handleSent)(void);
	static void (*_handleReceived)(void);
	static void (*_handleReceiveFailed)(void);
	static void (*_handleReceiveTimeout)(void);
	static void (*_handleReceiveTimestampAvailable)(void);

	/* register caches. */
	static uint8_t _syscfg[LEN_SYS_CFG];
	static uint8_t _sysctrl[LEN_SYS_CTRL];
	static uint8_t _sysstatus[LEN_SYS_STATUS];
	static uint8_t _txfctrl[LEN_TX_FCTRL];
	static uint8_t _sysmask[LEN_SYS_MASK];
	static uint8_t _chanctrl[LEN_CHAN_CTRL];

	/* PAN and short address. */
	static uint8_t _networkAndAddress[LEN_PANADR];

	/* internal helper that guide tuning the chip. */
	static bool _smartPower;
	static uint8_t _extendedFrameLength;
	static uint8_t _preambleCode;
	static uint8_t _channel;
	static uint8_t _preambleLength;
	static uint8_t _pulseFrequency;
	static uint8_t _dataRate;
	static uint8_t _pacSize;
	static DW1000Time _antennaDelay;

	/* internal helper to remember how to properly act. */
	static bool _permanentReceive;
	static bool _frameCheck;

	// whether RX or TX is active
	static int _deviceMode;

	/* Arduino interrupt handler */
	static void handleInterrupt();

	/* not yet implemented/considered settings - hence private. */
	// TODO implement MAC, addressing and auto-acknowledge
	static void setFrameFilter(bool val);
	// note: not sure if going to be implemented for now
	static void setDoubleBuffering(bool val);
	// TODO is implemented, but needs testing
	static void useExtendedFrameLength(bool val);
	// TODO is implemented, but needs testing	
	static void waitForResponse(bool val);

	/* tuning according to mode. */
	static void tune();

	/* device status flags */
	static bool isReceiveTimestampAvailable();
	static bool isTransmitDone();
	static bool isReceiveDone();
	static bool isReceiveFailed();
	static bool isReceiveTimeout();
	static bool isClockProblem();

	/* interrupt state handling */
	static void clearInterrupts();
	static void clearAllStatus();
	static void clearReceiveStatus();
	static void clearReceiveTimestampAvailableStatus();
	static void clearTransmitStatus();

	/* internal helper to read/write system registers. */
	static void readSystemEventStatusRegister();
	static void readSystemConfigurationRegister();
	static void writeSystemConfigurationRegister();
	static void readNetworkIdAndDeviceAddress();
	static void writeNetworkIdAndDeviceAddress();
	static void readSystemEventMaskRegister();
	static void writeSystemEventMaskRegister();
	static void readChannelControlRegister();
	static void writeChannelControlRegister();
	static void readTransmitFrameControlRegister();
	static void writeTransmitFrameControlRegister();

	/* clock management. */
	static void enableClock(uint8_t clock);

	/* LDE micro-code management. */
	static void manageLDE();

	/* reading and writing bytes from and to DW1000 module. */
	static void readBytes(nrf_drv_spi_t spi, uint8_t cmd, uint16_t offset, uint8_t data[], unsigned int n);
	static void readBytesOTP(nrf_drv_spi_t spi, uint16_t address, uint8_t data[]);
	static void writeBytes(nrf_drv_spi_t spi, uint8_t cmd, uint16_t offset, uint8_t data[], unsigned int n);

	/* writing numeric values to bytes. */
	static void writeValueToBytes(uint8_t data[], long val, unsigned int n); 

	/* internal helper for bit operations on multi-bytes. */
	static bool getBit(uint8_t data[], unsigned int n, unsigned int bit);
	static void setBit(uint8_t data[], unsigned int n, unsigned int bit, bool val);
	
	/* Register is 6 bit, 7 = write, 6 = sub-adressing, 5-0 = register value
	 * Total header with sub-adressing can be 15 bit. */
	static const uint8_t WRITE = 0x80; // regular write
	static const uint8_t WRITE_SUB = 0xC0; // write with sub address
	static const uint8_t READ = 0x00; // regular read
	static const uint8_t READ_SUB = 0x40; // read with sub address

	/* clocks available. */
	static const uint8_t AUTO_CLOCK = 0x00;
	static const uint8_t XTI_CLOCK = 0x01;
	static const uint8_t PLL_CLOCK = 0x02;

	/* SPI configs. */
	//static const SPISettings _fastSPI;
	//static const SPISettings _slowSPI;
	//static const SPISettings* _currentSPI;
};

extern DW1000Class DW1000;

#endif // DW1000Time
