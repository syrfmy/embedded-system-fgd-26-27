#include <DHT.h>

#define DHTPIN 3      // Pin Digital MCU yang terhubung ke pin DATA DHT22
#define DHTTYPE DHT22 // Menentukan tipe sensor (DHT22/AM2302)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Pengujian Sensor DHT22/AM2302"));
  dht.begin();
}

void loop() {
  // Jeda minimal 2 detik antar pembacaan (spesifikasi sampling rate DHT22)
  delay(2000);

  float kelembapan = dht.readHumidity();
  float suhu = dht.readTemperature(); // Membaca suhu dalam satuan Celsius

  // Memeriksa jika pembacaan gagal (kembalian NaN)
  if (isnan(kelembapan) || isnan(suhu)) {
    Serial.println(F("Gagal membaca data dari sensor DHT22!"));
    return;
  }

  // Menampilkan hasil pembacaan ke Serial Monitor
  Serial.print(F("Kelembapan: "));
  Serial.print(kelembapan);
  Serial.print(F("%  |  Suhu: "));
  Serial.print(suhu);
  Serial.println(F(" *C"));
}
