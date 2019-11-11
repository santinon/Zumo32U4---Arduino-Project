/* This program displays the value sensed by each of the line sensors (so detects black and white lines and prints associated values on the screen)

   Because the screen is of limited size, we only print one value at a time

   WARNING: You need to place the jumper on the bottom PCB on the DN4 and DN2 position.
   See https://www.pololu.com/docs/0J63/3.5

   We use the library Zumo32U4LineSensors
   See https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_line_sensors.html
*/


#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4LineSensors lineSensors; // this is the tools for reading the sensors values (it is a class)

// there are 5 sensors, we just #define NUM_SENSORS 5 to not have to use directly the number 5 all along the program
#define NUM_SENSORS 5

// creates a list of 5 integers (see: https://www.tutorialspoint.com/cplusplus/cpp_arrays.htm), we are going to store the sensors values (5 values because 5 sensors) in that list
int lineSensorValues[NUM_SENSORS];

// to keep track of which sensor we display on the lcd screen
int selectedSensorIndex = 0;

// we will only read sensors value every 100ms. "lastSampleTime" is to store the time we last read the sensors
int lastSampleTime = 0;



void setup()
{
  // this initializes the sensors and various memory space used by the library (see: https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_line_sensors.html#a3873997ed35fbc1c6c57411f6cee1f2d)
  lineSensors.initFiveSensors();
}



void loop()
{

  // once 100ms have elapsed, we enter this if statement. We do that because we need to keep monitoring the buttons below (so we need the loop to be as fast as possible)
  // But if we go too fast, we cannot see what is printed on the screen (so, we only read and print on the screen every 100 ms, but check the buttons state all the time)
  // millis() = will return number of milliseconds passed since the program started
  if ((millis() - lastSampleTime) >= 100){

    // update lastSampleTime to now, so we enter this function again in 100ms
    lastSampleTime = millis();

    // Read all line sensors (via "lineSensors") and put the values in the "lineSensorValues" list
    lineSensors.read(lineSensorValues, QTR_EMITTERS_ON); // "QTR_EMITTERS_ON" turns the emitters on (at the same time)
    // (for curious people, the actual function that reads from the sensors is: https://github.com/pololu/zumo-32u4-arduino-library/blob/master/QTRSensors.cpp#L424)
    /* The reason why the values are low for white and high for black is that:
       our measure = time it takes for the sensor to come back to the state LOW after being pulled to HIGH (It gets HIGH state when signal came back)
       All depends on the amount of infrared light coming back to the sensor, which depends on the color of the surface,
       black surfaces absorb more light, so less light is coming back to the sensor, so it takes more time to go back to a LOW state (High state states longer)   */

    // We print the sensor value currently selected
    lcd.clear();
    lcd.print("Index: ");
    lcd.print(selectedSensorIndex);  // which sensor we display
    lcd.gotoXY(0, 1);
    lcd.print("Val:");
    lcd.print(lineSensorValues[selectedSensorIndex]);
  }

  // If button A is pressed, select the previous sensor.
  if (buttonA.getSingleDebouncedPress()){
    selectedSensorIndex = (selectedSensorIndex + NUM_SENSORS - 1) % NUM_SENSORS;
  }

  // If button B is pressed, select the next sensor.
  if (buttonB.getSingleDebouncedPress()){
    selectedSensorIndex = (selectedSensorIndex + 1) % NUM_SENSORS;
  }
  
  /*
   * Explanation of modulo "%": 
   * 1%5 = 1; 2%5 = 2 ; 3%5 = 3 ; 4%5 = 4; 5%5 = 0; (and then: 6%5 = 1; 7%5 = 2; 8%5 = 3; 9%5 = 4; 10%5 = 0)
   * so when we arrived at the last sensor, we will restart by the first one again, like a loop (the modulo allows loops actually)
    if curious read more about modulo: https://en.wikipedia.org/wiki/Modulo_operation
    and in c++ it is the % operator: https://www.cprogramming.com/tutorial/modulus.html
   */
   
  }
