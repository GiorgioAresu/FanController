// Include the library
#include <FanController.h>

// Sensor wire is plugged into port 2 on the Arduino.
// For a list of available pins on your board,
// please refer to: https://www.arduino.cc/en/Reference/AttachInterrupt
#define SENSOR_PIN 2

// Choose a threshold in milliseconds between readings.
// A smaller value will give more updated results,
// while a higher value will give more accurate and smooth readings
#define SENSOR_THRESHOLD 1000

// Initialize library
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD);

/*
   The setup function. We only start the library here
*/
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Fan Controller Library Demo");

  // Start up the library
  fan.begin();
}

/*
   Main function, get and show the temperature
*/
void loop(void)
{
  // Call fan.getSpeed() to get fan RPM.
  Serial.print("Current speed: ");
  unsigned int rpms = fan.getSpeed(); // Send the command to get RPM
  Serial.print(rpms);
  Serial.println("RPM");

  // Not really needed, just avoiding spamming the monitor,
  // readings will be performed no faster than once every THRESHOLD ms anyway
  delay(250);
}
