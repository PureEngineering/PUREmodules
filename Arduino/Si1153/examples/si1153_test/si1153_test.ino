#include <Si1153.h>

Si1153 si1153;

/**
 * Setup for configuration
 */
void setup() {

    uint8_t conf[4];
  
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

    /*
     * Configuration for channel 1
     */
    conf[0] = 0B00000000;
    conf[1] = 0B00000010, 
    conf[2] = 0B01000000;
    conf[3] = 0B01000001;
    
    si1153.config_channel(1, conf);

    /*
     * Configuation for channel 3
     */
    conf[0] = 0B00000000;
    conf[1] = 0B00000011, 
    conf[2] = 0B01000000;
    conf[3] = 0B10000001;

    si1153.config_channel(3, conf);

    // Send start command
    si1153.send_command(Si1153::START);
    
}

/**
 * Loops and reads data from registers
 */
void loop() {

    uint8_t data[3];

    data[0] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_0, 1);
    data[1] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_1, 1);
    data[2] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_2, 1);

    Serial.println(si1153.get_int_from_bytes(data, sizeof(data)));
    
    delay(100);
}
