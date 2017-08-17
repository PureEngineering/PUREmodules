/* Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 * 
 *   4. This software must only be used in a processor manufactured by Nordic
 *   Semiconductor ASA, or in a processor manufactured by a third party that
 *   is used in combination with a processor manufactured by Nordic Semiconductor.
 * 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdint.h>
#include <string.h>
#include "ble_srv_common.h"
#include "app_error.h"
//#include "SEGGER_RTT.h"
#include "ble_fdcs.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

/**@brief Function for initiating our new service.
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
 // ALREADY_DONE_FOR_YOU: Declaration of a function that will take care of some housekeeping of ble connections related to our service and characteristic
 //This function is called by the SoftDevice when a BLE stack event has occurs. To keep our service up to date with the latest connection events we will 
 //call ble_our_service_on_ble_evt() from the dispatch function
void ble_fdc_service_on_ble_evt(ble_fdcs_t * p_our_service, ble_evt_t * p_ble_evt)
{
    // OUR_JOB: Step 3.D Implement switch case handling BLE events related to our service. 
	switch (p_ble_evt->header.evt_id)
	{
		case BLE_GAP_EVT_CONNECTED:
			p_our_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
			break;
		case BLE_GAP_EVT_DISCONNECTED:
			p_our_service->conn_handle = BLE_CONN_HANDLE_INVALID;
			break;
		default:
			// No implementation needed.
			break;
	}
}

/**@brief Function for adding our new characterstic to "Our service" that we initiated in the previous tutorial. 
 *
 * @param[in]   p_our_service        Our Service structure.
 *
 */
static uint32_t fdc_char_add(ble_fdcs_t * p_our_service)
{
    // OUR_JOB: Step 2.A, Add a custom characteristic UUID
	uint32_t			err_code;
	ble_uuid_t			char_uuid;
	ble_uuid128_t		base_uuid = BLE_UUID_FDC_BASE_UUID;
	char_uuid.uuid		= BLE_UUID_FDC_CHARACTERISTC_UUID;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    // OUR_JOB: Step 2.F Add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
	char_md.char_props.read = 1;
	char_md.char_props.write = 1;

    
    // OUR_JOB: Step 3.A, Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
	ble_gatts_attr_md_t cccd_md;
	memset(&cccd_md, 0, sizeof(cccd_md));
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
	char_md.p_cccd_md           = &cccd_md;
	char_md.char_props.notify   = 1;
   
    
    // OUR_JOB: Step 2.B, Configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));  
	attr_md.vloc = BLE_GATTS_VLOC_STACK;
    
    
    // OUR_JOB: Step 2.G, Set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    // OUR_JOB: Step 2.C, Configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
	attr_char_value.p_uuid 		= &char_uuid;
	attr_char_value.p_attr_md 	= &attr_md;
    
    // OUR_JOB: Step 2.H, Set characteristic length in number of bytes
	NRF_LOG_RAW_INFO("REACH HERER----------"); NRF_LOG_FLUSH();
	attr_char_value.max_len = 4;
	attr_char_value.init_len = 4;
	uint8_t value[4]     ={0x12, 0x34, 0x56, 0x78};
	attr_char_value.p_value = value;
	
    // OUR_JOB: Step 2.E, Add our new characteristic to the service
	err_code = sd_ble_gatts_characteristic_add(p_our_service->service_handle,
										&char_md,
										&attr_char_value,
										&p_our_service->char_handles);
										
		NRF_LOG_RAW_INFO("end of add char"); NRF_LOG_FLUSH();
	    if (err_code != NRF_SUCCESS)
    {
		NRF_LOG_RAW_INFO("FAILED----------"); NRF_LOG_FLUSH();
        return err_code;
    }

    return NRF_SUCCESS;
}
 
 
void ble_fdcs_init(ble_fdcs_t * p_our_service)
{
    // STEP 3: Declare 16 bit service and 128 bit base UUIDs and add them to BLE stack table     
	uint32_t err_code;
	ble_uuid_t         service_uuid;
	ble_uuid128_t      base_uuid = BLE_UUID_FDC_BASE_UUID;
	service_uuid.uuid = BLE_UUID_FDC_SERVICE;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
	APP_ERROR_CHECK(err_code);
    
	// OUR_JOB: Step 3.B, Set our service connection handle to default value. I.e. an invalid handle since we are not yet in a connection.
	p_our_service->conn_handle = BLE_CONN_HANDLE_INVALID;
    
	// STEP 4: Add our service
	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, 
											&service_uuid, 
											&p_our_service->service_handle);
	APP_ERROR_CHECK(err_code);
	
	err_code = fdc_char_add(p_our_service);
	if (err_code != NRF_SUCCESS)
    {
		NRF_LOG_RAW_INFO("FAILED222---------- %d", err_code); NRF_LOG_FLUSH();
       
    }
}
	

/*
Function to be called when updating characteristic value
hvx : Handle Value X, where x symbolize either notification or indication
So to do a notification we declare a variable, hvx_params, of type ble_gatts_hvx_params_t. This will hold the necessary parameters to do a notification and provide them to the sd_ble_gatts_hvx() function. Here is what we will store in the variable:

handle: The SoftDevice needs to know what characteristic value we are working on. In applications with two or more characteristics naturally we will need to reference the handle of the specific characteristic value we want to use. Our example only has one characteristic and we will use the handle stored in p_our_service->char_handles.value_handle.
type: The SoftDevice needs to know what "hvx type" we want to do; a notification or indication. As we are doing a notification we use BLE_GATT_HVX_NOTIFICATION. The other option would be BLE_GATT_HVX_INDICATION.
offset: Your characteristic value might be a sequence of many bytes. If you want to transmit only a couple of these bytes and the bytes are located in the middle of the sequence you can use the offset to extract them. Since we want to update all of our four bytes we will set the offset to zero.
p_len: The SoftDevice needs to know how many bytes to transmit. There is no need to send 20 bytes every time if you only have four bytes of relevant data. As an example, let's say you have a characteristic with the following sequence of bytes: 0x01, 0x02, 0x03, 0x04, 0x05 and you want to send just the 3rd and the 4th byte. Then set offset to 2 and len to 2.
p_data: Here we add a pointer to the actual data.

*/
void fdc_ch0_characteristic_update(ble_fdcs_t *p_our_service, int32_t *temperature_value)
{
    // OUR_JOB: Step 3.E, Update characteristic value
	if (p_our_service->conn_handle != BLE_CONN_HANDLE_INVALID)
	{
		uint16_t				len = 4;
		ble_gatts_hvx_params_t	hvx_params;
		memset(&hvx_params, 0, sizeof(hvx_params));
		
		hvx_params.handle = p_our_service->char_handles.value_handle;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = 0;
		hvx_params.p_len  = &len;
		hvx_params.p_data = (uint8_t*)temperature_value;  

		sd_ble_gatts_hvx(p_our_service->conn_handle, &hvx_params);


	}

}