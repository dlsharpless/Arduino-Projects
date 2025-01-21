int start_time = 0;
int mole;
int score = 0;
bool scored = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(5));
  // setup pins 2-4 as LEDs and pins 11-13 as their respective buttons
  for (int i = 2; i < 5; i++) {
    pinMode(i, OUTPUT);
    pinMode(i + 9, INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // check if half a second has passed since last mole appearance
  if (millis() > start_time + 500) {
    //reset mole selection
    digitalWrite(mole, LOW);
    mole = random(2, 5);
    digitalWrite(mole, HIGH);
    scored = false;
    start_time = millis();
  }
  // check if mole is whacked
  if (digitalRead(mole + 9) == HIGH && scored == false) {
    // count the whack
    digitalWrite(mole, LOW);
    score++;
    scored = true;
    Serial.println(score);
  }
}
