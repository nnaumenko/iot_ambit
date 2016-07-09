/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H

#include <Arduino.h>
#include "stringmap.h"
#include "mg811.h"

//if true, then passwords, auth tokens, etc. are included in the debug output
const boolean EEPROM_DEBUG_PRINT_INSECURE = true;

#define CONFIG_TEXT_LENGTH 33 //including trailing 0 character

#define EEPROM_SAVED_PARAMETERS_ADDRESS 0

struct EepromSavedParametersStorage {
  char authToken[CONFIG_TEXT_LENGTH] = {0};
  char wifiSsid[CONFIG_TEXT_LENGTH] = {0};
  char wifiPassword[CONFIG_TEXT_LENGTH] = {0};
  unsigned int MG811CalPoint0Calibrated = 0;
  unsigned int MG811CalPoint0Raw = 0;
  unsigned int MG811CalPoint1Calibrated = 100;
  unsigned int MG811CalPoint1Raw = 1024;
  MG811Filter filterMG811 = MG811_FILTER_OFF;
  unsigned int filterMG811LowPassFrequency = 40;
  byte rejectCalibrationMG811 = 0;
  byte sensorSerialOutput = 0;
};

enum class EepromSavedParameter {
  UNKNOWN = STRINGMAP_ITEM_DEFAULT,
  BLYNK_AUTH_TOKEN = 1,
  WIFI_SSID = 2,
  WIFI_PASSWORD = 3,
  MG811_CALPOINT0_CAL = 4,
  MG811_CALPOINT0_RAW = 5,
  MG811_CALPOINT1_CAL = 6,
  MG811_CALPOINT1_RAW = 7,
  MG811FILTER_TYPE = 8,
  MG811FILTER_LOWPASS_FREQ = 9,
  MG811_REJECT_CALIBRATION = 10,
  MG811_SERIAL_OUT = 11,
};

extern const PROGMEM QuickStringMapItem stringMapEepromSavedParameterInternalNames[];

#ifdef ESP8266
#define EEPROM_SIZE 512
#endif

static_assert ((sizeof(EepromSavedParametersStorage) + EEPROM_SAVED_PARAMETERS_ADDRESS) <= EEPROM_SIZE,
               "struct EepromSavedParametersStorage does not fit into EEPROM (check EepromSavedParametersStorage size and address)");

extern EepromSavedParametersStorage eepromSavedParametersStorage;

void saveConfig(void);
void loadConfig(void);

#endif
