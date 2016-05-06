/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <ESP8266WiFi.h>

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

// class BufferedPrint

BufferedPrint::BufferedPrint(Print * destination) {
  this->destination = destination;
}

BufferedPrint::~BufferedPrint() {
  this->printBuffer();
}

size_t BufferedPrint::write(uint8_t character) {
  buffer[bufferPosition++] = (char)character;
  if (bufferPosition >= BUFFERED_PRINT_SIZE) printBuffer();
  return (sizeof(character));
}

void BufferedPrint::printBuffer(void) {
  destination->write((char *)this->buffer, bufferPosition);
  bufferPosition = 0;
}

// class WebPage

void WebPage::bodyBegin(void) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigBodyBegin);
}

void WebPage::bodyEnd(void) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigBodyEnd);
}

void WebPage::sectionBegin(const __FlashStringHelper * displayName) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigSectionBegin1);
  this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigSectionBegin2);
}

void WebPage::sectionEnd(void) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigSave);
  this->client->print((__FlashStringHelper *)htmlConfigSectionEnd);
}

void WebPage::subsectionBegin(const __FlashStringHelper * displayName) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigSubsectionBegin1);
  this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigSubsectionBegin2);
}

void WebPage::subsectionEnd(void) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigSubsectionEnd);
}

void WebPage::textParameter(const __FlashStringHelper * displayName,
                            const __FlashStringHelper * internalName,
                            char * value,
                            const __FlashStringHelper * tooltipText) {
  if (this->client == NULL) return;
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart1);
  this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart2);
  if (tooltipText != NULL) {
    this->client->print((__FlashStringHelper *)htmlConfigTooltipBegin);
    this->client->print(tooltipText);
    this->client->print((__FlashStringHelper *)htmlConfigTooltipEnd);
  }
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart3);
  this->client->print(internalName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart4);
  if (value != NULL) {
    this->client->print(value);
  }
  this->client->print((__FlashStringHelper *)htmlConfigParameterTextPart5);
}

void WebPage::textParameter(const __FlashStringHelper * displayName,
                            const __FlashStringHelper * internalName,
                            long value,
                            const __FlashStringHelper * tooltipText) {
  const int maxCharsPerLongValue = 12; //max 10 digits + optional minus sign + trailing zero
  const int decimalRadix = 10;
  char valueAsText[maxCharsPerLongValue] = {0};
  char * valueAsText2 = ltoa (value, valueAsText, decimalRadix);
  this->textParameter(displayName, internalName, valueAsText2, tooltipText);
}

void WebPage::selectParameterBegin(const __FlashStringHelper * displayName,
                                   const __FlashStringHelper * internalName,
                                   const __FlashStringHelper * tooltipText) {
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart1);
  this->client->print(displayName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart2);
  if (tooltipText != NULL) {
    this->client->print((__FlashStringHelper *)htmlConfigTooltipBegin);
    this->client->print(tooltipText);
    this->client->print((__FlashStringHelper *)htmlConfigTooltipEnd);
  }
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart3);
  this->client->print(internalName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart4);
}

void WebPage::selectParameterEnd(void) {
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectPart5);
}

void WebPage::selectParameterOption(const __FlashStringHelper * optionDisplayName, int optionValue, int actualValue) {
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart1);
  this->client->print(optionValue, DEC);
  if (optionValue == actualValue)
    this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart2Selected);
  else
    this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart2);
  this->client->print(optionDisplayName);
  this->client->print((__FlashStringHelper *)htmlConfigParameterSelectOptionPart3);

}

//misc
/*
void webServerPrintProgmem(WiFiClient &client, const char * progmemBuffer) {
  size_t progmemBufferSize = strlen_P(progmemBuffer);
  client.write_P(progmemBuffer, progmemBufferSize); //write_P is not cross-platform
}
*/
int compareWithProgmem(char * ramBuffer, const char * progmemBuffer) {
  size_t tempBufferSize = strlen_P(progmemBuffer);
  char tempBuffer[tempBufferSize + 1]; //strlen does not include trailing zero
  strncpy_P(tempBuffer, progmemBuffer, tempBufferSize);
  tempBuffer[tempBufferSize] = 0;
  return (strcmp(ramBuffer, tempBuffer));
}

int hexDigitToInt(char hexDigit) {
  if ((hexDigit >= '0') && (hexDigit <= '9')) return (hexDigit - '0');
  if ((hexDigit >= 'A') && (hexDigit <= 'F')) return (hexDigit - 'A' + 10);
  if ((hexDigit >= 'a') && (hexDigit <= 'f')) return (hexDigit - 'a' + 10);
  return (0);
}

