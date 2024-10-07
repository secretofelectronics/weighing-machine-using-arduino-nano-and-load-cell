#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Pushbutton.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;
float reading;
float lastReading;
// REPLACE WITH YOUR CALIBRATION FACTOR
#define CALIBRATION_FACTOR 235

// LCD I2C Address (usually 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16x2 display

// Button
#define BUTTON_PIN 4
Pushbutton button(BUTTON_PIN);

void displayWeight(float weight) {
  lcd.clear();                      // Clear the display
  lcd.setCursor(0, 0);              // Set cursor to the first row
  lcd.print("Weight:");             // Display static text
  lcd.setCursor(0, 1);              // Set cursor to the second row
  if (weight >= 1000) {
    weight = weight / 1000;
    lcd.print(weight, 2);           // Display weight in kilograms with 2 decimal places
    lcd.print(" Kg");
  } else {
    lcd.print(weight, 3);           // Display weight in grams with 3 decimal places
    lcd.print(" g");
  }
}

void setup() {
  Serial.begin(57200);

  // Initialize the I2C LCD
  lcd.init();                       // Initialize the LCD
  lcd.backlight();                  // Turn on the backlight

  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(CALIBRATION_FACTOR);   // Set the calibration factor
  scale.tare();                          // Reset the scale to 0
}

void loop() {

  if (button.getSingleDebouncedPress()) {
    Serial.print("tare...");
    scale.tare();
  }

  if (scale.wait_ready_timeout(200)) {
    reading = scale.get_units();
    Serial.print("Weight: ");
    Serial.print(reading);
    Serial.println(" g");
    if (reading != lastReading) {
      if (reading < 10) {
        displayWeight(0);            // Display the weight on the LCD in kg
      } else {
        displayWeight(reading);      // Display the weight on the LCD
      }
    }
    lastReading = reading;
    delay(1000);
  } else {
    Serial.println("HX711 not found.");
  }
}