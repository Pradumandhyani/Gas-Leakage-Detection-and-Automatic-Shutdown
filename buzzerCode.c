#include <Servo.h>
#include <SoftwareSerial.h>

// Gas sensor pin configuration
const int gasSensorPin = A0;
const int buzzerPin = 4; // Connect the buzzer to digital pin 4

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

// GSM setup
SoftwareSerial gsmSerial(10, 11); // RX, TX

void setup() {
  // Attach the servo to the control pin
  servoMotor.attach(servoControlPin);
  pinMode(buzzerPin, OUTPUT);


  // Initialize the relay pin as an OUTPUT
  pinMode(relayPin, OUTPUT);

  // Initialize the LED pin as an OUTPUT
  pinMode(ledPin, OUTPUT);

  // Initialize Serial communication for debugging
  Serial.begin(9600);

  // Initialize GSM communication
  gsmSerial.begin(9600); // Setting the baud rate of GSM Module

  randomSeed(analogRead(0));
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
    turnOffLED();
    SendMessage();
    turnOnBuzzer();

  } else {
    // Gas level is safe, turn on the regulator (if it was turned off)
    turnOnRegulator();
    turnOffBuzzer();

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
void turnOnBuzzer() {
  // Turn on the buzzer
  digitalWrite(buzzerPin, HIGH);
}

void turnOffBuzzer() {
  // Turn off the buzzer
  digitalWrite(buzzerPin, LOW);
}


void SendMessage() {
  gsmSerial.println("AT+CMGF=1"); // Sets the GSM Module in Text Mode
  delay(1000); // Delay of 1000 milliseconds or 1 second
  gsmSerial.println("AT+CMGS=\"+919557335254\"\r"); // Replace with the mobile number
  delay(1000);
  gsmSerial.println("Gas Leakage Detected. Turn off the gas supply."); // The SMS text you want to send
  delay(100);
  gsmSerial.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}
