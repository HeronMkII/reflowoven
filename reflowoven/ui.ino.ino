#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

void setup() {
    lcd.begin(16,2);
    lcd.print("Press Button");
    delay(1000);
    lcd.clear();
}

void loop() {
    lcd.setCursor(0, 1);
    delay(100);
    lcd.setRGB(0, 255, 0); // set to green
    lcd.print("Safe");
    delay(5000);
    lcd.clear();
    lcd.setRGB(255, 0, 0); // set to red
    lcd.print("Danger");
    delay(5000);
    lcd.clear();
}
