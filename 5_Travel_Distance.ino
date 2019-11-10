 /* This program uses the encoder library provided by pololu to move a specific distance and then stop
   
 *  This prgram uses both PWM and interrupt in the background
 
 *  It allow us to absctract away from the very low level of changing and checking the pin state
 *  And to focus instead on the high-level logic we want: move the motor until we move X cm   
 */


 /* "getCountsLeft()" informations:
* Returns the number of counts that have been detected from the left-side encoder by using "encoders" module. 
* We keep moving while we did not counted "number_of_events_to_travel" passages of the magnet. 
* These counts start at 0. Positive counts correspond to forward movement of the left side of the Zumo, while negative counts correspond to backwards movement.
* "counts" is returned as signed 16-bit integer. When count > 32767, it will overflow down to -32768. When count < -32768, it will overflow up to 32767.
* See more infos: https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_encoders.html

* From https://www.pololu.com/docs/0J63/3.4, we know that there is 909.7 events per full rotation of the motor
* You can measure the wheel diameter and compute value to travel desired distances 
* (never perfect on the zumo because we use a chain system which cannot be approaximated by a wheel)
  */
  
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;

Zumo32U4Motors motors;

// "encoders" allows to read counts from the encoders, telling us how much each motor has turned and in what direction
// Encoders are monitored in the background using interrupts so, code can perform other tasks without missing encoder counts (see: https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_encoders.html)
Zumo32U4Encoders encoders;

int number_of_events_to_travel = 1000;



void setup()
{
  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();

  // small pause to give you time to remove your finger
  delay(1000);
}



void loop()
{
  // move forward
  motors.setSpeeds(100, 100);
  

  while (encoders.getCountsLeft() < number_of_events_to_travel) {
    // do nothing, motors keep moving
  }
  // once done, stop the motors
  motors.setSpeeds(0, 0);

  // print encoder value on the screen to verify
  // you should see 1000 or a bit more displayed
  lcd.clear();
  lcd.print(encoders.getCountsLeft());

  // wait before going back
  delay(1000);

  // move backward
  motors.setSpeeds(-100, -100);
  
  while (encoders.getCountsLeft() > 0) {
    // do nothing, motors keep moving
  }
  // once done, stop the motors
  motors.setSpeeds(0, 0);

  // print encoder value on the screen to verify
  // you should see exactly 0 or a bit less displayed
  lcd.clear();
  lcd.print(encoders.getCountsLeft());

  // wait before going forward
  delay(1000);
}
