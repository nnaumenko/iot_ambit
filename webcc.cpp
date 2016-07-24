/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include "diag.h"
#include "version.h"

#include "eeprom_config.h"

#include "webcc.h"
#include "webcc_html.h"
#include "uitexts.h"


const unsigned int WEB_SERVER_PORT = 80;
WiFiServer webServer(WEB_SERVER_PORT);

enum class HTTPRequestMethod {
  UNKNOWN,
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
  UNKNOWN,
  ASTERISK,
  ROOT,
};

const char PROGMEM stringMapHTTPRequestPath_Asterisk[] = "*";
const char PROGMEM stringMapHTTPRequestPath_Root[] = "/";

const PROGMEM ProgmemStringMapItem stringMapHTTPRequestPath [] = {
  {(StringMapKey)HTTPRequestPath::ASTERISK, stringMapHTTPRequestPath_Asterisk},
  {(StringMapKey)HTTPRequestPath::ROOT,     stringMapHTTPRequestPath_Root},
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
    virtual void execute(char * value, HTTPRequestPart part);
    void finish(void);
  public:
    HTTPRequestMethod getMethod(void);
    HTTPRequestPath getPath(void);
  private:
    HTTPRequestMethod reqMethod = HTTPRequestMethod::UNKNOWN;
    HTTPRequestPath reqPath = HTTPRequestPath::UNKNOWN;
  private:
    void setParameter(EepromSavedParameter parameterId, char * value);
    EepromSavedParameter lastParameterName;
    boolean configUpdated = false;

};

void WebConfigReqPartHandler::execute(char * value, HTTPRequestPart part) {
  if (part == HTTPRequestPart::METHOD) {
    ProgmemStringMap methodFinder (stringMapHTTPRequestMethod);
    this->reqMethod = (HTTPRequestMethod)methodFinder.find(value);
    DiagLog.print(F("Method: "));
    methodFinder.print(DiagLog, (StringMapKey)reqMethod);
    DiagLog.println();
    if (reqMethod == HTTPRequestMethod::OPTIONS) return;
    if (reqMethod == HTTPRequestMethod::GET) return;
    if (reqMethod == HTTPRequestMethod::POST) return;
    this->parser->setError(HTTPStatusCode::NOT_IMPLEMENTED);
  }

  if (part == HTTPRequestPart::PATH) {
    ProgmemStringMap pathFinder (stringMapHTTPRequestPath);
    this->reqPath = (HTTPRequestPath)pathFinder.find(value);
    DiagLog.print(F("Path: "));
    pathFinder.print(DiagLog, (StringMapKey)this->reqPath);
    DiagLog.println();
    if (this->reqMethod == HTTPRequestMethod::OPTIONS) {
      if (this->reqPath == HTTPRequestPath::ASTERISK) return;
      if (this->reqPath == HTTPRequestPath::ROOT) return;
      this->parser->setError(HTTPStatusCode::NOT_FOUND);
    }
    if (this->reqMethod == HTTPRequestMethod::GET) {
      if (this->reqPath == HTTPRequestPath::ROOT) return;
      this->parser->setError(HTTPStatusCode::NOT_FOUND);
    }
    if (this->reqMethod == HTTPRequestMethod::POST) {
      if (this->reqPath == HTTPRequestPath::ROOT) return;
      this->parser->setError(HTTPStatusCode::NOT_FOUND);
    }
  }

  if ((part == HTTPRequestPart::GET_QUERY_NAME) ||
      (part == HTTPRequestPart::POST_QUERY_NAME)) {
    QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
    lastParameterName = (EepromSavedParameter)parameterNameFinder.find(value);
    DiagLog.print(F("Parameter: "));
    parameterNameFinder.print(DiagLog, (long)lastParameterName);
    DiagLog.println();
  }

  if ((part == HTTPRequestPart::GET_QUERY_VALUE) ||
      (part == HTTPRequestPart::POST_QUERY_VALUE)) {
    DiagLog.print(F("Value: "));
    DiagLog.println(value);
    if (lastParameterName != EepromSavedParameter::UNKNOWN)
      this->setParameter(lastParameterName, value);
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
    case EepromSavedParameter::MG811FILTER_TYPE:
      eepromSavedParametersStorage.filterMG811 = (ADCFilter)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811FILTER_LOWPASS_FREQ:
      eepromSavedParametersStorage.filterMG811LowPassFrequency = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_REJECT_CALIBRATION:
      eepromSavedParametersStorage.rejectCalibrationMG811 = (byte)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MISC_SERIAL_OUT:
      eepromSavedParametersStorage.sensorSerialOutput = (byte)valueAsNumber;
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

  }
}

void WebConfigReqPartHandler::finish(void) {
  if (this->configUpdated) {
    saveConfig();
    loadConfig();
    DiagLog.print(F("["));
    DiagLog.print(millis());
    DiagLog.println(F("] Config updated in EEPROM."));
  }
}

HTTPRequestMethod WebConfigReqPartHandler::getMethod(void) {
  return (this->reqMethod);
}

HTTPRequestPath WebConfigReqPartHandler::getPath(void) {
  return (this->reqPath);
}

void webConfigBegin(void) {
  webServer.begin();
}

