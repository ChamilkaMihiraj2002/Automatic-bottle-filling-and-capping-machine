
//Red = 5V, Brm = Gnd, Orn = COntrolor PWM
// Check what is Home Position
// see if it can move beyond 180 degrees.

//servo.attach() - assign arduino pin connected to PWM pin of servo
//servo.datach() - opposite of above?
//servo.write(angle) - move servo to given angle
//servo.writeMicroseconds(us)- 1000 is fully CCW. 1500 is middle. 2000 is fully CW

#include <Servo.h>
Servo servo;  

int servoPin = 5;
 
void setup()
{
  servo.attach(servoPin);
}
 
 
void loop()
{
  servo.write(0);      // Turn SG90 servo to Home Position. 0 Degrees
  delay(1000);   

  servo.write(180);      // Turn SG90 servo Left to 180 degrees
  delay(1000);          

  servo.write(0);      
  delay(1000);
}
