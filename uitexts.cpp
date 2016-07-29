/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "uitexts.h"
#include "adc.h"
#include "eeprom_config.h"

//////////////////////////////////////////////////////////////////////
// StringMaps for Select Parameters
// This section contains all user interface texts which can be seen in browser
//////////////////////////////////////////////////////////////////////

//Section & subsection display names, parameter display names and parameter tooltips

const char PROGMEM webccUIText_Caption[] = "Module Configuration";

const char PROGMEM webccUIText_SectionDisplayName_Connection[] = "Connection";

const char PROGMEM webccUIText_SubsectionDisplayName_WiFi[]       = "WiFi";
const char PROGMEM webccUIText_ParameterDisplayName_WiFiSSID[]      = "SSID";
const char PROGMEM webccUIText_ParameterDisplayName_WiFiPassword[]  = "Password";
const char PROGMEM webccUIText_ParameterTooltip_WiFiPassword[]        = "Leave blank if connecting to open WiFi network";

const char PROGMEM webccUIText_SubsectionDisplayName_Blynk[]      = "Blynk";
const char PROGMEM webccUIText_ParameterDisplayName_BlynkAuth[]     = "Auth Token";
const char PROGMEM webccUIText_ParameterTooltip_BlynkAuth[]           = "Auth token is provided by Blynk app";
const char PROGMEM webccUIText_ParameterDisplayName_BlynkServer[]   = "Server";
const char PROGMEM webccUIText_ParameterTooltip_BlynkServer[]         = "Use Blynk Cloud (blynk-cloud.com) or enter your private server";
const char PROGMEM webccUIText_ParameterDisplayName_BlynkPort[]     = "Port";
const char PROGMEM webccUIText_ParameterTooltip_BlynkPort[]           = "Default Blynk port is 8442";

const char PROGMEM webccUIText_SectionDisplayName_ADC[] = "MG811";

const char PROGMEM webccUIText_SubsectionDisplayName_ADCCalibration[] = "Calibration points";
const char PROGMEM webccUIText_ParameterDisplayName_ADCCalPoint0Cal[]   = "Point 0: ppm value";
const char PROGMEM webccUIText_ParameterDisplayName_ADCCalPoint0Raw[]   = "Point 0: raw value";
const char PROGMEM webccUIText_ParameterDisplayName_ADCCalPoint1Cal[]   = "Point 1: ppm value";
const char PROGMEM webccUIText_ParameterDisplayName_ADCCalPoint1Raw[]   = "Point 1: raw value";
const char PROGMEM webccUIText_ParameterDisplayName_ADCCalReject[]      = "Reject calibration data";
const char PROGMEM webccUIText_ParameterTooltip_ADCCalReject[]            = "No ppm value calculation possible. Instead of ppm value, "
    "inverted raw ADC value is used. Enable this option if you would like to view trend only.";

const char PROGMEM webccUIText_SubsectionDisplayName_Filter[]           = "Filtering";
const char PROGMEM webccUIText_ParameterDisplayName_ADCFilterType[]         = "Filter type";
const char PROGMEM webccUIText_ParameterTooltip_ADCFilterType[]               = "Off = no filtering<br>"
    "Averaging = rolling average over fixed amount of points<br>"
    "Low-pass = low-pass filter with user-defined threshould frequency";
const char PROGMEM webccUIText_SelectOption_ADCFilterOff[]                      = "Off";
const char PROGMEM webccUIText_SelectOption_ADCFilterAverage[]                  = "Averaging";
const char PROGMEM webccUIText_SelectOption_ADCFilterLowPass[]                  = "Low-pass";

const char PROGMEM webccUIText_ParameterDisplayName_ADCFilterLowpassFreq[]  = "Low-pass filter frequency";
const char PROGMEM webccUIText_ParameterTooltip_ADCFilterLowpassFreq[]        = "x 0.01 Hz";

const char PROGMEM webccUIText_SectionDisplayName_Misc[] = "Misc";

const char PROGMEM webccUIText_SubsectionDisplayName_SerialPort[]       = "Serial port";
const char PROGMEM webccUIText_ParameterDisplayName_SerialDataOut[]       = "Send sensor data to serial port";

const char PROGMEM webccUIText_SubsectionDisplayName_Startup[]          = "Startup";
const char PROGMEM webccUIText_ParameterDisplayName_StartupDelay[]        = "StartupDelay";
const char PROGMEM webccUIText_ParameterTooltip_StartupDelay[]              = "x 100 milliseconds";

//////////////////////////////////////////////////////////////////////
// Sections StringMap
//////////////////////////////////////////////////////////////////////

