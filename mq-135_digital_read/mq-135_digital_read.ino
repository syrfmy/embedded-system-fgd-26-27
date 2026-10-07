#define MQ135_DO 2

void setup() {
  Serial.begin(9600);
  pinMode(MQ135_DO, INPUT);
  Serial.println("MQ-135 warming up...");
  delay(3000);
}

void loop() {
  int state = digitalRead(MQ135_DO);

  if (state == LOW) {
    Serial.println("Gas detected! (above threshold)");
  } else {
    Serial.println("Air OK (below threshold)");
  }

  delay(1000);
}