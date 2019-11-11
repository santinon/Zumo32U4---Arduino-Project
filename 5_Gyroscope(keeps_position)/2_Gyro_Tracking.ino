/* This program integrates the speed of rotation to get the angular position of the robot
 * 
 * For that we need a precise measure of time elapsed between each measurement
 * 
 * And then position = time_elasped * speed
 * 
 */



#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
L3G gyro;

long current_measured_time;
long last_measured_time;
long dt;

int offset_gyro;  //  gyro has small offset at rest (we measure and remove it from each reading )
int current_gyro; // current measurement

double delta_angle; // change in angle sine last reading
double current_angle;  // heading of the robot


// The conversion from gyro digits to degrees per second (dps) is determined by the sensitivity of the gyro: 0.07 degrees per second per digit.
double convertion_ratio = 0.07 / 1000000;

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
  // We just read the gyro 1000 times and store the mean value
  int total = 0;
  int n_samples = 1000;
  
  for (int i = 0; i < n_samples; i++)
  {
    // Wait for new data to be available, then read it.
    // Trust me, nothing to understand here, it is very low level
    // the logic:  loop is very fast to execute -> might read twice the same value as the gyro also needs some time to upadte its measurement -> wait new available value before reading it
    while(!gyro.readReg(L3G::STATUS_REG) & 0x08) {
      // do nothing 
    }

    // gyro has a new data now, we can read it
    gyro.read();

    // Add the Z axis reading to the total.
    total += gyro.g.z;
  }

  // the average is the offset
  offset_gyro = total / n_samples;

  // we start at angle 0, this is arbitrary, we just say that whatever the robot angle is now, it is considered to be 0
  current_angle = 0;

  // we need a starting reference timewise
  last_measured_time = micros();
}



void loop()
{
   // update gyro measurement
  gyro.read();
  // get time of current measure
  current_measured_time = micros();
  // remove offset and store as current_gyro
  current_gyro = gyro.g.z - offset_gyro;

  // get elapsed time, we call it dt
  dt = current_measured_time - last_measured_time;
  // change in angle is speed * dt and we need a convertion ratio from the unit of the gyro to degrees
  delta_angle = current_gyro * dt * convertion_ratio;
  // our current angle is updated with the delta angle measured
  current_angle += delta_angle;

  // prepare last_measured_time for next time in the loop
  last_measured_time = current_measured_time;

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
