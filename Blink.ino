const int ledPin = 13;

void setup() {
  Serial.begin(9600); // Communication with Pi
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'H') {
      digitalWrite(ledPin, HIGH); // Fusion Alert
    } else if (cmd == 'L') {
      digitalWrite(ledPin, LOW);  // Clear
    }
  }
}