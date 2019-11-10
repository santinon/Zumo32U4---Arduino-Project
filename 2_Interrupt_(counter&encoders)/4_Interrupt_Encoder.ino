/* The previous programs could not get the direction of the rotation, the counter kept increasing (whatever forward or backward)
 
 *  This program counts both up and down according to the direction of rotation, 
 *  so we have an aboslute rotation position of the motor.
 
 *  To do so, we need to have two sensors and we detect the ordering in state change for each sensors
 *  In one direction sensor A is triggered before sensor B
 *  In the other direction, it is B before A
 
 *  => We do it only for the RIGHT motor only (because has the interrupt function)

 *  See https://www.pololu.com/docs/0J63/3.4 for how the encoder is mapped out on the PIN (basically, a motor has 3 magnets (that turns with from inside) and 2 sensors outside for magnet-detection)
 *  Notice that, because the processor is limited in PIN that can do the interrupt function, we have to use a XOR function 
 *  to be able to only use one interrupt PIN for the two sensors A and B ( => so sensors A&B use only one pin thanks to the XOR function)
 */


#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Motors motors;

#define PIN_RIGHT_XOR  7   // interrupt
#define PIN_RIGHT_B    23  // non-interrupt
    
int count_right;

// last values of A and B sensors (can be 1 or 0)
bool last_right_A;
bool last_right_B;

// new values of A and B sensors (can be 1 or 0)
bool new_right_B;
bool new_right_A;



void setup()
{
  // pinMode is input when we sense
  pinMode(PIN_RIGHT_B, INPUT);
  pinMode(PIN_RIGHT_XOR, INPUT);

  // initialize last values, see on_interrupt() for explanation
  last_right_B = digitalRead(PIN_RIGHT_B);
  last_right_A = digitalRead(PIN_RIGHT_XOR) ^ last_right_B; 

  // initialize counter to 0
  count_right = 0;

  // inittialise the interrupt
  attachInterrupt(digitalPinToInterrupt(PIN_RIGHT_XOR), on_interrupt, CHANGE);
}



void loop()
{
  // We do not have to do anything in the loop, we can just print
  // Try moving the right wheel to see the counter going up and down
  lcd.gotoXY(0, 0);
  lcd.print(count_right);
}



// define interrupt function
void on_interrupt()
{
    // see https://www.pololu.com/docs/0J63/3.4 to understand the signals A and B (and the boolean logic "XOR" )
    
    // reads B directly
    new_right_B = digitalRead(PIN_RIGHT_B);
    // reconstruct A from PIN_RIGHT_XOR and B:
    // We follow the logic that (A^B)^B = A,   where:    (A^B) = digitalRead(PIN_RIGHT_XOR)
    // => so: (A XOR B) XOR B = (A^B)^B = digitalRead(PIN_RIGHT_XOR)^B = (PIN_RIGHT_XOR)^B  = A
    new_right_A = digitalRead(PIN_RIGHT_XOR) ^ new_right_B;

    // fancy way to recover the direction
    // A XOR B = (new_right_A ^ last_right_B) = 1 if the motor turns forward (0 otherwise)   "my visual-way :(if it goes first through A: new A will be advanced compared to B previously, so 1)"
    // A XOR B = (last_right_A ^ new_right_B) = 0 if the motor turns forward (1 otherwise)   "my visual-way :(if it goes first through A: the new B will be in advanced compared to previous A because of the cycle, so 0)"
    count_right += (new_right_A ^ last_right_B) - (last_right_A ^ new_right_B);

    // update last values with current values to get ready for next time "on_interrupt()" is called
    last_right_A = new_right_A;
    last_right_B = new_right_B;
}

/*
 * Help : In one direction sensor A is triggered before sensor B. In the other direction, it is B before A
 * All this XOR thing just gives an idea if the direction has changed or not and the counter is modified (by consideration of the occuring direction) 
 * The XOR is a bolean logic thing that allows us to know the direction in which the right motor turns 
 */
