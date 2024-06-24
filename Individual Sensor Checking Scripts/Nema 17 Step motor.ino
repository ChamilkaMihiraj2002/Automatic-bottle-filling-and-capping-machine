// Define pin connections
const int dirPin = 2;  // Direction pin
const int stepPin = 5; // Step pin
const int enPin = 8;   // Enable pin

void setup() {
  // Set the pin modes
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enPin, OUTPUT);

  // Enable the motor
  digitalWrite(enPin, LOW); // LOW to enable the motor

  // Set initial direction
  digitalWrite(dirPin, HIGH); // Set direction to forward
}

void loop() {
  // Rotate forward
  stepMotor();
}

// Function to perform a single step
void stepMotor() {

  for (int i = 0; i < 200; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(6000); // Adjust the delay to control the speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(6000); // Adjust the delay to control the speed
  }
 
}
