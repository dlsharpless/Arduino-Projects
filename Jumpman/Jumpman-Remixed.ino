#include <LiquidCrystal.h>

// LCD pins for the shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP_UPPER '.'         // Use '.' for the head
#define SPRITE_JUMP_LOWER 3
#define SPRITE_TERRAIN_EMPTY ' '      // Use ' ' for empty space
#define SPRITE_TERRAIN_SOLID 4
#define SPRITE_TERRAIN_SOLID_RIGHT 5
#define SPRITE_TERRAIN_SOLID_LEFT 6

#define HERO_HORIZONTAL_POSITION 1    // Horizontal position of hero on the screen
#define TERRAIN_WIDTH 16

// Hero positions
#define HERO_POSITION_RUN1 1
#define HERO_POSITION_RUN2 2
#define HERO_POSITION_JUMP1 3
#define HERO_POSITION_JUMP2 4
#define HERO_POSITION_JUMP3 5
#define HERO_POSITION_JUMP4 6
#define HERO_POSITION_JUMP5 7
#define HERO_POSITION_JUMP6 8
#define HERO_POSITION_JUMP7 9
#define HERO_POSITION_JUMP8 10

// Button definitions
#define BTN_NONE -1
#define BTN_SELECT 4
#define BTN_DOWN 2

static char terrainUpper[TERRAIN_WIDTH + 1];
static char terrainLower[TERRAIN_WIDTH + 1];
static unsigned int previousScore = -1;
static bool isPaused = false;  // Pause state
static bool scoreShownInPause = false; // Tracks if score is already shown in pause state

// Read the button state
int readButton() {
  int adcValue = analogRead(A0);
  if (adcValue < 380) return BTN_DOWN;    // "DOWN" button for pause
  if (adcValue < 790) return BTN_SELECT; // "SELECT" button for jump
  return BTN_NONE;
}

// Initialize custom characters
void initializeGraphics() {
  static byte graphics[] = {
    B01100, B01100, B00000, B01110, B11100, B01100, B11010, B10011, // Run1
    B01100, B01100, B00000, B01100, B01100, B01100, B01100, B01110, // Run2
    B11110, B01101, B11111, B10000, B00000, B00000, B00000, B00000, // Jump lower
    B11110, B01101, B11111, B00000, B00000, B00000, B00000, B00000, // Jump fully in top row
    B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111, // Terrain solid
    B00011, B00011, B00011, B00011, B00011, B00011, B00011, B00011, // Terrain solid right
    B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000  // Terrain solid left
  };

  for (int i = 0; i < 7; ++i) {
    lcd.createChar(i + 1, &graphics[i * 8]);
  }

  for (int i = 0; i < TERRAIN_WIDTH; ++i) {
    terrainUpper[i] = SPRITE_TERRAIN_EMPTY;
    terrainLower[i] = SPRITE_TERRAIN_EMPTY;
  }
}

// Update the terrain, ensuring gaps and obstacles are manageable
void advanceTerrain(char* terrain, char newTerrain) {
  for (int i = 0; i < TERRAIN_WIDTH - 1; ++i) {
    terrain[i] = terrain[i + 1];
  }
  terrain[TERRAIN_WIDTH - 1] = newTerrain;
}

// Generate new terrain
void generateTerrain(byte* terrainType, byte* terrainDuration) {
  if (--(*terrainDuration) == 0) {
    if (*terrainType == 0) {
      *terrainType = random(1, 3);  // Create a block
      *terrainDuration = random(2, 4);  // Manageable block length
    } else {
      *terrainType = 0;  // Empty space
      *terrainDuration = random(5, 10);
    }
  }
}

