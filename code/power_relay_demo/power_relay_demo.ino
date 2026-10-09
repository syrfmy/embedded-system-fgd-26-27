#define RELAY_PIN 3

void setup() {
  Serial.begin(9600);
  Serial.println(F("Inisialisasi Pengujian Relai JQC-3FF..."));

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  digitalWrite(RELAY_PIN, LOW);
  Serial.println(F("[AKTIF] Relai ON (Kontak COM-NO Terhubung)"));
  delay(3000);

  digitalWrite(RELAY_PIN, HIGH);
  Serial.println(F("[MATI] Relai OFF (Kontak COM-NO Terputus)"));
  delay(3000);
}
