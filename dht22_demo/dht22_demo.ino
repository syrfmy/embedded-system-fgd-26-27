#include <DHT.h>

#define DHTPIN 2        // Data pin
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);  // DHT22 needs ~2s between readings

  float h = dht.readHumidity();
  float t = dht.readTemperature();  // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT22! Check wiring.");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %  |  Temperature: ");
  Serial.print(t);
  Serial.println(" C");
}