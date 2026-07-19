#include <Arduino_LSM9DS1.h>

// Deadzone threshold in degrees. If tilt is less than this, the board is "Level"
const float TILT_THRESHOLD = 15.0; 

void setup() {
  Serial.begin(115200);
  
  // Configure Onboard RGB LED pins as outputs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Initialize all LEDs to OFF state (Active HIGH logic = OFF)
  digitalWrite(LEDR, HIGH); 
  digitalWrite(LEDG, HIGH);  
  digitalWrite(LEDB, HIGH);

  // Initialize internal IMU hardware
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1); 
  }

  Serial.println("IMU Spatial Orientation & RGB Control Initialized.");
}

void loop() {
  if (IMU.accelerationAvailable()) {
    float x, y, z;
    
    // Read the raw accelerometer gravity vectors
    IMU.readAcceleration(x, y, z);

    // Calculate Roll and Pitch angles in degrees
    float roll  = atan2(y, z) * 57.29578;
    float pitch = atan2(-x, sqrt(y * y + z * z)) * 57.29578;

    String directionLabel = "LEVEL";
    
    // Default hardware configuration: All color registers closed
    bool redState = false;
    bool greenState = false;
    bool blueState = false;

    // 1. Evaluate Pitch Limits (Forward / Backward)
    if (pitch > TILT_THRESHOLD) {
      directionLabel = "TILTED FORWARD";
      redState = true; // Turn Red color element ON
    } 
    else if (pitch < -TILT_THRESHOLD) {
      directionLabel = "TILTED BACKWARD";
      redState = true; // Turn Red color element ON
    }
    
    // 2. Evaluate Roll Limits (Left / Right)
    if (roll > TILT_THRESHOLD) {
      if (directionLabel == "LEVEL") directionLabel = "TILTED LEFT";
      else directionLabel += " & LEFT";
      blueState = true; // Turn Blue color element ON
    } 
    else if (roll < -TILT_THRESHOLD) {
      if (directionLabel == "LEVEL") directionLabel = "TILTED RIGHT";
      else directionLabel += " & RIGHT";
      blueState = true; // Turn Blue color element ON
    }

    // 3. Fallback: If no boundary thresholds are broken, the position is Level
    if (!redState && !blueState) {
      greenState = true; // Turn Green color element ON
    }

    // --- APPLY HARDWARE LOGIC STATES (Active LOW: LOW = ON, HIGH = OFF) ---
    digitalWrite(LEDR, redState   ? LOW : HIGH);
    digitalWrite(LEDG, greenState ? LOW : HIGH);
    digitalWrite(LEDB, blueState  ? LOW : HIGH);

    // Print calculated telemetry data matrix down to serial pipeline
    Serial.print("Roll: "); Serial.print(roll, 1); Serial.print("°\t");
    Serial.print("Pitch: "); Serial.print(pitch, 1); Serial.print("°\t -> ");
    Serial.println(directionLabel);

    delay(100); // 10Hz rapid real-time color mapping adjustment cadence
  }
}