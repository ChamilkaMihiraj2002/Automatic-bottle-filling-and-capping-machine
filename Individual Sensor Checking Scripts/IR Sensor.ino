// Define the pin where the IR sensor is connected
const int irSensorPin1 = 2;
const int irSensorPin2 = 3;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  
  // Set the IR sensor pin as input
  pinMode(irSensorPin1, INPUT);
  pinMode(irSensorPin2, INPUT);
}

void loop() {
  // Read the value from the IR sensor
  int sensorValue1 = digitalRead(irSensorPin1);
  int sensorValue2 = digitalRead(irSensorPin2);
  
  // If an object is detected
  if (sensorValue1 == LOW) {
    // Print a message to the serial monitor
    Serial.println("Object 1 detected");
  } else {
    // Print a message to the serial monitor
    Serial.println("No object 1 detected");
  }
  
  // Wait for a short period before checking again
  delay(1000);

   if (sensorValue2 == LOW) {
    // Print a message to the serial monitor
    Serial.println("Object 2 detected");
  } else {
    // Print a message to the serial monitor
    Serial.println("No object 2 detected");
  }
  delay(1000);
}
