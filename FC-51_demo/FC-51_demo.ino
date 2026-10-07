#define IR_PIN 2
#define LED_PIN LED_BUILTIN

void setup() {
  Serial.begin(9600);
  pinMode(IR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("FC-51 ready.");
}

void loop() {
  int state = digitalRead(IR_PIN);

  if (state == LOW) {
    Serial.println("Obstacle detected!");
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println("No obstacle.");
    digitalWrite(LED_PIN, LOW);
  }

  delay(200);
}