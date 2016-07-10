#Web-configurable ESP8266-based room air monitor

This project is a ESP8266-based device to monitor room air temperature, humidity and CO2 concentration. It also contains optional OneWire temperature sensor which can be used for measuring outdoors temperature.

The only controls are Config Button and Fault LED. The configuration is performed via web-server hosted by device. During the initial setup user holds Config Button and powers the device on. After that device sets up a wireless access point with known SSID and password (SSID and password can be seen via Serial Monitor), and Fault LED starts flashing. User connects to this access point with PC or mobile device to view, modify and save parameters using web browser.

After setup is complete, the device is powered off-then-on with Config Button released. Device connect to configured wireless network and operates routinely without supervision. The measurements are viewed via Blynk application. If the connection to Blynk server cannot be established or any sensor fails, Fauld LED is lit.

ADC input of ESP8266 is used to collect the data from MG811 CO2 sensor. Since MG811 requires analog input impedance of 100..1000 gigaohm, it can not be connected directly to ADC input of ESP-12 module. Moreover, the ESP-12 analog input operates in range 0,2 .. 1,2 V, and voltage above 1,8 V will damage it. Thus, an operational amplifier with 10^13 ohm input impedance is used to amplify sensor�s signal (gain = 5). Since opamp is powered by 3,3 V the maximum voltage it can provide is about 3V. Then the voltage divider is used to convert the amplified signal into range 0..1,2 V.

## Highlights

* Simple prototype project implementing user-friendly configuration via web interface.
* Monitor room air temperature, humidity and CO2 concentration
* Uses [Blynk]( http://www.blynk.cc/) for data viewing & archiving
* Hardware platform is based on standalone ESP8266 (ESP-12 module); no Arduino required
* Project uses Arduino IDE for ESP8266 programming, can be adapted for Arduino & WiFi or Ethernet Shields with minimum effort
* A cheap DHT11 temperature/humidity sensor (e.g. for room temperature)
* Optional DS18B20 temperature sensor (e.g. for outside temperature)
* MG811 CO2 concentration sensor for the room CO2 concentration
* MG811 signal can be filtered using rolling average or low-pass filter

## Installing and configuring

Note: this project uses [Blynk](http://www.blynk.cc/) for data viewing & archiving. Alternatively the sensor data and debug information can be reviewed with the serial port monitor.

### Installing firmware for ESP8266

* Connect the sensors/buttons/resistors/etc. to ESP-12 according to the included wiring diagram
* Connect ESP-12 to UART<->USB adapter (e.g. CP2102)
* Download [binary file](https://github.com/nnaumenko/room_monitor_esp8266/raw/master/binary/room_monitor_esp8266.bin)
* Download [NodeMCU-flasher](https://github.com/nodemcu/nodemcu-flasher) ([Win32 version](https://github.com/nodemcu/nodemcu-flasher/raw/master/Win32/Release/ESP8266Flasher.exe) / [Win64 version](https://github.com/nodemcu/nodemcu-flasher/raw/master/Win64/Release/ESP8266Flasher.exe)) or use [any other ESP8266 programming tool](http://www.esp8266.com/wiki/doku.php?id=loading_firmware#prerequisites)
* Connect GPIO0 to ground (install Prog jumper JP3 according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf))
* Power-up the device
* Upload binary file into ESP8266 starting with address 0x000000
* Disconnect GPIO0 from ground (remove Prog jumper JP3 according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf))
* Open serial port terminal (such as Putty) and make sure serial and set baud rate to 9600
* Connect GPIO12 to ground (hold Config Button according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf))
* Power-down then power-up the device
* GPIO2 (Fault LED according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf)) is blinking; you may disconnect GPIO12 from ground (or release Hold Button)
* Device information, including access point name, password and IP address is printed on the serial port terminal

### Alternatively: compile project and install firmware with Arduino IDE

* Connect the sensors/buttons/resistors/etc. to ESP-12 according to the included wiring diagram
* Connect ESP-12 to UART<->USB adapter (e.g. CP2102)
* Make sure that [ESP8266 core for Arduino](https://github.com/esp8266/Arduino#installing-with-boards-manager) and all required libraries (see below) are installed
* Connect GPIO0 to ground (install Prog jumper JP3 according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf))
* Download and open the Arduino IDE project and compile it
* Power-up the device and upload firmware with Arduino IDE
* Disconnect GPIO0 from ground (remove Prog jumper JP3 according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf))
* Connect GPIO12 to ground (hold Config Button according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf))
* Open Serial Monitor in Arduino IDE
* Power-down then power-up the device
* GPIO2 (Fault LED according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf)) is blinking; you may disconnect GPIO12 from ground (or release Hold Button)
* Device information, including access point name, password and IP address is printed in Serial Monitor window

### Configuring

* Connect to the wireless network created by the device and enter device IP address in the web browser
* Enter SSID, password and [Blynk auth token](http://docs.blynk.cc/#getting-started-getting-started-with-the-blynk-app-4-auth-token) in web interface, and press Save Settings button
* Enter MG811 calibration data and filtering parameters, and press Save Settings button.
* Optionally: if you prefer sensor data not to be sent to serial port (serial port activity causes LED installed on some ESP modules blink periodicall, which can be annoying), set Data Values Serial Output parameter to Disabled
* Make sure GPIO12 is not connected to ground (release Config Button according to [wiring diagram](https://github.com/nnaumenko/room_monitor_esp8266/blob/master/hardware/esp8266_room_monitor.pdf))
* Restart the device and wait for connection with Blynk server to establish (if no connection can be established, GPIO2 is on / red Fault LED is lit)
* [Run the Blynk project](http://docs.blynk.cc/#getting-started-getting-started-with-the-blynk-app-6-run-the-project) on your Android or iOS device

### Setting up Blynk

* Install [Blynk](http://www.blynk.cc/getting-started/) on your Android or iOS device; install latest Blynk library for Arduino IDE
* Create a new Blynk account or log if you already have one
* [Import the Blynk configuration](http://docs.blynk.cc/#sharing-share-your-project-configuration) using the following link generated by Blynk application: [tinyurl.com/h8appo8](tinyurl.com/h8appo8)
* Copy an [Auth Token](http://docs.blynk.cc/#getting-started-getting-started-with-the-blynk-app-4-auth-token); you will have to enter it during device setup.

### Calibrating MG811 sensor

The calibration method is similar to one described [here](http://www.veetech.org.uk/Prototype_CO2_Monitor.htm).

* Switch on the device and wait until MG811 readings are stable (this may take up to 48 hours).
* Expose MG811 to the outside air. Make sure that the air temperature & humidity are as close to the room temperature & humidity as possible.
* Wait until sensor readings are stable.
* Observe the uncalibrated MG811 reading from ADC with Blynk or Serial Monitor. Assume that the ADC value corresponds to 400 ppm (low limit of MG811).
* Expose MG811 to the exhaled air according to the method described [here](http://www.veetech.org.uk/Prototype_CO2_Monitor.htm).
* Wait until sensor readings are stable.
* Observe the uncalibrated MG811 reading from ADC with Blynk or Serial Monitor. Assume that the ADC value corresponds to 40000 ppm (the CO2 concentration in the exhaled air).
* Restart the device in config mode as mentioned above and enter MG811 calibration data.
* If possible, verify calibrated MG811 readings with reference meter.

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

Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko) All rights reserved

This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.