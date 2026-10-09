#include <Servo.h>

const int GAS_PIN = 2;
const int RELAY_PIN = 3;
const int PIR_PIN = 8;
const int SERVO_PIN = 9;

const int SERVO_CLOSED_ANGLE = 45;
const int SERVO_OPEN_ANGLE = 140;

Servo myServo;
volatile bool emergencyFlag = false;

unsigned long openStartTime = 0;
bool isOpen = false;
const unsigned long HOLD_TIME = 1000;

void gasDetectedISR() {
  if (digitalRead(GAS_PIN) == LOW) {
    emergencyFlag = true;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(GAS_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  digitalWrite(RELAY_PIN, LOW);

  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_CLOSED_ANGLE);

  attachInterrupt(digitalPinToInterrupt(GAS_PIN), gasDetectedISR, FALLING);

  Serial.println("System Initialized. Running normal PIR + Servo loop...");
}

void loop() {
  if (emergencyFlag) {
    runEmergencyLoop();
  } else {
    runRegularLoop();
  }
}

void runRegularLoop() {
  int pirState = digitalRead(PIR_PIN);
  unsigned long currentMillis = millis();

  if (pirState == HIGH && !isOpen) {
    Serial.println("Motion detected! Opening servo...");
    myServo.write(SERVO_OPEN_ANGLE);
    isOpen = true;
    openStartTime = currentMillis;
  }

  if (isOpen && (currentMillis - openStartTime >= HOLD_TIME)) {
    if (pirState == LOW) {
      Serial.println("Motion cleared. Closing servo...");
      myServo.write(SERVO_CLOSED_ANGLE);
      isOpen = false;
    } else {
      openStartTime = currentMillis;
    }
  }

  delay(50);
}

void runEmergencyLoop() {
  detachInterrupt(digitalPinToInterrupt(GAS_PIN));

  Serial.println("EMERGENCY ALERT: Gas detected! Activating relay.");
  digitalWrite(RELAY_PIN, HIGH);
  myServo.write(SERVO_CLOSED_ANGLE);
  isOpen = false;

  while (digitalRead(GAS_PIN) == LOW) {
    Serial.println("WARNING: Gas levels still high. Holding emergency state...");
    delay(1000);
  }

  Serial.println("Gas cleared. Deactivating relay and resuming normal operation.");
  digitalWrite(RELAY_PIN, LOW);
  emergencyFlag = false;

  delay(100);
  attachInterrupt(digitalPinToInterrupt(GAS_PIN), gasDetectedISR, FALLING);
}
