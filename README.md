# FanController
Arduino library to control 3 and 4 pins fans

At this moment it supports up to 6 fans, depending on board ([https://www.arduino.cc/en/Reference/AttachInterrupt](https://www.arduino.cc/en/Reference/AttachInterrupt))

#### serial
If you're getting an error like
```
Traceback (most recent call last):

  File "C:\Program Files (x86)\Arduino\hardware\espressif\esp32/tools/esptool.py", line 25, in <module>

    import serial

ImportError: No module named serial

exit status 1
Error compiling for board ESP32 Dev Module.
```
then see [arduino-esp32](https://github.com/espressif/arduino-esp32/issues/13). TL;DR:

```$ pip install pyserial```