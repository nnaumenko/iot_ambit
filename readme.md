#Web-configurable ESP8266-based room air monitor

This project is a ESP8266-based device to monitor room air temperature, humidity and CO2 concentration. It also contains optional OneWire temperature sensor which can be used for measuring outdoors temperature.

The only controls are Config Button and Fault LED. The configuration is performed via web-server hosted by device. During the initial setup user holds Config Button and powers the device on. After that device sets up a wireless access point with known SSID and password (SSID and password can be seen via Serial Monitor), and Fault LED starts flashing. User connects to this access point with PC or mobile device to view, modify and save parameters using web browser.

After setup is complete, the device is powered off-then-on with Config Button released. Device connect to configured wireless network and operates routinely without supervision. The measurements are viewed via Blynk application. If the connection to Blynk server cannot be established or any sensor fails, Fauld LED is lit.

## Highlights

* Simple IoT device implementing user-friendly configuration via web interface.
* Monitor room air temperature, humidity and CO2 concentration
* Uses [Blynk]( http://www.blynk.cc/) for data viewing & archiving
* Hardware platform is based on standalone ESP8266 (ESP-12 module); no Arduino required
* Project uses Arduino IDE for ESP8266 programming, can be adapted for Arduino & WiFi or Ethernet Shields with minimum effort
* AM2302 temperature/humidity sensor (e.g. for room temperature)
* Optional DS18B20 temperature sensor (e.g. for outside temperature)
* MG811 CO2 concentration sensor for the room CO2 concentration with calibration capability
* MG811 signal can be filtered using rolling average or low-pass filter

## Quick links

[Download ESP8266 binary file](https://github.com/nnaumenko/iot_ambit/raw/master/binary/iot_ambit.bin).

[Installing and configuring](https://github.com/nnaumenko/iot_ambit/wiki/Installing-and-configuring)

## Hardware

Please refer [attached schematic](https://github.com/nnaumenko/iot_ambit/blob/master/hardware/esp8266_room_monitor.pdf) for recommended hardware setup.

ADC input of ESP8266 is used to collect the data from MG811 CO2 sensor. Since MG811 requires analog input impedance of 100..1000 gigaohm, it can not be connected directly to ADC input of ESP-12 module. Moreover, the ESP-12 analog input operates in range 0,2 .. 1,2 V, and voltage above 1,8 V will damage it. Thus, an operational amplifier with 10^13 ohm input impedance is used to amplify sensor’s signal (gain = 5). Since opamp is powered by 3,3 V the maximum voltage it can provide is about 3V. Then the voltage divider is used to convert the amplified signal into range 0..1,2 V.

## References

###ESP8266 core for Arduino

This project is compiled with Arduino IDE using [ESP8266 core for Arduino](https://github.com/esp8266/Arduino/).

###Blynk

This project uses [Blynk]( http://www.blynk.cc/) for data viewing & archiving.

The following [Blynk configuration](tinyurl.com/h8appo8) can be used with this project ([installation instructions](http://docs.blynk.cc/#sharing-share-your-project-configuration)).

###Arduino Libraries

This project uses the following Arduino libraries:

* [OneWire library for Dallas/Maxim 1-Wire Chips](https://github.com/PaulStoffregen/OneWire)
* [Arduino plug and go library for the Maxim (previously Dallas) DS18B20 (and similar) temperature ICs] (https://github.com/milesburton/Arduino-Temperature-Control-Library)
* [Arduino library for DHT11DHT22, etc Temp & Humidity Sensors] (https://github.com/adafruit/DHT-sensor-library)
* [Blynk library for embedded hardware](https://github.com/blynkkk/blynk-library/releases/)

###Dynamic Tabs

Webconfig page uses modified version of [Dynamic Tabs](https://akrabat.com/dynamic-javascript-tabs/) by Rob Allen. The license for Dynamic Tabs is as follows:

>Dynamic Tabs 1.0.3

>Copyright (c) 2005 Rob Allen (rob at akrabat dot com)

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Copyright and License

Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko) All rights reserved

This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.