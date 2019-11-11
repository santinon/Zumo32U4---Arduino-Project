/* This program uses the line-detection-information to follow the black-line
 *  
 *  The logical process is this:
 *  - if the black-line is centered according tot he robot, go straight
 *  - if the black-line is a bit on the right, turn right
 *  - if the black-line is a bit on the left, turn left
 *  
 * WARNING: You need to place the jumper on the bottom PCB on the DN4 and DN2 position (as for the 2 previous codes)
 * See https://www.pololu.com/docs/0J63/3.5
 * 
 * We use the library Zumo32U4LineSensors
 * See https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_line_sensors.html
 */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;

#define NUM_SENSORS 5

// list of int-values (one for each sensor value)
int lineSensorValues[NUM_SENSORS]; // for raw sensor values 

int lineDetectionThreshold = 500;  // our threshold that we decided from observation

//list of bool-values (one for each sensor value)
bool lineDetected[NUM_SENSORS];    // for boolean values, line or not line 

// default motors speed (useful when debugging: slow it for better behavior understanding)
int motorSpeed = 200; 

int lastSampleTime = 0;



void setup()
{
  // initializes the sensors
  lineSensors.initFiveSensors();

  // Wait for button A to be pressed and released.
  lcd.clear();
  lcd.print(F("Press A"));
  lcd.gotoXY(0, 1);
  lcd.print(F("to start"));
  buttonA.waitForButton();
  lcd.clear();
}



void loop()
{
  // read sensors with emiters on 
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  // read "lineSensors" and put values in the "lineSensorValues" liste after triggering-on emiters via "QTR_EMITTERS_ON"

  // convert to boolean, line or not line
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (lineSensorValues[i] > lineDetectionThreshold) {
      lineDetected[i] = true;
    } else {
      lineDetected[i] = false;
    }
  } 

  // we print 0 if there is no line, 
  //          1 if a line, 
  //          for each sensor in order (5 total: 0,1,2,3,4)
  lcd.gotoXY(0,0);
  for (int i = 0; i < NUM_SENSORS; i++) {
    lcd.print(lineDetected[i]);
  }



  // the feedback control starts here

  // if the middle sensor is activated (is true or =1)
  if (lineDetected[2]) {
    // goes straight
    motors.setSpeeds(motorSpeed, motorSpeed);

  } else {
    // middle sensor not activated

    // and left sensor activated
    if (lineDetected[1]) {
      // turns left
      motors.setSpeeds(-motorSpeed, motorSpeed); // left-wheel goes back and right-wheel goes forward => left turning
    } 
    else if (lineDetected[3]) {
      // and right sensor activated
      // turns right
      motors.setSpeeds(motorSpeed, -motorSpeed); // left-wheel goes forward and right-wheel goes back => right turning
    }
  }

  
}
