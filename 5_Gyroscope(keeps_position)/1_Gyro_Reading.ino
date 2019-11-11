/* This program read the values from the gyroscope
 *  
 *  The gyroscope measure the speed of rotation
 * 
 *  Some info are here: https://www.pololu.com/docs/0J63/3.7
 *  And here for code samples:  https://github.com/pololu/zumo-32u4-arduino-library/blob/master/L3G.cpp
 * 
 */



#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
L3G gyro;  // gyroscope special class



void setup()
{
  // some intitializtaion to communicate with the gyro
  Wire.begin();
  gyro.init();

  // setting up some registers to make the values more stable by applying filters
  // 800 Hz output data rate,
  // low-pass filter cutoff 100 Hz
  gyro.writeReg(L3G::CTRL1, 0b11111111);
  // 2000 dps full scale
  gyro.writeReg(L3G::CTRL4, 0b00100000);
  // High-pass filter disabled
  gyro.writeReg(L3G::CTRL5, 0b00000000);
}



void loop()
{
  // this read values from the gyro
  gyro.read();

  // we print 
  lcd.clear();
  lcd.print(gyro.g.z); // z axis is pointing up from the robot

  // and wait a bit
  delay(100);

  
}
