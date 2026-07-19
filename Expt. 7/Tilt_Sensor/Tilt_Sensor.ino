#include <Arduino_LSM9DS1.h>

// Deadzone threshold in degrees. If tilt is less than this, the board is considered "Level"
const float TILT_THRESHOLD = 15.0; 

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor to open

  // Initialize the internal IMU hardware
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1); // Freeze if hardware fails
  }

  Serial.println("IMU Spatial Orientation Initialized.");
  Serial.println("Keep the board flat on boot.");
}

void loop() {
  if (IMU.accelerationAvailable()) {
    float x, y, z;
    
    // Read the raw accelerometer data (measured in G-forces)
    IMU.readAcceleration(x, y, z);

    // Calculate Roll and Pitch angles in degrees using standard trigonometry
    // atan2 outputs radians, which we convert to degrees by multiplying by 180 / PI (~57.295)
    float roll  = atan2(y, z) * 57.29578;
    float pitch = atan2(-x, sqrt(y * y + z * z)) * 57.29578;

    // String container to hold our dynamic classification label text
    String directionLabel = "LEVEL";

    // 1. Check Primary Pitch Orientations (Forward / Backward)
    if (pitch > TILT_THRESHOLD) {
      directionLabel = "TILTED FORWARD";
    } 
    else if (pitch < -TILT_THRESHOLD) {
      directionLabel = "TILTED BACKWARD";
    }
    
    // 2. Check Primary Roll Orientations (Left / Right)
    // We combine strings if the board is tilted diagonally (e.g., Forward + Left)
    if (roll > TILT_THRESHOLD) {
      if (directionLabel == "LEVEL") directionLabel = "TILTED LEFT";
      else directionLabel += " & LEFT";
    } 
    else if (roll < -TILT_THRESHOLD) {
      if (directionLabel == "LEVEL") directionLabel = "TILTED RIGHT";
      else directionLabel += " & RIGHT";
    }

    // Print calculated telemetry data and directional classification matrix
    Serial.print("Roll: "); Serial.print(roll, 1); Serial.print("°\t");
    Serial.print("Pitch: "); Serial.print(pitch, 1); Serial.print("°\t -> ");
    Serial.println(directionLabel);

    delay(200); // 5Hz data refresh rate tracking cycle 
  }
}