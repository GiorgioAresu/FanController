/*
  FanController.h - Library to control up to six fans.
  Created by Giorgio Aresu, November 13, 2016.
  Released into the public domain.
*/
#ifndef FanController_h
#define FanController_h

#include "Arduino.h"
#include "PWM.h"

class FanController
{
	public:
		FanController(byte sensorPin, unsigned int sensorThreshold, byte pwmPin = 0, byte pwmMode = 0);
		void begin();
		unsigned int getSpeed();
		void setDutyCycle(byte dutyCycle);
		byte getDutyCycle();

	#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || (__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
		bool setPwmFrequency(int32_t freq);
		int32_t getPwmFrequency();
	#endif

	enum pwmMode {           // PWM operation mode based on PWM.h created by https://code.google.com/archive/p/arduino-pwm-frequency-library/downloads, Arduino forums post: https://forum.arduino.cc/t/pwm-frequency-library/114988
    	SW_PWM,              // Will use default Arduino pwm function, specifically analogWrite
    	HW_PWM,              // Will use hardware based Timer driven fast PWM mode with additional option of setting the frequency 
	};
	
	private:
		static FanController *_instances[6];
		byte _sensorPin;
		byte _sensorInterruptPin;
		unsigned int _sensorThreshold;
		byte _pwmPin;
		byte _pwmMode;
		byte _pwmDutyCycle;
		#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || (__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
			int32_t _pwmFrequency;
		#endif
		byte _instance;
		unsigned int _lastReading;
		volatile unsigned int _halfRevs;
		unsigned long _lastMillis;
		void _trigger();
		void _attachInterrupt();
		static void _triggerCaller(byte instance);
		static void _triggerExt0();
		static void _triggerExt1();
		static void _triggerExt2();
		static void _triggerExt3();
		static void _triggerExt4();
		static void _triggerExt5();
};

#endif