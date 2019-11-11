/* This program showcase your first feedback control loop
 *  
 *  We use the angle measure reconstructed from the gyro
 *  
 *  We provide a target angle to reach, here angle=0
 *  
 *  We measure the error between actual and targetet
 *  
 *  We act proportionally to the error to come back to the target
 * 
 * ==> basically when the Zumo is moved on a surface, it will maintains its positions somehow, very cool effect, try it out!
 */


 
#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
L3G gyro;

long current_measured_time;
long last_measured_time;
long dt;

int offset_gyro;
int current_gyro;

double delta_angle;
double current_angle;

// The conversion from gyro digits to degrees per second (dps) is determined by the sensitivity of the gyro: 0.07 degrees per second per digit.
double convertion_ratio = 0.07 / 1000000;

int maxSpeed = 200;

int target_angle = 0; // where we want the robot to head
int error_angle;      // error between target and measured
int turnSpeed;        // speed applied to the motor, proportional to the error

// time tracking for plotting only every 100ms
int lastSampleTime = 0;



void setup()
{
  Wire.begin();
  gyro.init();
  // 800 Hz output data rate, low-pass filter cutoff 100 Hz
  gyro.writeReg(L3G::CTRL1, 0b11111111);
  // 2000 dps full scale
  gyro.writeReg(L3G::CTRL4, 0b00100000);
  // High-pass filter disabled
  gyro.writeReg(L3G::CTRL5, 0b00000000);


  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();

  // Delay to give the user time to remove their finger.
  lcd.clear();
  lcd.print(F("Cal Gyro"));
  delay(1000);

  // Calibrate the gyro.
  int total = 0;
  int n_samples = 1000;
  for (int i = 0; i < n_samples; i++)
  {
    // Wait for new data to be available, then read it.
    while(!gyro.readReg(L3G::STATUS_REG) & 0x08);
    gyro.read();

    // Add the Z axis reading to the total.
    total += gyro.g.z;
  }
  offset_gyro = total / n_samples;


  //
  current_angle = 0;
  gyro.read();
  last_measured_time = micros();
}


void loop()
{
  // same as before to go from speed to position
  gyro.read();
  current_gyro = gyro.g.z - offset_gyro;

  current_measured_time = micros();
  dt = current_measured_time - last_measured_time;
  last_measured_time = current_measured_time;
  
  delta_angle = (double)dt * (double)current_gyro * convertion_ratio;
  current_angle += delta_angle;


  // start of your feedback control:
  // compute error
  error_angle = current_angle - target_angle;
  // define speed as a multiple of the error
  turnSpeed = -100 * error_angle;
  // we just constraint the speed within [-maxSpeed, maxSpeed]
  turnSpeed = constrain(turnSpeed, -maxSpeed, maxSpeed);
  // and apply it to the motors
  motors.setSpeeds(-turnSpeed, turnSpeed);
  // all of that as fast as we can
  

  // we print only every 100ms, this loop need to go very fast for dt to be small and reduce the error in the integration process
  if ((millis() - lastSampleTime) >= 100)
  {
    lastSampleTime = millis();

    lcd.clear();
     // print delta_angle on top
    lcd.gotoXY(0, 0);
    lcd.print(delta_angle);

     // print current_angle on bottom
    lcd.gotoXY(0, 1);
    lcd.print(current_angle);
  }  

  
}
