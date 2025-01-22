#include <LiquidCrystal.h>

// Select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Define some values used by the panel and buttons
int lcdKey = 0;
int adcKeyIn = 0;

int weapon = 0;
bool pressed = false;

String line0[9] = {
  // Possible responses to rock
  "Rock--------Draw",
  "Paper------I win",
  "Scissors-You win",
  // Possible responses to paper
  "Rock-----You win",
  "Paper-------Draw",
  "Scissors---I win",
  // Possible responses to scissors
  "Rock-------I win",
  "Paper----You win",
  "Scissors----Draw"
};

String line1[3] = {
  "<<    Rock    >>",
  "<<   Paper    >>",
  "<<  Scissors  >>"
};

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

int readButtons() {
  adcKeyIn = analogRead(0); // Read the value from the sensor

  if (adcKeyIn < 50) return btnRIGHT;
  if (adcKeyIn < 195) return btnUP;
  if (adcKeyIn < 380) return btnDOWN;
  if (adcKeyIn < 555) return btnLEFT;
  if (adcKeyIn < 790) return btnSELECT;

  return btnNONE; // When all others fail, return this
}

void setup() {
  randomSeed(analogRead(0));
  lcd.begin(16, 2); // Start the library
  gameOn();
}

void gameOn() {
  // Display the play screen
  lcd.setCursor(0, 0);
  lcd.print("Select a weapon:");
  lcd.setCursor(0, 1);
  lcd.print(line1[weapon]);
}

void gameOver() {
  // CPU plays randomly and displays result
  lcd.setCursor(0, 0);
  lcd.print(line0[random(3 * weapon, 3 * weapon + 3)]);
  lcd.setCursor(0, 1);
  lcd.print(line1[weapon]);
}

void loop() {
  lcdKey = readButtons(); // Read the buttons

  // Depending on which button was pushed, perform an action
  switch (lcdKey) {
    case btnUP:
    case btnRIGHT:
      if (!pressed) {
        pressed = true;

        if (weapon < 2) {
          weapon++;
        } else {
          weapon = 0;
        }
      }
      gameOn();
      break;
    case btnDOWN:
    case btnLEFT:
      if (!pressed) {
        pressed = true;

        if (weapon > 0) {
          weapon--;
        } else {
          weapon = 2;
        }
      }
      gameOn();
      break;
    case btnSELECT:
      if (!pressed) {
        pressed = true;
        gameOver();
      }
      break;
    case btnNONE:
      pressed = false;
      break;
  }
}
