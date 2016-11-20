/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCONFIG_H
#define WEBCONFIG_H

#include <Arduino.h>
#include "stringmap.h"
#include "eeprom_config.h"
#include "webcc.h"

namespace webconfig {

#ifdef MODULE_TEXT
#undef MODULE_TEXT
#endif
#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value

#ifdef FSH
#undef FSH
#endif
#define FSH(name) reinterpret_cast<const __FlashStringHelper *>(name)

class Texts {
    MODULE_TEXT(moduleName, "WebConfig");

    MODULE_TEXT(pathGET, "/webconfig");
    MODULE_TEXT(pathPOST, "/webconfig_set");
    MODULE_TEXT(formMethod, "POST");
} __attribute__((packed));

class TextsUI {
    MODULE_TEXT(formCaption, "Module Configuration");
    MODULE_TEXT(submitButton, "Save settings");
    MODULE_TEXT(sectionConnection, "Connection");
    MODULE_TEXT(subsectionWiFi, "WiFi");
    MODULE_TEXT(parameterNameWiFiSSID, "SSID");
    MODULE_TEXT(parameterNameWiFiPassword, "Password");
    MODULE_TEXT(parameterTooltipWiFiPassword, "Leave blank if connecting to open WiFi network");
    MODULE_TEXT(subsectionBlynk, "Blynk");
    MODULE_TEXT(parameterNameBlynkAuth, "Auth Token");
    MODULE_TEXT(parameterTooltipBlynkAuth, "Auth token is provided by Blynk app");
    MODULE_TEXT(parameterNameBlynkServer, "Server");
    MODULE_TEXT(parameterTooltipBlynkServer, "Use Blynk Cloud (blynk-cloud.com) or enter your private server");
    MODULE_TEXT(parameterNameBlynkPort, "Port");
    MODULE_TEXT(parameterTooltipBlynkPort, "Default Blynk port is 8442");
    MODULE_TEXT(sectionADC, "MG811");
    MODULE_TEXT(subsectionADCCalibration, "Calibration points");
    MODULE_TEXT(parameterNameADCCalPoint0Cal, "Point 0: ppm value");
    MODULE_TEXT(parameterNameADCCalPoint0Raw, "Point 0: raw value");
    MODULE_TEXT(parameterNameADCCalPoint1Cal, "Point 1: ppm value");
    MODULE_TEXT(parameterNameADCCalPoint1Raw, "Point 1: raw value");
    MODULE_TEXT(parameterNameADCCalReject, "Reject calibration data");
    MODULE_TEXT(parameterTooltipADCCalReject, "No ppm value calculation possible. Instead of ppm value, "
                "inverted raw ADC value is used. Enable this option if you would like to view trend only.");
    MODULE_TEXT(subsectionFilter, "Filtering");
    MODULE_TEXT(parameterNameADCFilterType, "Filter type");
    MODULE_TEXT(parameterTooltipADCFilterType, "Off = no filtering<br>"
                "Averaging = rolling average over fixed amount of points<br>"
                "Low-pass = low-pass filter with user-defined threshould frequency");
    MODULE_TEXT(parameterOptionADCFilterOff, "Off");
    MODULE_TEXT(parameterOptionADCFilterAverage, "Averaging");
    MODULE_TEXT(parameterOptionADCFilterLowPass, "Low-pass");
    MODULE_TEXT(parameterNameADCFilterLowpassFreq, "Low-pass filter frequency");
    MODULE_TEXT(parameterTooltipADCFilterLowpassFreq, "x 0.01 Hz");
    MODULE_TEXT(sectionMisc, "Misc");
    MODULE_TEXT(subsectionSerialPort, "Serial port");
    MODULE_TEXT(parameterNameSerialDataOut, "Send sensor data to serial port");
    MODULE_TEXT(subsectionStartup, "Startup");
    MODULE_TEXT(parameterNameStartupDelay, "Startup Delay");
    MODULE_TEXT(parameterTooltipStartupDelay, "x 100 milliseconds");

