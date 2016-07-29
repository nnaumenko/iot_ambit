/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "diag.h"
#include "eeprom_config.h"
#include "http.h"
#include "version.h"
#include "uitexts.h"
#include "webcc.h"
#include "webcc_html.h"
#include "webconfig.h"

enum class HTTPRequestMethod {
  UNKNOWN = 0,
  OPTIONS,
  GET,
  POST,
};

const char PROGMEM stringMapHTTPRequestMethod_Options[] = "OPTIONS";
const char PROGMEM stringMapHTTPRequestMethod_Get[] = "GET";
const char PROGMEM stringMapHTTPRequestMethod_Post[] = "POST";

const PROGMEM ProgmemStringMapItem stringMapHTTPRequestMethod [] = {
  {(StringMapKey)HTTPRequestMethod::OPTIONS,  stringMapHTTPRequestMethod_Options},
  {(StringMapKey)HTTPRequestMethod::GET,      stringMapHTTPRequestMethod_Get},
  {(StringMapKey)HTTPRequestMethod::POST,     stringMapHTTPRequestMethod_Post},
  {(StringMapKey)HTTPRequestMethod::UNKNOWN,  NULL}
};

enum class HTTPRequestPath {
  UNKNOWN = 0,
  ASTERISK,
  ROOT,
  SETCONFIG,
};

const char PROGMEM stringMapHTTPRequestPath_Asterisk[] = "*";
const char PROGMEM stringMapHTTPRequestPath_Root[] = "/";
const char PROGMEM stringMapHTTPRequestPath_SetConfig[] = "/setconfig";

const PROGMEM ProgmemStringMapItem stringMapHTTPRequestPath [] = {
  {(StringMapKey)HTTPRequestPath::ASTERISK, stringMapHTTPRequestPath_Asterisk},
  {(StringMapKey)HTTPRequestPath::ROOT,     stringMapHTTPRequestPath_Root},
  {(StringMapKey)HTTPRequestPath::SETCONFIG, stringMapHTTPRequestPath_SetConfig},
  {(StringMapKey)HTTPRequestPath::UNKNOWN,  NULL}
};

const char PROGMEM HTTPStatusCode_100_Continue[]            = "Continue";
const char PROGMEM HTTPStatusCode_200_OK[]                  = "OK";
const char PROGMEM HTTPStatusCode_303_SeeOther[]            = "See Other";
const char PROGMEM HTTPStatusCode_400_BadRequest[]          = "Bad Request";
const char PROGMEM HTTPStatusCode_404_NotFound[]            = "Not Found";
const char PROGMEM HTTPStatusCode_405_MethodNotAllowed[]    = "Method Not Allowed";
const char PROGMEM HTTPStatusCode_414_RequestURITooLong[]   = "Request-URI Too Long";
const char PROGMEM HTTPStatusCode_500_InternalServerError[] = "Internal Server Error";
const char PROGMEM HTTPStatusCode_501_NotImplemented[]      = "Not Implemented";

const PROGMEM ProgmemStringMapItem stringMapHTTPStatusCode [] = {
  {(StringMapKey)HTTPStatusCode::CONTINUE,              HTTPStatusCode_100_Continue},
  {(StringMapKey)HTTPStatusCode::OK,                    HTTPStatusCode_200_OK},
  {(StringMapKey)HTTPStatusCode::SEE_OTHER,             HTTPStatusCode_303_SeeOther},
  {(StringMapKey)HTTPStatusCode::BAD_REQUEST,           HTTPStatusCode_400_BadRequest},
  {(StringMapKey)HTTPStatusCode::NOT_FOUND,             HTTPStatusCode_404_NotFound},
  {(StringMapKey)HTTPStatusCode::METHOD_NOT_ALLOWED,    HTTPStatusCode_405_MethodNotAllowed},
  {(StringMapKey)HTTPStatusCode::REQUEST_URI_TOO_LONG,  HTTPStatusCode_414_RequestURITooLong},
  {(StringMapKey)HTTPStatusCode::INTERNAL_SERVER_ERROR, HTTPStatusCode_500_InternalServerError},
  {(StringMapKey)HTTPStatusCode::NOT_IMPLEMENTED,       HTTPStatusCode_501_NotImplemented},
  {(StringMapKey)HTTPStatusCode::UNKNOWN,               NULL}
};

class WebConfigReqPartHandler : public HTTPReqPartHandler {
  public:
    virtual void handleReqPart(char * value, HTTPRequestPart part);
  public:
    HTTPRequestMethod getMethod(void);
    HTTPRequestPath getPath(void);
    void resetCheckBoxParameters(void);
    void finish(void);
    void redirectToRoot(void);
  private:
    HTTPRequestMethod reqMethod = HTTPRequestMethod::UNKNOWN;
    HTTPRequestPath reqPath = HTTPRequestPath::UNKNOWN;
  private:
    void setParameter(EepromSavedParameter parameterId, char * value);
    EepromSavedParameter lastParameterName;
    boolean configUpdated = false;
  private:
    void sendHttpStatus(HTTPStatusCode statusCode);
    void sendConfigPage(void);
    void sendOptionsPage(void);
};

