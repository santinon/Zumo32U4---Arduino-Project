/* This is the same as the avoid one 
 * but we simply change the sign of the motro commands in each logical steps. 
 * Follow is just the opposite of Avoid
 * 
 *  WARNING: You need to put the jumper on the bottom sensor PCB between to the RGT and LFT pin for the side sensors to work
 * 
 */

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;

#define LINE_SENSOR_LED  11

#define FRONT_SENSOR  22
#define RIGHT_SENSOR  4
#define LEFT_SENSOR  20

bool front_sensor_left_led_state; 
bool front_sensor_right_led_state;
bool left_sensor_left_led_state;
bool right_sensor_right_led_state;

int brightnessLevel = 2;
int motorSpeed = 75;

void setup()
{
  pinMode(LINE_SENSOR_LED, OUTPUT);
  digitalWrite(LINE_SENSOR_LED, LOW);

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

  // logic to follow objects

  if (left_sensor_left_led_state) {
    motors.setSpeeds(-motorSpeed, motorSpeed);
  }
  else if (right_sensor_right_led_state) {
    motors.setSpeeds(motorSpeed, -motorSpeed);
  }
  else if (front_sensor_left_led_state & !front_sensor_right_led_state) {
    motors.setSpeeds(-motorSpeed/2, motorSpeed);

  }
  else if (!front_sensor_left_led_state & front_sensor_right_led_state) {
    motors.setSpeeds(motorSpeed, -motorSpeed/2);        
  }
  else if (front_sensor_left_led_state & front_sensor_right_led_state) {
    // go straight
    motors.setSpeeds(motorSpeed, motorSpeed);
  }
  else {
    // nothing detected turn around
    motors.setSpeeds(motorSpeed, -motorSpeed);
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
