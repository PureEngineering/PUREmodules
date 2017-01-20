#include "Si1153.h"

Si1153 si1153;

void setup() {
  // put your setup code here, to run once:
  
  uint8_t ADCCONFIGx;
  uint8_t ADCSENSx;
  uint8_t ADCPOSTx;
  uint8_t MEASCONFIGx;
  uint8_t configurations[4];
  Wire.begin();
  Serial.begin(9600);
  // Configure CHAN_LIST, enable channel 1 and 3
  si1153.param_set(Si1153::CHAN_LIST, 0B001010 >> 2);

  /*
   * Configure timing parameters
   */
  si1153.param_set(Si1153::MEASRATE_H, 0);
  si1153.param_set(Si1153::MEASRATE_L, 1);
  si1153.param_set(Si1153::MEASCOUNT_0, 5);
  si1153.param_set(Si1153::MEASCOUNT_1, 10);
  si1153.param_set(Si1153::MEASCOUNT_2, 0);


  ADCCONFIGx=  0B00000000;
  ADCSENSx=    0B00000010;
  ADCPOSTx=    0B01000000;
  MEASCONFIGx= 0B01000001;
  
  configurations[0] = ADCCONFIGx;
  configurations[1] = ADCSENSx;
  configurations[2] = ADCPOSTx;
  configurations[3] = MEASCONFIGx;

  si1153.config_channel(1,configurations);

  ADCCONFIGx=  0B00000000;
  ADCSENSx=    0B00000011;
  ADCPOSTx=    0B01000000;
  MEASCONFIGx= 0B10000001;

  configurations[0] = ADCCONFIGx;
  configurations[1] = ADCSENSx;
  configurations[2] = ADCPOSTx;
  configurations[3] = MEASCONFIGx;

  si1153.config_channel(3,configurations);

  si1153.send_command(Si1153::START);
}

void loop() {
  // put your main code here, to run repeatedly:
      uint8_t data[3];

    data[0] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT0, 1);
    data[1] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT1, 1);
    data[2] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT2, 1);
    Serial.println(si1153.bytes_to_int(data, sizeof(data)));
    
    delay(100);

}
