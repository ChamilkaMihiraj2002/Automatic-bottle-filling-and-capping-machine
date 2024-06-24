#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <FlowSensor.h>
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
const int proximitySensorPin = 3;//set
const int startPointIR = 0;//set
const int bottelCheckIR = 22;//set
const int CappCheckIR = 2;//set
const int flowSensorPin = 7;//set
const int buzzerPin = 24;// not set

//motor control pins
//Nema 17
const int stepPin = 5;//CLK+   set
const int dirPin = 6;//CW+     set
const int enablePin = 4;//EN+  set

//conveyor
const int conveyorMotorPin = 9;// set

//Capper Motor Relay and servo motor
const int capperMotorPin = 10;//set
int servoPin = 11;//set

//Water Pump Relay and flow sensor
const int waterPumpPin = 8;//set
const uint16_t sensorType = 450; 
FlowSensor flowSensor(sensorType, flowSensorPin);

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

  // Nema 17 Enable
  digitalWrite(enablePin, LOW); // enable the motor
  digitalWrite(dirPin, LOW); // Set direction 

  // Initialize the flow sensor
  flowSensor.begin(flowSensorISR);
  // Start the Serial communication
  Serial.begin(9600);
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
      displayMessage("Conevey start");
      digitalWrite(conveyorMotorPin, HIGH);
      int hasObsacleproximitySensor = digitalRead(proximitySensorPin);
      if(hasObsacleproximitySensor == LOW) {
        stopConveyor();
        displayMessage("Bottel Set..");
        int hasObsaclebottelCheckIR = digitalRead(bottelCheckIR);
        if(hasObsaclebottelCheckIR == HIGH) {
          delay(2000);
          moveBottleToHolder(200);
          delay(4000);
          fillBottle(volume);
          moveBottleToHolder(300);
          capBottle();

          displayMessage("Capping is done.");
          delay(2000);
          moveBottleToHolder(100);
          int hasObsacleCappCheckIR = digitalRead(CappCheckIR);
          if(hasObsacleCappCheckIR == HIGH) {
            displayMessage("Capp not found");
            beep(1000, 500);
            while (true) {
              displayMessage("Press 'A' to restart");
              key = getKey();
              if (key == 'A')
              {
                break;
              }
            }
          } else {
            displayMessage("Capp ok");
            moveBottleToHolder(400);
            digitalWrite(dirPin, HIGH);
            moveBottleToHolder(400);
            digitalWrite(dirPin, LOW);
          }
        }
      }
      
    }else {
      stopConveyor();
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
}

void displayFlowVolume(float flowRate, float totalVolume) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Flow Rate: ");
  lcd.print(flowRate/10, 1);  // Print with 1 decimal place
  lcd.print(" mL/min");
  lcd.setCursor(0, 1);
  lcd.print("Total Vol: ");
  lcd.print(totalVolume/10, 1);  // Print with 1 decimal place
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
  digitalWrite(waterPumpPin, HIGH);  // Turn the water pump on by activating the relay
  flowSensor.resetPulse();
  int delaytime = limit * 32;
  delay(delaytime);
  digitalWrite(waterPumpPin, LOW);
  flowSensor.read(0); // Pass 0 if no additional calibration is needed
  float flowRate = flowSensor.getFlowRate_m();// Calculate flow rate in L/min
  float totalVolume = flowSensor.getVolume();
  displayFlowVolume(flowRate, totalVolume);
}


//Interrupt Service Routine for the flow sensor
void flowSensorISR() {
  flowSensor.count();
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
