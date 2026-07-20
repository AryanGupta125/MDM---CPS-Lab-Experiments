#include <PDM.h>

// Sound amplitude threshold (Adjust this value based on your environment)
const int SOUND_THRESHOLD = 1500; 

// Buffer to read short samples into
short sampleBuffer[256];
volatile int samplesRead = 0;

void setup() {
  Serial.begin(115200); // 115200 for fast telemetry streaming
  while (!Serial);

  // Configure Onboard RGB LED pins as outputs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Turn all LEDs OFF initially (Active HIGH = OFF)
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  // Configure the data receive callback for the PDM microphone
  PDM.onReceive(onPDMdata);

  // Initialize PDM microphone with 1 channel (mono) at 16 kHz
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM microphone!");
    while (1);
  }
  
  Serial.println("Microphone Initialized. Speak or clap to trigger LED.");
}

void loop() {
  // Check if sound samples are ready to be processed
  if (samplesRead > 0) {
    int maxAmplitude = 0;

    // Find the peak amplitude in the current buffer batch
    for (int i = 0; i < samplesRead; i++) {
      int absoluteValue = abs(sampleBuffer[i]);
      if (absoluteValue > maxAmplitude) {
        maxAmplitude = absoluteValue;
      }
    }

    // Clear the interrupt sample counter flag
    samplesRead = 0;

    // Stream telemetry to the serial architecture
    Serial.print("Peak Amplitude: ");
    Serial.println(maxAmplitude);

    // --- SOUND EVALUATION CONTROL PIPELINE ---
    if (maxAmplitude > SOUND_THRESHOLD) {
      // Sound threshold breached: Flash RED Hazard Warning Alert
      digitalWrite(LEDR, LOW);  // Red ON
      digitalWrite(LEDG, HIGH); // Green OFF
      digitalWrite(LEDB, HIGH); // Blue OFF
    } else {
      // Normal environment: Keep Solid GREEN Safety Status
      digitalWrite(LEDR, HIGH); // Red OFF
      digitalWrite(LEDG, LOW);  // Green ON
      digitalWrite(LEDB, HIGH); // Blue OFF
    }
  }
}

/**
 * Callback function processed automatically when the PDM microphone 
 * hardware buffer fills up with new sound wave data.
 */
void onPDMdata() {
  int bytesAvailable = PDM.available();

  // Read the raw audio bytes into our short array buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 2 bytes per short sample
  samplesRead = bytesAvailable / 2;
}