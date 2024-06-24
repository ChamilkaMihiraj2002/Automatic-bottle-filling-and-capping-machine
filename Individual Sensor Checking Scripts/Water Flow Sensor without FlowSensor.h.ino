// Constants
const int flowSensorPin = 2; // Pin where the flow sensor is connected
const int waterpumpPin = 8; // Pin where the water pump is connected
const float calibrationFactor = 4.0; // Calibration factor for YF-S201 sensor

// Variables
volatile int pulseCount = 0;
float flowRate = 0.0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;
bool waterFillingComplete = false; // Flag to track if water filling is complete
unsigned long completionTime = 0; // Time when the water filling was completed

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);

  // Set the flow sensor pin as input
  pinMode(flowSensorPin, INPUT);
  digitalWrite(flowSensorPin, HIGH); // Enable pull-up resistor

  // Attach interrupt to the flow sensor pin
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);

  // Set the water pump pin as output and turn on the pump
  pinMode(waterpumpPin, OUTPUT);
  digitalWrite(waterpumpPin, HIGH);

  // Initialize timing
  oldTime = millis();
}

void loop() {
  if (!waterFillingComplete) {
    waterpump();
  } else {
    // If water filling is complete, wait for 10 seconds before restarting
    if (millis() - completionTime >= 10000) {
      // Reset variables to restart the water pump
      totalMilliLitres = 0;
      waterFillingComplete = false;
      digitalWrite(waterpumpPin, HIGH); // Turn on the pump
      Serial.println("Restarting water pumping.");
    } else {
      // Add any additional functionality you want to execute after water filling is complete
      Serial.println("Water filling complete. Waiting to restart...");
      delay(1000); // Wait for 1 second before the next status message
    }
  }
}

void waterpump() {
  if (totalMilliLitres <= 50000) {
    // If a second has passed, calculate the flow rate and volume
    if ((millis() - oldTime) > 1000) {
      // Disable the interrupt while calculating
      detachInterrupt(digitalPinToInterrupt(flowSensorPin));

      // Calculate the flow rate in L/min
      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
      
      // Reset the timer and pulse count
      oldTime = millis();
      pulseCount = 0;

      // Calculate the flow in milliliters
      flowMilliLitres = (flowRate / 60.0) * 1000;
      
      // Add to the total volume
      totalMilliLitres += flowMilliLitres;

      // Print the flow rate and total volume to the Serial Monitor
      Serial.print("Flow rate: ");
      Serial.print(flowRate * 1000 / 60); // Corrected to show mL/min
      Serial.print(" mL/min");
      Serial.print("\t");
      Serial.print("Total: ");
      Serial.print(totalMilliLitres); // Corrected to show total milliliters
      Serial.println(" mL");

      // Re-enable the interrupt
      attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);
    }
  } else {
    digitalWrite(waterpumpPin, LOW); // Turn off the pump
    waterFillingComplete = true; // Set the flag to indicate water filling is complete
    completionTime = millis(); // Record the time when the filling was completed
  }

  // Add a small delay to avoid a busy loop
  delay(100);
}

void pulseCounter() {
  // Increment the pulse count
  pulseCount++;
}
