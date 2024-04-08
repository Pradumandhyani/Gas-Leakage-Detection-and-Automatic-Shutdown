#include <Servo.h>

// Gas sensor pin configuration
const int gasSensorPin = A0;

// Servo motor control pin
const int servoControlPin = 9; // Connect the servo's signal wire to digital pin 9

// Relay control pin for the exhaust fan
const int relayPin = 2; // Change this to the actual pin number you are using for the relay

// LED control pin connected to the relay
const int ledPin = 3; // Connect the LED's anode (longer leg) to the relay COM, and cathode (shorter leg) to the relay NO.

// Threshold for gas leakage detection (adjust as needed)
const int gasThreshold = 250; // Change this value depending on your sensor's sensitivity

// Create a Servo object
Servo servoMotor;

void setup() {
  // Attach the servo to the control pin
  servoMotor.attach(servoControlPin);

  // Initialize the relay pin as an OUTPUT
  pinMode(relayPin, OUTPUT);

  // Initialize the LED pin as an OUTPUT
  pinMode(ledPin, OUTPUT);

  // Initialize Serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read the gas sensor value
  int gasValue = analogRead(gasSensorPin);

  // Print the gas value for debugging
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // Check if gas value exceeds the threshold
  if (gasValue > gasThreshold) {
    // Gas leakage detected, turn off the regulator, exhaust fan, and LED
    turnOffRegulator();
    turnOffFan();
    turnOffLED(); // Turn off the LED
  } else {
    // Gas level is safe, turn on the regulator (if it was turned off)
    turnOnRegulator();

    // Check if the fan is off, and turn it on
    if (!fanIsOn()) {
      turnOnFan();
    }

    // Check if the LED is off, and turn it on
    if (!ledIsOn()) {
      turnOnLED();
    }
  }

  // Add a delay between readings to prevent rapid on/off switching
  delay(1000); // Adjust the delay time as needed
}

void turnOnRegulator() {
  // Set the servo motor to the "on" position (adjust the angle as needed)
  servoMotor.write(0); // 0 degrees or the desired "on" position
}

void turnOffRegulator() {
  // Set the servo motor to the "off" position (adjust the angle as needed)
  servoMotor.write(90); // 90 degrees or the desired "off" position
}

void turnOnFan() {
  // Turn on the exhaust fan using the relay
  digitalWrite(relayPin, LOW); // Check the relay module polarity, it might be HIGH for ON
}

void turnOffFan() {
  // Turn off the exhaust fan using the relay
  digitalWrite(relayPin, HIGH); // Check the relay module polarity, it might be LOW for OFF
}

bool fanIsOn() {
  // Check if the exhaust fan is on
  return digitalRead(relayPin) == LOW; // Check the relay module polarity, it might be HIGH for ON
}

void turnOnLED() {
  // Turn on the LED
  digitalWrite(ledPin, HIGH);
}

void turnOffLED() {
  // Turn off the LED
  digitalWrite(ledPin, LOW);
}

bool ledIsOn() {
  // Check if the LED is on
  return digitalRead(ledPin) == HIGH;
}
