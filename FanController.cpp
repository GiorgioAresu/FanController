#include "Arduino.h"
#include "FanReader.h"

FanReader::FanReader(byte pin, unsigned int threshold)
{
	_pin = pin;
	_interruptPin = digitalPinToInterrupt(pin);
	_threshold = threshold;
}

void FanReader::begin()
{
	static byte instance;
	_instance = instance;
	_instances[instance] = this;
	digitalWrite(_pin, HIGH);
	_attachInterrupt();
	instance++;
}

unsigned int FanReader::getSpeed() {
	unsigned int elapsed = millis() - _lastMillis;
	if (elapsed > _threshold)
	{
		detachInterrupt(_interruptPin);
		double correctionFactor = 1000.0 / elapsed;
		_lastReading = correctionFactor * _halfRevs / 2 * 60;
		_halfRevs = 0;
		_lastMillis = millis();
		_attachInterrupt();
		return _lastReading;
	}
}

void FanReader::_attachInterrupt()
{
	switch (_instance)
	{
	case 0:
		attachInterrupt(_interruptPin, _triggerExt0, FALLING);
		break;
	case 1:
		attachInterrupt(_interruptPin, _triggerExt1, FALLING);
		break;
	case 2:
		attachInterrupt(_interruptPin, _triggerExt2, FALLING);
		break;
	case 3:
		attachInterrupt(_interruptPin, _triggerExt3, FALLING);
		break;
	case 4:
		attachInterrupt(_interruptPin, _triggerExt4, FALLING);
		break;
	case 5:
		attachInterrupt(_interruptPin, _triggerExt5, FALLING);
		break;
	}
}

FanReader * FanReader::_instances[6];

void FanReader::_trigger()
{
	_halfRevs++;
}

void FanReader::_triggerCaller(byte instance)
{
	if (FanReader::_instances[instance] != nullptr)
	{
		FanReader::_instances[instance]->_trigger();
	}
}

void FanReader::_triggerExt0() { FanReader::_triggerCaller(0); }
void FanReader::_triggerExt1() { FanReader::_triggerCaller(1); }
void FanReader::_triggerExt2() { FanReader::_triggerCaller(2); }
void FanReader::_triggerExt3() { FanReader::_triggerCaller(3); }
void FanReader::_triggerExt4() { FanReader::_triggerCaller(4); }
void FanReader::_triggerExt5() { FanReader::_triggerCaller(5); }