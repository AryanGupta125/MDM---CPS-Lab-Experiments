#include <Arduino_LSM9DS1.h>

// Define a threshold for tilting (adjust between 0.1 and 0.9 depending on sensitivity)
const float TILT_THRESHOLD = 0.4; 

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  // Initialize the IMU sensor
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Initialize RGB LED pins as outputs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Turn all LEDs OFF initially (Nano 33 BLE RGB is active LOW)
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Tilt the board to change LED colors!");
}

void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    // Turn off all LEDs first before deciding which one to turn on
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);

    // Check tilt directions based on X and Y acceleration axes
    if (x > TILT_THRESHOLD) {
      // Tilted Right -> RED
      digitalWrite(LEDR, LOW);
      Serial.println("Tilted Right [RED]");
    } 
    else if (x < -TILT_THRESHOLD) {
      // Tilted Left -> GREEN
      digitalWrite(LEDG, LOW);
      Serial.println("Tilted Left [GREEN]");
    } 
    else if (y > TILT_THRESHOLD) {
      // Tilted Forward -> BLUE
      digitalWrite(LEDB, LOW);
      Serial.println("Tilted Forward [BLUE]");
    } 
    else if (y < -TILT_THRESHOLD) {
      // Tilted Backward -> CYAN (Green + Blue combined)
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, LOW);
      Serial.println("Tilted Backward [CYAN]");
    } 
    else {
      // Board is relatively flat -> LEDs stay OFF
      Serial.print("Flat -> X: ");
      Serial.print(x);
      Serial.print("\tY: ");
      Serial.println(y);
    }

    delay(150); // Small delay to stabilize the readings and serial prints
  }
}