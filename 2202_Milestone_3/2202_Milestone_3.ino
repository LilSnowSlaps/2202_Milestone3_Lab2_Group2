//FLuid Version

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

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

// Function prototypes
void moveForward(int duration);
void turnLeft(int duration);
void turnRight(int duration);
void driveSquare();
void zigzag();

#define DEBUG_DRIVE_SPEED    1
#define DEBUG_ENCODER_COUNT  1

#include <Adafruit_NeoPixel.h>

// Function declarations
void Indicator(); // for mode/heartbeat on Smart LED
void setMotor(int dir, int pwm, int in1, int in2);
void ARDUINO_ISR_ATTR encoderISR(void* arg);

// Encoder structure
struct Encoder {
   const int chanA; // GPIO pin for encoder channel A
   const int chanB; // GPIO pin for encoder channel B
   long pos; // current encoder position
};

// Port pin constants
#define LEFT_MOTOR_A        35 // GPIO35 pin 28 (J35) Motor 1 A
#define LEFT_MOTOR_B        36// GPIO36 pin 29 (J36) Motor 1 B
#define RIGHT_MOTOR_A       37 // GPIO37 pin 30 (J37) Motor 2 A
#define RIGHT_MOTOR_B       38// GPIO8 pin 31 (J38) Motor 2 B
#define ENCODER_LEFT_A      15// left encoder A signal is connected to pin 8 GPIO15 (J15)
#define ENCODER_LEFT_B      16 // left encoder B signal is connected to pin 8 GPIO16 (J16)
#define ENCODER_RIGHT_A     11 // right encoder A signal is connected to pin 19 GPIO11 (J11)
#define ENCODER_RIGHT_B     12 // right encoder B signal is connected to pin 20 GPIO12 (J12)
#define MODE_BUTTON         0 // GPIO0  pin 27 for Push Button 1
#define MOTOR_ENABLE_SWITCH 3 // DIP Switch S1-1 pulls Digital pin D3 to ground when on,connected to pin 15 GPIO3 (J3)
#define POT_R1              1 // when DIP Switch S1-3 is on, Analog AD0 (pin 39) GPIO1 is connected to Poteniometer R1
#define SMART_LED           21 // when DIP Switch S1-4 is on, Smart LED is connected to pin 23 GPIO21 (J21)
#define SMART_LED_COUNT     1  // number of SMART LEDs in use

// Constants
const int cDisplayUpdate = 100; // update interval for Smart LED in milliseconds
const int cNumMotors = 2; // number of DC motors
const int cIN1Pin[] = {LEFT_MOTOR_A, RIGHT_MOTOR_A}; // GPIO pin(s) for INT1
const int cIN1Chan[] = {0, 1}; // PWM channe(s) for INT1
const int c2IN2Pin[] = {LEFT_MOTOR_B, RIGHT_MOTOR_B}; // GPIO pin(s) for INT2
const int cIN2Chan[] = {2, 3}; // PWM channel(s) for INT2
const int cPWMRes = 8; // bit resolution for PWM
const int cMinPWM = 150; // PWM value for minimum speed that turns motor
const int cMaxPWM = pow(2, cPWMRes) - 1; // PWM value for maximum speed
const int cPWMFreq = 20000; // frequency of PWM signal
const int cCountsRev = 1096; // encoder pulses per motor revolution
const int d = 5; //assumed diameter (5cm)
const long pi = 3.14; //rounded value of pi

// Variables
boolean motorsEnabled = true; // motors enabled flag
boolean timeUp3sec = false; // 3 second timer elapsed flag
boolean timeUp2sec = false; // 2 second timer elapsed flag
unsigned char driveSpeed; // motor drive speed (0-255)
unsigned char driveIndex; // state index for run mode
unsigned long timerCount3sec = 0; // 3 second timer count in milliseconds
unsigned long timerCount2sec = 0; // 2 second timer count in milliseconds
unsigned long displayTime; // heartbeat LED update timer
unsigned long previousMicros; // last microsecond count
unsigned long currentMicros; // current microsecond count

// Declare SK6812 SMART LED object
//   Argument 1 = Number of LEDs (pixels) in use
//   Argument 2 = ESP32 pin number
//   Argument 3 = Pixel type flags, add together as needed:
//     NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//     NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//     NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//     NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//     NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel SmartLEDs(SMART_LED_COUNT, SMART_LED, NEO_RGB + NEO_KHZ800);

// smart LED brightness for heartbeat
unsigned char LEDBrightnessIndex = 0;
unsigned char LEDBrightnessLevels[] =
{5,15,30,45,60,75,90,105,120,135,150,165,180,195,210,225,240,255,240,225,210,195,180,165,150,135,120,105,90,75,60,45,30,15};

unsigned int  robotModeIndex = 0; // robot operational state
unsigned int  modeIndicator[6] = { // colours for different modes
   SmartLEDs.Color(255,0,0), //   red - stop
   SmartLEDs.Color(0,255,0), //   green - run
   SmartLEDs.Color(0,0,255), // blue - empty case
   SmartLEDs.Color(255,255,0), //   yellow - empty case
   SmartLEDs.Color(0,255,255), //  cyan - empty case
   SmartLEDs.Color(255,0,255) //   magenta - empty case
};

Encoder encoder[] = {{ENCODER_LEFT_A, ENCODER_LEFT_B, 0},
        // left encoder, 0 position
                     {ENCODER_RIGHT_A, ENCODER_RIGHT_B, 0}};
        // right encoder, 0 position





