Thermostat
==========

Remote Controlled Thermostat

This unit will replace mechanical thermostat on the wall.
Features:
* Clear LCD display of current temperature and operating status
* Desired temperature adjustment via rotary encoder
* On/Off button (incorporated into rotary encoder)
* RGB Led with current status (red : off, yellow: boiler standby, red: boiler on)
* Ambient light sensor
* Infrared distance sensor, used for activating LCD backlight when user's hand is near.
* Remote control via Xbee
** Reports current status (temperature, humidity, light level and other parameters)
** Accepts commands (toggle state, set temperature)


Arduino sketch
==========

Currently implemented:

* Rotary Encoder
* Rotary Encoder RGB Led
* Rotary Encoder Switch
* 20x4 LCD
* LCD backlight control
* distance sensor (IR Led + photodiode)
* Light sensor
* relay output (uses external 2-channel relay block)


To Do:
==========

* XBee support
* Temperature sensor
* Humidity Sensor (DHT unit)
* NodeJS server
