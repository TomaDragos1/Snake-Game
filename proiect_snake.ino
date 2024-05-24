#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SoftwareSerial.h>
//rst 3
//ce 4
//dc 5
//din 6
//clk 7
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//Pins for MP3 player

#define RX_PIN 10
#define TX_PIN 11
#define BUSY_PIN 8 // Pin to check the BUSY status

//Pin for RGB lights

#define BLUE 12
#define GREEN 13
#define RED 2

//MP3 player

DFRobotDFPlayerMini mp3;
SoftwareSerial softwareSerial(RX_PIN, TX_PIN);

//Variabels for buttons and width and height of the screen

#define SCREEN_WIDTH 84
#define SCREEN_HEIGHT 48


#define UP_BUTTON A1
#define DOWN_BUTTON A2
#define LEFT_BUTTON A3
#define RIGHT_BUTTON A0

#define SNAKE_LENGTH 60
#define CELL_SIZE 4

//structure for my snake

struct Point {
  int x;
  int y;
};


//snake variables

Point snake[SNAKE_LENGTH];
int snakeLength = 1;
Point food;
int direction = 0; // 0: right, 1: down, 2: left, 3: up

int score = 0;
int upgrade = 0;

unsigned long lastMoveTime = 0;
unsigned long moveInterval = 600; // Speed of the snake

//generate the food for the snake

void placeFood() {
  bool validPosition = false;
  
  while (!validPosition) {
    food.x = random(SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
    food.y = random(SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
    
    validPosition = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snake[i].x == food.x && snake[i].y == food.y) {
        validPosition = false;
        break;
      }
    }
}
}

//function that displays start game on the screen
//and plays a mario start music while reverse countdown from 3 to 1

void start_game(){
  moveInterval = 600;
  score = 0;
  upgrade = 0;

  analogWrite(BLUE, 255);
  analogWrite(GREEN, 0);
  analogWrite(RED, 0);

  //display start game

  display.setContrast(50);
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(BLACK);
  display.setCursor(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - 10);
  display.println("Start Game");
  display.display();
  delay(3000);
  mp3.volume(20);
  mp3.play(2); // Example to play the track again
  display.clearDisplay();

  //display 3, 2, 1
  display.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10);
  display.println("3");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10);
  display.println("2");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10);
  display.println("1");
  display.display();
  delay(1000);
}


void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
  //veri if the mp3 playes works
  if (mp3.begin(softwareSerial)) {
    Serial.println("Connected to DFPlayer");
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
  display.begin();

  //buttons for up, down, left, right

  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  //pin color output

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  //lcd display

  start_game();

  display.begin();
  display.setContrast(50);
  display.clearDisplay();

  //initialize the snake and put the first food
  //on the lcd

  snake[0] = {0, 0};
  placeFood();
}

//function that returns my current direction
// and i can't go to the opossite direction
void handleInput() {
  if (digitalRead(UP_BUTTON) == LOW && direction != 1) {
    direction = 3;
  }
  if (digitalRead(DOWN_BUTTON) == LOW && direction != 3) {
    direction = 1;
  }
  if (digitalRead(LEFT_BUTTON) == LOW && direction != 0) {
    direction = 2;
  }
  if (digitalRead(RIGHT_BUTTON) == LOW && direction != 2) {
    direction = 0;
  }
}

//update the snake for every second of my game

void moveSnake() {
  //move snake
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  //go to the direction pressed last time by the player
  switch (direction) {
    case 0: snake[0].x += CELL_SIZE; break;
    case 1: snake[0].y += CELL_SIZE; break;
    case 2: snake[0].x -= CELL_SIZE; break;
    case 3: snake[0].y -= CELL_SIZE; break;
  }
}

//draw the game

void drawGame() {
  display.clearDisplay();
  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snake[i].x, snake[i].y, CELL_SIZE, CELL_SIZE, BLACK);
  }
  // Draw food
    if (food.x >= 0 && food.y >= 0) {
      display.fillRect(food.x, food.y, CELL_SIZE, CELL_SIZE, BLACK);
    }
  display.display();
}

//reste game if you lose

void resetGame() {
  snakeLength = 1;
  snake[0] = {0, 0};
  direction = 0;

  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(BLACK);
  display.setCursor(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - 10);
  display.println("Game Over");
  display.display();
  mp3.volume(15);
  mp3.play(3); // Example to play the track again
  delay(3000);


  placeFood();

  start_game();
}

//check function for the collisions

void checkCollision() {
  // Check if snake eats the food
  if (snake[0].x == food.x && snake[0].y == food.y) {
    // Serial.print("EATEN THE FRUIT");
    snakeLength++;
    score++;
    if (snakeLength > SNAKE_LENGTH) snakeLength = SNAKE_LENGTH;
    placeFood();
  }

  // Check for collision with walls
  if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH ||
      snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT) {
    resetGame();
  }

  // Check for collision with itself
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      resetGame();
    }
  }
}

//function that plays music depeding on the level that i currently am
//and check if it finnished
void checkMusicFinished() {
  if (digitalRead(BUSY_PIN) == HIGH && upgrade == 0) {
    Serial.println("Track finished playing");
    mp3.volume(20);
    mp3.play(1); 
  } 

  if (digitalRead(BUSY_PIN) == HIGH && upgrade == 1) {
    Serial.println("Track finished playing");
    mp3.volume(20);
    mp3.play(4);
  } 

  if (digitalRead(BUSY_PIN) == HIGH && upgrade == 2) {
    Serial.println("Track finished playing");
    mp3.volume(20);
    mp3.play(6);
  } 
}


void loop() {
  //handle the button pressed
  handleInput();

  //go to the second level and display on lcd
  if (score >= 3 && upgrade == 0) {
      moveInterval = 500;
      analogWrite(BLUE, 0);
      analogWrite(GREEN, 255);
      analogWrite(RED, 0);
      display.setContrast(50);
      display.clearDisplay();
      display.setTextSize(0.5);
      display.setTextColor(BLACK);
      display.setCursor(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - 10);
      display.println("LEVEL 2");
      display.display();
      mp3.volume(20);
      mp3.play(5);
      delay(2000);
      upgrade = 1;
  }
  //go to the third level and display
  if (score >= 6 && upgrade == 1) {
      moveInterval = 400;
      analogWrite(BLUE, 0);
      analogWrite(GREEN, 0);
      analogWrite(RED, 255);
      display.setContrast(50);
      display.clearDisplay();
      display.setTextSize(0.5);
      display.setTextColor(BLACK);
      display.setCursor(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - 10);
      display.println("LEVEL 3");
      display.display();
      mp3.volume(20);
      mp3.play(5);
      delay(2000);
      upgrade = 2;
  }
  
  //timer for my snake to determine the speed
  unsigned long currentTime = millis();
  if (currentTime - lastMoveTime >= moveInterval) {
    //logic of the snake
    lastMoveTime = currentTime;
    checkCollision();
    moveSnake();
    drawGame();
  }
  //check to see if its music and play it
  checkMusicFinished();
}
