#define RELAY_PIN 7

void setup() {
  Serial.begin(9600);
  Serial.println(F("Inisialisasi Pengujian DC Motor via Relai..."));

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  Serial.println(F("[STATUS] Mengaktifkan Relai (DC Motor NYALA)"));
  digitalWrite(RELAY_PIN, LOW);
  delay(5000);

  Serial.println(F("[STATUS] Mematikan Relai (DC Motor MATI)"));
  digitalWrite(RELAY_PIN, HIGH);
  delay(3000);
}
