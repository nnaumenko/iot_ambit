/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include "eeprom_config.h"

#include "webconfig.h"
#include "web_content.h"

const unsigned int WEB_SERVER_PORT = 80;
WiFiServer webServer(WEB_SERVER_PORT);

const char PROGMEM webServerParameterInternalNameWifiSsid[] = "wifissid";
const char PROGMEM webServerParameterInternalNameWifiPassword[] = "wifipass";
const char PROGMEM webServerParameterInternalNameBlynkAuth[] = "blynkauth";
const char PROGMEM webServerParameterInternalNameAdcCalPoint0Calibrated[] = "adccal0cal";
const char PROGMEM webServerParameterInternalNameAdcCalPoint1Calibrated[] = "adccal1cal";
const char PROGMEM webServerParameterInternalNameAdcCalPoint0Raw[] = "adccal0raw";
const char PROGMEM webServerParameterInternalNameAdcCalPoint1Raw[] = "adccal1raw";
const char PROGMEM webServerParameterInternalNameAdcRejectCal[] = "adcrejectcal";
const char PROGMEM webServerParameterInternalNameAdcFilter[] = "adcfilter";
const char PROGMEM webServerParameterInternalNameAdcFilterFreq[] = "adcfiltfreq";
const char PROGMEM webServerParameterInternalNameMiscSensorSerialOutput[] = "miscserout";

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
  private:
    HttpRequestMethod reqMethod = HttpRequestMethod::UNKNOWN;
    HttpRequestPath reqPath = HttpRequestPath::UNKNOWN;
    EepromSavedParameter lastParameterName;
  private:
    void setParameter(EepromSavedParameter parameterId, char * value);
    boolean configUpdated = false;

};

