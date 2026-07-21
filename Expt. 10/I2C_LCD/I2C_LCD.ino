#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

unsigned long executionCounter = 0;

void setup() {
  // Initialize the LCD (16 columns, 2 rows)
  lcd.begin(16, 2);

  // Set the backlight color (Blue)
  lcd.setRGB(0, 0, 255);

  // Clear the display
  lcd.clear();

  // Print fixed string to Line 0
  lcd.setCursor(0, 0);
  lcd.print("Standard LCD1602");

  // Print boot message
  lcd.setCursor(0, 1);
  lcd.print("Booting Core...");

  delay(1500);
}

void loop() {
  // Clear only the second line
  lcd.setCursor(0, 1);
  lcd.print("                ");   // 16 spaces

  // Print counter
  lcd.setCursor(0, 1);
  lcd.print("Cycles: ");
  lcd.print(executionCounter);

  executionCounter++;

  delay(1000);
}