const PROGMEM ProgmemStringMapItem webccUI_parameterSections[] = {
  {(StringMapKey)webccUISections::CAPTION,          webccUIText_Caption},
  {(StringMapKey)webccUISections::CONNECTION,       webccUIText_SectionDisplayName_Connection},
  {(StringMapKey)webccUISections::WIFI,             webccUIText_SubsectionDisplayName_WiFi},
  {(StringMapKey)webccUISections::BLYNK,            webccUIText_SubsectionDisplayName_Blynk},
  {(StringMapKey)webccUISections::ADC,              webccUIText_SectionDisplayName_ADC},
  {(StringMapKey)webccUISections::ADC_CALIBRATION,  webccUIText_SubsectionDisplayName_ADCCalibration},
  {(StringMapKey)webccUISections::ADC_FILTER,       webccUIText_SubsectionDisplayName_Filter},
  {(StringMapKey)webccUISections::MISC,             webccUIText_SectionDisplayName_Misc},
  {(StringMapKey)webccUISections::SERIAL_PORT,      webccUIText_SubsectionDisplayName_SerialPort},
  {(StringMapKey)webccUISections::STARTUP,          webccUIText_SubsectionDisplayName_Startup},
  {(StringMapKey)webccUISections::UNKNOWN,          NULL},
};
ProgmemStringMap webccUIParameterSections (webccUI_parameterSections);

//////////////////////////////////////////////////////////////////////
// Parameter Display Names StringMap
//////////////////////////////////////////////////////////////////////

const PROGMEM ProgmemStringMapItem webccUI_parameterDisplayNames[] = {
  {(StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN,          webccUIText_ParameterDisplayName_BlynkAuth},
  {(StringMapKey)EepromSavedParameter::WIFI_SSID,                 webccUIText_ParameterDisplayName_WiFiSSID},
  {(StringMapKey)EepromSavedParameter::WIFI_PASSWORD,             webccUIText_ParameterDisplayName_WiFiPassword},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL,       webccUIText_ParameterDisplayName_ADCCalPoint0Cal},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW,       webccUIText_ParameterDisplayName_ADCCalPoint0Raw},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL,       webccUIText_ParameterDisplayName_ADCCalPoint1Cal},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW,       webccUIText_ParameterDisplayName_ADCCalPoint1Raw},
  {(StringMapKey)EepromSavedParameter::MG811FILTER_TYPE,          webccUIText_ParameterDisplayName_ADCFilterType},
  {(StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ,  webccUIText_ParameterDisplayName_ADCFilterLowpassFreq},
  {(StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION,  webccUIText_ParameterDisplayName_ADCCalReject},
  {(StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT,           webccUIText_ParameterDisplayName_SerialDataOut},
  {(StringMapKey)EepromSavedParameter::BLYNK_SERVER,              webccUIText_ParameterDisplayName_BlynkServer},
  {(StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT,         webccUIText_ParameterDisplayName_BlynkPort},
  {(StringMapKey)EepromSavedParameter::STARTUP_DELAY,             webccUIText_ParameterDisplayName_StartupDelay},
  {(StringMapKey)EepromSavedParameter::UNKNOWN, NULL}
};
ProgmemStringMap webccUIParameterDisplayNames (webccUI_parameterDisplayNames);

//////////////////////////////////////////////////////////////////////
// Tooltips StringMap
//////////////////////////////////////////////////////////////////////

const PROGMEM ProgmemStringMapItem webccUI_parameterTooltips[] = {
  {(StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN,          webccUIText_ParameterTooltip_BlynkAuth},
  {(StringMapKey)EepromSavedParameter::WIFI_PASSWORD,             webccUIText_ParameterTooltip_WiFiPassword},
  {(StringMapKey)EepromSavedParameter::MG811FILTER_TYPE,          webccUIText_ParameterTooltip_ADCFilterType},
  {(StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ,  webccUIText_ParameterTooltip_ADCFilterLowpassFreq},
  {(StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION,  webccUIText_ParameterTooltip_ADCCalReject},
  {(StringMapKey)EepromSavedParameter::BLYNK_SERVER,              webccUIText_ParameterTooltip_BlynkServer},
  {(StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT,         webccUIText_ParameterTooltip_BlynkPort},
  {(StringMapKey)EepromSavedParameter::STARTUP_DELAY,             webccUIText_ParameterTooltip_StartupDelay},
  {(StringMapKey)EepromSavedParameter::UNKNOWN, NULL}
};
ProgmemStringMap webccUIParameterTooltips (webccUI_parameterTooltips);

//////////////////////////////////////////////////////////////////////
// Select Parameters StringMaps
//////////////////////////////////////////////////////////////////////

const ProgmemStringMapItem PROGMEM webccUI_SelectOptions_ADCFilter[] = {
  {(StringMapKey)ADCFilter::OFF,      webccUIText_SelectOption_ADCFilterOff},
  {(StringMapKey)ADCFilter::AVERAGE,  webccUIText_SelectOption_ADCFilterAverage},
  {(StringMapKey)ADCFilter::LOWPASS,  webccUIText_SelectOption_ADCFilterLowPass},
  {(StringMapKey)ADCFilter::OFF,      NULL}
};
ProgmemStringMap webccUISelectADCFilter (webccUI_SelectOptions_ADCFilter);
