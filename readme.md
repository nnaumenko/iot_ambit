#Web-configurable ESP8266-based room air monitor


This is slightly different version of [ESP8266-based temperature monitor](https://github.com/nnaumenko/webconfig-ESP8266-tempmon), adapted to measure room air parameters: temperature, humidity and CO2.

## Highlights

* Simple prototype project implementing user-friendly configuration via web interface.
* Monitor room air temperature, humidity and CO2 concentration
* Uses [Blynk]( http://www.blynk.cc/) for data viewing & archiving
* Hardware platform is based on standalone ESP8266 (ESP-12 module); no Arduino required
* Project uses Arduino IDE for ESP8266 programming, can be adapted for Arduino & WiFi Shield with minimum effort
* A cheap DHT11 temperature/humidity sensor (e.g. for room temperature)
* Optional DS18B20 temperature sensor (e.g. for outside temperature)
* MG811 CO2 concentration sensor for the room CO2 concentration
* MG811 signal can be filtered using rolling average or low-pass filter

## Changes

The differences from [previous project](https://github.com/nnaumenko/webconfig-ESP8266-tempmon):

ADC input of ESP8266 is used to collect the data from MG811 CO2 sensor. Since MG811 requires analog input impedance of 100..1000 gigaohm, it can not be connected directly to ADC input of ESP-12 module. Moreover, the ESP-12 analog input operates in range 0,2 .. 1,2 V, and voltage above 1,8 V will damage it. Thus, an operational amplifier with 10^13 ohm input impedance is used to amplify sensor�s signal (gain = 5). Since opamp is powered by 3,3 V the maximum voltage it can provide is about 3V. Then the voltage divider is used to convert the amplified signal into range 0..1,2 V.

The other minor differences are:
* Removed amber LED (Prog) and green Led (Run). Only red LED (Fault) is used.
* Removed Reset button; power-off/power-on the device instead of pressing Reset.
* Since DS18B20 is optional, no response from DS18B20 does not switch on Fault LED.
* Eeprom config can save/load value of any type
* Webconfig modified to simplify form generation
* MG811 calibration form is added to webconfig

## Setting up Blynk

Note: this device uses [Blynk]( http://www.blynk.cc/) for data viewing & archiving. Alternatively the sensor data and debug information can be reviewed with the serial port monitor.

* Install [Blynk](http://www.blynk.cc/getting-started/) on your Android or iOS device; install Blynk library for Arduino IDE
* Create a new Blynk account or log if you already have one
* [Import the Blynk configuration](http://docs.blynk.cc/#sharing-share-your-project-configuration) using the following link generated by Blynk application: [tinyurl.com/h8appo8](tinyurl.com/h8appo8)
* Copy an [Auth Token](http://docs.blynk.cc/#getting-started-getting-started-with-the-blynk-app-4-auth-token); you will have to enter it during device setup.

## Setting up the device

Note: this device uses [Blynk]( http://www.blynk.cc/) for data viewing & archiving. Alternatively the sensor data and debug information can be reviewed with the serial port monitor.

* Connect the sensors/buttons/resistors/etc. to ESP-12 according to the included wiring diagram
* [Install ESP8266 extension](https://github.com/esp8266/Arduino#installing-with-boards-manager) for Arduino IDE
* Connect ESP-12 to UART<->USB adapter (e.g. CP2102)
* Open the Arduino IDE project, compile and upload to ESP-12
* Hold Config Button and (re)start the device; use serial monitor in Arduino IDE to view wireless network SSID and password, along with device IP address
* Connect to the wireless network created by the device and enter device IP address in the web browser
* Enter SSID, password and [Blynk auth token](http://docs.blynk.cc/#getting-started-getting-started-with-the-blynk-app-4-auth-token) in web interface form and press Save Settings button.
* Enter MG811 calibration data and press Save Settings button.
* Restart the device and wait for connection with Blynk server to establish (if no connection can be established, red LED is lit)
* [Run the Blynk project](http://docs.blynk.cc/#getting-started-getting-started-with-the-blynk-app-6-run-the-project)

## Calibrating MG811 sensor

The calibration method is similar to one described [here](http://www.veetech.org.uk/Prototype_CO2_Monitor.htm).

1. Switch on the device and wait until MG811 readings are stable (this may take up to 48 hours).
2. Expose MG811 to the outside air. Make sure that the air temperature & humidity are as close to the room temperature & humidity as possible.
3. Wait until sensor readings are stable.
4. Observe the uncalibrated MG811 reading from ADC with Blynk or Serial Monitor. Assume that the ADC value corresponds to 400 ppm (low limit of MG811).
5. Expose MG811 to the exhaled air according to the method described [here](http://www.veetech.org.uk/Prototype_CO2_Monitor.htm).
6. Wait until sensor readings are stable.
7. Observe the uncalibrated MG811 reading from ADC with Blynk or Serial Monitor. Assume that the ADC value corresponds to 40000 ppm (the CO2 concentration in the exhaled air).
8. Restart the device in config mode as mentioned above and enter MG811 calibration data.
9. If possible, verify calibrated MG811 readings with reference meter.

## Copyright and License

Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko) All rights reserved

This software may be modified and distributed under the terms of the MIT license. See the LICENSE file for details.