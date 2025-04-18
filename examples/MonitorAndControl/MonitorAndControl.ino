// Include the library
#include <FanController.h>

// Sensor (tachometer) pin (3rd on 4 pin fans)
// For a list of available pins on your board,
// please refer to: https://www.arduino.cc/en/Reference/AttachInterrupt
#define SENSOR_PIN 2

// Choose a threshold in milliseconds between readings.
// A smaller value will give more updated results,
// while a higher value will give more accurate and smooth readings
#define SENSOR_THRESHOLD 1000

// PWM pin (4th on 4 pin fans)
#define PWM_PIN 9

// Initialize library
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN);

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
  unsigned int rpms = fan.getSpeed(); // Send the command to get RPM
  byte dutyCycle = fan.getDutyCycle();
  Serial.printf("Current speed: %5d RPM | Duty cycle: %3d%%\n", rpms, dutyCycle);

  // Get new speed from Serial (0-100%)
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    int input = line.toInt();

    // Constrain a 0-100 range
    byte target = max(min(input, 100), 0);

    // Print obtained value
    Serial.printf("Setting duty cycle: % 3d%%\n", target);

    // Set fan duty cycle
    fan.setDutyCycle(target);
  }

  // Not really needed, just avoiding spamming the monitor,
  // readings will be performed no faster than once every THRESHOLD ms anyway
  delay(250);
}
