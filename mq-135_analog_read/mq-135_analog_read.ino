#define MQ135_AO A0

void setup() {
  Serial.begin(9600);
  Serial.println("MQ-135 warming up...");
  delay(3000);  // short warm-up for demo; give it longer for stable readings
}

void loop() {
  int value = analogRead(MQ135_AO);

  Serial.print("Air quality raw value: ");
  Serial.println(value);

  delay(1000);
}