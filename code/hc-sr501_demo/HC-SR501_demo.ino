#define PIR_PIN 2;
volatile bool motionDetected = false;

void pirISR() {
  motionDetected = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Inisialisasi Pengujian Sensor HC-SR501..."));
  pinMode(PIR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);
}

void loop() {
  if (motionDetected) {
    Serial.println(F("[TERDETEKSI] Pergerakan objek termal terdeteksi!"));
    motionDetected = false;
  } else {
    Serial.println(F("[STANBY] Tidak ada gerakan (Kondisi Aman)"));
  }

  delay(1000);
}