void webConfigSendHttpStatus(Stream &client, HTTPStatusCode statusCode) {
  ProgmemStringMap statusCodeText (stringMapHTTPStatusCode);
  client.print(F("HTTP/1.1 "));
  client.println((int)statusCode, DEC);
  statusCodeText.print(client, (int)statusCode);

  DiagLog.print(F("HTTP status code: "));
  DiagLog.print((int)statusCode, DEC);
  DiagLog.print(F(" "));
  statusCodeText.print(DiagLog, (int)statusCode);
  DiagLog.println();

}

void webConfigSendConfigPage(Stream &client) {
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Sending config webpage."));

  client.print(F("Content-Type: text/html\r\nCache-control: no-cache\r\nConnection: close\r\n\r\n"));

  WebccHTML webConfigPage(client);
  QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);//code below assumes this StringMap only handles PROGMEM strings

  webConfigPage.bodyBegin();
  //Code below assumes that StringMap.find always returns PROGMEM string
  webConfigPage.sectionBegin(webccUIText_SectionDisplayName_Connection);
  webConfigPage.subsectionBegin(webccUIText_SubsectionDisplayName_WiFi);
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::WIFI_SSID),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::WIFI_SSID),
                              eepromSavedParametersStorage.wifiSsid,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::WIFI_SSID));
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::WIFI_PASSWORD),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::WIFI_PASSWORD),
                              eepromSavedParametersStorage.wifiPassword,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::WIFI_PASSWORD));
  webConfigPage.subsectionBegin(webccUIText_SubsectionDisplayName_Blynk);
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN),
                              eepromSavedParametersStorage.authToken,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN));
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER),
                              eepromSavedParametersStorage.blynkServer,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER));
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT),
                              eepromSavedParametersStorage.blynkServerPort,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT));
  webConfigPage.sectionBegin(webccUIText_SectionDisplayName_ADC);
  webConfigPage.subsectionBegin(webccUIText_SubsectionDisplayName_ADCCalibration);
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL),
                              (long)eepromSavedParametersStorage.MG811CalPoint0Calibrated,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL));
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW),
                              (long)eepromSavedParametersStorage.MG811CalPoint0Raw,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW));
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL),
                              (long)eepromSavedParametersStorage.MG811CalPoint1Calibrated,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL));
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW),
                              (long)eepromSavedParametersStorage.MG811CalPoint1Raw,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW));
  webConfigPage.selectParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION),
                                parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION),
                                webccUISelectEnableDisable,
                                (int)eepromSavedParametersStorage.rejectCalibrationMG811,
                                webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION));
  webConfigPage.subsectionBegin(webccUIText_SubsectionDisplayName_Filter);
  webConfigPage.selectParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MG811FILTER_TYPE),
                                parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811FILTER_TYPE),
                                webccUISelectADCFilter,
                                (unsigned int)eepromSavedParametersStorage.filterMG811,
                                webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MG811FILTER_TYPE));
  webConfigPage.textParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ),
                              parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ),
                              eepromSavedParametersStorage.filterMG811LowPassFrequency,
                              webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ));
  webConfigPage.sectionBegin(webccUIText_SectionDisplayName_Misc);
  webConfigPage.subsectionBegin(webccUIText_SubsectionDisplayName_SerialPort);
  webConfigPage.selectParameter(webccUIParameterDisplayNames.find((StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT),
                                parameterNameFinder.find((StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT),
                                webccUISelectEnableDisable,
                                (int)eepromSavedParametersStorage.sensorSerialOutput,
                                webccUIParameterTooltips.find((StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT));
  webConfigPage.bodyEnd();

  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Config webpage sent to client."));
}

void webConfigSendOptionsPage(Print &client) {
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Sending options page."));

  client.print(F("Allow: OPTIONS, GET, POST\r\n"));
  client.print(F("Server: ESP8266 WebConfig, firmware v. "));
  client.print(FIRMWARE_VERSION_MAJOR);
  client.print(F("."));
  client.print(FIRMWARE_VERSION_MINOR);
  client.print(F("\r\n"));
  client.print(F("Content-Type: text/html\r\n\r\n"));
};

void webConfigRun(void) {
  // Check if a client has connected
  WiFiClient primaryClient = webServer.available();
  if (!primaryClient) {
    return;
  }
  // Wait until the client sends some data
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] web client connected"));
  while (!primaryClient.available()) {
    delay(1);
    yield();
  }
  HTTPStream client(primaryClient);
  HTTPReqParser parser;
  WebConfigReqPartHandler handler;
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] begin parsing."));
  parser.begin(client);
  parser.setHandler(handler);
  do {
    parser.parse();
    yield();
  } while (!parser.finished());
  handler.finish();
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] end parsing."));
  DiagLog.print(F("Result: "));
  if (parser.isError()) DiagLog.println(F("error")); else DiagLog.println(F("OK"));
  webConfigSendHttpStatus(client, parser.getStatusCode());
  if (parser.getStatusCode() == HTTPStatusCode::OK) {
    if (handler.getMethod() == HTTPRequestMethod::OPTIONS) webConfigSendOptionsPage(client);
    else
      webConfigSendConfigPage(client);
  }
  client.flush();
}
