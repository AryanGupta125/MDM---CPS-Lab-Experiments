#include <PDM.h>

// Buffer to store 16-bit PCM audio samples
// Standard frame size for Nano 33 BLE Sense PDM stream
short sampleBuffer[256];

// Flag set by the hardware DMA interrupt callback
volatile int samplesRead = 0;

// Variables for sound level metrics
float rmsLevel = 0.0;
int peakAmplitude = 0;

/**
 * Interrupt Service Routine / Callback function executed 
 * asynchronously when PDM audio data becomes available.
 */
void onPDMdata() {
  int bytesAvailable = PDM.available();
  // Read raw audio bytes into our 16-bit buffer
  samplesRead = PDM.read(sampleBuffer, bytesAvailable) / 2;
}

void setup() {
  // Serial baud rate set to 115200 bps
  Serial.begin(115200);
  while (!Serial);

  // Configure PDM data handler callback
  PDM.onReceive(onPDMdata);

  // Optional: Set microphone hardware gain (Default is 20, Range: 0 to 80)
  // PDM.setGain(30);

  // Initialize PDM with:
  // - Channels: 1 (Mono)
  // - Sample Rate: 16000 Hz (16kHz)
  if (!PDM.begin(1, 16000)) {
    Serial.println("Error: Failed to initialize MP34DT05 PDM Microphone!");
    while (1); // Halt execution on failure
  }

  Serial.println("MP34DT05 Sound Level Logger Online.");
  Serial.println("RMS_Level\tPeak_Amplitude");
}

void loop() {
  // Process incoming audio payload when new samples arrive
  if (samplesRead > 0) {
    long sumSquares = 0;
    int maxVal = 0;

    for (int i = 0; i < samplesRead; i++) {
      int sample = sampleBuffer[i];
      
      // Compute peak magnitude (absolute maximum deviation)
      int absSample = abs(sample);
      if (absSample > maxVal) {
        maxVal = absSample;
      }

      // Sum squares for Root Mean Square (RMS) calculation
      sumSquares += (long)sample * sample;
    }

    // Calculate RMS value for average sound power intensity
    rmsLevel = sqrt((float)sumSquares / samplesRead);
    peakAmplitude = maxVal;

    // Output raw telemetry to Serial Monitor / Plotter
    Serial.print("RMS:");
    Serial.print(rmsLevel, 2);
    Serial.print("\tPeak:");
    Serial.println(peakAmplitude);

    // Clear sample flag to await next DMA frame batch
    samplesRead = 0;
  }

  // Small execution pause to avoid flooding serial buffers
  delay(10);
}