    MODULE_TEXT(sendingConfig, "Sending config page");
    MODULE_TEXT(configSent, "Config page sent");
} __attribute__((packed));

extern const Texts PROGMEM texts;
extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

template <class Diag>
class WebConfig : public Module <WebConfig<Diag>> {
  public:
    boolean onHTTPReqStart(void);
    boolean onHTTPReqPath(const char * path);
    boolean onHTTPReqMethod(const char * method);
    boolean onHTTPReqPOSTQuery(const char * name, const char * value);
    boolean onRespond(Print &client);
    boolean onHTTPReqEnd(boolean error);
  private:
    boolean pathGet = false;
    boolean pathPost = false;
    boolean configUpdated = false;
  public:
    const char * PROGMEM moduleName(void);
    const char * PROGMEM getMainPath(void);
  private:
    void resetCheckBoxParameters(void);
    void sendConfigPage(Print &client);
    void setParameter(EepromSavedParameter parameterId, const char * value);
};

template <class Diag>
boolean WebConfig<Diag>::onHTTPReqStart(void) {
  pathGet = false;
  pathPost = false;
  configUpdated = false;
  return (true);
}

template <class Diag>
boolean WebConfig<Diag>::onHTTPReqPath(const char * path) {
  if (!strcmp_P(path, texts.pathGET)) pathGet = true;
  if (!strcmp_P(path, texts.pathPOST)) pathPost = true;
  return (pathGet || pathPost);
}

template <class Diag>
boolean WebConfig<Diag>::onHTTPReqMethod(const char * method) {
  HTTPRequestMethod m = HTTPRequestHelper::getMethod(method);
  if (pathGet && (m == HTTPRequestMethod::GET)) return (true);
  if (pathPost && (m == HTTPRequestMethod::POST))
  {
    resetCheckBoxParameters();
    return (true);
  }
  return (false);
}

template <class Diag>
boolean WebConfig<Diag>::onHTTPReqPOSTQuery(const char * name, const char * value) {
  if (!pathPost) return (true);
  QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
  EepromSavedParameter parameterName = (EepromSavedParameter)parameterNameFinder.find(name);
  setParameter(parameterName, value);
  return (true);
}

template <class Diag>
boolean WebConfig<Diag>::onRespond(Print &client) {
  if (pathGet) {
    sendConfigPage(client);
    return (true);
  }
  if (pathPost) {
    HTTPResponseHeader::redirect(client, FSH(texts.pathGET));
    return (true);
  }
  return (false);
}

template <class Diag>
boolean WebConfig<Diag>::onHTTPReqEnd(boolean error) {
  if (pathPost && !error) {
    if (configUpdated) saveConfig();
    loadConfig();
  }
  pathGet = false;
  pathPost = false;
  configUpdated = false;
  return (true);
}

template <class Diag>
const char * PROGMEM WebConfig<Diag>::moduleName (void) {
  return (texts.moduleName);
}

template <class Diag>
const char * PROGMEM WebConfig<Diag>::getMainPath(void) {
  return (texts.pathGET);
}

template <class Diag>
void WebConfig<Diag>::resetCheckBoxParameters(void) {
  eepromSavedParametersStorage.rejectCalibrationMG811 = 0;
  eepromSavedParametersStorage.sensorSerialOutput = 0;
}

template <class Diag>
void WebConfig<Diag>::sendConfigPage(Print &client) {
  Diag::instance()->log (Diag::Severity::INFORMATIONAL, FSH(textsUI.sendingConfig));
  HTTPResponseHeader::contentHeader(client, HTTPContentType::HTML);
  QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
  webcc::WebccForm form(client);
  form.bodyBegin(texts.pathPOST, textsUI.formCaption, textsUI.submitButton, texts.formMethod);
  form.sectionBegin(textsUI.sectionConnection);
  form.subsectionBegin(textsUI.subsectionWiFi);
  form.textParameter(textsUI.parameterNameWiFiSSID,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::WIFI_SSID),
                     eepromSavedParametersStorage.wifiSsid);
  form.textParameter(textsUI.parameterNameWiFiPassword,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::WIFI_PASSWORD),
                     eepromSavedParametersStorage.wifiPassword,
                     textsUI.parameterTooltipWiFiPassword);
  form.subsectionBegin(textsUI.subsectionBlynk);
  form.textParameter(textsUI.parameterNameBlynkAuth,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN),
                     eepromSavedParametersStorage.authToken,
                     textsUI.parameterTooltipBlynkAuth);
  form.textParameter(textsUI.parameterNameBlynkServer,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER),
                     eepromSavedParametersStorage.blynkServer,
                     textsUI.parameterTooltipBlynkServer);
  form.textParameter(textsUI.parameterNameBlynkPort,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT),
                     eepromSavedParametersStorage.blynkServerPort,
                     textsUI.parameterTooltipBlynkPort);
  form.sectionBegin(textsUI.sectionADC);
  form.subsectionBegin(textsUI.subsectionADCCalibration);
  form.textParameter(textsUI.parameterNameADCCalPoint0Cal,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL),
                     eepromSavedParametersStorage.MG811CalPoint0Calibrated);
  form.textParameter(textsUI.parameterNameADCCalPoint0Raw,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW),
                     eepromSavedParametersStorage.MG811CalPoint0Raw);
  form.textParameter(textsUI.parameterNameADCCalPoint1Cal,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL),
                     eepromSavedParametersStorage.MG811CalPoint1Calibrated);
  form.textParameter(textsUI.parameterNameADCCalPoint1Raw,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW),
                     eepromSavedParametersStorage.MG811CalPoint1Raw);
  form.checkboxParameter(textsUI.parameterNameADCCalReject,
                         parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION),
                         (boolean)eepromSavedParametersStorage.rejectCalibrationMG811,
                         textsUI.parameterTooltipADCCalReject);
  form.subsectionBegin(textsUI.subsectionFilter);
  form.selectParameter(textsUI.parameterNameADCFilterType,
                       parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811FILTER_TYPE),
                       (long)eepromSavedParametersStorage.filterMG811,
                       textsUI.parameterTooltipADCFilterType,
                       true,
                       (long)ADCFilter::OFF, textsUI.parameterOptionADCFilterOff,
                       (long)ADCFilter::AVERAGE, textsUI.parameterOptionADCFilterAverage,
                       (long)ADCFilter::LOWPASS, textsUI.parameterOptionADCFilterLowPass
                      );
  form.textParameter(textsUI.parameterNameADCFilterLowpassFreq,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ),
                     eepromSavedParametersStorage.filterMG811LowPassFrequency,
                     textsUI.parameterTooltipADCFilterLowpassFreq);
  form.sectionBegin(textsUI.sectionMisc);
  form.subsectionBegin(textsUI.subsectionSerialPort);
  form.checkboxParameter(textsUI.parameterNameSerialDataOut,
                         parameterNameFinder.find((StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT),
                         (boolean)eepromSavedParametersStorage.sensorSerialOutput);
  form.subsectionBegin(textsUI.subsectionStartup);
  form.textParameter(textsUI.parameterNameStartupDelay,
                     parameterNameFinder.find((StringMapKey)EepromSavedParameter::STARTUP_DELAY),
                     eepromSavedParametersStorage.startupDelay,
                     textsUI.parameterTooltipStartupDelay);
  form.bodyEnd();
  Diag::instance()->log(Diag::Severity::DEBUG, FSH(textsUI.configSent));
}

