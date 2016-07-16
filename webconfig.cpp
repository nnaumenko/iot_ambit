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

#include "eeprom_config.h"

#include "webconfig.h"
#include "web_content.h"

const unsigned int WEB_SERVER_PORT = 80;
WiFiServer webServer(WEB_SERVER_PORT);

enum class HttpRequestMethod {
  UNKNOWN = STRINGMAP_ITEM_DEFAULT,
  OPTIONS,
  GET,
  POST,
};

const PROGMEM QuickStringMapItem stringMapHttpRequestMethod [] = {
  {(StringMapKey)HttpRequestMethod::OPTIONS, "OPTIONS"},
  {(StringMapKey)HttpRequestMethod::GET, "GET"},
  {(StringMapKey)HttpRequestMethod::POST, "POST"},
  {(StringMapKey)HttpRequestMethod::UNKNOWN}
};

enum class HttpRequestPath {
  UNKNOWN = STRINGMAP_ITEM_DEFAULT,
  ASTERISK,
  ROOT,
};

const PROGMEM QuickStringMapItem stringMapHttpRequestPath [] = {
  {(StringMapKey)HttpRequestPath::ASTERISK, "*"},
  {(StringMapKey)HttpRequestPath::ROOT, "/"},
  {(StringMapKey)HttpRequestPath::UNKNOWN}
};

const PROGMEM QuickStringMapItem stringMapHttpStatusCode [] = {
  {(StringMapKey)HttpStatusCode::CONTINUE, "Continue"},
  {(StringMapKey)HttpStatusCode::OK, "OK"},
  {(StringMapKey)HttpStatusCode::SEE_OTHER, "See Other"},
  {(StringMapKey)HttpStatusCode::BAD_REQUEST, "Bad Request"},
  {(StringMapKey)HttpStatusCode::NOT_FOUND, "Not Found"},
  {(StringMapKey)HttpStatusCode::METHOD_NOT_ALLOWED, "Method Not Allowed"},
  {(StringMapKey)HttpStatusCode::REQUEST_URI_TOO_LONG, "Request-URI Too Long"},
  {(StringMapKey)HttpStatusCode::INTERNAL_SERVER_ERROR, "Internal Server Error"},
  {(StringMapKey)HttpStatusCode::NOT_IMPLEMENTED, "Not Implemented"},
  {(StringMapKey)HttpStatusCode::UNKNOWN}
};

class WebConfigReqPartHandler : public HttpReqPartHandler {
  public:
    virtual void execute(char * value, HttpRequestPart part);
    void finish(void);
  public:
    HttpRequestMethod getMethod(void);
    HttpRequestPath getPath(void);
  private:
    HttpRequestMethod reqMethod = HttpRequestMethod::UNKNOWN;
    HttpRequestPath reqPath = HttpRequestPath::UNKNOWN;
  private:
    void setParameter(EepromSavedParameter parameterId, char * value);
    EepromSavedParameter lastParameterName;
    boolean configUpdated = false;

};

