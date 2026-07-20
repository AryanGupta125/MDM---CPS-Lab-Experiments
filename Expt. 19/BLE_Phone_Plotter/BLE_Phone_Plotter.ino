#include <Arduino_LSM9DS1.h>
#include <HardwareBLESerial.h>
HardwareBLESerial &bleSerial = HardwareBLESerial::getInstance();
void setup() {
Serial.begin(9600);
while(!bleSerial.beginAndSetupBLE("Aryan"));
while(!IMU.begin());
}
void loop() {
float x, y, z;
if (IMU.accelerationAvailable()) {
IMU.readAcceleration(x, y, z);
bleSerial.poll();
Serial.print(x); bleSerial.print(x);
Serial.print('\t'); bleSerial.print('\t');
Serial.print(y); bleSerial.print(y);
Serial.print('\t'); bleSerial.print('\t');
Serial.println(z); bleSerial.println(z);
}
delay(10); // to avoid flooding bleSerial
}