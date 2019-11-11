/* This program use the proximity sensor to sense if an obstacle is in front and tries to avoid it
 *  
 *  We reuse the previous code but only use one brightness level for the sensor to take our decision (we choose the value from the previous "1_Read_Proximity" code)
 *  
 *  In addition, we use the side proximity-sensors of the RIGHT and LEFT side to make the behavior of the robot more robust
 *  
 *  WARNING: You need to put the jumper on the bottom sensor PCB between to the RGT and LFT pin for the side sensors to work
 *  
 */



#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

// Line sensor led pin, with is an IR emitting diode. We will just turn it off to avoid noisy reading
#define LINE_SENSOR_LED  11

// Proximity-sensor PINs, all three of them
#define FRONT_SENSOR  22
#define RIGHT_SENSOR  4
#define LEFT_SENSOR  20

// variable to store the sate of each sensor
// front sensor make sense with both left and right LED
// side sensors make sense only with the side LED
// NOTE: when we flash the left LED, both the front and side left LED turn on;  when we flash the right LED, both the front and side right LED turn on
bool front_sensor_left_led_state; 
bool front_sensor_right_led_state;
bool left_sensor_left_led_state;
bool right_sensor_right_led_state;

// we are using only the brightnessLevel of 2, because we decided it triggers at a good distance for this task
int brightnessLevel = 2;

// default speed of the robot
int motorSpeed = 75;

void setup()
{
  // Turing off the line-sensor LED, set to OUTPUT, and turn to LOW state
  pinMode(LINE_SENSOR_LED, OUTPUT);
  digitalWrite(LINE_SENSOR_LED, LOW);

  // Setting pin mode of all proximity-sensors to INPUT
  // The INPUT_PULLUP means that by defaut, if nothing is connected to the pin, then the returned value is 1 (it is pulled towards 1 by default)
  pinMode(FRONT_SENSOR, INPUT_PULLUP);
  pinMode(RIGHT_SENSOR, INPUT_PULLUP);
  pinMode(LEFT_SENSOR, INPUT_PULLUP);
    
  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();
}


void loop()
{

  // read front and left sensor for LEFT led
  Zumo32U4IRPulses::start(Zumo32U4IRPulses::Left, brightnessLevel, 420);
  delayMicroseconds(421);
  front_sensor_left_led_state = !digitalRead(FRONT_SENSOR);
  left_sensor_left_led_state = !digitalRead(LEFT_SENSOR);
  Zumo32U4IRPulses::stop();

  // read front and right sensor for RIGHT led
  Zumo32U4IRPulses::start(Zumo32U4IRPulses::Right, brightnessLevel, 420);
  delayMicroseconds(421);
  front_sensor_right_led_state = !digitalRead(FRONT_SENSOR);
  right_sensor_right_led_state = !digitalRead(RIGHT_SENSOR);
  Zumo32U4IRPulses::stop();


  // logic to avoid obstacle

  // if something on the left side
  if (left_sensor_left_led_state) {
    // turn right abrupty
    motors.setSpeeds(motorSpeed, -motorSpeed);
  }
  // else if something on the right side
  else if (right_sensor_right_led_state) {
    // turn left abrupty
    motors.setSpeeds(-motorSpeed, motorSpeed);
  }
  // else, if the front left is ON and the front right is OFF
  // something ahead on the left side
  else if (front_sensor_left_led_state & !front_sensor_right_led_state) {
    // turn right slightly
    motors.setSpeeds(motorSpeed, -motorSpeed/2);    
  }
  // else, if the front left is OFF and the front right is ON
  // something ahead on the right side
  else if (!front_sensor_left_led_state & front_sensor_right_led_state) {
    // turn left slightly
    motors.setSpeeds(-motorSpeed/2, motorSpeed);
  }
  // else if both sensors are ON -> obstacle is ahead
  else if (front_sensor_left_led_state & front_sensor_right_led_state) {
    // turn left abrupty
    motors.setSpeeds(-motorSpeed, motorSpeed);
  }
  // else no sensors are on
  else {
    // so we go straight
    motors.setSpeeds(motorSpeed, motorSpeed);
  }

  // print all sensor info on screen
  lcd.gotoXY(0, 0);
  lcd.print(left_sensor_left_led_state);
  lcd.print(' ');
  lcd.print(front_sensor_left_led_state);
  lcd.print(' ');  
  lcd.print(front_sensor_right_led_state);
  lcd.print(' ');  
  lcd.print(right_sensor_right_led_state);

  
}
