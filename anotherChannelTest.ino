// Include iBus Library
#include <IBusBM.h>

// Create iBus Object
IBusBM ibus;

// Define pins
#define motorR1 4
#define motorR2 5
#define motorL3 7
#define motorL4 8
#define speedR 3
#define speedL 9

// Channel Values
int ch1 = 0; // Left - Right
int ch2 = 0; // Forward - Reverse
int ch3 = 0; // Speed
bool ch5 = 0; // Mode Control

// Motor starts at 0 speed
int motorSpeedR = 0;
int motorSpeedL = 0;

// Motor direction: 1 = forward; 0 = backward
int directionR = 1;
int directionL = 1;

// Read value of the flick switch
bool readSwitch(byte channel)
{
    int value = ibus.readChannel(channel);
    // Check whether there is signal detected or not
    if (value < 1000)
    {
      return false;
      // meaning stay in ground mode
    }
    // value greater than 1700 for flight mode 
    return (value > 1700);  
}

// Right motor control function
void motorControlR(int speed, int direction)
{
  // Determine direction
  if (direction == 1)
  {
    // Forward
    digitalWrite(motorR1, LOW);
    digitalWrite(motorR2, HIGH);
  }
  else
  {
    // Backward
    digitalWrite(motorR1, HIGH);
    digitalWrite(motorR2, LOW);
  }

  // Determine speed
  analogWrite(speedR, speed);
}

// Left motor control function
void motorControlL(int speed, int direction)
{
  // Determine direction
  if (direction == 1)
  {
    // Forward
    digitalWrite(motorL3, LOW);
    digitalWrite(motorL4, HIGH);
  }
  else
  {
    // Backward
    digitalWrite(motorL3, HIGH);
    digitalWrite(motorL4, LOW);
  }

  // Determine speed
  analogWrite(speedL, speed);
}

void setup()
{
  // Serial monitor for debugging
  Serial.begin(115200);
  ibus.begin(Serial);

  // Set all motor pins to outputs
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL3, OUTPUT);
  pinMode(motorL4, OUTPUT);
  pinMode(speedR, OUTPUT);
  pinMode(speedL, OUTPUT);

  analogWrite(speedR, 0);
  analogWrite(speedL, 0);

  delay(500);
}

void loop()
{
  ch1 = ibus.readChannel(0);
  ch2 = ibus.readChannel(1)
  ch3 = ibus.readChannel(2)
  ch5 = ibus.readChannel(4)
}