#include <Wire.h>
#include "rgb_lcd.h" // Include the Grove LCD RGB library

// Create an instance of the LCD object
rgb_lcd lcd;

// Configure your target RGB backlight mix (Values between 0 and 255)
const int colorR = 0;   // Red element
const int colorG = 128; // Green element
const int colorB = 255; // Blue element (This creates a bright Cyan/Light Blue)

unsigned long executionCounter = 0;

void setup() {
  // Initialize the LCD's dimensions: 16 columns and 2 rows
  lcd.begin(16, 2);
  
  // Apply the custom backlight configuration mixture
  lcd.setRGB(colorR, colorG, colorB);
  
  // Clear any existing pixel data on the panel arrays
  lcd.clear();
  
  // Write a fixed string to the top line (Default baseline cursor position is 0, 0)
  lcd.print("Grove I2C LCD");
  
  // Move cursor manually to: Column 0, Row 1 (the secondary line)
  lcd.setCursor(0, 1);
  lcd.print("Booting Core...");
  delay(1500);
}

void loop() {
  // Clear out only the second line metadata text fields by targeting coordinates
  lcd.setCursor(0, 1);
  lcd.print("                "); // Write 16 spaces to wipe the line clean without flickering
  
  // Reposition to start of the bottom line
  lcd.setCursor(0, 1);
  lcd.print("Cycles: ");
  lcd.print(executionCounter);
  
  executionCounter++;
  
  delay(1000); // Rhythmic 1Hz update loop rate execution
}