void setup() {
  // Initialize motor control pins
  pinMode(LEFT_MOTOR_A, OUTPUT);
  pinMode(LEFT_MOTOR_B, OUTPUT);
  pinMode(RIGHT_MOTOR_A, OUTPUT);
  pinMode(RIGHT_MOTOR_B, OUTPUT);
  #if defined DEBUG_DRIVE_SPEED || DEBUG_ENCODER_COUNT
   Serial.begin(115200);
#endif

   // Set up motors and encoders
   for (int k = 0; k < cNumMotors; k++) {
      ledcAttachPin(cIN1Pin[k], cIN1Chan[k]);
        // attach INT1 GPIO to PWM channel
      ledcSetup(cIN1Chan[k], cPWMFreq, cPWMRes);
        // configure PWM channel frequency and resolution
      ledcAttachPin(c2IN2Pin[k], cIN2Chan[k]);
        // attach INT2 GPIO to PWM channel
      ledcSetup(cIN2Chan[k], cPWMFreq, cPWMRes);
        // configure PWM channel frequency and resolution
      pinMode(encoder[k].chanA, INPUT);
        // configure GPIO for encoder channel A input
      pinMode(encoder[k].chanB, INPUT);
        // configure GPIO for encoder channel B input
      // configure encoder to trigger interrupt with each rising edge, on channel A
      attachInterruptArg(encoder[k].chanA, encoderISR, &encoder[k], RISING);
  }

   // Set up SmartLED
   SmartLEDs.begin(); // initialize smart LEDs object (REQUIRED)
   SmartLEDs.clear();  // clear pixel
   SmartLEDs.setPixelColor(0,SmartLEDs.Color(0,0,0)); // set pixel colors to 'off'
   SmartLEDs.show(); // send the updated pixel colors to the hardware

   pinMode(MOTOR_ENABLE_SWITCH, INPUT_PULLUP); // set up motor enable switch with internal pullup
   pinMode(MODE_BUTTON, INPUT_PULLUP); // Set up mode pushbutton
   modePBDebounce = 0; // reset debounce timer count
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
               mode = mode & 3;                            // keep mode index between 0 and 7
            }
         }
      }

  switch (mode){
    case 0:

    //move forwards 3s
    moveForward(3000);
    stop();
    //turn 45* left
    turnLeft(500);
 stop();
    //drive forwards 2s
    moveForward(2000);
 stop();
    //turn 90* right
    turnRight(1000);
 stop();
    //forwards 2s
    moveForward(2000);
 stop();
    //backwards 8.243s
    moveBackward(8.243);
 stop();
    //turn 45* right
    turnRight(500);
 stop();
    //forwards for 5.828s
    moveForward(5.828);
 stop();
    //turn left 90*
    turnLeft(1000);
     stop();
    break;

    case 1:
  // Move forward for 3 meters
    moveForward(3000); // Assuming each meter takes 1000 milliseconds to cross
    
    // Turn left (1000)
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
    
    break;

    case 2:
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

    case3:
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

void moveForward(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
   setMotor(1, driveSpeed, cIN1Chan[0],cIN2Chan[0]); // forward left motor
    setMotor(1,driveSpeed,cIN1Chan[1],cIN2Chan[1]); // forward right motor
     delay(duration);
}

void moveBackward(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH);
  setMotor(-1, driveSpeed, cIN1Chan[0],cIN2Chan[0]); // reverse left motor
  setMotor(-1, driveSpeed, cIN1Chan[1],cIN2Chan[1]); // reverse right motor
  delay(duration);
}

void turnLeft(int duration) {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, HIGH);
  digitalWrite(RIGHT_MOTOR_A, HIGH);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  setMotor(-1, driveSpeed, cIN1Chan[0],cIN2Chan[0]); // reverse left motor
  setMotor(1, driveSpeed, cIN1Chan[1],cIN2Chan[1]); // reverse right motor
  delay(duration);
}

void turnRight(int duration) {
  digitalWrite(LEFT_MOTOR_A, HIGH);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, HIGH);
  setMotor(1, driveSpeed, cIN1Chan[0],cIN2Chan[0]); // reverse left motor
  setMotor(-1, driveSpeed, cIN1Chan[1],cIN2Chan[1]); // reverse right motor
   delay(duration);
}

void stop(){
  setMotor(0, driveSpeed, cIN1Chan[0],cIN2Chan[0]); // reverse left motor
  setMotor(0, driveSpeed, cIN1Chan[1],cIN2Chan[1]); // reverse right motor
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

// send motor control signals, based on direction and pwm (speed)
void setMotor(int dir, int pwm, int in1, int in2) {
   if (dir == 1) {
       // forward
      ledcWrite(in1, pwm);
      ledcWrite(in2, 0);
   }
   else if (dir == -1) {
       // reverse
      ledcWrite(in1, 0);
      ledcWrite(in2, pwm);
   }
   else {
       // stop
      ledcWrite(in1, 0);
      ledcWrite(in2, 0);
   }
}

void ARDUINO_ISR_ATTR encoderISR(void* arg) {
   Encoder* s = static_cast<Encoder*>(arg);
      // cast pointer to static structure

   int b = digitalRead(s->chanB);
      // read state of channel B
   if (b > 0) {
      // high, leading channel A
      s->pos++;
      // increase position
   }
   else {
      // low, lagging channel A
      s->pos--;
      // decrease position
   }
}
















