/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "web_content.h"

const char PROGMEM httpStatusCode200[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
const char PROGMEM httpStatusCode404[] = "HTTP/1.1 404 Not Found\r\n";
const char PROGMEM httpStatusCode405[] = "HTTP/1.1 405 Method Not Allowed\r\n";

const char PROGMEM webServerHeadBegin[] =
  "<!DOCTYPE html>\r\n"
  "<html>\r\n"
  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n"
  "<head>\r\n"
  "<title>Module configuration</title>\r\n"
  "<style type=\"text/css\" media=\"all\">\r\n"
  "body {font-family:Arial;background-color:#D0FFD0;color:#000000;font-size:85%;}\r\n"
  "h1{text-align:center;font-size:120%;font-weight:bold;}\r\n"
  "h2{text-align:center;font-size:110%;font-weight:bold;}\r\n"
  "div{text-align:center;margin:auto;width:20em;padding:1em;background-color:#66FF66;border-style:solid;border-color:black;border-width:2px;}\r\n"
  "table{margin:auto;}\r\n"
  "td{text-align:left;}\r\n"
  "</style>\r\n";

const char PROGMEM webServerHeadBody[] =
  "</head>\r\n"
  "<body>\r\n"
  "<h1>Module configuration</h1>\r\n";

const char PROGMEM webServerBodyEnd[] =
  "</body>\r\n"
  "</html>\r\n";

const char PROGMEM webServerFormBegin1[] =
  "<div class=\"cfg_section\">\r\n"
  "<form method=\"GET\" action=\"/\">\r\n"
  "<h2>";

const char PROGMEM webServerFormBegin2[] =
  "</h2><br>\r\n"
  "<table>\r\n";

const char PROGMEM webServerFormInputBegin[] =
  "<tr><td>";

const char PROGMEM webServerFormInputInput[] =
  "</td><td><input type=\"text\" name=\"";

const char PROGMEM webServerFormInputRadioHeaderEnd[] = 
  "</td><td></td></tr>\n";

const char PROGMEM webServerFormInputRadio[] =
  "<tr><td><input type=\"radio\" name=\"";

const char PROGMEM webServerFormInputValue[] =
  "\" value=\"";

const char PROGMEM webServerFormInputEnd[] =
  "\"></td></tr>\r\n";

const char PROGMEM webServerFormInputRadioEnd[] =
  "\"></td><td>";

const char PROGMEM webServerFormInputRadioEndChecked[] =
  "\" checked></td><td>";


const char PROGMEM webServerFormInputRadioFinish[] =
  "</td></tr>\r\n";

const char PROGMEM webServerFormEnd[] =
  "</table><br>\r\n"
  "<input type=\"submit\" value=\"Save settings\">\r\n"
  "</form></div>\r\n<br>\r\n";

const char PROGMEM webServerFormParameterDisplayNameCommonOff[] = "Off";
const char PROGMEM webServerFormParameterDisplayNameCommonOn[] = "On";

const char PROGMEM webServerWifiFormCaption[] = "Connection settings";
const char PROGMEM webServerMG811FormCaption[] = "MG811 calibration & filtering";
const char PROGMEM webServerMiscFormCaption[] = "Miscellaneous";

const char PROGMEM webServerWifiFormParameterSsid[] = "ssid";
const char PROGMEM webServerWifiFormParameterPassword[] = "password";
const char PROGMEM webServerWifiFormParameterAuth[] = "auth";

const char PROGMEM webServerWifiFormParameterDisplayNameSsid[] = "SSID";
const char PROGMEM webServerWifiFormParameterDisplayNamePassword[] = "Password";
const char PROGMEM webServerWifiFormParameterDisplayNameAuth[] = "Auth Token";

const char PROGMEM webServerMG811FormParameterCalPoint0Ppm[] = "p0ppm";
const char PROGMEM webServerMG811FormParameterCalPoint0Adc[] = "p0adc";
const char PROGMEM webServerMG811FormParameterCalPoint1Ppm[] = "p1ppm";
const char PROGMEM webServerMG811FormParameterCalPoint1Adc[] = "p1adc";

const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint0Ppm[] = "Calibration point 0: PPM value";
const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint0Adc[] = "Calibration point 0: uncalibrated ADC value";
const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint1Ppm[] = "Calibration point 1: PPM value";
const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint1Adc[] = "Calibration point 1: uncalibrated ADC value";

const char PROGMEM webServerMG811FormParameterRejectCal[] = "calreject";
const char PROGMEM webServerMG811FormParameterFilter[] = "filter";
const char PROGMEM webServerMG811FormParameterFilterFreq[] = "fltfreq";
const char PROGMEM webServerFormParameterSensorSerialOutput[] = "sout";

const char PROGMEM webServerMG811FormParameterDisplayNameRejectCal[] = "Reject calibration data and use inverted uncalibrated value (no exact ppm calculation, trend only):";
const char PROGMEM webServerMG811FormParameterDisplayNameFilter[] = "Filter:";
const char PROGMEM webServerMG811FormParameterDisplayNameFilterFreq[] = "Low-pass filter frequency (x 0.01 Hz)";
const char PROGMEM webServerFormParameterDisplayNameSensorSerialOutput[] = "Serial output";

const char PROGMEM webServerMG811FormParameterDisplayFilterOff[] = "Off";
const char PROGMEM webServerMG811FormParameterDisplayFilterAverage[] = "Averaging";
const char PROGMEM webServerMG811FormParameterDisplayFilterLowpass[] = "Low-pass";

