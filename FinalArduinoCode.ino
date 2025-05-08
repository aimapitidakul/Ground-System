// This is the code used to control the movement of the gound system.

// Include the iBus Library
#include <IBusBM.h>

// Create iBus object
IBusBM ibus;

// Define pins
  // Right
const int ENA = 3;   
const int IN1 = 4;   
const int IN2 = 5;   
  // Left
const int ENB = 9;  
const int IN3 = 7;   
const int IN4 = 8;   

// Receiver channel mappings
const int CH2 = 1; // Forward/Backward
const int CH4 = 3; // Left/Right
int ch5 = 0; // Mode Control

// Define mode selection from channel 5
enum Mode {Deactivated, Ground, Flight};

// Function to determine the operating mode based on channel 5 input
Mode modeChoose()
{
    ch5 = ibus.readChannel(4); // Read channel 5 (index 4 in iBus)

    if (ch5 < 900)  // No signal detected
    {
        return Deactivated; 
    }
    else if (ch5 < 1400)  // Ground mode active
    {
      return Ground;
    }
    
    return Flight;  // Otherwise, Flight mode active
}

// Function to read iBus channel and map it to -100 to 100
int mappedPower100 (int channel) 
{
    int rawValue = ibus.readChannel(channel);
    // Convert from 1000-2000 to -100 to 100
    return constrain(map(rawValue, 1000, 2000, -100, 100), -100, 100); 
}

// Function to set motor direction
void motorDirection(int speed, int pin1, int pin2) 
{
    if (speed > 0) //Forward
    {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } 
    else if (speed < 0) //Backward
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    } 
    else //Stop
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW); 
    }
}

// Function to set motor speeds
void motorSpeed(int leftSpeed, int rightSpeed) 
{
    // Constrain speeds to prevent exceeding 100% power 
    // Allow finer control over turning
    leftSpeed = constrain(leftSpeed, -100, 100);
    rightSpeed = constrain(rightSpeed, -100, 100);

    // Convert percentage speed to PWM value (0-255)
    int leftPWM = map(abs(leftSpeed), 0, 100, 40, 255);
    int rightPWM = map(abs(rightSpeed), 0, 100, 40, 255);

    // Ensure motor stops at speed = 0
    if (leftSpeed == 0) leftPWM = 0;
    if (rightSpeed == 0) rightPWM = 0;

    // Set direction
    motorDirection(rightSpeed, IN1, IN2);
    motorDirection(leftSpeed, IN3, IN4);

    // Apply PWM speed
    analogWrite(ENA, leftPWM);
    analogWrite(ENB, rightPWM);
}

// Function to stop both motors
void stopMotors()
{
    motorSpeed(0, 0);
}

void setup() 
{
    Serial.begin(115200); // For debugging
    ibus.begin(Serial); 
    // Motor pins = OUTPUT
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}
void loop()
{
    // Determine current mode
    Mode currentMode = modeChoose();

    if (currentMode == Deactivated)
    {
        Serial.println("No Signal Detected");
        stopMotors();
        return;  // Exit loop early
    }
    if (currentMode == Ground)
    {
        Serial.println("Mode: Ground");
        // Read throttle and steering values from iBus
        int throttle = mappedPower100(CH2);
        int turning = mappedPower100(CH4);

        // Prevent unintended movement when ch4 stick is centered
        if (abs(turning) < 10) turning = 0;

        // Adjust turning influence
        float turnFactor = 0.5; 
        // Reduce extreme turns (0.5 means turning only affects 50% of motor speed)

        int leftSpeed = throttle + (turning * turnFactor);
        int rightSpeed = throttle - (turning * turnFactor);

        // Debugging output
        Serial.print("Turning: ");
        Serial.print(turning);
        Serial.print(", Throttle: ");
        Serial.println(throttle);

        // Set motor speeds
        motorSpeed(leftSpeed, rightSpeed);    
    }
    else if (currentMode == Flight)
    {
        Serial.println("Mode: Flight - No ground movement");
        stopMotors();
    }

    delay(500);  // Small delay to stabilize control
} 






