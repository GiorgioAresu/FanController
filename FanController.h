/*
  FanController.h - Library to control up to six fans.
  Created by Giorgio Aresu, November 13, 2016.
  Released into the public domain.
*/
#ifndef FanController_h
#define FanController_h

#include "Arduino.h"

class FanController
{
	public:
		FanController(byte sensorPin, unsigned int sensorThreshold, byte pwmPin = 0);
		void begin();
		unsigned int getSpeed();
		void setDutyCycle(byte dutyCycle);
		byte getDutyCycle();
	private:
		static FanController *_instances[6];
		byte _sensorPin;
		byte _sensorInterruptPin;
		byte _sensorThreshold;
		byte _pwmPin;
		byte _pwmDutyCycle;
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