#define INTERVAL 1000
const int RELAY_PIN = 12;

unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(RELAY_PIN, ledState ? HIGH : LOW);
  }
}
