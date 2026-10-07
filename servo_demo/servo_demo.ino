#include <Servo.h>

#define SERVO_PIN 9

Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  Serial.println("SG90 servo demo started.");
}

void loop() {
  // Sweep 0 -> 180
  for (int angle = 0; angle <= 180; angle += 1) {
    myServo.write(angle);
    delay(15);
  }
  Serial.println("Reached 180 degrees");
  delay(500);

  // Sweep 180 -> 0
  for (int angle = 180; angle >= 0; angle -= 1) {
    myServo.write(angle);
    delay(15);
  }
  Serial.println("Reached 0 degrees");
  delay(500);
}