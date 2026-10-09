#include <Servo.h>
#define SERVO_PIN 9

Servo myServo;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Inisialisasi Pengujian SG90 Mikro Servo..."));

  myServo.attach(SERVO_PIN);
  myServo.write(0);
  delay(1000);
}

void loop() {
  Serial.println(F("[POSISI] Memutar ke sudut 0 Derajat"));
  myServo.write(0);
  delay(2000);

  Serial.println(F("[POSISI] Memutar ke sudut 90 Derajat"));
  myServo.write(90);
  delay(2000);

  Serial.println(F("[POSISI] Memutar ke sudut 180 Derajat"));
  myServo.write(180);
  delay(2000);

  Serial.println(F("[SWEEP] Melakukan pergerakan bertahap (0 -> 180 Derajat)"));
  for (int pos = 0; pos <= 180; pos += 10) {
    myServo.write(pos);
    delay(100);
  }

  Serial.println(F("[SWEEP] Melakukan pergerakan bertahap (180 -> 0 Derajat)"));
  for (int pos = 180; pos >= 0; pos -= 10) {
    myServo.write(pos);
    delay(100);
  }
}
