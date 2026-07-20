#include <Arduino.h>
#include <Arduino_APDS9960.h>
#include <nrf.h>

#define PROXIMITY_THRESHOLD 150
#define TARGET_COUNT        10

bool detected = false;

//------------------------------------------------------
// Configure TIMER3 as 32-bit hardware counter
//------------------------------------------------------
void setupCounter()
{
  NRF_TIMER3->TASKS_STOP = 1;
  NRF_TIMER3->TASKS_CLEAR = 1;

  NRF_TIMER3->MODE = TIMER_MODE_MODE_Counter;
  NRF_TIMER3->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
  NRF_TIMER3->PRESCALER = 0;

  NRF_TIMER3->TASKS_START = 1;
}

//------------------------------------------------------
// Read counter
//------------------------------------------------------
uint32_t getCounter()
{
  NRF_TIMER3->TASKS_CAPTURE[0] = 1;
  return NRF_TIMER3->CC[0];
}

//------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  unsigned long t = millis();
  while (!Serial && millis() - t < 3000);

  Serial.println();
  Serial.println("Nano 33 BLE Sense");
  Serial.println("Initializing...");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  setupCounter();

  if (!APDS.begin())
  {
    Serial.println("ERROR: APDS9960 initialization failed!");
    while (1);
  }

  Serial.println("APDS9960 Initialized");
  Serial.println("Waiting for proximity...");
}

//------------------------------------------------------
void loop()
{
  if (APDS.proximityAvailable())
  {
    int proximity = APDS.readProximity();

    Serial.print("Proximity = ");
    Serial.println(proximity);

    if (proximity >= PROXIMITY_THRESHOLD)
    {
      if (!detected)
      {
        detected = true;

        // Increment hardware counter
        NRF_TIMER3->TASKS_COUNT = 1;

        uint32_t count = getCounter();

        Serial.print("Detection Count = ");
        Serial.println(count);

        if (count >= TARGET_COUNT)
        {
          digitalWrite(LED_BUILTIN, HIGH);

          Serial.println("-------------------------");
          Serial.println("TARGET REACHED");
          Serial.println("-------------------------");
        }
      }
    }
    else
    {
      detected = false;
    }
  }

  delay(20);
}