byte hexToByte(const char * hexValue) {
  if (!hexValue[0]) return (0);
  if (!hexValue[1]) return (0);
  const int RADIX_HEXADECIMAL = 16;
  int mostSignificant = hexDigitToInt(hexValue[0]) * RADIX_HEXADECIMAL;
  int leastSignificant = hexDigitToInt(hexValue[1]);
  return ((byte)(mostSignificant + leastSignificant));
}

  const size_t querySize = query.length() + 1;
  char queryBuffer[querySize + 1];
  query.toCharArray(queryBuffer, querySize);
  queryBuffer[querySize] = 0;

  const size_t STRING_BUFFERS_SIZE = 32;
  char parameterBuffer[STRING_BUFFERS_SIZE + 1];
  char valueBuffer[STRING_BUFFERS_SIZE + 1];
  char valueBufferNoEscape[STRING_BUFFERS_SIZE + 1];

  boolean configUpdated = false;

  char * currentParameter = queryBuffer;
  do {
    const char CHAR_PARAMETER_SEPARATOR = '&';
    const char CHAR_VALUE_SEPARATOR = '=';
    const char CHAR_ESCAPE = '%';

    //get parameter name to parameterBuffer
    char * currentValue = strchr(currentParameter, CHAR_VALUE_SEPARATOR);
    if (currentValue == NULL) break;
    memset(parameterBuffer, 0, STRING_BUFFERS_SIZE + 1); //strncpy does not add trailing zero so entire buffer is filled with zeroes
    strncpy(parameterBuffer, currentParameter, (currentValue - currentParameter) < STRING_BUFFERS_SIZE ? currentValue - currentParameter : STRING_BUFFERS_SIZE);

    //get parameter value (in char * form) to valueBuffer
    currentParameter = strchr(currentValue, CHAR_PARAMETER_SEPARATOR);
    memset(valueBuffer, 0, STRING_BUFFERS_SIZE + 1); //strncpy does not add trailing zero so entire buffer is filled with zeroes
    currentValue++;
    if (currentParameter == NULL) strncpy(valueBuffer, currentValue, STRING_BUFFERS_SIZE);
    else strncpy(valueBuffer, currentValue, (currentParameter - currentValue) < STRING_BUFFERS_SIZE ? currentParameter - currentValue : STRING_BUFFERS_SIZE);

    //convert escape characters (%XX) to normal characters
    //conversion source: valueBuffer
    //conversion result: valueBufferNoEscape
    memset(valueBufferNoEscape, 0, STRING_BUFFERS_SIZE + 1);
    for (int i = 0, j = 0; i < strlen(valueBuffer);) {
      if (valueBuffer[i] != CHAR_ESCAPE) {
        valueBufferNoEscape[j] = valueBuffer[i];
        i++;
        j++;
        continue;
      }
      const size_t ESCAPE_CODE_LENGTH = 3;
      char escapeCode[ESCAPE_CODE_LENGTH];
      if (strlen(&(valueBuffer[i])) < ESCAPE_CODE_LENGTH) break;
      escapeCode[0] = valueBuffer[i + 1];
      escapeCode[1] = valueBuffer[i + 2];
      escapeCode[2] = 0;
      char escapeCodeValue = hexToByte(escapeCode);
      if (escapeCodeValue) {
        valueBufferNoEscape[j] = escapeCodeValue;
        j++;
      }
      i += ESCAPE_CODE_LENGTH;
    }
    //Compare acquired parameter name with known parameter names and set parameters in RAM if required
    float valueAsNumber = atof (valueBufferNoEscape);
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameWifiSsid)) {
      Serial.print(F("Setting SSID to "));
      Serial.println(valueBufferNoEscape);
      strncpy(eepromSavedParameters.wifiSsid, valueBufferNoEscape, sizeof(eepromSavedParameters.wifiSsid));
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameWifiPassword)) {
      Serial.print(F("Setting password to "));
      Serial.println(valueBufferNoEscape);
      strncpy(eepromSavedParameters.wifiPassword, valueBufferNoEscape, sizeof(eepromSavedParameters.wifiPassword));
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameBlynkAuth)) {
      Serial.print(F("Setting auth token to "));
      Serial.println(valueBufferNoEscape);
      strncpy(eepromSavedParameters.authToken, valueBufferNoEscape, sizeof(eepromSavedParameters.authToken));
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameAdcCalPoint0Calibrated)) {
      Serial.print(F("Setting MG811 calibration point 0 PPM value to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.MG811CalPoint0Calibrated = (unsigned int)valueAsNumber;
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameAdcCalPoint1Calibrated)) {
      Serial.print(F("Setting MG811 calibration point 1 PPM value to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.MG811CalPoint1Calibrated = (unsigned int)valueAsNumber;
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameAdcCalPoint0Raw)) {
      Serial.print(F("Setting MG811 calibration point 0 ADC value to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.MG811CalPoint0Raw = (unsigned int)valueAsNumber;
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameAdcCalPoint1Raw)) {
      Serial.print(F("Setting MG811 calibration point 1 ADC value to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.MG811CalPoint1Raw = (unsigned int)valueAsNumber;
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameAdcRejectCal)) {
      Serial.print(F("Setting MG811 calibration reject to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.rejectCalibrationMG811 = (byte)valueAsNumber;
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameAdcFilter)) {
      Serial.print(F("Setting MG811 filter to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.filterMG811 = (MG811Filter)valueAsNumber;
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameAdcFilterFreq)) {
      Serial.print(F("Setting MG811 low-pass frequency to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.filterMG811LowPassFrequency = (unsigned int)valueAsNumber;
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerParameterInternalNameMiscSensorSerialOutput)) {
      Serial.print(F("Setting sensor readings' serial output to "));
      Serial.println(valueAsNumber);
      eepromSavedParameters.sensorSerialOutput = (byte)valueAsNumber;
      configUpdated = true;
    }
    if (currentParameter != NULL) currentParameter++;
  } while (currentParameter != NULL);

  //If any parameter in RAM is updated, save parameters in EEPROM
  //To make sure parameter is written correctly, reload parameters from EEPROM after saving
  if (configUpdated) {
    saveConfig();
    loadConfig();
  }
}

void webServerBegin(void) {
  webServer.begin();
}

void webServerSendCode404(Print &client) {
  Serial.println(F("Response: 404"));
  BufferedPrint bufPrint(&client);
//  webServerPrintProgmem(client, httpStatusCode404);
  bufPrint.print((__FlashStringHelper *)httpStatusCode404);
}

void webServerSendCode405(Print &client) {
  Serial.println(F("Response: 405"));
  BufferedPrint bufPrint(&client);
  //  webServerPrintProgmem(client, httpStatusCode405);
  bufPrint.print((__FlashStringHelper *)httpStatusCode405);
}

void webServerSendConfigPage(Print &client) {
  Serial.println(F("Response: 200"));
  BufferedPrint bufPrint(&client);
  //  webServerPrintProgmem(client, httpStatusCode200);
  bufPrint.print((__FlashStringHelper *)httpStatusCode200);
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Sending config webpage."));

  WebPage webConfigPage(&bufPrint);

  webConfigPage.bodyBegin();

  webConfigPage.sectionBegin(F("Connection"));
  webConfigPage.subsectionBegin(F("WiFi"));
  webConfigPage.textParameter(F("SSID"),
                              (__FlashStringHelper *)webServerParameterInternalNameWifiSsid,
                              eepromSavedParameters.wifiSsid);
  webConfigPage.textParameter(F("Password"),
                              (__FlashStringHelper *)webServerParameterInternalNameWifiPassword,
                              eepromSavedParameters.wifiPassword,
                              F("Leave blank if connecting to open WiFi network"));
  webConfigPage.subsectionEnd();
  webConfigPage.subsectionBegin(F("Blynk"));
  webConfigPage.textParameter(F("Auth token"),
                              (__FlashStringHelper *)webServerParameterInternalNameBlynkAuth,
                              eepromSavedParameters.authToken,
                              F("Auth token is provided by Blynk app"));
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.sectionBegin(F("MG811"));
  webConfigPage.subsectionBegin(F("Calibration points"));
  webConfigPage.textParameter(F("Point 0: ppm value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint0Calibrated,
                              (long)eepromSavedParameters.MG811CalPoint0Calibrated);
  webConfigPage.textParameter(F("Point 0: raw value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint0Raw,
                              (long)eepromSavedParameters.MG811CalPoint0Raw);
  webConfigPage.textParameter(F("Point 1: ppm value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint1Calibrated,
                              (long)eepromSavedParameters.MG811CalPoint1Calibrated);
  webConfigPage.textParameter(F("Point 1: raw value"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcCalPoint1Raw,
                              (long)eepromSavedParameters.MG811CalPoint1Raw);
  webConfigPage.selectParameterBegin(F("Reject calibration data"),
                                     (__FlashStringHelper *)webServerParameterInternalNameAdcRejectCal,
                                     F("No ppm value calculation possible. Instead of ppm value, "
                                       "inverted raw ADC value is used. Enable this option if you would like to view trend only."));
  webConfigPage.selectParameterOption(F("Enable"), 1, (int)eepromSavedParameters.rejectCalibrationMG811);
  webConfigPage.selectParameterOption(F("Disable"), 0, (int)eepromSavedParameters.rejectCalibrationMG811);
  webConfigPage.selectParameterEnd();
  webConfigPage.subsectionEnd();
  webConfigPage.subsectionBegin(F("Filtering"));
  webConfigPage.selectParameterBegin(F("Filter"),
                                     (__FlashStringHelper *)webServerParameterInternalNameAdcFilter);
  webConfigPage.selectParameterOption(F("None"), 0, (unsigned int)eepromSavedParameters.filterMG811);
  webConfigPage.selectParameterOption(F("Averaging"), 1, (unsigned int)eepromSavedParameters.filterMG811);
  webConfigPage.selectParameterOption(F("Low-pass"), 2, (unsigned int)eepromSavedParameters.filterMG811);
  webConfigPage.selectParameterEnd();
  webConfigPage.textParameter(F("Low-pass filter frequency"),
                              (__FlashStringHelper *)webServerParameterInternalNameAdcFilterFreq,
                              eepromSavedParameters.filterMG811LowPassFrequency,
                              F("x 0.01 Hz"));
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.sectionBegin(F("Misc"));
  webConfigPage.subsectionBegin(F("Serial port"));
  webConfigPage.selectParameterBegin(F("Data values serial output"),
                                     (__FlashStringHelper *)webServerParameterInternalNameMiscSensorSerialOutput);
  webConfigPage.selectParameterOption(F("Enable"), 1, (int)eepromSavedParameters.sensorSerialOutput);
  webConfigPage.selectParameterOption(F("Disable"), 0, (int)eepromSavedParameters.sensorSerialOutput);
  webConfigPage.selectParameterEnd();
  webConfigPage.subsectionEnd();
  webConfigPage.sectionEnd();

  webConfigPage.bodyEnd();

  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config webpage sent to client."));

}

void webServerRun(void) {
  // Check if a client has connected
  WiFiClient client = webServer.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] web client connected"));
  while (!client.available()) {       //???
    delay(1);
  }

  //Read the method from the HTTP request
  const char CHAR_REQUEST_SEPARATOR = ' ';
  String requestMethod = client.readStringUntil(CHAR_REQUEST_SEPARATOR);
  Serial.print(F("["));
  Serial.print(millis());
  Serial.print(F("] method: "));
  Serial.println(requestMethod);
  //Read the URI from the HTTP request
  String requestURI = client.readStringUntil(CHAR_REQUEST_SEPARATOR);
  Serial.print(F("["));
  Serial.print(millis());
  Serial.print(F("] URI: "));
  Serial.println(requestURI);
  //Read the final part of HTTP request (includes cookies, post requests, etc...)
  //Currently not used, only printed in debug output
  String requestFinalPart = client.readString(); //string size may be a problem when ported to Arduino
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Final part of request: "));
  Serial.print(requestFinalPart);
  client.flush();

  //Get Path and Query from request URI
  String requestUriPath = "";
  String requestUriQuery = "";
  int requestPathQuerySeparatorPos = requestURI.indexOf('?');
  if (requestPathQuerySeparatorPos == -1) {
    requestUriPath = requestURI;
  }
  else {
    requestUriPath = requestURI.substring(0, requestPathQuerySeparatorPos);
    requestUriQuery = requestURI.substring(requestPathQuerySeparatorPos + 1);
  }
  Serial.print(F("URI Path:"));
  Serial.println(requestUriPath);
  Serial.print(F("URI Query:"));
  Serial.println(requestUriQuery);

  //Check method (only GET is accepted)
  String methodGet = "GET";
  if (requestMethod != methodGet) {
    webServerSendCode405(client);
    return;
  }
  //Check URI (only / is accepted)
  String webServerURIroot = "/";
  if (requestUriPath != webServerURIroot) {
    webServerSendCode404(client);
    return;
  }

  //Check for query, set and save parameters if included in GET request
  if (requestUriQuery != "") parseQuery (requestUriQuery);
  webServerSendConfigPage(client);
}
