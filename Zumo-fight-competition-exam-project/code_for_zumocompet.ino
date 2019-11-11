// This code is for a Zumo-fight-competition (within a white ring with black borders)
// NOTE: after the setup and the loop, order of function declarations does not matter



#include <Wire.h>
#include <Zumo32U4.h>

//DEFINE CONSTANTES/PINS
//constantes
#define QTR_THRESHOLD     700  // color-light-reflectance threshold (for black&white line-detection)
#define REVERSE_SPEED     200  // 0 is stopped, 400 is full speed
#define TURN_SPEED        300  // speed for turn
#define FORWARD_SPEED     200  // speed for going-forward
#define REVERSE_DURATION  100  // ms
#define TURN_DURATION     300  // ms
#define NUM_SENSORS 5         // because 5 line-sensors in front of the Zumo
//pins
#define FRONT_SENSOR  22      // pin of 3 detections exists
#define LINE_SENSOR_LED  11   // pin of 5 line-sensors

//classes ?
Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer; //for depart-noise
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors; //for IR line-sensors

unsigned int lineSensorValues[NUM_SENSORS]; //"unsigned" to economize memory (instead "int" only)
int LeftNum;
int RightNum;



void setup()
{
  // Uncomment if necessary to correct motor directions:
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);

// setup the line-sensors 
// remember : line sensors are IR-emitters and IR-receptors (IR = infra-red)
  pinMode(LINE_SENSOR_LED, OUTPUT);   // line-sensors can emitt IR ("output")
  digitalWrite(LINE_SENSOR_LED, LOW); // line-sensors are LOW (not HIGH) state in the beginning because they wait for a comming signal 

  pinMode(FRONT_SENSOR, INPUT_PULLUP); 

  lineSensors.initThreeSensors();

  waitForButtonAndCountDown();  // executes called function

  //Avoid enemy in the beginning (just moves, in case of an attack)
  motors.setSpeeds(200, -200); // left-turning
  delay(200);
  motors.setSpeeds(200, 200);  // goes forward
  delay(300);
  
}



void loop()
{
  // to interrupt zumo (during fight) press A
  if (buttonA.isPressed()){
    motors.setSpeeds(0, 0);
    buttonA.waitForRelease();
    waitForButtonAndCountDown();
  }

  lineSensors.read(lineSensorValues);
  // order of function-execution priorities
  BorderDetect();
  DetectEnemy();
  FollowEnemy();

}



// this is the way how we start (or restart) our zumo-fight competition : we press on button A 
// and then there is a yellow LED shinning and a little "melody" as a launch noise-signal (-> after this delay both zumo-robots are starting the fight)
void waitForButtonAndCountDown()
{
  ledYellow(1); //switch-on yellow LED
  lcd.clear();
  lcd.print(F("Press A"));

  buttonA.waitForButton();

  ledYellow(0); //switch-off yellow LED
  lcd.clear();

  // Play audible countdown ("bip, bip, bip, biiiip")
  for (int i = 0; i < 3; i++){
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}


// function detecting the distance to a Zumo-enemy, based on 8 different IR-waves intensities ("distance gradient") detecting from a cloth to a long distance (max 20-30 cm)
void DetectEnemy(){
  const static uint16_t brightnessLevels[] = { 1, 2, 4, 9, 17, 25, 32, 45 }; //list of intensities (uint16_t = unsigned char = 8 bit number from 0 to 255)
  static int nLevels = sizeof(brightnessLevels) /sizeof(uint16_t); //nLevels = 8 in fine
  LeftNum = 0;

  for (uint8_t i = 0; i < nLevels; i++) {
    Zumo32U4IRPulses::start(Zumo32U4IRPulses::Left, brightnessLevels[i], 420);  // sending PWM-pulsing-signal on LEFT LED using "Zumo32U4IRPulses" class (LED-variable, PWM-duty-cylce proportional brightnesslevel, signal-period ~ 38kHz)
    delayMicroseconds(421); // default pulse (guarantees we are not missing output pulses by reading the sensor too soon)
    LeftNum += int(!digitalRead(FRONT_SENSOR));
    //lcd.print(!digitalRead(FRONT_SENSOR));
    Zumo32U4IRPulses::stop();
  }
  RightNum = 0;

  for (uint8_t i = 0; i < nLevels; i++) {
    Zumo32U4IRPulses::start(Zumo32U4IRPulses::Right, brightnessLevels[i], 420); // sending PWM-pulsing-signal on RIGHT LED using "Zumo32U4IRPulses" class (LED-variable, PWM-duty-cylce proportional brightnesslevel, signal-period ~ 38kHz)
    delayMicroseconds(421);  // default pulse (guarantees we are not missing output pulses by reading the sensor too soon)
    RightNum += int(!digitalRead(FRONT_SENSOR));
    //lcd.print(!digitalRead(FRONT_SENSOR));
    Zumo32U4IRPulses::stop();
  }
  
}



// detects an approximative distance and mostly position of the enemy-zumo-robot compared to its position itself
void FollowEnemy(){
  //lcd.gotoXY(0, 0);
  //lcd.print("Enemy ");
  int diff = abs(LeftNum -RightNum);   // diff = threshold (in absolute values ("abs")) fixed to be sure about the direction of the zumo-enemy's position
  
  // if left, goes left
  if (LeftNum > RightNum and diff >=3) {
    motors.setSpeeds(-100,100);
  }
  // if right, goes right
  else if (RightNum > LeftNum and diff >=3) {
    motors.setSpeeds(100,-100);
  }
  // if not clearly at the left or at the right, somewhere in the middle -> go straight forward
  else{
    motors.setSpeeds(300,300); 
  }
  
}



// black-line detection code
void BorderDetect(){
  //lcd.gotoXY(0, 1);
  //lcd.gotoXY(0, 0);
  //lcd.print(lineSensorValues[0]);

  // If sensor-1 detects left-black-border-line, it stops first, turns to right, goes straight
  if (lineSensorValues[0] > QTR_THRESHOLD){
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  // If sensor-4 detects right-black-border-line, it stops first, turns to left, goes straight
  else if (lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD) // NUM_SENSORS = 5; so 5-1 = 4
  {
    // If rightmost sensor detects line, reverse and turn to the
    // left.
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  
}
