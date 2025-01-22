#include <LiquidCrystal.h>

const int RS = 8;
const int EN = 9;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
LiquidCrystal lcd(RS,  EN,  d4,  d5,  d6,  d7);

byte off[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte on[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, off);
  lcd.createChar(1, on);
}

void writeAll(int charVal) {
  for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 16; col++) {
      lcd.setCursor(col, row);
      lcd.write(charVal);
    }
  }
}

void loop() {
  // turn all pixels off
  writeAll(0);
  delay(2000);
  // turn all pixels on
  writeAll(1);
  delay(2000);
}