void WebConfigReqPartHandler::execute(char * value, HttpRequestPart part) {
  if (part == HttpRequestPart::METHOD) {
    QuickStringMap methodFinder (stringMapHttpRequestMethod);
    this->reqMethod = (HttpRequestMethod)methodFinder.find(value);
    DiagLog.print(F("Method: "));
    methodFinder.print(DiagLog, (StringMapKey)reqMethod);
    DiagLog.println();
    if (reqMethod == HttpRequestMethod::OPTIONS) return;
    if (reqMethod == HttpRequestMethod::GET) return;
    if (reqMethod == HttpRequestMethod::POST) return;
    this->parser->setError(HttpStatusCode::NOT_IMPLEMENTED);
  }

  if (part == HttpRequestPart::PATH) {
    QuickStringMap pathFinder (stringMapHttpRequestPath);
    this->reqPath = (HttpRequestPath)pathFinder.find(value);
    DiagLog.print(F("Path: "));
    pathFinder.print(DiagLog, (StringMapKey)this->reqPath);
    DiagLog.println();
    if (this->reqMethod == HttpRequestMethod::OPTIONS) {
      if (this->reqPath == HttpRequestPath::ASTERISK) return;
      if (this->reqPath == HttpRequestPath::ROOT) return;
      this->parser->setError(HttpStatusCode::NOT_FOUND);
    }
    if (this->reqMethod == HttpRequestMethod::GET) {
      if (this->reqPath == HttpRequestPath::ROOT) return;
      this->parser->setError(HttpStatusCode::NOT_FOUND);
    }
    if (this->reqMethod == HttpRequestMethod::POST) {
      if (this->reqPath == HttpRequestPath::ROOT) return;
      this->parser->setError(HttpStatusCode::NOT_FOUND);
    }
  }

  if ((part == HttpRequestPart::GET_QUERY_NAME) ||
      (part == HttpRequestPart::POST_QUERY_NAME)) {
    QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);
    lastParameterName = (EepromSavedParameter)parameterNameFinder.find(value);
    DiagLog.print(F("Parameter: "));
    parameterNameFinder.print(DiagLog, (int)lastParameterName);
    DiagLog.println();
  }

  if ((part == HttpRequestPart::GET_QUERY_VALUE) ||
      (part == HttpRequestPart::POST_QUERY_VALUE)) {
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
      eepromSavedParametersStorage.filterMG811 = (MG811Filter)valueAsNumber;
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

HttpRequestMethod WebConfigReqPartHandler::getMethod(void) {
  return (this->reqMethod);
}

HttpRequestPath WebConfigReqPartHandler::getPath(void) {
  return (this->reqPath);
}

void webConfigBegin(void) {
  webServer.begin();
}

void webConfigSendHttpStatus(Stream &client, HttpStatusCode statusCode) {
  QuickStringMap statusCodeText (stringMapHttpStatusCode);
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

  HtmlPage webConfigPage(client);
  QuickStringMap parameterNameFinder (stringMapEepromSavedParameterInternalNames);//code below assumes this StringMap only handles PROGMEM strings

  webConfigPage.bodyBegin();

  webConfigPage.sectionBegin(F("Connection"));
  webConfigPage.subsectionBegin(F("WiFi"));
  webConfigPage.textParameter(F("SSID"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::WIFI_SSID),
                              eepromSavedParametersStorage.wifiSsid);
  webConfigPage.textParameter(F("Password"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::WIFI_PASSWORD),
                              eepromSavedParametersStorage.wifiPassword,
                              F("Leave blank if connecting to open WiFi network"));
  webConfigPage.subsectionEnd();
  webConfigPage.subsectionBegin(F("Blynk"));
  webConfigPage.textParameter(F("Auth token"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN),
                              eepromSavedParametersStorage.authToken,
                              F("Auth token is provided by Blynk app"));
  webConfigPage.textParameter(F("Blynk server"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER),
                              eepromSavedParametersStorage.blynkServer,
                              F("Use Blynk Cloud (blynk-cloud.com) or private server"));
  webConfigPage.textParameter(F("Blynk port"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT),
                              eepromSavedParametersStorage.blynkServerPort,
                              F("Default port is 8442"));
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.sectionBegin(F("MG811"));
  webConfigPage.subsectionBegin(F("Calibration points"));
  webConfigPage.textParameter(F("Point 0: ppm value"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL),
                              (long)eepromSavedParametersStorage.MG811CalPoint0Calibrated);
  webConfigPage.textParameter(F("Point 0: raw value"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW),
                              (long)eepromSavedParametersStorage.MG811CalPoint0Raw);
  webConfigPage.textParameter(F("Point 1: ppm value"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL),
                              (long)eepromSavedParametersStorage.MG811CalPoint1Calibrated);
  webConfigPage.textParameter(F("Point 1: raw value"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW),
                              (long)eepromSavedParametersStorage.MG811CalPoint1Raw);
  webConfigPage.selectParameterBegin(F("Reject calibration data"),
                                     (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION),
                                     F("No ppm value calculation possible. Instead of ppm value, "
                                       "inverted raw ADC value is used. Enable this option if you would like to view trend only."));
  webConfigPage.selectParameterOption(F("Enable"), 1, (int)eepromSavedParametersStorage.rejectCalibrationMG811);
  webConfigPage.selectParameterOption(F("Disable"), 0, (int)eepromSavedParametersStorage.rejectCalibrationMG811);
  webConfigPage.selectParameterEnd();
  webConfigPage.subsectionEnd();
  webConfigPage.subsectionBegin(F("Filtering"));
  webConfigPage.selectParameterBegin(F("Filter"),
                                     (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811FILTER_TYPE));
  webConfigPage.selectParameterOption(F("None"), 0, (unsigned int)eepromSavedParametersStorage.filterMG811);
  webConfigPage.selectParameterOption(F("Averaging"), 1, (unsigned int)eepromSavedParametersStorage.filterMG811);
  webConfigPage.selectParameterOption(F("Low-pass"), 2, (unsigned int)eepromSavedParametersStorage.filterMG811);
  webConfigPage.selectParameterEnd();
  webConfigPage.textParameter(F("Low-pass filter frequency"),
                              (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ),
                              eepromSavedParametersStorage.filterMG811LowPassFrequency,
                              F("x 0.01 Hz"));
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.sectionBegin(F("Misc"));
  webConfigPage.subsectionBegin(F("Serial port"));
  webConfigPage.selectParameterBegin(F("Data values serial output"),
                                     (__FlashStringHelper *)parameterNameFinder.find((StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT));
  webConfigPage.selectParameterOption(F("Enable"), 1, (int)eepromSavedParametersStorage.sensorSerialOutput);
  webConfigPage.selectParameterOption(F("Disable"), 0, (int)eepromSavedParametersStorage.sensorSerialOutput);
  webConfigPage.selectParameterEnd();
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

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
  client.print(F("Server: ESP8266\r\n"));
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
  HttpStream client(primaryClient);
  HttpReqParser parser;
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
  if (parser.getStatusCode() == HttpStatusCode::OK) {
    if (handler.getMethod() == HttpRequestMethod::OPTIONS) webConfigSendOptionsPage(client);
    else
      webConfigSendConfigPage(client);
  }
  client.flush();
}

