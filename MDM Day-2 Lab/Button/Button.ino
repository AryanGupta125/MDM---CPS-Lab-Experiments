const int buttonPin = 2; // Push button pin
const int ledPin = 13;    // Built-in LED pin

void setup() {
  pinMode(ledPin, OUTPUT);
  // Fixed typo: Added the underscore for INPUT_PULLUP
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Initialize Serial Monitor at 9600 baud rate
  Serial.begin(9600);
  Serial.println("System Ready. Press the button...");
}

void loop() {
  // Button is pressed when the pin reads LOW (due to INPUT_PULLUP)
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);  // Fixed typo: changed 'dagatal Wave' to 'digitalWrite'
    Serial.println("Button Pressed -> LED ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("Button Released -> LED OFF");
  }
  
  delay(100); // Small delay to avoid flooding the Serial Monitor
}