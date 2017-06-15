/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "diag.h"
#include "version.h"
#include "eeprom_config.h"
#include "util_data.h"

#include <EEPROM.h>

using DiagLog = diag::DiagLog<>;

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

uint16_t calculateCheckSum (void) {
  return (util::checksum::crc16(&eepromSavedParametersStorage, sizeof(EepromSavedParametersStorage)));
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
  DiagLog::instance()->log(DiagLog::Severity::INFORMATIONAL, F("Config saved to EEPROM"));
}

void loadConfig(void) {
  EEPROM.begin(sizeof(eepromSavedParametersStorage));
  loadFromEEPROM(EEPROM_SAVED_PARAMETERS_ADDRESS,
                 (void *)&eepromSavedParametersStorage,
                 sizeof(eepromSavedParametersStorage));
  EEPROM.end();

  DiagLog::instance()->log(DiagLog::Severity::INFORMATIONAL, F("Config loaded from EEPROM"));
  DiagLog::instance()->log(DiagLog::Severity::DEBUG,
                           F("Config was saved with firmware version "),
                           eepromSavedParametersStorage.versionMajor,
                           '.',
                           eepromSavedParametersStorage.versionMinor);
  if ((eepromSavedParametersStorage.versionMajor != FIRMWARE_VERSION_MAJOR) ||
      (eepromSavedParametersStorage.versionMinor != FIRMWARE_VERSION_MINOR))
    DiagLog::instance()->log(DiagLog::Severity::WARNING, F("CONFIG SAVED WITH DIFFERENT FIRMWARE VERSION, PLEASE ACTIVATE CONFIG MODE AND REVIEW DATA"));
  DiagLog::instance()->log(DiagLog::Severity::NOTICE, F("Checksum: "), eepromSavedParametersStorage.checkSum);
  uint16_t actualCheckSum = calculateCheckSum();
  if (actualCheckSum != eepromSavedParametersStorage.checkSum) {
    DiagLog::instance()->log(DiagLog::Severity::WARNING, F("Actual checksum: "), actualCheckSum);
    DiagLog::instance()->log(DiagLog::Severity::WARNING, F("CONFIG CHECKSUM MISMATCH, PLEASE ACTIVATE CONFIG MODE AND REVIEW DATA"));
  }
  DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("WIFI network: "), eepromSavedParametersStorage.wifiSsid);
  if (EEPROM_DEBUG_PRINT_INSECURE) {
    DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("WIFI password: "), eepromSavedParametersStorage.wifiPassword);
    DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("Auth token: "), eepromSavedParametersStorage.authToken);
  }
  DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("MG811 cal points: "),
                           F("Raw="), eepromSavedParametersStorage.MG811CalPoint0Raw,
                           F(" Calibrated="), eepromSavedParametersStorage.MG811CalPoint0Calibrated,
                           F("ppm / Raw="), eepromSavedParametersStorage.MG811CalPoint1Raw,
                           F(" Calibrated="), eepromSavedParametersStorage.MG811CalPoint1Calibrated,
                           F("ppm"));

  switch (eepromSavedParametersStorage.filterMG811) {
    case ADCFilter::OFF:
      DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("MG811 filter: off"));
      break;
    case ADCFilter::AVERAGE:
      DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("MG811 filter: moving average"));
      break;
    case ADCFilter::LOWPASS:
      DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("MG811 filter: low-pass, limit frequency: "), eepromSavedParametersStorage.filterMG811LowPassFrequency, F(" x 0.01 Hz"));
      break;
    default:
      DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("MG811 filter: unknown ("), (int)eepromSavedParametersStorage.filterMG811, ')');
      break;
  }
  if (!eepromSavedParametersStorage.rejectCalibrationMG811)
    DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("MG811 calibration mode: use calibration data"));
  else
    DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("MG811 calibration mode: use uncalibrated value"));
  if (!eepromSavedParametersStorage.sensorSerialOutput)
    DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("Sensor readings' serial output: off"));
  else
    DiagLog::instance()->log(DiagLog::Severity::DEBUG, F("Sensor readings' serial output: on"));
}