template <class Diag>
void WebConfig<Diag>::setParameter(EepromSavedParameter parameterId, const char * value) {
  float valueAsNumber = atof (value);
  switch (parameterId) {
    case EepromSavedParameter::BLYNK_AUTH_TOKEN:
      strncpy(eepromSavedParametersStorage.authToken, value, sizeof(eepromSavedParametersStorage.authToken) - 1);
      eepromSavedParametersStorage.authToken[sizeof(eepromSavedParametersStorage.authToken) - 1] = 0;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::WIFI_SSID:
      strncpy(eepromSavedParametersStorage.wifiSsid, value, sizeof(eepromSavedParametersStorage.wifiSsid) - 1);
      eepromSavedParametersStorage.wifiSsid[sizeof(eepromSavedParametersStorage.wifiSsid) - 1] = 0;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::WIFI_PASSWORD:
      strncpy(eepromSavedParametersStorage.wifiPassword, value, sizeof(eepromSavedParametersStorage.wifiPassword) - 1);
      eepromSavedParametersStorage.wifiPassword[sizeof(eepromSavedParametersStorage.wifiPassword) - 1] = 0;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT0_CAL:
      eepromSavedParametersStorage.MG811CalPoint0Calibrated = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT0_RAW:
      eepromSavedParametersStorage.MG811CalPoint0Raw = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT1_CAL:
      eepromSavedParametersStorage.MG811CalPoint1Calibrated = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT1_RAW:
      eepromSavedParametersStorage.MG811CalPoint1Raw = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_REJECT_CALIBRATION:
      eepromSavedParametersStorage.rejectCalibrationMG811 = 1;
      this->configUpdated = true;
    case EepromSavedParameter::MG811FILTER_TYPE:
      eepromSavedParametersStorage.filterMG811 = (ADCFilter)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811FILTER_LOWPASS_FREQ:
      eepromSavedParametersStorage.filterMG811LowPassFrequency = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::BLYNK_SERVER:
      strncpy(eepromSavedParametersStorage.blynkServer, value, sizeof(eepromSavedParametersStorage.blynkServer) - 1);
      eepromSavedParametersStorage.blynkServer[sizeof(eepromSavedParametersStorage.blynkServer) - 1] = 0;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::BLYNK_SERVER_PORT:
      eepromSavedParametersStorage.blynkServerPort = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MISC_SERIAL_OUT:
      eepromSavedParametersStorage.sensorSerialOutput = 1;
      this->configUpdated = true;
    case EepromSavedParameter::STARTUP_DELAY:
      eepromSavedParametersStorage.startupDelay = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    default:
      break;
  }
}

#undef FSH

};

#endif
