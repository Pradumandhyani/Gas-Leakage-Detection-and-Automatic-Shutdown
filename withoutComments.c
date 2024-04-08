#include <Servo.h>
#include <SoftwareSerial.h>

const int gasSensorPin = A0;
const int buzzerPin = 4; 
const int servoControlPin = 9;
const int relayPin = 2;
const int ledPin = 3;
const int gasThreshold = 250;
Servo servoMotor;

SoftwareSerial gsmSerial(10, 11);

void setup() {
  servoMotor.attach(servoControlPin);
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  gsmSerial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  int gasValue = analogRead(gasSensorPin);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);
  if (gasValue > gasThreshold) {
    turnOffRegulator();
    turnOffFan();
    turnOffLED();
    SendMessage();
    turnOnBuzzer();
  } else {
    turnOnRegulator();
    turnOffBuzzer();
    if (!fanIsOn()) {
      turnOnFan();
    }
    if (!ledIsOn()) {
      turnOnLED();
    }
  }
  delay(1000);
}

void turnOnRegulator() {
  servoMotor.write(0);
}

void turnOffRegulator() {
  servoMotor.write(90);
}

void turnOnFan() {
  digitalWrite(relayPin, LOW);
}

void turnOffFan() {
  digitalWrite(relayPin, HIGH);
}

bool fanIsOn() {
  return digitalRead(relayPin) == LOW;
}

void turnOnLED() {
  digitalWrite(ledPin, HIGH);
}

void turnOffLED() {
  digitalWrite(ledPin, LOW);
}

bool ledIsOn() {
  return digitalRead(ledPin) == HIGH;
}

void turnOnBuzzer() {
  digitalWrite(buzzerPin, HIGH);
}

void turnOffBuzzer() {
  digitalWrite(buzzerPin, LOW);
}

void SendMessage() {
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+919997868818\"\r");
  delay(1000);
  gsmSerial.println("Gas detected");
  delay(100);
  gsmSerial.println((char)26);
  delay(1000);
}
