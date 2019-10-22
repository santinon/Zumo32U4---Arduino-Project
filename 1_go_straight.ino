/* This program toggles the motors ON and OFF
 * and is an introduction to the PWM (Pulse Width Modulation) application
 */

// Include necessary for using Zumo32u4 libraries
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;  // Instanciate the lcd class
Zumo32U4ButtonA buttonA; // Instanciate the buttonA class


// We use #define to give names to each PIN required to move the motors
// See explanation of define at https://www.techonthenet.com/c_language/constants/create_define.php
// See PIN mappping for motors at https://www.pololu.com/docs/0J63/3.3

#define MOTOR_SPEED_RIGHT  9
#define MOTOR_DIR_RIGHT  15

#define MOTOR_SPEED_LEFT 10
#define MOTOR_DIR_LEFT  16


// the setup function is executed only once
void setup()
{
  // We want to send commands so we set all PINs as OUTPUT (INPUT would be for receiving a signal for example)
  // See https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
  pinMode(MOTOR_SPEED_RIGHT, OUTPUT);
  pinMode(MOTOR_DIR_RIGHT, OUTPUT);
  pinMode(MOTOR_SPEED_LEFT, OUTPUT);
  pinMode(MOTOR_DIR_LEFT, OUTPUT);

  // We set the direction to LOW which is forward (HIGH is for backward)
  // See https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
  digitalWrite(MOTOR_DIR_RIGHT, LOW);
  digitalWrite(MOTOR_DIR_LEFT, LOW);

  lcd.clear();  // clear the lcd 
  lcd.print("Press A");  // ask user to press button A
  buttonA.waitForButton(); // wait for the button A to be pressed
  lcd.clear(); // clear the lcd again
}

// the loop function is executed in an infinite loop
// as soon as we reach the end of the loop() function, it starts again.
void loop()
{

  // Turn both motors ON
  digitalWrite(MOTOR_SPEED_RIGHT, HIGH);
  digitalWrite(MOTOR_SPEED_LEFT, HIGH);

  // see https://www.arduino.cc/reference/en/language/functions/time/delay/
  // you can even do shorter pauses in us
  delay(1000);  //  see https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/

  // Turn both motors OFF
  digitalWrite(MOTOR_SPEED_RIGHT, LOW);
  digitalWrite(MOTOR_SPEED_LEFT, LOW);

  // pause for some time in ms
  // What happens to the speed, why?
  // you can even do shorter pauses in us
  delayMicroseconds(10);

  // This part is a "bonus" not necessary for the robot movement
  // If you do very short pauses of 1us (pause_duration = 1 and use delayMicroseconds(pause_duration) )
  // Uncomment below to try it out
  // The speed of the robot decreases, why?
  lcd.gotoXY(0, 0); // reset printing position to top-left corner
  lcd.print(random(100)); // we print random numbers just to be able to see it is plotting very fast different things in each loop
}
