/* This make the LED fade back and forth
 * We control the LED brightness using a PWM signal
 * 
 * The PWM signal is generated using the function analogWrite()
 * See https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
 */
 
 
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;

#define YELLOW_LED  13

void setup()
{
  pinMode(YELLOW_LED, OUTPUT);

  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();
}


void loop()
{
  // Loop over i from 0 to 255
  for (int i = 0; i <= 255; i++) {
    // At every iteration, we set the PWM duty cycle to the value of i
    // See https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
    // The duty cycle of the PWM is coded on a 8 bit register, so 2^8 possible values (256), between 0 (always off) and 255 (always on). (255 options + the 0 option = 256 options)
    // (Why do we code from 0 to 255? Because the electronic stuff is done on that way, so we do all this little trick here)
    analogWrite(YELLOW_LED, i);
    // We pause for a bit before the next change of brighthess
    delay(1);
  } 

  // Same as above but from 255 to 0
  for (int i = 255; i >= 0; i--) {
    analogWrite(YELLOW_LED, i);
    delay(1);
  } 
  
  lcd.gotoXY(0, 0);
  lcd.print(random(100));
}
