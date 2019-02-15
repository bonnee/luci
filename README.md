# luci.ino
Automatic light control for arduino

This software was created in 2014 to control some external and internal lights using a photoresistor and some switches to change modes (Summer/Winter, Auto/Manual, etc...). It worked fine for the last few years, but the photoresistor wasn't really accurate.

I converted the software in 2018 to use the tsl2561 digital light sensor. The digitalization of the sensing system did complicate the communication between the controller and the sensor since the latter is ~20m apart from the Arduino and a I2C connection is unsuitable over these distances. A RS485 bus is set in place to solve this issue.
