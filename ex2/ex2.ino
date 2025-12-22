#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define SENSITIVITY 0.002381
#define TIME_REPORT 1000

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 27;
const int LOADCELL_SCK_PIN = 26;

unsigned long last_report = 0;

HX711 scale;

enum state {
  measure,
  report
};

state current_state = measure;

double reading = 0.0;

void setup() {
  Serial.begin(115200);
  lcd.init();                      // initialize the lcd 
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  lcd.backlight();
  lcd.clear();
}

void loop() {

  switch(current_state)
  {
    case measure:
      reading = scale.read()*SENSITIVITY;
      if (millis() - last_report >= TIME_REPORT) current_state = report;
      break;
    case report:
      lcd.home();
      Serial.print("HX711 reading: ");
      lcd.print("Weight: ");
      Serial.println(reading);
      lcd.print(reading, 2);
      current_state = measure;
      last_report = millis();
      break;
    default:
      break;
  }
  
}