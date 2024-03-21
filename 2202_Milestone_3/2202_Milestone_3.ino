/* Trial 2 */

#include <Arduino.h>

// Define motor control pins
#define LEFT_MOTOR_A 35
#define LEFT_MOTOR_B 36
#define RIGHT_MOTOR_A 37
#define RIGHT_MOTOR_B 38

// Define encoder pins
#define ENCODER_LEFT_A 15
#define ENCODER_LEFT_B 16
#define ENCODER_RIGHT_A 11
#define ENCODER_RIGHT_B 12

// Define constants for movement
#define DRIVE_SPEED 150 // Adjust as needed
#define TURN_SPEED 150  // Adjust as needed
#define TURN_DURATION 1000 // Time in milliseconds to turn

// Function prototypes
void moveForward(int duration);
void moveBackward(int duration);
void turnLeft(int duration);
void turnRight(int duration);

void setup() {
  // Initialize motor control pins
  pinMode(LEFT_MOTOR_A, OUTPUT);
  pinMode(LEFT_MOTOR_B, OUTPUT);
  pinMode(RIGHT_MOTOR_A, OUTPUT);
  pinMode(RIGHT_MOTOR_B, OUTPUT);
}

void loop() {
  // Move forward for 3 meters
  moveForward(3000); // Assuming each meter takes 1000 milliseconds to cross
  
  // Turn left
  turnLeft(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(3000);
  
  // Turn right
  turnRight(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(3000);
  
  // Turn left
  turnLeft(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(3000);
  
  // Turn right
  turnRight(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(3000);
  
  // Stop - End of loop
}

void moveForward(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void moveBackward(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH);
  delay(duration);
}

void turnLeft(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void turnRight(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH);
  delay(duration);
}

/* Trial 2 */
#include <Arduino.h>

// Define motor control pins
#define LEFT_MOTOR_A 35
#define LEFT_MOTOR_B 36
#define RIGHT_MOTOR_A 37
#define RIGHT_MOTOR_B 38

// Define encoder pins
#define ENCODER_LEFT_A 15
#define ENCODER_LEFT_B 16
#define ENCODER_RIGHT_A 11
#define ENCODER_RIGHT_B 12

// Define constants for movement
#define DRIVE_SPEED 150 // Adjust as needed
#define TURN_SPEED 150  // Adjust as needed
#define TURN_DURATION 1000 // Time in milliseconds to turn
#define DRIVE_DURATION 3000 // Time in milliseconds to drive forward

// Function prototypes
void moveForward(int duration);
void turnLeft(int duration);
void turnRight(int duration);

void setup() {
  // Initialize motor control pins
  pinMode(LEFT_MOTOR_A, OUTPUT);
  pinMode(LEFT_MOTOR_B, OUTPUT);
  pinMode(RIGHT_MOTOR_A, OUTPUT);
  pinMode(RIGHT_MOTOR_B, OUTPUT);
}

void loop() {
  // Move forward for 3 meters
  moveForward(DRIVE_DURATION);

  // Turn left
  turnLeft(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(DRIVE_DURATION);
  
  // Turn right
  turnRight(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(DRIVE_DURATION);
  
  // Turn left
  turnLeft(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(DRIVE_DURATION);
  
  // Turn right
  turnRight(TURN_DURATION);
  
  // Move forward for 3 meters
  moveForward(DRIVE_DURATION);
  
  // Stop - End of loop
}

void moveForward(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void turnLeft(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void turnRight(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH);
  delay(duration);
}

/* Trial 3 */
#include <Arduino.h>

// Define motor control pins
#define LEFT_MOTOR_A 35
#define LEFT_MOTOR_B 36
#define RIGHT_MOTOR_A 37
#define RIGHT_MOTOR_B 38

// Define constants for movement
#define DRIVE_SPEED 150 // Adjust as needed
#define TURN_SPEED 150  // Adjust as needed
#define TURN_DURATION 1000 // Time in milliseconds to turn
#define DRIVE_DURATION 3000 // Time in milliseconds to drive forward
#define SQUARE_DURATION 500 // Time in milliseconds to drive in a square

// Function prototypes
void moveForward(int duration);
void turnLeft(int duration);
void turnRight(int duration);
void driveSquare();
void zigzag();

void setup() {
  // Initialize motor control pins
  pinMode(LEFT_MOTOR_A, OUTPUT);
  pinMode(LEFT_MOTOR_B, OUTPUT);
  pinMode(RIGHT_MOTOR_A, OUTPUT);
  pinMode(RIGHT_MOTOR_B, OUTPUT);
}

void loop() {
  // Move forward for 3 meters
  moveForward(DRIVE_DURATION);

  // Execute zigzag pattern
  zigzag();

  // Execute square pattern
  driveSquare();

  // Return to starting position
  turnLeft(TURN_DURATION);
  turnLeft(TURN_DURATION);
  moveForward(DRIVE_DURATION);

  // Stop - End of loop
}

void moveForward(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void turnLeft(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void turnRight(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH);
  delay(duration);
}

void driveSquare() {
  for (int i = 0; i < 4; i++) {
    moveForward(SQUARE_DURATION);
    turnRight(TURN_DURATION);
  }
}

void zigzag() {
  for (int i = 0; i < 3; i++) {
    moveForward(DRIVE_DURATION);
    turnLeft(TURN_DURATION);
    moveForward(DRIVE_DURATION);
    turnRight(TURN_DURATION);
  }
}
