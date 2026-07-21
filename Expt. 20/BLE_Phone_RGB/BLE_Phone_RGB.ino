#include <HardwareBLESerial.h>

HardwareBLESerial &bleSerial = HardwareBLESerial::getInstance();

// Turn RGB LED ON/OFF (Nano 33 BLE built-in RGB LED is ACTIVE LOW)
void setRGB(bool red, bool green, bool blue)
{
  digitalWrite(LEDR, red ? LOW : HIGH);
  digitalWrite(LEDG, green ? LOW : HIGH);
  digitalWrite(LEDB, blue ? LOW : HIGH);
}

void setup()
{
  Serial.begin(9600);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Turn all colors OFF
  setRGB(false, false, false);

  while (!bleSerial.beginAndSetupBLE("RGB_LED"));

  Serial.println("BLE Ready");
}

void loop()
{
  bleSerial.poll();

  while (bleSerial.available())
  {
    char cmd = bleSerial.read();

    Serial.print("Received: ");
    Serial.println(cmd);

    switch (cmd)
    {
      case '1':
        Serial.println("RED");
        setRGB(true, false, false);
        break;

      case '2':
        Serial.println("GREEN");
        setRGB(false, true, false);
        break;

      case '3':
        Serial.println("BLUE");
        setRGB(false, false, true);
        break;

      case '4':
        Serial.println("WHITE");
        setRGB(true, true, true);
        break;

      case '5':
        Serial.println("YELLOW");
        setRGB(true, true, false);
        break;

      case '6':
        Serial.println("CYAN");
        setRGB(false, true, true);
        break;

      case '7':
        Serial.println("MAGENTA");
        setRGB(true, false, true);
        break;

      case '8':
        Serial.println("OFF");
        setRGB(false, false, false);
        break;
    }
  }
}