#include <Arduino_APDS9960.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Configure Onboard RGB LED pins as outputs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Initialize all RGB elements to OFF state initially (Active HIGH = OFF)
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
    // Light up built-in Blue LED if the physical chip failed to start
    digitalWrite(LEDB, LOW); 
    while (1);
  }
}

void loop() {
  // check if a proximity reading is available
  if (APDS.proximityAvailable()) {
    // read the proximity
    // - 0   => far away
    // - 255 => extremely close
    // - -1  => error
    int proximity = APDS.readProximity();

    // print value to the Serial Monitor
    Serial.print("Proximity Value: ");
    Serial.print(proximity);
    Serial.print(" -> Zone Status: ");

    // --- REAL-TIME DISTANCE THRESHOLD INTERACTION MAP ---
    if (proximity < 100) {
      // ZONE 1: SAFE (Solid Green)
      Serial.println("SAFE");
      digitalWrite(LEDR, HIGH); // Red OFF
      digitalWrite(LEDG, LOW);  // Green ON
      digitalWrite(LEDB, HIGH); // Blue OFF
    } 
    else if (proximity >= 100 && proximity < 200) {
      // ZONE 2: WARNING (Yellow = Mix Red and Green)
      Serial.println("WARNING DECREASING CLEARANCE");
      digitalWrite(LEDR, LOW);  // Red ON
      digitalWrite(LEDG, LOW);  // Green ON
      digitalWrite(LEDB, HIGH); // Blue OFF
    } 
    else {
      // ZONE 3: CRITICAL BREACH (Solid Red)
      Serial.println("HAZARD DETECTED");
      digitalWrite(LEDR, LOW);  // Red ON
      digitalWrite(LEDG, HIGH); // Green OFF
      digitalWrite(LEDB, HIGH); // Blue OFF
    }
  }

  // wait a bit before reading again
  delay(100);
}