void WebConfigReqPartHandler::execute(char * value, HttpRequestPart part) {
  if (part == HttpRequestPart::METHOD) {
    QuickStringMap methodFinder (stringMapHttpRequestMethod);
    this->reqMethod = (HttpRequestMethod)methodFinder.find(value);
    Serial.print(F("Method: "));
    methodFinder.print(Serial, (StringMapKey)reqMethod);
    Serial.println();
    if (reqMethod == HttpRequestMethod::OPTIONS) return;
    if (reqMethod == HttpRequestMethod::GET) return;
    if (reqMethod == HttpRequestMethod::POST) return;
    this->parser->setError(HttpStatusCode::NOT_IMPLEMENTED);
  }

  if (part == HttpRequestPart::PATH) {
    QuickStringMap pathFinder (stringMapHttpRequestPath);
    this->reqPath = (HttpRequestPath)pathFinder.find(value);
    Serial.print(F("Path: "));
    pathFinder.print(Serial, (StringMapKey)this->reqPath);
    Serial.println();
    if (this->reqMethod == HttpRequestMethod::OPTIONS) {
      if (this->reqPath == HttpRequestPath::ASTERISK) return;
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
    Serial.print(F("Parameter: "));
    parameterNameFinder.print(Serial, (int)lastParameterName);
    Serial.println();
  }

  if ((part == HttpRequestPart::GET_QUERY_VALUE) ||
      (part == HttpRequestPart::POST_QUERY_VALUE)) {
    Serial.print(F("Value: "));
    Serial.println(value);
    if (lastParameterName != EepromSavedParameter::UNKNOWN)
      this->setParameter(lastParameterName, value);
  }
}

void WebConfigReqPartHandler::setParameter(EepromSavedParameter parameterId, char * value) {
  float valueAsNumber = atof (value);
  switch (parameterId) {
    case EepromSavedParameter::BLYNK_AUTH_TOKEN:
      Serial.print(F("Setting auth token to "));
      Serial.println(value);
      strncpy(eepromSavedParametersStorage.authToken, value, sizeof(eepromSavedParametersStorage.authToken) - 1);
      eepromSavedParametersStorage.authToken[sizeof(eepromSavedParametersStorage.authToken) - 1] = 0;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::WIFI_SSID:
      Serial.print(F("Setting SSID to "));
      Serial.println(value);
      strncpy(eepromSavedParametersStorage.wifiSsid, value, sizeof(eepromSavedParametersStorage.wifiSsid) - 1);
      eepromSavedParametersStorage.wifiSsid[sizeof(eepromSavedParametersStorage.wifiSsid) - 1] = 0;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::WIFI_PASSWORD:
      Serial.print(F("Setting password to "));
      Serial.println(value);
      strncpy(eepromSavedParametersStorage.wifiPassword, value, sizeof(eepromSavedParametersStorage.wifiPassword) - 1);
      eepromSavedParametersStorage.wifiPassword[sizeof(eepromSavedParametersStorage.wifiPassword) - 1] = 0;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT0_CAL:
      Serial.print(F("Setting MG811 calibration point 0 PPM value to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.MG811CalPoint0Calibrated = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT0_RAW:
      Serial.print(F("Setting MG811 calibration point 0 ADC value to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.MG811CalPoint0Raw = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT1_CAL:
      Serial.print(F("Setting MG811 calibration point 1 PPM value to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.MG811CalPoint1Calibrated = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_CALPOINT1_RAW:
      Serial.print(F("Setting MG811 calibration point 1 ADC value to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.MG811CalPoint1Raw = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811FILTER_TYPE:
      Serial.print(F("Setting MG811 filter to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.filterMG811 = (MG811Filter)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811FILTER_LOWPASS_FREQ:
      Serial.print(F("Setting MG811 low-pass frequency to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.filterMG811LowPassFrequency = (unsigned int)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_REJECT_CALIBRATION:
      Serial.print(F("Setting MG811 calibration reject to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.rejectCalibrationMG811 = (byte)valueAsNumber;
      this->configUpdated = true;
      break;
    case EepromSavedParameter::MG811_SERIAL_OUT:
      Serial.print(F("Setting sensor readings' serial output to "));
      Serial.println(valueAsNumber);
      eepromSavedParametersStorage.sensorSerialOutput = (byte)valueAsNumber;
      this->configUpdated = true;
      break;
  }
}

void WebConfigReqPartHandler::finish(void) {
  if (this->configUpdated) {
    saveConfig();
    loadConfig();
    Serial.print(F("["));
    Serial.print(millis());
    Serial.println(F("] Config updated in EEPROM."));
  }
}

void webConfigBegin(void) {
  webServer.begin();
}

void webConfigSendHttpStatus(Stream &client, HttpStatusCode statusCode) {
  QuickStringMap statusCodeText (stringMapHttpStatusCode);
  client.print(F("HTTP/1.1 "));
  client.println((int)statusCode, DEC);
  statusCodeText.print(client, (int)statusCode);

  Serial.print(F("HTTP status code: "));
  Serial.print((int)statusCode, DEC);
  Serial.print(F(" "));
  statusCodeText.print(Serial, (int)statusCode);
  Serial.println();

}

void webConfigSendConfigPage(Stream &client) {
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Sending config webpage."));

  client.print(F("Content-Type: text/html\r\nCache-control: no-cache\r\nConnection: close\r\n\r\n"));

  HtmlPage webConfigPage(client);

  webConfigPage.bodyBegin();

  webConfigPage.sectionBegin(F("Connection"));
  webConfigPage.subsectionBegin(F("WiFi"));
  webConfigPage.textParameter(F("SSID"),
                              (__FlashStringHelper *)webServerParameterInternalNameWifiSsid,
                              eepromSavedParametersStorage.wifiSsid);
  webConfigPage.textParameter(F("Password"),
                              (__FlashStringHelper *)webServerParameterInternalNameWifiPassword,
                              eepromSavedParametersStorage.wifiPassword,
                              F("Leave blank if connecting to open WiFi network"));
  webConfigPage.subsectionEnd();
  webConfigPage.subsectionBegin(F("Blynk"));
  webConfigPage.textParameter(F("Auth token"),
                              (__FlashStringHelper *)webServerParameterInternalNameBlynkAuth,
                              eepromSavedParametersStorage.authToken,
                              F("Auth token is provided by Blynk app"));
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.sectionBegin(F("MG811"));
  webConfigPage.subsectionBegin(F("Calibration points"));
  webConfigPage.textParameter(F("Point 0: ppm value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint0Calibrated,
                              (long)eepromSavedParametersStorage.MG811CalPoint0Calibrated);
  webConfigPage.textParameter(F("Point 0: raw value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint0Raw,
                              (long)eepromSavedParametersStorage.MG811CalPoint0Raw);
  webConfigPage.textParameter(F("Point 1: ppm value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint1Calibrated,
                              (long)eepromSavedParametersStorage.MG811CalPoint1Calibrated);
  webConfigPage.textParameter(F("Point 1: raw value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint1Raw,
                              (long)eepromSavedParametersStorage.MG811CalPoint1Raw);
  webConfigPage.selectParameterBegin(F("Reject calibration data"),
                                     (__FlashStringHelper *)webServerParameterInternalNameAdcRejectCal,
                                     F("No ppm value calculation possible. Instead of ppm value, "
                                       "inverted raw ADC value is used. Enable this option if you would like to view trend only."));
  webConfigPage.selectParameterOption(F("Enable"), 1, (int)eepromSavedParametersStorage.rejectCalibrationMG811);
  webConfigPage.selectParameterOption(F("Disable"), 0, (int)eepromSavedParametersStorage.rejectCalibrationMG811);
  webConfigPage.selectParameterEnd();
  webConfigPage.subsectionEnd();
  webConfigPage.subsectionBegin(F("Filtering"));
  webConfigPage.selectParameterBegin(F("Filter"),
                                     (__FlashStringHelper *)webServerParameterInternalNameAdcFilter);
  webConfigPage.selectParameterOption(F("None"), 0, (unsigned int)eepromSavedParametersStorage.filterMG811);
  webConfigPage.selectParameterOption(F("Averaging"), 1, (unsigned int)eepromSavedParametersStorage.filterMG811);
  webConfigPage.selectParameterOption(F("Low-pass"), 2, (unsigned int)eepromSavedParametersStorage.filterMG811);
  webConfigPage.selectParameterEnd();
  webConfigPage.textParameter(F("Low-pass filter frequency"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcFilterFreq,
                              eepromSavedParametersStorage.filterMG811LowPassFrequency,
                              F("x 0.01 Hz"));
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.sectionBegin(F("Misc"));
  webConfigPage.subsectionBegin(F("Serial port"));
  webConfigPage.selectParameterBegin(F("Data values serial output"),
                                     (__FlashStringHelper *)webServerParameterInternalNameMiscSensorSerialOutput);
  webConfigPage.selectParameterOption(F("Enable"), 1, (int)eepromSavedParametersStorage.sensorSerialOutput);
  webConfigPage.selectParameterOption(F("Disable"), 0, (int)eepromSavedParametersStorage.sensorSerialOutput);
  webConfigPage.selectParameterEnd();
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.bodyEnd();

  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config webpage sent to client."));
}

void webConfigRun(void) {
  // Check if a client has connected
  WiFiClient primaryClient = webServer.available();
  if (!primaryClient) {
    return;
  }
  // Wait until the client sends some data
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] web client connected"));
  while (!primaryClient.available()) {
    delay(1);
    yield();
  }
  HttpStream client(primaryClient);
  HttpReqParser parser;
  WebConfigReqPartHandler handler;
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] begin parsing."));
  parser.begin(client);
  parser.setHandler(handler);
  do {
    parser.parse();
    yield();
  } while (!parser.finished());
  handler.finish();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] end parsing."));
  Serial.print(F("Result: "));
  if (parser.isError()) Serial.println(F("error")); else Serial.println(F("OK"));
  webConfigSendHttpStatus(client, parser.getStatusCode());
  if (parser.getStatusCode() == HttpStatusCode::OK) {
    webConfigSendConfigPage(client);
  }
  client.flush();
}

