#define RELAY_PIN 7

// Most relay modules are active LOW (LOW = relay ON).
// If yours is active HIGH, swap these two values.
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

const unsigned long INTERVAL = 2000;  // switch every 2 seconds

unsigned long previousMillis = 0;
bool relayState = false;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);  // start with relay off
  Serial.println("Relay timer demo started.");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;
    relayState = !relayState;

    digitalWrite(RELAY_PIN, relayState ? RELAY_ON : RELAY_OFF);
    Serial.println(relayState ? "Relay ON  - LED circuit closed" : "Relay OFF - LED circuit open");
  }
}