// Draw the hero and handle collisions
bool drawHero(byte heroPos, unsigned int score) {
  bool collided = false;

  // Save terrain at hero's position
  char upperSave = terrainUpper[HERO_HORIZONTAL_POSITION];
  char lowerSave = terrainLower[HERO_HORIZONTAL_POSITION];

  // Determine hero's position on the screen
  char upper = SPRITE_TERRAIN_EMPTY;
  char lower = SPRITE_TERRAIN_EMPTY;

  if (heroPos == HERO_POSITION_RUN1) lower = SPRITE_RUN1;
  else if (heroPos == HERO_POSITION_RUN2) lower = SPRITE_RUN2;
  else if (heroPos == HERO_POSITION_JUMP1 || heroPos == HERO_POSITION_JUMP8) lower = SPRITE_JUMP_LOWER;
  else if (heroPos == HERO_POSITION_JUMP2 || heroPos == HERO_POSITION_JUMP7) {
    upper = SPRITE_JUMP_UPPER;
    lower = SPRITE_JUMP_LOWER;
  } else if (heroPos >= HERO_POSITION_JUMP3 && heroPos <= HERO_POSITION_JUMP6) {
    upper = SPRITE_JUMP_LOWER; // Entire body moves to the top row
    lower = SPRITE_TERRAIN_EMPTY;
  }

  // Detect collisions
  if (upper != SPRITE_TERRAIN_EMPTY && upperSave != SPRITE_TERRAIN_EMPTY) collided = true;
  if (lower != SPRITE_TERRAIN_EMPTY && lowerSave != SPRITE_TERRAIN_EMPTY) collided = true;

  // Overlay hero on terrain
  terrainUpper[HERO_HORIZONTAL_POSITION] = upper;
  terrainLower[HERO_HORIZONTAL_POSITION] = lower;

  // Draw terrain
  lcd.setCursor(0, 0);
  lcd.print(terrainUpper);
  lcd.setCursor(0, 1);
  lcd.print(terrainLower);

  // Draw score
  if (score != previousScore && !isPaused) {
    lcd.setCursor(7, 0);
    lcd.print("Score:    ");  // Clear the entire score area
    lcd.setCursor(13, 0);
    lcd.print(score);
    previousScore = score;
  }

  // Restore terrain
  terrainUpper[HERO_HORIZONTAL_POSITION] = upperSave;
  terrainLower[HERO_HORIZONTAL_POSITION] = lowerSave;

  return collided;
}

void setup() {
  lcd.begin(16, 2);
  initializeGraphics();
}

void loop() {
  static byte heroPos = HERO_POSITION_RUN1;
  static byte terrainType = 0;
  static byte terrainDuration = 5;
  static unsigned int score = 0;
  static bool playing = false;

  int button = readButton();

  // Handle pause/resume functionality
  if (button == BTN_DOWN) {
    isPaused = !isPaused;  // Toggle pause state
    scoreShownInPause = false;  // Reset pause score display flag
    if (isPaused) {
      lcd.setCursor(0, 0);
      lcd.print("Paused          ");  // Show "Paused"
    } else {
      lcd.setCursor(0, 0);
      lcd.print(terrainUpper);        // Restore terrain display
    }
    delay(300);  // Debounce delay
    return;
  }

  // If paused, show the score once
  if (isPaused) {
    if (!scoreShownInPause) {
      lcd.setCursor(7, 0);
      lcd.print("Score:    ");  // Clear the entire score area
      lcd.setCursor(13, 0);
      lcd.print(score);
      scoreShownInPause = true;
    }
    return;
  }

  // Game start
  if (!playing) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press SELECT");
    lcd.setCursor(0, 1);
    lcd.print("to Start");
    if (button == BTN_SELECT) {
      lcd.clear();
      initializeGraphics();
      heroPos = HERO_POSITION_RUN1;
      score = 0;
      playing = true;
    }
    delay(100);
    return;
  }

  // Terrain updates
  advanceTerrain(terrainLower, terrainType == 1 ? SPRITE_TERRAIN_SOLID : SPRITE_TERRAIN_EMPTY);
  advanceTerrain(terrainUpper, terrainType == 2 ? SPRITE_TERRAIN_SOLID : SPRITE_TERRAIN_EMPTY);
  generateTerrain(&terrainType, &terrainDuration);

  // Handle jump
  if (button == BTN_SELECT && heroPos <= HERO_POSITION_RUN2) {
    heroPos = HERO_POSITION_JUMP1;
  }

  // Draw the hero and check for collisions
  if (drawHero(heroPos, score)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    delay(2000);
    playing = false;
    return;
  }

  // Update hero position
  if (heroPos == HERO_POSITION_RUN2 || heroPos == HERO_POSITION_JUMP8) {
    heroPos = HERO_POSITION_RUN1;
  } else {
    heroPos++;
  }

  // Increment score
  score++;
  delay(100);
}
