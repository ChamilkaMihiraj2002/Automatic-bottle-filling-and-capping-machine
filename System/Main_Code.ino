#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
Servo servo;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Keypad setup
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {19, 18, 17, 16};    // Row pinouts of the keypad
byte colPins[COLS] = {15, 14, 13, 12};    // Column pinouts of the keypad


// sensor pins
const int proximitySensorPin = 3;
const int startPointIR = 0;
const int bottelCheckIR = 22;
const int CappCheckIR = 2;
const int flowSensorPin = 7;
const int buzzerPin = 24;
const int stepmotorPowerPin = 26;

//motor control pins
//Nema 17
const int stepPin = 5;//CLK+   
const int dirPin = 6;//CW+   
const int enablePin = 4;//EN+  

//conveyor
const int conveyorMotorPin = 9;

//Capper Motor Relay and servo motor
const int capperMotorPin = 10;
int servoPin = 11;

//Water Pump Relay and flow sensor
const int waterPumpPin = 8;

// Variables to store the pulse count and flow rate
volatile uint16_t pulseCount = 0;
float flowRate = 0.0;
float totalVolume = 0.0; // Total volume in liters

// Time variables for calculating flow rate
unsigned long lastTime = 0;
unsigned long interval = 1000; // Interval for calculating flow rate (1 second)

// Calibration factor 
const float calibrationFactor = 4.5; 


char key;
int volume = 0;


void setup() {
  pinMode(proximitySensorPin, INPUT);
  pinMode(startPointIR, INPUT);
  pinMode(bottelCheckIR, INPUT);
  pinMode(CappCheckIR, INPUT);
  pinMode(flowSensorPin, INPUT);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(conveyorMotorPin, OUTPUT);
  pinMode(capperMotorPin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT);
  pinMode(stepmotorPowerPin, OUTPUT);

  // Nema 17 Enable
  digitalWrite(enablePin, LOW); // enable the motor
  digitalWrite(dirPin, LOW); // Set direction 

  // Attach interrupt to the flow sensor pin
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);

  // pump is off to start
  digitalWrite(waterPumpPin, LOW);

  //Servo motor 
  servo.attach(servoPin);

  //KEYPAD
  for (byte i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  for (byte i = 0; i < COLS; i++) {
    pinMode(colPins[i], OUTPUT);
  }
  Serial.begin(9600);

  //DISPLAY
  lcd.init(); 
  lcd.backlight();

  getMilliliterCount();
  lcd.setCursor(0, 0);
  lcd.print("Press A to Start");
  
}

void loop() {
  if (key == 'A') {
    displayMessage("Processing.");
    int hasObsacaleStartPoint = digitalRead(startPointIR);
    if(hasObsacaleStartPoint == LOW) {
      stopStepMotor();
      displayMessage("Conevey start");
      digitalWrite(conveyorMotorPin, HIGH);
      int hasObsacleproximitySensor = digitalRead(proximitySensorPin);
      if(hasObsacleproximitySensor == LOW) {
        stopConveyor();
        displayMessage("Bottel Set..");
        int hasObsaclebottelCheckIR = digitalRead(bottelCheckIR);
        if(hasObsaclebottelCheckIR == HIGH) {
          delay(2000);
          startStepMotor();
          moveBottleToHolder(200);
          stopStepMotor();
          delay(4000);
          fillBottle(volume);
          delay(2000);
          startStepMotor();
          moveBottleToHolder(350);
          stopStepMotor();
          capBottle();

          displayMessage("Capping is done.");
          delay(2000);
          startStepMotor();
          moveBottleToHolder(100);
          stopStepMotor();
          int hasObsacleCappCheckIR = digitalRead(CappCheckIR);
          if(hasObsacleCappCheckIR == HIGH) {
            displayMessage("Capp not found");
            beep(1000, 500);
            while (true) {
              displayMessage("Press 'A' to restart");
              key = getKey();
              if (key == 'A')
              {
                key = '\0';// Reset the code
                loop();
                break;
              }
            }
          } else {
            displayMessage("Capp ok");
            startStepMotor();
            moveBottleToHolder(400);
            stopStepMotor();
            digitalWrite(dirPin, HIGH);
            startStepMotor();
            moveBottleToHolder(400);
            stopStepMotor();
            digitalWrite(dirPin, LOW);
          }
        }
      }
      
    }else {
      stopConveyor();
      startStepMotor();
      moveBottleToHolderToStart();
    }
  } else {
    key = getKey();
  }
}

