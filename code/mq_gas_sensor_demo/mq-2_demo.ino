// Alokasi Pin Antarmuka Sensor MQ
#define MQ_ANALOG_PIN  = A0;
#define MQ_DIGITAL_PIN = 2;

// Variabel Global Interupsi
volatile bool gasTerdeteksi = false;

// Interrupt Service Routine (ISR) untuk pin DO
void ISR_GasDetected() {
  // Sinyal DO beralih ke LOW ketika gas melampaui threshold
  gasTerdeteksi = (digitalRead(MQ_DIGITAL_PIN) == LOW);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  Serial.println(F(" Inisialisasi Pengujian Sensor Seri MQ  "));

  // Konfigurasi Pin Digital
  pinMode(MQ_DIGITAL_PIN, INPUT_PULLUP);

  // Mendaftarkan Interupsi Eksternal pada Perubahan Logika Pin
  attachInterrupt(digitalPinToInterrupt(MQ_DIGITAL_PIN), ISR_GasDetected, CHANGE);

  Serial.println(F("Melakukan Pre-heating Sensor (Tunggu Stabil)..."));
  delay(2000); // Delay pemanasan
  Serial.println(F("Sensor Siap."));
}

void loop() {
  // 1. Pembacaan Nilai Raw Analog dari ADC MCU
  int rawADC = analogRead(MQ_ANALOG_PIN);

  // 2. Konversi Nilai ADC ke Tegangan Analog (0.0 - 5.0 V)
  float voltage = (rawADC / 1023.0) * 5.0;

  // 3. Menampilkan Hasil Akuisisi ke Serial Monitor
  Serial.print(F("Nilai ADC: "));
  Serial.print(rawADC);
  Serial.print(F(" | Tegangan AO: "));
  Serial.print(voltage, 2);
  Serial.print(F(" V | Status Digital DO: "));

  if (gasTerdeteksi) {
    Serial.println(F("BAHAYA! (Gas > Threshold)"));
  } else {
    Serial.println(F("Aman / Normal"));
  }

  // Interval sampling pembacaan rutin
  delay(1000);
}
