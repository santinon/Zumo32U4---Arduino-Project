/* This program count the number of time PIN 7 changes state using interruptions
 * PIN 7 is connected to the HALL effect sensor of the RIGHT motor
 * 
 * We use interrupt in this demo

 * See https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  ==> We tell the processor to initiate an interrupt-worker that:
        - monitor constantly "PIN_RIGHT_SENSOR" (pin 7), for this we need to use "digitalPinToInterrupt(PIN_RIGHT_SENSOR)" which is a nice helper from Arduino to do that
        - call the function "on_interrupt()" (see below under loop function) when it sees a "CHANGE" (trigger the interrupt whenever the pin changes value) on "PIN_RIGHT_SENSOR" (pin 7)
   See all the code to understand (last "on_interrupt()" function )
 */
 
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;

#define PIN_RIGHT_SENSOR  7

int count;



void setup()
{
  //initialise count to 0 at start
  count = 0;

  attachInterrupt(digitalPinToInterrupt(PIN_RIGHT_SENSOR), on_interrupt, CHANGE);
  // looks permanently PIN_RIGHT_SENSOR, if it changes (CHANGE) it calls function on_interrupt and +1 is added to the counter, then it continous the loop again&again
}



void loop()
{
  // we simply print count
  lcd.gotoXY(0, 0);
  lcd.print(count);

  // This time we can move the robot and the counter does not fail (things run in parallel)
  motors.setSpeeds(75, -75);
  delay(500);
  motors.setSpeeds(-75, 75);
  delay(500);
}



// the on_interrupt() function is called automatically using the interrupt mechanism (that we defined in setup using the "attachInterrupt()" function
// This function needs to be very quickly executed because it can be called hundreds of times per seconds. If you do something too time consuming, like waiting, or printing on the screen, your program might stop working or miss events.
void on_interrupt()
{
  count += 1;
}