void WebConfigReqPartHandler::handleReqPart(char * value, HTTPRequestPart part) {
  if (part == HTTPRequestPart::METHOD) {
    ProgmemStringMap methodFinder (stringMapHTTPRequestMethod);
    this->reqMethod = (HTTPRequestMethod)methodFinder.find(value);
    if (reqMethod == HTTPRequestMethod::OPTIONS) return;
    if (reqMethod == HTTPRequestMethod::GET) return;
    if (reqMethod == HTTPRequestMethod::POST) return;
    this->parser->setError(HTTPStatusCode::NOT_IMPLEMENTED);
  }

  if (part == HTTPRequestPart::PATH) {
    ProgmemStringMap pathFinder (stringMapHTTPRequestPath);
    this->reqPath = (HTTPRequestPath)pathFinder.find(value);
    if (this->reqPath == HTTPRequestPath::ASTERISK) {
      if (this->reqMethod != HTTPRequestMethod::OPTIONS) this->parser->setError(HTTPStatusCode::METHOD_NOT_ALLOWED);
      return;
    }
    if (this->reqPath == HTTPRequestPath::ROOT) {
      if (this->reqMethod != HTTPRequestMethod::GET) this->parser->setError(HTTPStatusCode::METHOD_NOT_ALLOWED);
      return;
    }
    if (this->reqPath == HTTPRequestPath::SETCONFIG) {
      if (this->reqMethod != HTTPRequestMethod::POST) this->parser->setError(HTTPStatusCode::METHOD_NOT_ALLOWED);
      this->resetCheckBoxParameters();
      return;
    }
    this->parser->setError(HTTPStatusCode::NOT_FOUND);
  }
  if (part == HTTPRequestPart::POST_QUERY_NAME) {
    if (this->reqPath != HTTPRequestPath::SETCONFIG) return;
    QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
    lastParameterName = (EepromSavedParameter)parameterNameFinder.find(value);
  }
  if (part == HTTPRequestPart::POST_QUERY_VALUE) {
    if (this->reqPath != HTTPRequestPath::SETCONFIG) return;
    if (lastParameterName != EepromSavedParameter::UNKNOWN)
      this->setParameter(lastParameterName, value);
  }
  if (part == HTTPRequestPart::FINISH) {
    this->finish();
    return;
  }
}

void WebConfigReqPartHandler::setParameter(EepromSavedParameter parameterId, char * value) {
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
  }
}

void WebConfigReqPartHandler::resetCheckBoxParameters(void) {
  eepromSavedParametersStorage.rejectCalibrationMG811 = 0;
  eepromSavedParametersStorage.sensorSerialOutput = 0;
}

void WebConfigReqPartHandler::sendHttpStatus(HTTPStatusCode statusCode) {
  ProgmemStringMap statusCodeText (stringMapHTTPStatusCode);
  this->client->print(F("HTTP/1.1 "));
  this->client->println((int)statusCode, DEC);
  statusCodeText.print(*this->client, (StringMapKey)statusCode);
  DiagLog.print(F("HTTP status code: "));
  DiagLog.print((int)statusCode, DEC);
  DiagLog.print(F(" "));
  statusCodeText.print(DiagLog, (int)statusCode);
  DiagLog.println();
}

void WebConfigReqPartHandler::sendConfigPage(void) {
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Sending config webpage."));
  this->client->print(F("Content-Type: text/html\r\nCache-control: no-cache\r\nConnection: close\r\n\r\n"));
  QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
  WebccHTMLStringMaps webConfigPage(*this->client,
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
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Config webpage sent to client."));
}

void WebConfigReqPartHandler::redirectToRoot(void) {
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Redirecting to root."));
  this->client->print(F("\r\nLocation: http://"));
  this->client->print(WiFi.softAPIP());
  this->client->print(F("/\r\n\r\n"));
}

void WebConfigReqPartHandler::sendOptionsPage(void) {
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Sending options page."));
  this->client->print(F("Allow: OPTIONS, GET, POST\r\n"));
  this->client->print(F("Server: ESP8266 webcc, firmware v. "));
  this->client->print(FIRMWARE_VERSION_MAJOR);
  this->client->print(F("."));
  this->client->print(FIRMWARE_VERSION_MINOR);
  this->client->print(F("\r\n"));
  this->client->print(F("Content-Type: text/html\r\n\r\n"));
}


void WebConfigReqPartHandler::finish(void) {
  if (this->reqPath == HTTPRequestPath::SETCONFIG) {
    if (this->configUpdated) saveConfig();
    loadConfig();
    this->sendHttpStatus(HTTPStatusCode::SEE_OTHER);
    this->redirectToRoot();
    return;
  }
  this->sendHttpStatus(this->parser->getStatusCode());
  if (this->parser->getStatusCode() == HTTPStatusCode::OK) {
    if (this->getMethod() == HTTPRequestMethod::OPTIONS) this->sendOptionsPage();
    else
      this->sendConfigPage();
  }
}

HTTPRequestMethod WebConfigReqPartHandler::getMethod(void) {
  return (this->reqMethod);
}

HTTPRequestPath WebConfigReqPartHandler::getPath(void) {
  return (this->reqPath);
}

//////////////////////////////////////////////////////////////////////
// Globals
//////////////////////////////////////////////////////////////////////

const unsigned int WEB_SERVER_PORT = 80;
WiFiServer webServer(WEB_SERVER_PORT);

HTTPReqParserStateMachine webConfigParser;
WebConfigReqPartHandler webConfigHandler;
WebConfigControl webConfig(webServer, webConfigParser, webConfigHandler);

//////////////////////////////////////////////////////////////////////
// WebConfig functions
//////////////////////////////////////////////////////////////////////


void webConfigBegin(void) {
  webServer.begin();
}

void webConfigRun(void) {
  webConfig.execute();
}
