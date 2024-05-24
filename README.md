Snake Game Firmware

This repository contains the firmware for a Snake Game implemented on an Arduino platform. The game uses a Nokia 5110 LCD display, a DFPlayer Mini MP3 module for sound effects, and RGB lights for visual feedback. The game includes multiple levels and plays different music tracks based on the player's progress.
Table of Contents

    Development Environment
    Libraries Used
    Features
    Setup and Configuration
    Functions Overview
    License

Development Environment

IDE: Arduino IDE
Version: The latest version compatible with the required libraries.
Libraries Used

The project utilizes several third-party libraries:

    DFRobotDFPlayerMini.h: For controlling the DFPlayer Mini MP3 module.
    SPI.h: For SPI communication.
    Adafruit_GFX.h: For general graphics functions.
    Adafruit_PCD8544.h: For the Nokia 5110 LCD display.
    SoftwareSerial.h: For serial communication on non-standard pins.

Features

    Snake Game Logic: Control the snake with buttons to eat food and grow.
    Nokia 5110 LCD Display: Shows the game interface.
    DFPlayer Mini MP3 Module: Plays background music and sound effects.
    RGB Lighting: Changes colors based on game events and levels.
    Multiple Levels: Increases game difficulty as the player progresses.

Setup and Configuration
Hardware Connections

    Nokia 5110 LCD Display:
        RST: Pin 3
        CE: Pin 4
        DC: Pin 5
        DIN: Pin 6
        CLK: Pin 7

    DFPlayer Mini MP3 Module:
        RX: Pin 10
        TX: Pin 11
        BUSY: Pin 8

    RGB Lights:
        BLUE: Pin 12
        GREEN: Pin 13
        RED: Pin 2

    Buttons:
        UP: A1
        DOWN: A2
        LEFT: A3
        RIGHT: A0

Initial Setup

    Connect the hardware components as per the pin configuration above.
    Install the necessary libraries in the Arduino IDE.
    Upload the firmware to the Arduino board.

Functions Overview
Initialization

setup(): Initializes the serial communication, MP3 module, LCD display, buttons, and RGB lights. Starts the game with a countdown and initial settings.
Game Functions

start_game(): Displays the start screen, plays the starting music, and performs a countdown before the game begins.

placeFood(): Generates random positions for the food on the screen, ensuring it does not overlap with the snake's body.

handleInput(): Reads button presses to change the direction of the snake.

moveSnake(): Updates the positions of the snake segments based on the current direction.

drawGame(): Renders the snake and the food on the LCD display.

checkCollision(): Checks for collisions of the snake with walls or itself and resets the game if a collision occurs.

resetGame(): Resets the game state and displays a "Game Over" message on the screen.

checkMusicFinished(): Verifies if the current music track has finished playing and plays the appropriate track for the current game level.
Main Loop

loop(): The main game loop that handles input, updates game state, checks for collisions, and manages music playback based on the game's progress.
