# Automatic Bottle Filling and Capping Machine

Welcome to the project wiki for the Automatic Bottle Filling and Capping Machine. This project is part of our 1st-year hardware course, focusing on developing an automated system using Arduino.

![83d085e8-5cb1-40f8-a6cb-617f3cf0702b](https://github.com/user-attachments/assets/0e351ede-7a43-424c-8934-312155eb6743)

## Project Overview

This project involves creating a machine that automatically fills bottles with a specified volume of liquid and then caps them. The system uses various sensors, motors, and a servo to achieve this.

https://github.com/user-attachments/assets/351a22a8-2827-4be3-8ac6-083ab7666a52

## Main Features

- Automated bottle detection and positioning
- Volume selection via a keypad
- Real-time display using an LCD
- Automated filling and capping
- Error detection and notification

## Usage Instructions

### Operating the Machine

1. **Power On**: Connect the Arduino Mega to the power supply.
2. **Volume Selection**:
   - Use the keypad to enter the desired volume of liquid.
   - Press 'D' to confirm the volume.
3. **Start the Process**:
   - Press 'A' to start the bottle filling and capping process.
4. **Monitoring**:
   - The LCD will display messages indicating the current status of the machine.
5. **Error Handling**:
   - If an error is detected (e.g., no cap detected), the buzzer will sound and the LCD will display an error message.

### Troubleshooting

- **No response from keypad**: Check the wiring of the keypad.
- **No display on LCD**: Ensure the correct I2C address and connections.
- **Motor not moving**: Verify motor connections and driver settings.

## Hardware Setup

### Components

- Arduino Mega
- Nema 17 Stepper Motor
- Servo Motor (SG90)
- Water Pump
- Flow Sensor
- Proximity Sensors
- Infrared (IR) Sensors
- LCD (LiquidCrystal_I2C)
- Keypad
- Buzzer
- Relays
- Conveyor Motor

### Wiring Diagram

![full](https://github.com/user-attachments/assets/df0eb261-6f8a-41fc-8242-8c0a5bf3d16d)

### Pin Configuration

- **Proximity Sensor**: Pin 3
- **Start Point IR Sensor**: Pin 0
- **Bottle Check IR Sensor**: Pin 22
- **Cap Check IR Sensor**: Pin 2
- **Flow Sensor**: Pin 7
- **Buzzer**: Pin 24
- **Nema 17 Motor**:
  - Step Pin: 5
  - Direction Pin: 6
  - Enable Pin: 4
- **Conveyor Motor**: Pin 9
- **Capper Motor**: Pin 10
- **Water Pump**: Pin 8
- **Servo Motor**: Pin 11
- **LCD**: Address 0x27, 16x2
- **Keypad**:
  - Row Pins: 19, 18, 17, 16
  - Column Pins: 15, 14, 13, 12

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
