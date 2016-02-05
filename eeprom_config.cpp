/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "eeprom_config.h"

#include <EEPROM.h>

char authToken[CONFIG_TEXT_LENGTH + 1] = {0};
char wifiSsid[CONFIG_TEXT_LENGTH + 1] = {0};
char wifiPassword[CONFIG_TEXT_LENGTH + 1] = {0};
unsigned int calPointsMG811 [MG811_CAL_POINTS][MG811_NUMBERS_PER_CAL_POINT];

const int EEPROM_ADDRESS_AUTH_TOKEN = 0;
const int EEPROM_ADDRESS_WIFI_SSID = CONFIG_TEXT_LENGTH;
const int EEPROM_ADDRESS_WIFI_PASSWORD = CONFIG_TEXT_LENGTH * 2;
const int EEPROM_ADDRESS_CAL_POINTS_MG811 = CONFIG_TEXT_LENGTH * 3;

const int SIZE_OF_EEPROM_DATA = CONFIG_TEXT_LENGTH * 3 + sizeof(calPointsMG811);

void saveToEEPROM (int address, const void * parameter, size_t length) {
  byte * parameterBytes = (byte *) parameter;
  for (int i = 0; i < length; i++) {
    EEPROM.write(address + i, parameterBytes[i]);
  }
}

void loadFromEEPROM (int address, void * parameter, size_t length) {
  byte * parameterBytes = (byte *) parameter;
  for (int i = 0; i < length; i++) {
    parameterBytes[i] = EEPROM.read(address + i);
  }
}

void saveConfig(void) {
  EEPROM.begin(SIZE_OF_EEPROM_DATA);
  saveToEEPROM(EEPROM_ADDRESS_AUTH_TOKEN, (void *)authToken, CONFIG_TEXT_LENGTH);
  saveToEEPROM(EEPROM_ADDRESS_WIFI_SSID, (void *)wifiSsid, CONFIG_TEXT_LENGTH);
  saveToEEPROM(EEPROM_ADDRESS_WIFI_PASSWORD, (void *)wifiPassword, CONFIG_TEXT_LENGTH);
  saveToEEPROM(EEPROM_ADDRESS_CAL_POINTS_MG811, (void *)calPointsMG811, sizeof(calPointsMG811));
  EEPROM.end();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config saved to EEPROM."));
}

void loadConfig(void) {
  EEPROM.begin(SIZE_OF_EEPROM_DATA);
  loadFromEEPROM(EEPROM_ADDRESS_AUTH_TOKEN, (void *) authToken, CONFIG_TEXT_LENGTH);
  loadFromEEPROM(EEPROM_ADDRESS_WIFI_SSID, (void *)wifiSsid, CONFIG_TEXT_LENGTH);
  loadFromEEPROM(EEPROM_ADDRESS_WIFI_PASSWORD, (void *)wifiPassword, CONFIG_TEXT_LENGTH);
  loadFromEEPROM(EEPROM_ADDRESS_CAL_POINTS_MG811, (void *)calPointsMG811, sizeof(calPointsMG811));
  EEPROM.end();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config loaded from EEPROM."));
  //Some parameters are commented in order not to expose them in the debug output.
  //Uncomment if this is not an issue for you.
  //Serial.print(F("Auth token: "));
  //Serial.println(authToken);
  Serial.print(F("WIFI network: "));
  Serial.println(wifiSsid);
  //Serial.print(F("WIFI password: "));
  //Serial.println(wifiPassword);
  Serial.print(F("MG811 cal points: "));
  Serial.print(F("ADC="));
  Serial.print(calPointsMG811 [0][MG811_CAL_POINT_ADC]);
  Serial.print(F(" CO2="));
  Serial.print(calPointsMG811 [0][MG811_CAL_POINT_PPM]);
  Serial.print(F("ppm / ADC="));
  Serial.print(calPointsMG811 [1][MG811_CAL_POINT_ADC]);
  Serial.print(F(" CO2="));
  Serial.print(calPointsMG811 [1][MG811_CAL_POINT_PPM]);
  Serial.println(F("ppm"));

}
