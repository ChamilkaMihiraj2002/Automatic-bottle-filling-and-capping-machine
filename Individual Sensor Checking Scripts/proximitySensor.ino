const int proximitySensorPin = 3; // Pin connected to the proximity sensor
int sensorValue = 0; // Variable to store the sensor value

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  pinMode(proximitySensorPin, INPUT); // Set the proximity sensor pin as an input
}

void loop() {
  // Read the value from the proximity sensor
  sensorValue = digitalRead(proximitySensorPin);
  
  // Print the sensor value to the Serial Monitor
  Serial.print("Proximity Sensor Value: ");
  Serial.println(sensorValue);
  
  // Check if an object is detected (assuming the sensor gives HIGH output when an object is detected)
  if (sensorValue == HIGH) {
    Serial.println("Object detected!");
  } else {
    Serial.println("No object detected.");
  }
  
  delay(500); // Wait for half a second before the next reading
}
