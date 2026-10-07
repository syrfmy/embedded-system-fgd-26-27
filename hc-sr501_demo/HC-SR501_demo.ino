#define PIR_PIN 2
#define LED_PIN LED_BUILTIN

int lastState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("PIR warming up (30 seconds)...");
  delay(30000);
  Serial.println("Ready.");
}

void loop() {
  int state = digitalRead(PIR_PIN);

  if (state == HIGH && lastState == LOW) {
    Serial.println("Motion detected!");
    digitalWrite(LED_PIN, HIGH);
  } else if (state == LOW && lastState == HIGH) {
    Serial.println("Motion ended.");
    digitalWrite(LED_PIN, LOW);
  }

  lastState = state;
  delay(50);
}