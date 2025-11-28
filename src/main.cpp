#include <PS2X_lib.h> //for v1.6
#include <Servo.h>
#include <DRV8825.h>

DRV8825 stepper;
PS2X ps2x; // create PS2 Controller Class
Servo servo_1; // create Servo object
Servo servo_2; // create Servo object

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
#define data A2
#define command A3
#define attention A4
#define clock A5

// Left Front Motor (L298N #1 - Motor A)
#define LEFT_FRONT_ENA   0   // PWM
#define LEFT_FRONT_IN1   12
#define LEFT_FRONT_IN2   13

// Left Rear Motor (L298N #1 - Motor B)
#define LEFT_REAR_ENB    0   // PWM
#define LEFT_REAR_IN3    10
#define LEFT_REAR_IN4    11

// Right Front Motor (L298N #2 - Motor A)
#define RIGHT_FRONT_ENA  0   // PWM
#define RIGHT_FRONT_IN1  6
#define RIGHT_FRONT_IN2  7

// Right Rear Motor (L298N #2 - Motor B)
#define RIGHT_REAR_ENB   0  // PWM
#define RIGHT_REAR_IN3   8
#define RIGHT_REAR_IN4   9

#define DEADZONE 15
#define Servo1 A0
#define Servo2 A1

#define stepDirection 0
#define stepPin 0

//  setDirection

int moveSpeed = 0;
int turnSpeed = 0;
int vibrate = 0;
int error = 0;
int type = 0;
bool servo1Pressed = false;
bool servo2Pressed = false;




void driveMotor(int in1Pin, int in2Pin, int pwmPin, int speed) {
  if (speed > 0) {
    // Forward
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    analogWrite(pwmPin, speed);
  } else if (speed < 0) {
    // Reverse
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    analogWrite(pwmPin, -speed);
  } else {
    // Stop (coast)
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    analogWrite(pwmPin, 0);
  }
}

void stopAllMotors() {
  // Left Front
  digitalWrite(LEFT_FRONT_IN1, LOW);
  digitalWrite(LEFT_FRONT_IN2, LOW);
  analogWrite(LEFT_FRONT_ENA, 0);
  
  // Left Rear
  digitalWrite(LEFT_REAR_IN3, LOW);
  digitalWrite(LEFT_REAR_IN4, LOW);
  analogWrite(LEFT_REAR_ENB, 0);
  
  // Right Front
  digitalWrite(RIGHT_FRONT_IN1, LOW);
  digitalWrite(RIGHT_FRONT_IN2, LOW);
  analogWrite(RIGHT_FRONT_ENA, 0);
  
  // Right Rear
  digitalWrite(RIGHT_REAR_IN3, LOW);
  digitalWrite(RIGHT_REAR_IN4, LOW);
  analogWrite(RIGHT_REAR_ENB, 0);
}

void setup() {
    Serial.begin(57600);
      // Left Front Motor pins
  pinMode(LEFT_FRONT_ENA, OUTPUT);
  pinMode(LEFT_FRONT_IN1, OUTPUT);
  pinMode(LEFT_FRONT_IN2, OUTPUT);
  
  // Left Rear Motor pins
  pinMode(LEFT_REAR_ENB, OUTPUT);
  pinMode(LEFT_REAR_IN3, OUTPUT);
  pinMode(LEFT_REAR_IN4, OUTPUT);
  
  // Right Front Motor pins
  pinMode(RIGHT_FRONT_ENA, OUTPUT);
  pinMode(RIGHT_FRONT_IN1, OUTPUT);
  pinMode(RIGHT_FRONT_IN2, OUTPUT);
  
  // Right Rear Motor pins
  pinMode(RIGHT_REAR_ENB, OUTPUT);
  pinMode(RIGHT_REAR_IN3, OUTPUT);
  pinMode(RIGHT_REAR_IN4, OUTPUT);

  // Initialize all motors to stopped
  stopAllMotors();

  stepper.begin(stepDirection, stepPin);

  servo_2.attach(Servo2); // Attach servo to pin A2
  servo_1.attach(Servo1);

  servo_1.write(90);
  servo_2.write(0);

    //CHANGES for v1.6 HERE!!! *************PAY ATTENTION************

        delay(2000);
        error = ps2x.config_gamepad(clock, command, attention, data, true, true); //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
        delay(2000);
        if(error == 0) {
            Serial.println("Found Controller, configured successful");
            Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
            Serial.println("holding L1 or R1 will print out the analog stick values.");
            Serial.println("Go to www.billporter.info for updates and to report bugs.");
        }

        else if(error == 1)
            Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

        else if(error == 2)
            Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

        else if(error == 3)
            Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

        Serial.print(ps2x.Analog(1), HEX);

        type = ps2x.readType();
        switch(type) {
        case 0:
        Serial.println("Unknown Controller type");
        break;
        case 1:
        Serial.println("DualShock Controller Found");
        break;
        case 2:
        Serial.println("GuitarHero Controller Found");
        break;
        }
    
}

