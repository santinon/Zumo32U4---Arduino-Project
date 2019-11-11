/* This program displays the value sensed by the front-proximity-sensor to check the distance
   
 * This demo does not use the dedicated library so you can see what it is doing under the hood (it is here to learn after all) and contains mainy useful explanations
 
 * The "problem" here is that the sensor can only return a digital state: 
 * - it is either 0 if something is sensed
 * - or 1 if nothing is sensed
  
 * Thus, to compute the distance of an object, we test the state of the sensor for different intensity/birgthness of the infrared LED (which makes a kind of "gradient-distance" indicator)
 => there are three sensors in the front of the car (one at the middle below, behind the metal barrier for protection, two others at same level left and right) and 2 transparent LEDs IR-emitors (infra red waves). The sensors receives the IR from the emitors
    the 3 distance-sensors are bigger than the line-senors-LED (used previously for the black line-detection)
 
 * -> Also, there is only one such sensor in the front of the robot usually used for that (so the 2 others on the side are not used currently)
 * So to get LEFT/RIGHT information, we use two LED, one on the LEFT and we only turn one ON at a time (so alternatively, we switch)
 * 
 * Another details is that the sensor is responsive to signals at only 38kHZ
 * and it needs to receive a signal for at least 7 to 15 cycles before turning ON (more details in the code bellow)
 * 
 * -> Because of the above (and the fact that the line-sensors are closed to the proximity-sensors), we need to manually deactivate the small (black/white)-line-sensor-LED (to not pollute/parasites our readings from the LEFT and RIGHT top leds)
 * we do it via the hardware-configurations (that are usually taken care of in the library)
 * See: https://pololu.github.io/zumo-32u4-arduino-library/class_zumo32_u4_proximity_sensors.html, for the dedicated library

 * More about proximity sensor https://www.pololu.com/docs/0J63/3.6
 * and www.creative-robotics.com/quadrature-intro
 */



#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;

// Line-sensor-led pin, with an IR emitting diode (we will just turn it off to avoid noisy reading later on)
#define LINE_SENSOR_LED  11

// Proximity sensor PIN
#define FRONT_SENSOR  22

// "brightnessLevels" = brightness levels = different intensity/birgthness we will test
// the values = PWM high-state-duration relative to the period of the signal (that we set later in the program)
// arbitrary values  
int brightnessLevels[] = { 1, 2, 4, 15, 32, 55, 85, 120 }; 

// This computes the lenght of the list above ("brightnessLevels")
// we compute the size of "brightnessLevels" in memory and divide by the size of a single integer element, So nLevels should be 8 here (8/1=8)
int nLevels = sizeof(brightnessLevels) /sizeof(int);

int leftState;
int rightState;



void setup()
{
  // Turning off the line sensor LED, set to OUTPUT, and turn to LOW state
  pinMode(LINE_SENSOR_LED, OUTPUT);
  digitalWrite(LINE_SENSOR_LED, LOW);

  // Setting proximity-sensor pin to INPUT
  // The "INPUT_PULLUP" means that by defaut, if nothing is connected to the pin, the returned value is 1 (it is pulled towards 1 by default) otherwise 0 if detection
  pinMode(FRONT_SENSOR, INPUT_PULLUP);

  lcd.clear();
  lcd.print(F("Press A"));
  buttonA.waitForButton();
  lcd.clear();
}



void loop()
{
  // Go to the origin on screen
  lcd.gotoXY(0, 0);


  // LEFT LED

  // for each level in "brightnessLevels", we turn IR-LED on, wait a bit so sensor can react to light, read the sensor state, turn the IR LED off, print value on screen, and start again
  for (int i = 0; i < nLevels; i++) { // nLevels = 8
   
   // We start sending a PWM pulsing signal on the LEFT LED, we use the "Zumo32U4IRPulses" class from the library. See start function in https://github.com/pololu/zumo-32u4-arduino-library/blob/master/Zumo32U4IRPulses.cpp
    // It takes three arguments:
    // - 1/ LED, we need to use their variable for it, called: "Zumo32U4IRPulses::Left", more here: (https://github.com/pololu/zumo-32u4-arduino-library/blob/master/Zumo32U4IRPulses.h)
    // - 2/ "brightnesslevel", which is proportional the duty cylce of the PWM. Cannot be higher that the signal period (which is set by the next parameter)
    // - 3/ "signal period" = 420 (is in a weird unit), see comments in https://github.com/pololu/zumo-32u4-arduino-library/blob/master/Zumo32U4IRPulses.h
       // The interval between consecutive the rising edges of pulses will be (1 + period) / (16 MHz).
       // The default value is 420, which results in a period very close to 38 kHz.
       // Indeed: 16000000 / (420 + 1) = 38.005 kHz
       // That is why we use 420 here, because the sensor reacts only to signal at 38kHZ frequency, the 420 is to set that frequency
    Zumo32U4IRPulses::start(Zumo32U4IRPulses::Left, brightnessLevels[i], 420);  // sending PWM-pulsing-signal on LEFT LED using "Zumo32U4IRPulses" class (LED-variable, PWM-duty-cylce proportional brightnesslevel, signal-period ~ 38kHz)
    

    // Now, we need to wait for the sensor to change state if it is receiving the light back
    // We do not wait for the light to come back (this is much faster that the timescale our processor operates at)-> wait for the sensor to have the time to react to the comming-back-light
    // In library: https://github.com/pololu/zumo-32u4-arduino-library/blob/master/Zumo32U4ProximitySensors.h#L205, they explain that, the delay between the start of the IR pulses and the
       // start of the sensor output pulse could be anywhere between 7 cycles /(38 kHz) and 15 cycles /(38 kHz), according to TSSP77038 datasheet
    // The default pulse on time of 16/(38 kHz) = 421us guarantees we are not missing output pulses by reading the sensor too soon.
    // This is why we pause for 421us. It is unrelated to the 420 period parameter we used above
    delayMicroseconds(421); // default pulse (guarantees we are not missing output pulses by reading the sensor too soon)

    // we read the sensor (we use "!" the value just to store it in a way that is easy for a human to interpret)
    // 0 for nothing detected
    // 1 for something detected
    leftState = !digitalRead(FRONT_SENSOR);

    // we can now stop the pulse on the IR LED
    Zumo32U4IRPulses::stop();

    // print on lcd
    lcd.print(leftState);
  }


  // For the right proximity-sensor. Go to next line
  lcd.gotoXY(0, 1);

  // same as above but for right LED
  for (int i = 0; i < nLevels; i++) {  // nLevels = 8
    Zumo32U4IRPulses::start(Zumo32U4IRPulses::Right, brightnessLevels[i], 420);   // sending PWM-pulsing-signal on RIGHT LED using "Zumo32U4IRPulses" class (LED-variable, PWM-duty-cylce proportional brightnesslevel, signal-period ~ 38kHz)
    delayMicroseconds(421);  // default pulse (guarantees we are not missing output pulses by reading the sensor too soon)
    rightState = !digitalRead(FRONT_SENSOR);    // 0 for nothing detected,  1 for something detected
    Zumo32U4IRPulses::stop();
    lcd.print(rightState);
  }
  
}