void displayMessage(const char* data) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(data);
  delay(500);
}

void displayFlowVolume(float flowRate, float totalVolume) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Flow Rate: ");
  lcd.print(flowRate, 1);  // Print with 1 decimal place
  lcd.print(" mL/min");
  lcd.setCursor(0, 1);
  lcd.print("Total Vol: ");
  lcd.print(totalVolume, 1);  // Print with 1 decimal place
  lcd.print(" mL");
}

void moveBottleToHolder(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(stepPin, HIGH); 
    delayMicroseconds(6000); // Adjust the delay to control the speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(6000); // Adjust the delay to control the speed
  }
}

void moveBottleToHolderToStart() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(stepPin, HIGH); 
    delayMicroseconds(6000); // Adjust the delay to control the speed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(6000); // Adjust the delay to control the speed
  }
}

void stopConveyor() {
  digitalWrite(conveyorMotorPin, LOW);
}



void fillBottle(int limit) {
  digitalWrite(waterPumpPin, HIGH);  // Turn the water pump on 
  pulseCount = 0;  // Reset pulse count
  unsigned long startTime = millis();
  unsigned long duration = limit * 32; 

  while (millis() - startTime < duration) {
    // Wait for the duration
  }
  digitalWrite(waterPumpPin, LOW);

  // Calculate the flow rate
  float timeElapsed = (millis() - startTime) / 1000.0;  // Time in seconds
  flowRate = (pulseCount / calibrationFactor) / timeElapsed;  // Flow rate in L/min
  totalVolume += (pulseCount / calibrationFactor) / 1000.0;  // Total volume in liters

  displayFlowVolume(flowRate, totalVolume);
}

// Interrupt service routine to count pulses
void pulseCounter() {
  pulseCount++;
}


void capBottle() {
  servo.write(180);//Turn SG90 servo Left to 180 degrees
  capMotorRun();          
  servo.write(0);//Turn SG90 servo to 0 degrees
}

void capMotorRun() {
  digitalWrite(capperMotorPin, HIGH);
  delay(2000);
  digitalWrite(capperMotorPin, LOW);
}

char getKey() {
  for (byte c = 0; c < COLS; c++) {
    digitalWrite(colPins[c], LOW);
    for (byte r = 0; r < ROWS; r++) {
      if (digitalRead(rowPins[r]) == LOW) {
        digitalWrite(colPins[c], HIGH);
        delay(50);
        return keys[r][c];
      }
    }
    digitalWrite(colPins[c], HIGH);
  }
  return 0;
}

void getMilliliterCount() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Volume:");
  lcd.setCursor(0, 1);
  lcd.print("ml: ");
  
  String input = "";
  char keyPress;
  int setVolume;

  
  while (true) {
    keyPress = getKey();
    if (keyPress >= '0' && keyPress <= '9') {
      input += keyPress;
      lcd.setCursor(4, 1);
      lcd.print(input);
    } else if (keyPress == 'D') {
      break;
    }
    delay(100); // debounce delay
  }
  
  setVolume = input.toInt();
  if (setVolume > 500) {
    getMilliliterCount();
  } else {
    volume = setVolume;
  }
}

void beep(unsigned int frequency, unsigned long duration) {
  tone(buzzerPin, frequency, duration); 
  delay(duration);                    
  noTone(buzzerPin);                    
}

void startStepMotor() {
  digitalWrite(stepmotorPowerPin,HIGH);
}


void stopStepMotor() {
  digitalWrite(stepmotorPowerPin,LOW);
}