/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

/** Operational Range
   Servo 1 : 0:70 [0:1.22] => R to L (to stay in the workspace)
   Servo 2 : Fix 2 at 70 -> Bolted [1.22]
   Servo 3 : 90:180 => D to U [1.57:3.14]
   Servo 4 : 20:120 => U to D => Note: Will fail after almost 1.3 minutes [0.349:2.094]
   Servo 5 : MO LIMITS (ROLL) [0:180 => L to R]
   Servo 6 : Open: 10 (0.174), close: 70 (1.22)
 */

/** Pick and Place Operation
 * Pick: 
 * 1. Servo 1 go the desired position
 * 2. Serco 3 -> 125
 * 3. Servo 4 go 0
 * 4. Servo 6 OPEN
 * 5. Servo 6 CLOSE
 * 6. Servo 3 -> 180
 * 
 * and then place
 */


#include <Servo.h>

Servo s1,s2,s3,s4,s5,s6;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

int minpos = 0;
int maxpos = 100;

void setup() {
//  s1.attach(3);
//  s2.attach(5);
    s3.attach(10);
//  s4.attach(9);
//  s5.attach(10);
//  s6.attach(11);

//  s1.write(135);
//  s3.write(150);
//  s4.write(50);
//  s5.write(100);
//  s6.write(10);   
}

void loop() {
//  for (pos = 90; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  for (pos = 180; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }

   for (pos = minpos; pos <= maxpos; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    s3.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  for (pos = maxpos; pos >= minpos; pos -= 1) { // goes from 180 degrees to 0 degrees
    s3.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
}
