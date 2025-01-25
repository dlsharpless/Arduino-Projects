int start_time = 0;
int mole;
int score = 0;
bool scored = false;

// LED pins and their corresponding button pins
const int ledPins[] = {2, 3, 4};
const int buttonPins[] = {11, 12, 13};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  // Setup LEDs and buttons
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);   // Set LED pins as OUTPUT
    pinMode(buttonPins[i], INPUT); // Set button pins as INPUT
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // Check if 500 ms has passed since the last mole appearance
  if (millis() > start_time + 500) {
    // Turn off the previous mole LED
    if (mole >= 0) {
      digitalWrite(ledPins[mole], LOW);
    }

    // Select a random mole (0 to 2 for LED indices)
    mole = random(3);
    digitalWrite(ledPins[mole], HIGH);
    scored = false;
    start_time = millis();
  }

  // Check if the mole is whacked
  if (digitalRead(buttonPins[mole]) == HIGH && scored == false) {
    // Turn off the mole LED
    digitalWrite(ledPins[mole], LOW);

    // Update score
    score++;
    scored = true;
    Serial.println(score);
  }
}
