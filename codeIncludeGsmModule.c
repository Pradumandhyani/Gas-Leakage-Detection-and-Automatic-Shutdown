#include <Servo.h>
#include <SoftwareSerial.h>

// Gas sensor pin configuration
const int gasSensorPin = A0;

// Servo motor control pin
const int servoControlPin = 9;  // Connect the servo's signal wire to digital pin 9

// Relay control pin for the exhaust fan
const int relayPin = 2;  // Change this to the actual pin number you are using for the relay

// Threshold for gas leakage detection (adjust as needed)
const int gasThreshold = 150;   // Change this value depending on your sensor's sensitivity

// Create a Servo object
Servo servoMotor;

// SoftwareSerial for GSM module
SoftwareSerial gsmSerial(7, 8); // RX, TX - Connect GSM module's TX to Arduino's RX, and RX to Arduino's TX

void setup() {
  // Attach the servo to the control pin
  servoMotor.attach(servoControlPin);

  // Initialize the relay pin as an OUTPUT
  pinMode(relayPin, OUTPUT);

  // Initialize Serial communication for debugging
  Serial.begin(9600);
  delay(1000); // Give time for the Serial Monitor to open

  // Initialize SoftwareSerial for the GSM module
  gsmSerial.begin(9600);
}

void loop() {
  // Read the gas sensor value
  int gasValue = analogRead(gasSensorPin);

  // Print the gas value for debugging
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // Check if gas value exceeds the threshold
  if (gasValue > gasThreshold) {
    // Gas leakage detected, turn off the regulator and exhaust fan
    turnOffRegulator();
    turnOffFan();
    Serial.println("Gas Leakage Detected! Fan and Regulator Turned Off.");
    
    // Send an SMS
    sendSMS("Gas Leakage Detected!");
  } else {
    // Gas level is safe, turn on the regulator (if it was turned off)
    turnOnRegulator();

    // Check if the fan is off, and turn it on
    if (!fanIsOn()) {
      turnOnFan();
      Serial.println("Fan Turned On.");
    }
  }

  // Add a delay between readings to prevent rapid on/off switching
  delay(1000);  // Adjust the delay time as needed
}

void turnOnRegulator() {
  // Set the servo motor to the "on" position (adjust the angle as needed)
  servoMotor.write(0);  // 0 degrees or the desired "on" position
}

void turnOffRegulator() {
  // Set the servo motor to the "off" position (adjust the angle as needed)
  servoMotor.write(90);  // 90 degrees or the desired "off" position
}

void turnOnFan() {
  // Turn on the exhaust fan using the relay
  digitalWrite(relayPin, LOW);  // Check the relay module polarity, it might be HIGH for ON
}

void turnOffFan() {
  // Turn off the exhaust fan using the relay
  digitalWrite(relayPin, HIGH);  // Check the relay module polarity, it might be LOW for OFF
}

bool fanIsOn() {
  // Check if the exhaust fan is on
  return digitalRead(relayPin) == LOW;  // Check the relay module polarity, it might be HIGH for ON
}

void sendSMS(String message) {
  gsmSerial.println("AT+CMGF=1"); // Set the GSM module to text mode
  delay(1000);
  gsmSerial.print("AT+CMGS=\""); // Start sending an SMS to the specified phone number
  gsmSerial.print("+1234567890"); // Replace with the recipient's phone number
  gsmSerial.println("\"");
  delay(1000);
  gsmSerial.println(message); // SMS message content
  delay(1000);
  gsmSerial.write(26); // Ctrl+Z to end the SMS
  delay(1000);
}
