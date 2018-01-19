#include <Wire.h>

#define P1237_ADDR 0x2A         // i2c Address for the P1237

int red = 0;
int green = 0;
int blue = 0;
int infrared = 0;

void configureSensorCondition1() {  
  Wire.beginTransmission(P1237_ADDR);
  Wire.write(byte(0x00));
  Wire.write(byte(0x84));
  Wire.endTransmission();
  
  Wire.beginTransmission(P1237_ADDR);
  Wire.write(byte(0x00));
  Wire.write(byte(0x04));
  Wire.endTransmission();
}

// Read one color
int readColor() {
  int color = 0;
  while(Wire.available() == 0);
  color = Wire.read()<<8;
  while(Wire.available() == 0);
  color |= Wire.read();
  return color;
}

void readRGBI() {
  Wire.beginTransmission(P1237_ADDR);
  Wire.write(byte(0x00)); // Originally set this to 3, didn't start at the byte we wanted (?)
  Wire.endTransmission();
  
  Wire.requestFrom(P1237_ADDR, 11);

  // Skip over first three bits (Settings registers)
  for(int i=0;i<3;i++){
    //wait for response
    while(Wire.available() == 0);
    Wire.read(); 
  }

  Serial.print( " :: Reading Colors :: ");
  red = readColor();
  green = readColor();
  blue = readColor();
  infrared = readColor();

  Serial.print("\t :: R: ");
  Serial.print(red);
  Serial.print(" G: ");
  Serial.print(green);
  Serial.print(" B: ");
  Serial.print(blue);
  Serial.print(" IR: ");
  Serial.print(infrared);
  Serial.print("\n");
}

// Setup i2c (Wire) and UART
void setup() {
  Serial.begin(9600);               // start serial for output
  Wire.setClock(400000);            // Set fast i2c speed
  Wire.begin(); 
  Serial.print(" :: Color/proximity sensor - P12347-01CT ::\n");
}

// the loop function runs over and over again forever
void loop() {

  configureSensorCondition1();
  delay(500);
  readRGBI();

  delay(1000);                      // Alter to change frequency of the color read
  
}

