#include <IBusBM.h>

IBusBM ibus;  // Create iBus object

int readChannelValue(byte channel) 
{
    int value = ibus.readChannel(channel);
    return value;  // Return the raw iBus value
}

bool readSwitch(byte channel, bool badSignal = false)
{
    int value = ibus.readChannel(channel);
    // Check whether there is signal detected or not
    if (value < 1000)
    {
      return badSignal;
    }
    return (value > 1700);  // Using 1500 as a proper threshold for a switch
}

void setup() 
{
  Serial.begin(115200);  // Start serial monitor for debugging
  
  // Attach iBus to hardware serial (must use Serial for Arduino Uno)
  ibus.begin(Serial);
}

void loop() 
{
  // Read and display first 5 channels
  for (byte i = 1; i < 8; i++) 
  {
        int value = readChannelValue(i);
        Serial.print("Ch");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(value);
        Serial.println();
  }

  // Read and display switch state on channel 6
  Serial.print("Ch1: ");
  Serial.println(readSwitch(0));
  Serial.print("Ch2: ");


  delay(1000);  // Small delay to avoid too much serial output
}
