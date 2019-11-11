/* This program converts line sensors values to boolean values (to know if there is a line below each sensors)

   We simply define a threshold of 500:
   - if the value is below, then it is WHITE, there is no line (LOW state reached first)
   - else, then it is BLACK, there is a line (HIGH state maintained because thakes time to go back to the LOW state)

   WARNING: You need to place the jumper on the bottom PCB on the DN4 and DN2 position.
   See https://www.pololu.com/docs/0J63/3.5

   We use the library Zumo32U4LineSensors
   See https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_line_sensors.html
*/


#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4LineSensors lineSensors;

// same as before, we define list to store the values of each sensors
#define NUM_SENSORS 5
int lineSensorValues[NUM_SENSORS];  // for raw sensor values (list of int-values)

int lineDetectionThreshold = 500;   // our threshold that we decided from observation
bool lineDetected[NUM_SENSORS];     // for boolean values, line or not line (list of bool-values)

int lastSampleTime = 0;

void setup()
{
  // this initializes the sensors
  lineSensors.initFiveSensors();
}

void loop()
{
  // read sensors with emitters on
  lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);
  // read "lineSensors" and put values in the list "lineSensorValues" after triggering on the emiters via "QTR_EMITTERS_ON"


  // for each index in the list lineSensorValues
  // that is from 0 to 4  (NUM_SENSORS = 5 and we use the < operator)
  for (int i = 0; i < NUM_SENSORS; i++) {

    // if above threshold, then there is a black-line
    // else no line
    if (lineSensorValues[i] > lineDetectionThreshold) {
      lineDetected[i] = true;
    } else {
      lineDetected[i] = false;
    }
  }

}
