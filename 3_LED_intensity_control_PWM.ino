/* This program shows that we can control the LED Intensity using the same principle as for motor control (PWD = pulse width modulation)
 *  We toggle the user LED ON and OFF with a pause in between
 *  Changing the duration of ON and OFF states, changes the light intensity (as for the speed)
 */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;

// See https://www.pololu.com/docs/0J63/3.2 for LED PIN description
#define YELLOW_LED  13

void setup()
{
  // Set LED PIN as OUTPUT
  pinMode(YELLOW_LED, OUTPUT);

  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();
}


void loop()
{
  // Turn LED ON
  digitalWrite(YELLOW_LED, HIGH);
  
  // Wait
  delayMicroseconds(100);

  // Turn LED OFF
  digitalWrite(YELLOW_LED, LOW);

  // Wait
  // Try playing with the values of each delays independently, e.g. delayMicroseconds(100) below. 
  // What will happen and why?
  delayMicroseconds(100);

  /* important: if you play with "delay" instead of "delayMicroseconds" you will switch-off/switch-on, like Morse-code instead of playing with the intensities
   => delayMicroseconds for intensities ; delay for morse (switch-on/switch-off)
  */

  /*   As for the motor exemple (code 1), doing something else that is timeconsuming in the loop will change the brightness of the LED
  * Uncomment the code below ( 3 lines) to see that the intensities decrease
*/
  //lcd.gotoXY(0, 0);
  //lcd.print(random(100));
  //delay(1000);
}
