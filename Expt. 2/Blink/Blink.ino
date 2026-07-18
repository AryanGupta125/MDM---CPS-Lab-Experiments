void setup() {
  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("System Initialized. Starting Blink Loop...");
}

// The loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("LED is ON");       // Print status to Serial Monitor
  delay(1000);                       // wait for a second

  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("LED is OFF");      // Print status to Serial Monitor
  delay(1000);                       // wait for a second
}


