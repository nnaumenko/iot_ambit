/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "diag_legacy.h"
#include "version.h"
#include "eeprom_config.h"

#include <EEPROM.h>

EepromSavedParametersStorage eepromSavedParametersStorage;

const PROGMEM QuickStringMapItem stringMapEepromSavedParameterInternalNames[] {
  {(StringMapKey)EepromSavedParameter::BLYNK_AUTH_TOKEN, "blynkauth"},
  {(StringMapKey)EepromSavedParameter::WIFI_SSID, "wifissid"},
  {(StringMapKey)EepromSavedParameter::WIFI_PASSWORD, "wifipass"},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT0_CAL, "adccal0cal"},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT0_RAW, "adccal0raw"},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT1_CAL, "adccal1cal"},
  {(StringMapKey)EepromSavedParameter::MG811_CALPOINT1_RAW, "adccal1raw"},
  {(StringMapKey)EepromSavedParameter::MG811FILTER_TYPE, "adcfilter"},
  {(StringMapKey)EepromSavedParameter::MG811FILTER_LOWPASS_FREQ, "adcfiltfreq"},
  {(StringMapKey)EepromSavedParameter::MG811_REJECT_CALIBRATION, "adcrejectcal"},
  {(StringMapKey)EepromSavedParameter::MISC_SERIAL_OUT, "miscserout"},
  {(StringMapKey)EepromSavedParameter::BLYNK_SERVER, "blynkserver"},
  {(StringMapKey)EepromSavedParameter::BLYNK_SERVER_PORT, "blynkport"},
  {(StringMapKey)EepromSavedParameter::STARTUP_DELAY, "stdel"},
  {(StringMapKey)EepromSavedParameter::UNKNOWN, ""}
};

void saveToEEPROM (int address, const void * parameter, size_t length) {
  byte * parameterBytes = (byte *) parameter;
  for (size_t i = 0; i < length; i++) {
    EEPROM.write(address + i, parameterBytes[i]);
  }
}

void loadFromEEPROM (int address, void * parameter, size_t length) {
  byte * parameterBytes = (byte *) parameter;
  for (size_t i = 0; i < length; i++) {
    parameterBytes[i] = EEPROM.read(address + i);
  }
}

uint16 calculateCheckSum (void) {
  return (0); //TODO: CRC16 checksum
}

void saveConfig(void) {
  eepromSavedParametersStorage.versionMajor = FIRMWARE_VERSION_MAJOR;
  eepromSavedParametersStorage.versionMinor = FIRMWARE_VERSION_MINOR;
  eepromSavedParametersStorage.checkSum = calculateCheckSum();
  EEPROM.begin(sizeof(eepromSavedParametersStorage) + EEPROM_SAVED_PARAMETERS_ADDRESS);
  saveToEEPROM(EEPROM_SAVED_PARAMETERS_ADDRESS,
               (void *)&eepromSavedParametersStorage,
               sizeof(eepromSavedParametersStorage));
  EEPROM.end();
  DiagLogLegacy.print(F("["));
  DiagLogLegacy.print(millis());
  DiagLogLegacy.println(F("] Config saved to EEPROM."));
}

void loadConfig(void) {
  EEPROM.begin(sizeof(eepromSavedParametersStorage));
  loadFromEEPROM(EEPROM_SAVED_PARAMETERS_ADDRESS,
                 (void *)&eepromSavedParametersStorage,
                 sizeof(eepromSavedParametersStorage));
  EEPROM.end();
  DiagLogLegacy.print(F("["));
  DiagLogLegacy.print(millis());
  DiagLogLegacy.println(F("] Config loaded from EEPROM."));
  DiagLogLegacy.print(F("Config saved with firmware version "));
  DiagLogLegacy.print(eepromSavedParametersStorage.versionMajor);
  DiagLogLegacy.print(F("."));
  DiagLogLegacy.println(eepromSavedParametersStorage.versionMinor);
  if ((eepromSavedParametersStorage.versionMajor != FIRMWARE_VERSION_MAJOR) ||
      (eepromSavedParametersStorage.versionMinor != FIRMWARE_VERSION_MINOR))
    DiagLogLegacy.println(F("CONFIG SAVED WITH DIFFERENT FIRMWARE VERSION, PLEASE ACTIVATE CONFIG MODE AND REVIEW DATA"));
  DiagLogLegacy.print(F("Checksum: 0x"));
  DiagLogLegacy.println(eepromSavedParametersStorage.checkSum, HEX);
  if (calculateCheckSum() != eepromSavedParametersStorage.checkSum) DiagLogLegacy.print(F("CONFIG CHECKSUM WRONG, PLEASE ACTIVATE CONFIG MODE AND REVIEW DATA"));
  DiagLogLegacy.print(F("WIFI network: "));
  DiagLogLegacy.println(eepromSavedParametersStorage.wifiSsid);
  if (EEPROM_DEBUG_PRINT_INSECURE) {
    DiagLogLegacy.print(F("WIFI password: "));
    DiagLogLegacy.println(eepromSavedParametersStorage.wifiPassword);
    DiagLogLegacy.print(F("Auth token: "));
    DiagLogLegacy.println(eepromSavedParametersStorage.authToken);
  }
  DiagLogLegacy.print(F("MG811 cal points: "));
  DiagLogLegacy.print(F("Raw="));
  DiagLogLegacy.print(eepromSavedParametersStorage.MG811CalPoint0Raw);
  DiagLogLegacy.print(F(" Calibrated="));
  DiagLogLegacy.print(eepromSavedParametersStorage.MG811CalPoint0Calibrated);
  DiagLogLegacy.print(F("ppm / Raw="));
  DiagLogLegacy.print(eepromSavedParametersStorage.MG811CalPoint1Raw);
  DiagLogLegacy.print(F(" Calibrated="));
  DiagLogLegacy.print(eepromSavedParametersStorage.MG811CalPoint1Calibrated);
  DiagLogLegacy.println(F("ppm"));
  DiagLogLegacy.print(F("MG811 filter: "));
  switch (eepromSavedParametersStorage.filterMG811) {
    case ADCFilter::OFF:
      DiagLogLegacy.println(F("off"));
      break;
    case ADCFilter::AVERAGE:
      DiagLogLegacy.println(F("moving average"));
      break;
    case ADCFilter::LOWPASS:
      DiagLogLegacy.print(F("low-pass, limit frequency: "));
      DiagLogLegacy.print(eepromSavedParametersStorage.filterMG811LowPassFrequency);
      DiagLogLegacy.println(F(" x 0.01 Hz"));
      break;
    default:
      DiagLogLegacy.print(F("unknown ("));
      DiagLogLegacy.print((int)eepromSavedParametersStorage.filterMG811, DEC);
      DiagLogLegacy.println(F(")"));
      break;
  }
  DiagLogLegacy.print(F("MG811 calibration mode: "));
  if (!eepromSavedParametersStorage.rejectCalibrationMG811)
    DiagLogLegacy.println(F("use calibration data"));
  else
    DiagLogLegacy.println(F("use uncalibrated value"));
  DiagLogLegacy.print(F("Sensor readings' serial output: "));
  if (!eepromSavedParametersStorage.sensorSerialOutput)
    DiagLogLegacy.println(F("off"));
  else
    DiagLogLegacy.println(F("on"));
}