void loop(){
/* You must Read Gamepad to get new values
Read GamePad and set vibration values
ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
if you don't enable the rumble, use ps2x.read_gamepad(); with no values

you should call this at least once a second
*/




if(error == 1) //skip loop if no controller found
return;

if(type == 2){ //Guitar Hero Controller

ps2x.read_gamepad(0, false); //read controller

if(ps2x.ButtonPressed(GREEN_FRET))
Serial.println("Green Fret Pressed");
if(ps2x.ButtonPressed(RED_FRET))
Serial.println("Red Fret Pressed");
if(ps2x.ButtonPressed(YELLOW_FRET))
Serial.println("Yellow Fret Pressed");
if(ps2x.ButtonPressed(BLUE_FRET))
Serial.println("Blue Fret Pressed");
if(ps2x.ButtonPressed(ORANGE_FRET))
Serial.println("Orange Fret Pressed");


if(ps2x.ButtonPressed(STAR_POWER))
Serial.println("Star Power Command");

if(ps2x.Button(UP_STRUM)) //will be TRUE as long as button is pressed
Serial.println("Up Strum");
if(ps2x.Button(DOWN_STRUM))
Serial.println("DOWN Strum");


if(ps2x.Button(PSB_START)) //will be TRUE as long as button is pressed
Serial.println("Start is being held");
if(ps2x.Button(PSB_SELECT))
Serial.println("Select is being held");


if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
{
Serial.print("Wammy Bar Position:");
Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
}
}

else { //DualShock Controller

ps2x.read_gamepad(0, false); //read controller and set large motor to spin at 'vibrate' speed

// LEFT STICK Y = Forward/Backward
// RIGHT STICK X = Turn Left/Right
int stickY = ps2x.Analog(PSS_LY);  // Left stick Y
int stickX = ps2x.Analog(PSS_RX);  // Right stick X

// Convert 0-255 to -255 to +255 (center = 0)
int moveSpeed = map(stickY, 0, 255, 255, -255);  // Up = forward
int turnSpeed = map(stickX, 0, 255, -255, 255);  // Right = positive

// Apply deadzone
if (abs(moveSpeed) < DEADZONE) moveSpeed = 0;
if (abs(turnSpeed) < DEADZONE) turnSpeed = 0;

// Mix movement and turning
int leftSpeed  = constrain(moveSpeed + turnSpeed, -255, 255);
int rightSpeed = constrain(moveSpeed - turnSpeed, -255, 255);

// Drive LEFT side motors
driveMotor(LEFT_FRONT_IN1, LEFT_FRONT_IN2, LEFT_FRONT_ENA, leftSpeed);
driveMotor(LEFT_REAR_IN3, LEFT_REAR_IN4, LEFT_REAR_ENB, leftSpeed);

// Drive RIGHT side motors
driveMotor(RIGHT_FRONT_IN1, RIGHT_FRONT_IN2, RIGHT_FRONT_ENA, rightSpeed);
driveMotor(RIGHT_REAR_IN3, RIGHT_REAR_IN4, RIGHT_REAR_ENB, rightSpeed);


if(ps2x.Button(PSB_START)) //will be TRUE as long as button is pressed
Serial.println("Start is being held");
if(ps2x.Button(PSB_SELECT))
Serial.println("Select is being held");


if(ps2x.Button(PSB_PAD_UP)) { //will be TRUE as long as button is pressed
Serial.print("Up held this hard: ");
Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
stepper.setDirection(DRV8825_CLOCK_WISE);
stepper.step();
}
if(ps2x.Button(PSB_PAD_RIGHT)){
Serial.print("Right held this hard: ");
Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
}
if(ps2x.Button(PSB_PAD_LEFT)){
Serial.print("LEFT held this hard: ");
Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
}
if(ps2x.Button(PSB_PAD_DOWN)){
Serial.print("DOWN held this hard: ");
Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
stepper.setDirection(DRV8825_COUNTERCLOCK_WISE);
stepper.step();
}


vibrate = ps2x.Analog(PSAB_BLUE); //this will set the large motor vibrate speed based on
//how hard you press the blue (X) button

if (ps2x.NewButtonState()) //will be TRUE if any button changes state (on to off, or off to on)
{



if(ps2x.Button(PSB_L3))
Serial.println("L3 pressed");
if(ps2x.Button(PSB_R3))
Serial.println("R3 pressed");
if(ps2x.Button(PSB_L2))
Serial.println("L2 pressed");
if(ps2x.Button(PSB_R2))
Serial.println("R2 pressed");
if(ps2x.Button(PSB_GREEN))
{
    Serial.println("Triangle pressed");
}
}


}


if(ps2x.ButtonPressed(PSB_RED)) //will be TRUE if button was JUST pressed
{
    Serial.println("Circle just pressed");
    if(servo2Pressed){
        servo_2.write(90);
    }
    else{
        servo_2.write(0);
    }
    servo2Pressed = !servo2Pressed;
    delay(50);
}

if(ps2x.ButtonPressed(PSB_PINK)) //will be TRUE if button was JUST released
{
    Serial.println("Square just released");
    if(servo1Pressed){
        servo_1.write(90);
    }
    else{
        servo_1.write(120);
    }
    servo1Pressed = !servo1Pressed;
    delay(50);
}

if(ps2x.ButtonPressed(PSB_BLUE)) {
    Serial.println("X just changed");
}
if(ps2x.ButtonReleased(PSB_BLUE)) {
}
//will be TRUE if button was JUST pressed OR released


if(ps2x.Button(PSB_R2))
{
    Serial.println(moveSpeed);
}
if (ps2x.ButtonReleased(PSB_R2))
    {
        
    }

if(ps2x.Button(PSB_L2))
{
    Serial.println(turnSpeed);
}
    if (ps2x.ButtonReleased(PSB_L2))
    {
    }



if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
{
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC);
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC);
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);
}
    delay(50);
}



