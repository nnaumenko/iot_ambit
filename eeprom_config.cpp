/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "eeprom_config.h"

#include <EEPROM.h>

EepromSavedParameters eepromSavedParameters;

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
  EEPROM.begin(sizeof(eepromSavedParameters) + EEPROM_SAVED_PARAMETERS_ADDRESS);
  saveToEEPROM(EEPROM_SAVED_PARAMETERS_ADDRESS,
               (void *)&eepromSavedParameters,
               sizeof(eepromSavedParameters));
  EEPROM.end();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config saved to EEPROM."));
}

void loadConfig(void) {
  EEPROM.begin(sizeof(eepromSavedParameters));
  loadFromEEPROM(EEPROM_SAVED_PARAMETERS_ADDRESS,
                 (void *)&eepromSavedParameters,
                 sizeof(eepromSavedParameters));
  EEPROM.end();
  Serial.print(F("["));
  Serial.print(millis());
  Serial.println(F("] Config loaded from EEPROM."));
  Serial.print(F("WIFI network: "));
  Serial.println(eepromSavedParameters.wifiSsid);
  if (EEPROM_DEBUG_PRINT_INSECURE) {
    Serial.print(F("WIFI password: "));
    Serial.println(eepromSavedParameters.wifiPassword);
    Serial.print(F("Auth token: "));
    Serial.println(eepromSavedParameters.authToken);
  }
  Serial.print(F("MG811 cal points: "));
  Serial.print(F("Raw="));
  Serial.print(eepromSavedParameters.MG811CalPoint0Raw);
  Serial.print(F(" Calibrated="));
  Serial.print(eepromSavedParameters.MG811CalPoint0Calibrated);
  Serial.print(F("ppm / Raw="));
  Serial.print(eepromSavedParameters.MG811CalPoint1Raw);
  Serial.print(F(" Calibrated="));
  Serial.print(eepromSavedParameters.MG811CalPoint1Calibrated);
  Serial.println(F("ppm"));
  Serial.print(F("MG811 filter: "));
  switch (eepromSavedParameters.filterMG811) {
    case MG811_FILTER_OFF:
      Serial.println(F("off"));
      break;
    case MG811_FILTER_AVERAGE:
      Serial.println(F("moving average"));
      break;
    case MG811_FILTER_LOWPASS:
      Serial.print(F("low-pass, limit frequency: "));
      Serial.print(eepromSavedParameters.filterMG811LowPassFrequency);
      Serial.println(F(" x 0.01 Hz"));
      break;
    default:
      Serial.print(F("unknown ("));
      Serial.print((int)eepromSavedParameters.filterMG811, DEC);
      Serial.println(F(")"));
      break;
  }
  Serial.print(F("MG811 calibration mode: "));
  if (!eepromSavedParameters.rejectCalibrationMG811)
    Serial.println(F("use calibration data"));
  else
    Serial.println(F("use uncalibrated value"));
  Serial.print(F("Sensor readings' serial output: "));
  if (!eepromSavedParameters.sensorSerialOutput)
    Serial.println(F("off"));
  else
    Serial.println(F("on"));
}
