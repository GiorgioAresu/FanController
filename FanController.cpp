#include "Arduino.h"
#include "FanController.h"

#if defined(ARDUINO_ARCH_ESP32)
	#include <analogWrite.h>
#endif

#if defined(ESP8266)
	#define ISR_PREFIX ICACHE_RAM_ATTR
#else
	#define ISR_PREFIX
#endif

FanController::FanController(byte sensorPin, unsigned int sensorThreshold, byte pwmPin, byte pwmMode)
{
	_sensorPin = sensorPin;
	_sensorInterruptPin = digitalPinToInterrupt(sensorPin);
	_sensorThreshold = sensorThreshold;
	_pwmPin = pwmPin;
	_pwmMode = pwmMode;
	pinMode(pwmPin, OUTPUT);
	if (pwmMode == SW_PWM){
		_pwmDutyCycle = 100;
	}
	#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || (__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
		if (pwmMode == HW_PWM){
			_pwmDutyCycle = 100;
			_pwmFrequency = 1000;        // Start with conservative 1kHz frequency
			InitTimersSafe();
			if(SetPinFrequencySafe(_pwmPin, _pwmFrequency)){
				pwmWrite(_pwmPin, _pwmDutyCycle * 2.55f);
			}
		}
	#endif
}

void FanController::begin()
{
	static byte instance;
	_instance = instance;
	_instances[instance] = this;
#if defined(ARDUINO_ARCH_ESP32)
	analogWriteResolution(10);
	analogWriteFrequency(25000);
#endif
	digitalWrite(_sensorPin, HIGH);
	setDutyCycle(_pwmDutyCycle);
	_attachInterrupt();
	instance++;
}

unsigned int FanController::getSpeed() {
	unsigned int elapsed = millis() - _lastMillis;
	if (elapsed > _sensorThreshold)
	{
		detachInterrupt(_sensorInterruptPin);
		double correctionFactor = 1000.0 / elapsed;
		_lastReading = correctionFactor * _halfRevs / 2 * 60;
		_halfRevs = 0;
		_lastMillis = millis();
		_attachInterrupt();
	}
	return _lastReading;
}

void FanController::setDutyCycle(byte dutyCycle) {
	_pwmDutyCycle = min((int)dutyCycle, 100);
	if (_pwmMode == SW_PWM){
		analogWrite(_pwmPin, 2.55f * _pwmDutyCycle);
	}
	else if (_pwmMode == HW_PWM){
		pwmWrite(_pwmPin, 2.55f * _pwmDutyCycle);
	}
}

byte FanController::getDutyCycle() {
	return _pwmDutyCycle;
}

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || (__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
bool FanController::setPwmFrequency(int32_t freq){
	if (_pwmMode == SW_PWM){
		return false;
	}
	bool freqSet = SetPinFrequencySafe(_pwmPin, freq);
	if (freqSet){
		_pwmFrequency = freq;
	}
	return freqSet;
}

int32_t FanController::getPwmFrequency(){
	if (_pwmMode == SW_PWM){
		return 0;
	}
	else {
		return _pwmFrequency;
	}
}
#endif

void FanController::_attachInterrupt()
{
	switch (_instance)
	{
	case 0:
		attachInterrupt(_sensorInterruptPin, _triggerExt0, FALLING);
		break;
	case 1:
		attachInterrupt(_sensorInterruptPin, _triggerExt1, FALLING);
		break;
	case 2:
		attachInterrupt(_sensorInterruptPin, _triggerExt2, FALLING);
		break;
	case 3:
		attachInterrupt(_sensorInterruptPin, _triggerExt3, FALLING);
		break;
	case 4:
		attachInterrupt(_sensorInterruptPin, _triggerExt4, FALLING);
		break;
	case 5:
		attachInterrupt(_sensorInterruptPin, _triggerExt5, FALLING);
		break;
	}
}

FanController * FanController::_instances[6];

void FanController::_trigger()
{
	_halfRevs++;
}

void FanController::_triggerCaller(byte instance)
{
	if (FanController::_instances[instance] != nullptr)
	{
		FanController::_instances[instance]->_trigger();
	}
}

ISR_PREFIX void FanController::_triggerExt0() { FanController::_triggerCaller(0); }
ISR_PREFIX void FanController::_triggerExt1() { FanController::_triggerCaller(1); }
ISR_PREFIX void FanController::_triggerExt2() { FanController::_triggerCaller(2); }
ISR_PREFIX void FanController::_triggerExt3() { FanController::_triggerCaller(3); }
ISR_PREFIX void FanController::_triggerExt4() { FanController::_triggerCaller(4); }
ISR_PREFIX void FanController::_triggerExt5() { FanController::_triggerCaller(5); }