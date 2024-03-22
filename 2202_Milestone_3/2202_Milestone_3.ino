//FLuid Version

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

//variable declarations
unsigned int mode = 0;
unsigned int modePBDebounce = 0;
const int cServoPin          = 41;                 // GPIO pin for servo motor
const int cServoChannel      = 5;                  // PWM channel used for the RC servo motor


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
  // Setup Motor
  pinMode(cServoPin, OUTPUT);                         // sets the servo to output
  ledcSetup(cServoChannel, 50, 14);                // setup for channel for 50 Hz, 14-bit resolution
  ledcAttachPin(cServoPin, cServoChannel);         // assign servo pin to servo channel
}

void loop() {

   // Mode pushbutton debounce and toggle
      if (!digitalRead(3)) {                                         // if pushbutton GPIO goes LOW (nominal push)
         // Start debounce
         if (modePBDebounce <= 25) {                                           // 25 millisecond debounce time
            modePBDebounce = modePBDebounce + 1;                               // increment debounce timer count
            if (modePBDebounce > 25) {                                         // if held for at least 25 mS
               modePBDebounce = 1000;                                          // change debounce timer count to 1 second
            }
         }
         if (modePBDebounce >= 1000) {                                         // maintain 1 second timer count until release
            modePBDebounce = 1000;
         }
      }
      else {                                                                   // pushbutton GPIO goes HIGH (nominal release)
         if(modePBDebounce <= 26) {                                            // if release occurs within debounce interval
            modePBDebounce = 0;                                                // reset debounce timer count
         }
         else {
            modePBDebounce = modePBDebounce + 1;                               // increment debounce timer count
            if(modePBDebounce >= 1025) {                                       // if pushbutton was released for 25 mS
               modePBDebounce = 0;                                             // reset debounce timer count
               mode++;                                               // switch to next mode
               mode = mode & 4;                            // keep mode index between 0 and 7
            }
         }
      }

  switch (mode){
    
    case 1:
    moveForward(3000);
    setServoPosition(45); // Set servo position to 45 degrees
    delay(1000);
    moveForward(3000);
    setServoPosition(0); // Set servo position to 0 degrees
    delay(1000);
    moveBackward(3000);
    setServoPosition(135); // Set servo position to 135 degrees
    delay(1000);
    moveForward(3000);
    setServoPosition(45); // Set servo position to 45 degrees
    delay(1000);
    moveBackward(3000);
    setServoPosition(135); // Set servo position to 135 degrees
    delay(1000);
    moveForward(3000);
    break;
    
    case 0:

    //move forwards 3s
    moveForward(3000);

setServoPosition(45); // Set servo position to 45 degrees
  delay(1000); // Wait for 1 second

    //turn 45* left
    turnLeft(500);

setServoPosition(135); // Set servo position to 45 degrees
  delay(1000); // Wait for 1 second

    //drive forwards 2s
    moveForward(2000);

    //turn 90* right
    turnRight(1000);

    //forwards 2s
    moveForward(2000);

    //backwards 8.243s
    moveBackward(8.243);

    //turn 45* right
    turnRight(500);

    //forwards for 5.828s
    moveForward(5.828);

    //turn left 90*
    turnLeft(1000);
    break;

    case 2:
  // Move forward for 3 meters
    moveForward(3000); // Assuming each meter takes 1000 milliseconds to cross

    // Turn left (1000)
    turnLeft(TURN_DURATION);
  

    // Move forward for 3 meters
    moveForward(3000);
    
setServoPosition(0); // Set servo position to 45 degrees
  delay(1000); // Wait for 1 second


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
    
    break;

    case 3:
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
    
    break;

    case4:
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
    break;
  }
}

void moveForward(int duration) { //move forwards
  digitalWrite(LEFT_MOTOR_A, HIGH); //forward channel high
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, HIGH); //forward channel high
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void moveBackward(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH); //backward channel high
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH); //backward channel high
  delay(duration);
}

void turnLeft(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH); //backwards channel high
  digitalWrite(RIGHT_MOTOR_A, HIGH); //forwards channel high
  digitalWrite(RIGHT_MOTOR_B, LOW);
  delay(duration);
}

void turnRight(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH); //forwrads channel high
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH); //backwards channel high
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

void setServoPosition(int degrees) {  //writes the servo to a duty cycle   
  int dutyCycle = degreesToDutyCycle(degrees);
  ledcWrite(cServoChannel, dutyCycle);
}

int degreesToDutyCycle(int degrees) { //maps servo degrees to duty cycle
  // Map degrees (0-180) to duty cycle (500-2500)
  return map(degrees, 0, 180, 500, 2500);
}













