char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void showBoard() {
  for (int i = 0; i < 9; i++) {
    Serial.print(board[i]);
    if (i % 3 == 2) {
      Serial.println(); // new line after every third element
    } else {
      Serial.print(" "); // space between elements
    }
  }
}

void reset() {
  for (int i = 0; i < 9; i++) {
    board[i] = i + '1';
  }
  Serial.println("Press any key to play again.");
  while (Serial.available() == 0) {
    // wait for user input
  }
  Serial.read(); // clear the input buffer
}

void checkLine(int a, int b, int c) {
  if (board[a] == board[b] && board[b] == board[c]) {
    showBoard();
    Serial.print(board[a]);
    Serial.println(" wins!");
    reset();
  }
}

void checkBoard() {
  // search for win/loss
  checkLine(0, 1, 2);
  checkLine(3, 4, 5);
  checkLine(6, 7, 8);
  checkLine(0, 3, 6);
  checkLine(1, 4, 7);
  checkLine(2, 5, 8);
  checkLine(0, 4, 8);
  checkLine(2, 4, 6);
  //search for full board
  bool full = true;
  for (int i = 0; i < 9; i++) {
    if (board[i] != 'X' && board[i] != 'O') {
      full = false;
      break;
    }
  }
  if (full) {
    // cat's game
    showBoard();
    Serial.println("Cat's game...it's a draw!");
    reset();
  }
}

void goCPU() {
  int choiceCPU = random(9);
  while (board[choiceCPU] == 'X' || board[choiceCPU] == 'O') {
    choiceCPU = random(9);
  }
  board[choiceCPU] = 'O';
}

void loop() {
  // put your main code here, to run repeatedly:
  showBoard();
  Serial.println("Where would you like to place an X (1-9)?");
  while (Serial.available() == 0) {
    // wait for user input
  }
  int chosenIndex = Serial.readString().toInt() - 1;
  if (board[chosenIndex] == 'X' || board[chosenIndex] == 'O') {
    Serial.println("That space is already taken!");
  } else if (chosenIndex < 0 || chosenIndex > 8) {
    Serial.println("Invalid selection!");
  } else {
    board[chosenIndex] = 'X';
    checkBoard();
    goCPU();
    checkBoard();
  }
}
