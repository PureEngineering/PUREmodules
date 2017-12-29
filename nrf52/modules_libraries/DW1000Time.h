#ifndef DW1000TIME_H
#define DW1000TIME_H


#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_drv_twi.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "ble_nus.h"

// Time resolution in micro-seconds of time based registers/values.
// Each bit in a timestamp counts for a period of approx. 15.65ps
#define TIME_RES 0.000015650040064103f
#define TIME_RES_INV 63897.6f

// Speed of radio waves [m/s] * timestamp resolution [~15.65ps] of DW1000
#define DISTANCE_OF_RADIO 0.0046917639786159f

// time stamp byte length
#define LEN_STAMP 5



struct DW1000Time_Value
	{
		long long int timestamp;
	} DW1000Time_value;


	// time factors (relative to [us]) for setting delayed transceive
	static const float SECONDS = 1e6;
	static const float MILLISECONDS = 1e3;
	static const float MICROSECONDS = 1;
	static const float NANOSECONDS = 1e-3;

	// timer/counter overflow (40 bits)
	static const long long unsigned int TIME_OVERFLOW = 1099511627776;

	void DW1000Time_init();
	void DW1000Time_setTime(float timeUs);

	long long int DW1000Time_getTimestamp();
	void DW1000Time_setTimestamp(uint8_t data[]);

	float DW1000Time_getAsFloat();
	void DW1000Time_getAsBytes(uint8_t data[]);
	float DW1000Time_getAsMeters();

	
	void DW1000Time_assign(long long int assign);
	void DW1000Time_add(long long int add);
	void DW1000Time_sub(long long int sub); 
	void DW1000Time_mult(float mult);
	void DW1000Time_div(float factor);
	bool DW1000Time_cmp(long long int cmp);


#ifdef __cplusplus
}
#endif

#endif // DW1000Time
