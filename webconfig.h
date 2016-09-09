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
#include "webcc_html.h"
#include "uitexts.h"

namespace webconfig {

#ifdef MODULE_TEXT
#undef MODULE_TEXT
#endif
#define MODULE_TEXT(name,value) const char name [sizeof(value)] = value

#ifdef PTEXT
#undef PTEXT
#endif
#define PTEXT(name) (__FlashStringHelper *)name

class Texts {
  public:
    MODULE_TEXT(moduleName, "WebConfig");

    MODULE_TEXT(pathGET, "/webconfig");
    MODULE_TEXT(pathPOST, "/webconfig_set");
    MODULE_TEXT(formMethod, "POST");
} __attribute__((packed));

extern const Texts PROGMEM texts;

#undef MODULE_TEXT

template <class Diag>
class WebConfig : public Module <WebConfig<Diag>> {
  public:
    boolean onHTTPReqStart(void) {
      pathGet = false;
      pathPost = false;
      configUpdated = false;
      return (true);
    }
    boolean onHTTPReqPath(const char * path) {
      if (!strcmp_P(path, texts.pathGET)) pathGet = true;
      if (!strcmp_P(path, texts.pathPOST)) pathPost = true;
      return (pathGet || pathPost);
    }
    boolean onHTTPReqMethod(const char * method) {
      HTTPRequestMethod m = HTTPRequestHelper::getMethod(method);
      if (pathGet && (m == HTTPRequestMethod::GET)) return (true);
      if (pathPost && (m == HTTPRequestMethod::POST))
      {
        resetCheckBoxParameters();
        return (true);
      }
      return (false);
    }
    boolean onHTTPReqPOSTQuery(const char * name, const char * value) {
      if (!pathPost) return (true);
      QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
      EepromSavedParameter parameterName = (EepromSavedParameter)parameterNameFinder.find(name);
      setParameter(parameterName, value);
      return (true);
    }
    boolean onRespond(Print &client) {
      if (pathGet) {
        sendConfigPage(client);
        return (true);
      }
      if (pathPost) {
        HTTPResponseHeader::redirect(client, PTEXT(texts.pathGET));
        return (true);
      }
      return (false);
    }
    boolean onHTTPReqEnd(boolean error) {
      if (pathPost && !error) {
        if (configUpdated) saveConfig();
        loadConfig();
      }
      pathGet = false;
      pathPost = false;
      configUpdated = false;
      return (true);
    }
  private:
    boolean pathGet = false;
    boolean pathPost = false;
    boolean configUpdated = false;
  public:
    const char * PROGMEM moduleName (void) {
      return (texts.moduleName);
    }
    const char * PROGMEM getMainPath(void) {
      return (texts.pathGET);
    }
  private:
    void resetCheckBoxParameters(void) {
      eepromSavedParametersStorage.rejectCalibrationMG811 = 0;
      eepromSavedParametersStorage.sensorSerialOutput = 0;
    }
    void sendConfigPage(Print &client) {
      Diag::instance()->timestamp();
      Diag::instance()->println("Sending config webpage.");
      HTTPResponseHeader::contentHeader(client, HTTPContentType::HTML);
      QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
      WebccHTMLStringMaps webConfigPage(client,
                                        webccUIParameterSections,
                                        webccUIParameterDisplayNames,
                                        parameterNameFinder,
                                        webccUIParameterTooltips);
      webConfigPage.begin((StringMapKey)webccUISections::CAPTION);
      webConfigPage.section((StringMapKey)webccUISections::CONNECTION);
      webConfigPage.subsection((StringMapKey)webccUISections::WIFI);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::WIFI_SSID, eepromSavedParametersStorage.wifiSsid);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::WIFI_PASSWORD, eepromSavedParametersStorage.wifiPassword);
      webConfigPage.subsection((StringMapKey)webccUISections::BLYNK);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN, eepromSavedParametersStorage.authToken);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::BLYNK_SERVER, eepromSavedParametersStorage.blynkServer);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT, eepromSavedParametersStorage.blynkServerPort);
      webConfigPage.section((StringMapKey)webccUISections::ADC);
      webConfigPage.subsection((StringMapKey)webccUISections::ADC_CALIBRATION);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL, eepromSavedParametersStorage.MG811CalPoint0Calibrated);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW, eepromSavedParametersStorage.MG811CalPoint0Raw);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL, eepromSavedParametersStorage.MG811CalPoint1Calibrated);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW, eepromSavedParametersStorage.MG811CalPoint1Raw);
      webConfigPage.checkBoxParameter((StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION, (boolean)eepromSavedParametersStorage.rejectCalibrationMG811);
      webConfigPage.subsection((StringMapKey)webccUISections::ADC_FILTER);
      webConfigPage.selectParameter((StringMapKey)EepromSavedParameter::MG811FILTER_TYPE, (long)eepromSavedParametersStorage.filterMG811, webccUISelectADCFilter);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ, eepromSavedParametersStorage.filterMG811LowPassFrequency);
      webConfigPage.section((StringMapKey)webccUISections::MISC);
      webConfigPage.subsection((StringMapKey)webccUISections::SERIAL_PORT);
      webConfigPage.checkBoxParameter((StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT, (boolean)eepromSavedParametersStorage.sensorSerialOutput);
      webConfigPage.subsection((StringMapKey)webccUISections::STARTUP);
      webConfigPage.textParameter((StringMapKey)EepromSavedParameter::STARTUP_DELAY, eepromSavedParametersStorage.startupDelay);
      webConfigPage.end();
      Diag::instance()->timestamp();
      Diag::instance()->println("Config webpage sent.");
    }
    void setParameter(EepromSavedParameter parameterId, const char * value) {
      float valueAsNumber = atof (value);
      switch (parameterId) {
        case EepromSavedParameter::BLYNK_AUTH_TOKEN:
          Serial.println("!auth token!");
          strncpy(eepromSavedParametersStorage.authToken, value, sizeof(eepromSavedParametersStorage.authToken) - 1);
          eepromSavedParametersStorage.authToken[sizeof(eepromSavedParametersStorage.authToken) - 1] = 0;
          this->configUpdated = true;
          break;
        case EepromSavedParameter::WIFI_SSID:
          Serial.println("!wifi ssid!");
          strncpy(eepromSavedParametersStorage.wifiSsid, value, sizeof(eepromSavedParametersStorage.wifiSsid) - 1);
          eepromSavedParametersStorage.wifiSsid[sizeof(eepromSavedParametersStorage.wifiSsid) - 1] = 0;
          this->configUpdated = true;
          break;
        case EepromSavedParameter::WIFI_PASSWORD:
          Serial.println("!wifi pass!");
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
};
};

#endif
