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

// PWM pin (4th on 4 pin fans)
// It's possible to control 3 wire fans via. external mosfet, typically N-channel FET
#define PWM_PIN 9

// Frequnecy of PWM pin is avaliable for AVR platform, HW_PWM mode is set by additional constructor parameter
// pwmMode: 0 - Using regular Arduino analogWrite, 1 - Using timer based fastPWM mode
#define PWM_MODE 1

// Initialize library
FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN, PWM_MODE);

/*
   The setup function. We only start the library here
*/
void setup(void){
  Serial.begin(9600);  // start serial port
  uint32_t startMillis = millis();
  while (!Serial && ((millis() - startMillis) < 3000) ) {
  }
  if (!Serial) {
    // Serial port failed to initialize, take appropriate action
  } else {
    Serial.println("Fan Controller Library Demo");
  }
  // Start up the library
  fan.begin();
}


/*
  Main function, get and show fan speed, change PWM frequnecy to 25 kHz
  Most 4pin / 4wire fans require PWM pulse at 25kHz freq. per their datasheets.
  For 3pin / 3wire fans if connected via. external mosfet for speed control will benefit from PWM freq. above audiable range (Audibale range is 20Hz-20kHz) so 25 kHz will work great for that. 
*/
void loop(void)
{
  // Call fan.getSpeed() to get fan RPM.
  Serial.print("Current speed: ");
  unsigned int rpms = fan.getSpeed();      // Send the command to get RPM
  Serial.print(rpms);
  Serial.println("RPM");

  if (fan.setPwmFrequency(25000)){         // Set PWM freq. to 25kHz
    Serial.println("PWM Frequency Set");
  }

  // Get new speed from Serial (0-100%)
  if (Serial.available() > 0) {
    // Parse speed
    int input = Serial.parseInt();

    // Constrain a 0-100 range
    byte target = max(min(input, 100), 0);

    // Print obtained value
    Serial.print("Setting duty cycle: ");
    Serial.println(target, DEC);

    // Set fan duty cycle
    fan.setDutyCycle(target);

    // Get duty cycle
    byte dutyCycle = fan.getDutyCycle();
    Serial.print("Duty cycle: ");
    Serial.println(dutyCycle, DEC);

    // Get PWM Mode
    if (!_pwmMode){
      Serial.println("Standard Arduino PWM Mode");
    }
    else if (_pwmMode == 1){
      Serial.println("Timer Based PWM Mode");
    }

    // Get PWM freq.
    int32_t pwmFreq = fan.getPwmFrequency();
    Serial.print("PWM Freq. Set [Hz]: ");
    Serial.println(pwmFreq);
  }

  // Not really needed, just avoiding spamming the monitor,
  // readings will be performed no faster than once every THRESHOLD ms anyway
  delay(250);
}
