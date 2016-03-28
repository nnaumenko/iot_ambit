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
MG811Filter filterMG811 = MG811_FILTER_OFF;
unsigned int filterMG811LowPassFrequency = 40;
byte rejectCalibrationMG811 = 0;
byte SensorSerialOutput = 0;

const int EEPROM_ADDRESS_AUTH_TOKEN = 0;
const int EEPROM_ADDRESS_WIFI_SSID = CONFIG_TEXT_LENGTH;
const int EEPROM_ADDRESS_WIFI_PASSWORD = CONFIG_TEXT_LENGTH * 2;
const int EEPROM_ADDRESS_CAL_POINTS_MG811 = CONFIG_TEXT_LENGTH * 3;
const int EEPROM_ADDRESS_FILTER_MG811 = EEPROM_ADDRESS_CAL_POINTS_MG811 + sizeof(calPointsMG811);
const int EEPROM_ADDRESS_FILTER_MG811_FREQ = EEPROM_ADDRESS_FILTER_MG811 + sizeof(filterMG811);
const int EEPROM_ADDRESS_MG811_REJECT_CAL = EEPROM_ADDRESS_FILTER_MG811_FREQ + sizeof(filterMG811LowPassFrequency);
const int EEPROM_ADDRESS_SENSOR_SERIAL_OUTPUT = EEPROM_ADDRESS_MG811_REJECT_CAL + sizeof(rejectCalibrationMG811);

const int SIZE_OF_EEPROM_DATA = CONFIG_TEXT_LENGTH * 3 +
                                sizeof(calPointsMG811) +
                                sizeof(filterMG811) +
                                sizeof(filterMG811LowPassFrequency) +
                                sizeof(rejectCalibrationMG811) +
                                sizeof(SensorSerialOutput);

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
  saveToEEPROM(EEPROM_ADDRESS_FILTER_MG811, (void *)&filterMG811, sizeof(filterMG811));
  saveToEEPROM(EEPROM_ADDRESS_FILTER_MG811_FREQ, (void *)&filterMG811LowPassFrequency, sizeof(filterMG811LowPassFrequency));
  saveToEEPROM(EEPROM_ADDRESS_MG811_REJECT_CAL, (void *)&rejectCalibrationMG811, sizeof(rejectCalibrationMG811));
  saveToEEPROM(EEPROM_ADDRESS_SENSOR_SERIAL_OUTPUT, (void *)&SensorSerialOutput, sizeof(SensorSerialOutput));
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
  loadFromEEPROM(EEPROM_ADDRESS_FILTER_MG811, (void *)&filterMG811, sizeof(filterMG811));
  loadFromEEPROM(EEPROM_ADDRESS_FILTER_MG811_FREQ, (void *)&filterMG811LowPassFrequency, sizeof(filterMG811LowPassFrequency));
  loadFromEEPROM(EEPROM_ADDRESS_MG811_REJECT_CAL, (void *)&rejectCalibrationMG811, sizeof(rejectCalibrationMG811));
  loadFromEEPROM(EEPROM_ADDRESS_SENSOR_SERIAL_OUTPUT, (void *)&SensorSerialOutput, sizeof(SensorSerialOutput));
  EEPROM.end();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config loaded from EEPROM."));
  //WiFi password and Blynk Auth Token are commented in order not to expose them in the debug output.
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
  Serial.print(F("MG811 filter: "));
  switch (filterMG811) {
    case MG811_FILTER_OFF:
      Serial.println(F("off"));
      break;
    case MG811_FILTER_AVERAGE:
      Serial.println(F("moving average"));
      break;
    case MG811_FILTER_LOWPASS:
      Serial.print(F("low-pass, limit frequency: "));
      Serial.print(filterMG811LowPassFrequency);
      Serial.println(F(" x 0.01 Hz"));
      break;
    default:
      Serial.print(F("unknown ("));
      Serial.print((int)filterMG811, DEC);
      Serial.println(F(")"));
      break;
  }
  Serial.print(F("MG811 calibration mode: "));
  if (!rejectCalibrationMG811)
    Serial.println(F("use calibration data"));
  else
    Serial.println(F("use uncalibrated value"));
  Serial.print(F("Sensor readings' serial output: "));
  if (!SensorSerialOutput)
    Serial.println(F("off"));
  else
    Serial.println(F("on"));

}
