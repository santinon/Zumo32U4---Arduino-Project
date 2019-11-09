/* This program counts the number of time PIN 8 changes state
 * PIN 8 is connected to the HALL effect sensor of the LEFT motor
 * This sensor detect the passage of a magnet that is connected to the shaft of the motor
 * IMPORTANT : turn the left "wheels" of you zumo machine!
 */
 
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;

// See https://www.pololu.com/docs/0J63/3.4 to learn more about
// We use the left wheel in this demo (pin 8)
#define PIN_LEFT_SENSOR  8

// We prepare variables to store information
int count; // number of time the sensor changes state
int previous_state; // previous state to be able to compare and see a change
int current_state; // current state to store the current reading of the sensor


void setup()
{
  // Set pin as INPUT because we want to sense (we do not "send" a signal outside (OUTPUT) no, what we want is to read one (so INPUT))
  pinMode(PIN_LEFT_SENSOR, INPUT);

  // initiate count to 0
  count = 0;
  
  // see https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
  // initiate: previous_state = state of PIN_LEFT_SENSOR   (= 0 or 1)
  previous_state = digitalRead(PIN_LEFT_SENSOR);
}


void loop()
{
  // each time we restart the loop, the current state of PIN_LEFT_SENSOR is readed
  current_state = digitalRead(PIN_LEFT_SENSOR);

  // if the state has changed since last execution of the loop
  if (current_state != previous_state) {
    // add one to count
    count += 1; 
  }

  // set previous_state to current_state before next execution of the loop
  previous_state = current_state;

  // print count on the top-left of the screen
  lcd.gotoXY(0, 0);
  lcd.print(count);
  lcd.gotoXY(0, 1);
  lcd.print(previous_state);

  // If we do something else while we try to monitor PIN_LEFT_SENSOR inside that loop
  // Then we miss a lot of events on PIN_LEFT_SENSOR because we are busy doing something else
  // Try by uncommenting the code below and see the counter totally fails
  //motors.setSpeeds(75, -75);
  //delay(500);
  //motors.setSpeeds(-75, 75);
  //delay(500);
  
}
