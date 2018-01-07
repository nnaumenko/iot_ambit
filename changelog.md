# Changelog

## 0.18.0 - work in progress

### Added

* Changelog

### Changed

* Switched to Major.Minor.Patch version format

## 0.17 - 30 Oct 2017

### Fixed

* Fixed bug which caused incorrect ADC calibration factor and offset calculation

## 0.16 - 18 Oct 2017

### Fixed

* Fixed Krack WPA2 vulnerability

## 0.15 - 15 Jun 2017

### Added

* Integrity of parameters saved in EEPROM is controlled by checksum

## 0.14 - 12 May 2017

### Added

* Diagnostic Log console allows much more convenient viewing of diagnostic messages via webserver (path /diag); previously diagnosic messages could only be viewed via wired serial connection

* Diagnostic Log last messages can be exported in JSON format via webserver

## 0.13 - 22 Apr 2017

### Added

* Diagnostic Log last messages are now accessible via webserver (path /diag)

## 0.12 - 4 Mar 2017

### Added

* List of Modules included in firmware is viewable via webserver (root path)

### Fixed

* Webconfig is now not accessible via webserver when not in Config Mode

## 0.11 - 20 Nov 2016

### Changed

* Standardised Diagnostic Log message format (now includes timestamp, sequential number and message severity)

## 0.10 - 12 Nov 2016

### Changed

* Improved handling of malformed HTTP requests

### Fixed

* Fixed incorrect HTTP redirect in Webconfig

## 0.9 - 5 Nov 2016

### Fixed

* Fixed minor bugs caused by changed software architecture

## 0.8 - 9 Sep 2016

### Changed

* New software architecture; internal changes only

## 0.7 - 7 Aug 2016

### Changed

* Minor webserver stability improvements

## 0.6 - 30 Jul 2016

### Added

* Startup Delay parameter to prevent multiple devices from
simultaneously connecting after power outage and exceeding 
WiFi access point capabilities

### Changed

* In Webconfig "Enable/Disable" Dropdown Lists replaced with Checkboxes
* Save Settings button in Webconfig now saves all settings rather then settings in current tab
* Paramters are now saved via POST-redirect-GET sequence

### Removed

* Connection limit in Config Mode removed (maximum 4 devices connected to access point, ESP8266 limitation)

## 0.5 - 24 Jul 2016

### Added

* Compile-time option to create either open or password-protected WiFi in config mode

### Changed

* Limit of one connected device to access point in Config Mode introduced

## 0.4 - 16 Jul 2016

### Added

* Parameters for private Blynk server

### Changes

* Keeping track which firmware version was used to save parameters

## 0.3 - 9 Jul 2016

### Changed

* Webconfig parameters are now sent as POST (previously GET) request
* More stable HTTP request parser

### Fixed

* Possible memory fragmentation issues (due to use of String) fixed

## 0.2 - 6 May 2016

### Added

* Tooltips for parameters in Webconfig

### Changed

* Webconfig now uses tab-oriented UI
* Radio Buttons replaced with Dropdown Lists in Webconfig
* Webconfig parameters are beter organised into sections and subsections

### Fixed

* Webserver occasional connection reset fixed
* Webconfig HTML page slowdown due to excess packet fragmentation fixed

## 0.1 - 29 Mar 2016

### Added

* Rolling average and lowpass filters for ADC/MG811
* Option to discard MG811 calibration values

## 0.0 -  5 Feb 2016

Forked project from my other small project [Web-configurable ESP8266-based temperature monitor](https://github.com/nnaumenko/webconfig-ESP8266-tempmon).
