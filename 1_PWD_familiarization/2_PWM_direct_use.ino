/* This program make use of PWMs (Pulse Width Modulation) to drive the motors 
 * PWM are controlled by a dedicated circuit in the microprocessors, 
 * so you can do something else in the loop while the PWM are being generated via the "setSpeeds" (see below)
 * 
 * -> What have been done in the first code ("1_go_straight.ino") is now "automatic" somehow and allow to do other things in parallel
 * -> (it "optimize" the process occuring and you don't need to code everything in the same time, so just better).
 * 
 * We use the motor library for that, see https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_motors.html
 */
 
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors; // the motor library https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_motors.html

void setup()
{
  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();
}


void loop()
{
   // This turn both motor foward at speed of 75. 
   // Speed is expressed in arbitrary untis between -400 to 400.
   // See: https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_motors.html#afdc238f045d9c919afd01f143f2fdbbb
  motors.setSpeeds(75, 75);

  // ALL this below just show you that other things can occur in parallel (thanks to the PWM)
  // This time you can do other things in the loop and it won't impact the speed of the robot
  // (Uncommenting the line below does not impact the speed)
  lcd.gotoXY(0, 0);
  lcd.print(random(100));
  // You can even try adding a long delay of 1 seconds
  // Uncommenting the line below does not impact the speed
  delay(1000);
}
