/* This program use the "1_ReadProximity" example and turns it into a code that face toward any object in front of it
 *  The goal is to always have the same measured distance on the left and on the right
 *  The logic is to check if somehting is closer to one side or the other, and then turn in the direction to compensate it
 *  
 *  To measure the distance of an object, we simply sum the number of times a sensor is ON, over each of the "brightnessLevels"
 *  -> big distance to left, go to left to make it smaller
 *  -> big distance to right, go to right to make it smaller
 *  
 */


 
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

#define LINE_SENSOR_LED  11

#define FRONT_SENSOR  22

int brightnessLevels[] = { 1, 2, 4, 15, 32, 55, 85, 120 };
int nLevels = sizeof(brightnessLevels) /sizeof(int);

int leftState;
int rightState;

int leftSum;
int rightSum;
int error;

int motorSpeed = 100;

int lastSampleTime = 0;


  
void setup()
{
  pinMode(LINE_SENSOR_LED, OUTPUT);
  digitalWrite(LINE_SENSOR_LED, LOW);

  // Setting pin mode to INPUT of all proximity-sensors. "INPUT_PULLUP" means that by defaut, if nothing connected to pin, then the returned value is 1 (pulled towards 1 by default)
  pinMode(FRONT_SENSOR, INPUT_PULLUP);
  // nothing = 1, something = 0

  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();
  
}



void loop()
{
  // Important : the "Sums" (left or right) indicate the distance to an object (so higher, so farer)

  
  // LEFT LED
  leftSum = 0;
  for (int i = 0; i < nLevels; i++) {
    Zumo32U4IRPulses::start(Zumo32U4IRPulses::Left, brightnessLevels[i], 420);
    delayMicroseconds(421);
    leftState = !digitalRead(FRONT_SENSOR); // nothing = 1, something = 0 becomes: nothing = 0, something = 1
    leftSum += leftState;
    Zumo32U4IRPulses::stop();
  }

  // RIGHT LED
  rightSum = 0;
  for (int i = 0; i < nLevels; i++) {
    Zumo32U4IRPulses::start(Zumo32U4IRPulses::Right, brightnessLevels[i], 420);
    delayMicroseconds(421);
    rightState = !digitalRead(FRONT_SENSOR); // nothing = 1, something = 0 becomes: nothing = 0, something = 1
    rightSum += rightState;
    Zumo32U4IRPulses::stop();
  }

  // we compute the error (difference between the left and right distance)
  error = leftSum - rightSum;

  // if both sensor read the same
  if (error == 0) {
    // stop moving (you can try moving in a straight line to have the robot following you)
    motors.setSpeeds(0, 0);
  // if left closer than right
  } else if (error > 0) {
    // -> turn left
    motors.setSpeeds(-motorSpeed, motorSpeed);
  // else right closer than left 
  } else {
    // -> turn right
    motors.setSpeeds(motorSpeed, -motorSpeed);
  }


  if ((millis() - lastSampleTime) >= 100) // "millis" time in millisecondes since the programm is launched
  {
    lastSampleTime = millis();

    // print info on screen every 100 ms
    lcd.clear();
    lcd.print(leftSum);
    lcd.print("-");
    lcd.print(rightSum);
    lcd.print("=");
    lcd.print(error);
  }  

  
}
