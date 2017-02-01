#include <Si1153.h>

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
  
  //Configure CHAN_LIST (Each bit represents a channel). This code enables channel 1 and 3.
  si1153.param_set(Si1153::CHAN_LIST, 0B00001010);

  /*
   * Configure timing parameters
   */
  
  //Set base period of 800us
  si1153.param_set(Si1153::MEASRATE_H, 0); 
  si1153.param_set(Si1153::MEASRATE_L, 1);

  //These three values are used by  MEASCONFIGx per channel to decide on the period per channel
  si1153.param_set(Si1153::MEASCOUNT_0, 5); //Selecting Meascount = 5 sets the period to 4ms
  si1153.param_set(Si1153::MEASCOUNT_1, 10);//Selecting Meascount = 10 sets the period to 8ms
  si1153.param_set(Si1153::MEASCOUNT_2, 10);

  //Setup Channel 1
  ADCCONFIGx=  0B00000000;  //Set Decim_rate to 0.
  ADCSENSx=    0B00000010;  //Set SW_gain to 0 and HW_GAIN to 2.
  ADCPOSTx=    0B01000000;  
  MEASCONFIGx= 0B01000001;  //MEASCONFIG[7:6] selects which MEASCOUNT to use for period. This selects MEASCOUNT1.
  
  configurations[0] = ADCCONFIGx;
  configurations[1] = ADCSENSx;
  configurations[2] = ADCPOSTx;
  configurations[3] = MEASCONFIGx;

  si1153.config_channel(1,configurations);

  //Setup Channel 3
  ADCCONFIGx=  0B00000000;   //Set Decim_rate to 0.
  ADCSENSx=    0B00000010;   //Set SW_gain to 0 and HW_Gain to 2.
  ADCPOSTx=    0B01000000;
  MEASCONFIGx= 0B10000001;   //MEASCONFIG[7:6] selects which MEASCOUNT to use for period. This selects MEASCOUNT2.

  configurations[0] = ADCCONFIGx;
  configurations[1] = ADCSENSx;
  configurations[2] = ADCPOSTx;
  configurations[3] = MEASCONFIGx;

  si1153.config_channel(3,configurations);

  si1153.send_command(Si1153::START);     //Start the system
}

void loop() {
    Serial.println("Starting");
    uint8_t data[3];
    uint8_t channel3_data[3];
    
    data[0] = si1153.read_byte(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT0);
    data[1] = si1153.read_byte(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT1);
    data[2] = si1153.read_byte(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT2);
    Serial.print("Channel 1 Output: ");
    Serial.println(si1153.bytes_to_int(data, sizeof(data)));

    //Uncomment this code to see Channel 3's Output
    /*channel3_data[0] = si1153.read_byte(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT3);
    channel3_data[1] = si1153.read_byte(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT4);
    channel3_data[2] = si1153.read_byte(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT5);
    Serial.print("Channel 3 Output: ");
    Serial.println(si1153.bytes_to_int(channel3_data, sizeof(channel3_data)));
    */
    delay(250);

}