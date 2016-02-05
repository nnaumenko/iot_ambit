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

void webServerPrintProgmem(WiFiClient &client, const char * progmemBuffer) {
  size_t tempBufferSize = strlen_P(progmemBuffer);
  char tempBuffer[tempBufferSize + 1]; //strlen does not include trailing zero
  strncpy_P(tempBuffer, progmemBuffer, tempBufferSize);
  tempBuffer[tempBufferSize] = 0;
  client.print(tempBuffer);
}

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

void parseQuery(const String &query) {//this function using c string functions to avoid string memory reallocation bugs
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
    if (!compareWithProgmem(parameterBuffer, webServerWifiFormParameterSsid)) {
      Serial.print(F("Setting SSID to "));
      Serial.println(valueBufferNoEscape);
      strncpy(wifiSsid, valueBufferNoEscape, sizeof(wifiSsid));
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerWifiFormParameterPassword)) {
      Serial.print(F("Setting password to "));
      Serial.println(valueBufferNoEscape);
      strncpy(wifiPassword, valueBufferNoEscape, sizeof(wifiPassword));
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerWifiFormParameterAuth)) {
      Serial.print(F("Setting auth token to "));
      Serial.println(valueBufferNoEscape);
      strncpy(authToken, valueBufferNoEscape, sizeof(authToken));
      configUpdated = true;
    }
    if (!compareWithProgmem(parameterBuffer, webServerMG811FormParameterCalPoint0Ppm)) {
      Serial.print(F("Setting MG811 calibration point 0 PPM value to "));
      Serial.println(valueAsNumber);
      calPointsMG811[0][MG811_CAL_POINT_PPM] = (unsigned int)valueAsNumber;
      configUpdated = true;
    }   
    if (!compareWithProgmem(parameterBuffer, webServerMG811FormParameterCalPoint1Ppm)) {
      Serial.print(F("Setting MG811 calibration point 1 PPM value to "));
      Serial.println(valueAsNumber);
      calPointsMG811[1][MG811_CAL_POINT_PPM] = (unsigned int)valueAsNumber;
      configUpdated = true;
    }   
    if (!compareWithProgmem(parameterBuffer, webServerMG811FormParameterCalPoint0Adc)) {
      Serial.print(F("Setting MG811 calibration point 0 ADC value to "));
      Serial.println(valueAsNumber);
      calPointsMG811[0][MG811_CAL_POINT_ADC] = (unsigned int)valueAsNumber;
      configUpdated = true;
    }   
    if (!compareWithProgmem(parameterBuffer, webServerMG811FormParameterCalPoint1Adc)) {
      Serial.print(F("Setting MG811 calibration point 1 ADC value to "));
      Serial.println(valueAsNumber);
      calPointsMG811[1][MG811_CAL_POINT_ADC] = (unsigned int)valueAsNumber;
      configUpdated = true;
    }   
    if (currentParameter == NULL) break;
    currentParameter++;
  } while (1);

  //If any parameter in RAM is updated, save parameters in EEPROM
  //To make sure parameter is written correctly, reload parameters from EEPROM after saving
  if (configUpdated) {
    saveConfig();
    loadConfig();
  }
}

void webServerParameter(WiFiClient &client, const char * progmemDisplayName, const char * progmemParameterName, const char * value) {
  webServerPrintProgmem(client, webServerFormInputBegin);
  webServerPrintProgmem(client, progmemDisplayName);
  webServerPrintProgmem(client, webServerFormInputInput);
  webServerPrintProgmem(client, progmemParameterName);
  webServerPrintProgmem(client, webServerFormInputValue);
  client.print(value);
  webServerPrintProgmem(client, webServerFormInputEnd);
}

void webServerParameter(WiFiClient &client, const char * progmemDisplayName, const char * progmemParameterName, long value) {
  webServerPrintProgmem(client, webServerFormInputBegin);
  webServerPrintProgmem(client, progmemDisplayName);
  webServerPrintProgmem(client, webServerFormInputInput);
  webServerPrintProgmem(client, progmemParameterName);
  webServerPrintProgmem(client, webServerFormInputValue);
  client.print(value);
  webServerPrintProgmem(client, webServerFormInputEnd);
}

void webServerBegin(void) {
  webServer.begin();
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
    Serial.println(F("Response: 405"));
    webServerPrintProgmem(client, httpStatusCode405);
    return;
  }

  //Check URI (only / is accepted)
  String webServerURIroot = "/";
  if (requestUriPath != webServerURIroot) {
    Serial.println(F("Response: 404"));
    webServerPrintProgmem(client, httpStatusCode404);
    return;
  }

  //Check for query, set and save config if requested
  if (requestUriQuery != "") parseQuery (requestUriQuery);

  //Send the config webpage
  Serial.println(F("Response: 200"));
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Sending config webpage."));

  webServerPrintProgmem(client, httpStatusCode200);
  webServerPrintProgmem(client, webServerHeadBegin);
  webServerPrintProgmem(client, webServerHeadBody);

  webServerPrintProgmem(client, webServerFormBegin1);
  webServerPrintProgmem(client, webServerWifiFormCaption);
  webServerPrintProgmem(client, webServerFormBegin2);
  webServerParameter(client, webServerWifiFormParameterDisplayNameSsid, webServerWifiFormParameterSsid, wifiSsid);
  webServerParameter(client, webServerWifiFormParameterDisplayNamePassword, webServerWifiFormParameterPassword, wifiPassword);
  webServerParameter(client, webServerWifiFormParameterDisplayNameAuth, webServerWifiFormParameterAuth, authToken);
  webServerPrintProgmem(client, webServerFormEnd);

  webServerPrintProgmem(client, webServerFormBegin1);
  webServerPrintProgmem(client, webServerMG811FormCaption);
  webServerPrintProgmem(client, webServerFormBegin2);
  webServerParameter(client, webServerMG811FormParameterDisplayNameCalPoint0Ppm, webServerMG811FormParameterCalPoint0Ppm, (long)calPointsMG811[0][MG811_CAL_POINT_PPM]);
  webServerParameter(client, webServerMG811FormParameterDisplayNameCalPoint0Adc, webServerMG811FormParameterCalPoint0Adc, (long)calPointsMG811[0][MG811_CAL_POINT_ADC]);
  webServerParameter(client, webServerMG811FormParameterDisplayNameCalPoint1Ppm, webServerMG811FormParameterCalPoint1Ppm, (long)calPointsMG811[1][MG811_CAL_POINT_PPM]);
  webServerParameter(client, webServerMG811FormParameterDisplayNameCalPoint1Adc, webServerMG811FormParameterCalPoint1Adc, (long)calPointsMG811[1][MG811_CAL_POINT_ADC]);
  webServerPrintProgmem(client, webServerFormEnd);

//  webServerPrintProgmem(client, webServerFormBegin1);
//  webServerPrintProgmem(client, webServerMiscFormCaption);
//  webServerPrintProgmem(client, webServerFormBegin2);
//  webServerPrintProgmem(client, webServerFormEnd);
  
  webServerPrintProgmem(client, webServerBodyEnd);

  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config webpage sent to client."));
}
