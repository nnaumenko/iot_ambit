/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "diag.h"

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
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Config saved to EEPROM."));
}

void loadConfig(void) {
  EEPROM.begin(sizeof(eepromSavedParametersStorage));
  loadFromEEPROM(EEPROM_SAVED_PARAMETERS_ADDRESS,
                 (void *)&eepromSavedParametersStorage,
                 sizeof(eepromSavedParametersStorage));
  EEPROM.end();
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] Config loaded from EEPROM."));
  DiagLog.print(F("Config saved with firmware version "));
  DiagLog.print(eepromSavedParametersStorage.versionMajor);
  DiagLog.print(F("."));
  DiagLog.println(eepromSavedParametersStorage.versionMinor);
  if ((eepromSavedParametersStorage.versionMajor != FIRMWARE_VERSION_MAJOR) ||
      (eepromSavedParametersStorage.versionMinor != FIRMWARE_VERSION_MINOR))
    DiagLog.print(F("CONFIG SAVED WITH DIFFERENT FIRMWARE VERSION, PLEASE ACTIVATE CONFIG MODE AND REVIEW DATA"));
  DiagLog.print(F("Checksum: 0x"));
  DiagLog.println(eepromSavedParametersStorage.checkSum, HEX);
  if (calculateCheckSum() != eepromSavedParametersStorage.checkSum) DiagLog.print(F("CONFIG CHECKSUM WRONG, PLEASE ACTIVATE CONFIG MODE AND REVIEW DATA"));
  DiagLog.print(F("WIFI network: "));
  DiagLog.println(eepromSavedParametersStorage.wifiSsid);
  if (EEPROM_DEBUG_PRINT_INSECURE) {
    DiagLog.print(F("WIFI password: "));
    DiagLog.println(eepromSavedParametersStorage.wifiPassword);
    DiagLog.print(F("Auth token: "));
    DiagLog.println(eepromSavedParametersStorage.authToken);
  }
  DiagLog.print(F("MG811 cal points: "));
  DiagLog.print(F("Raw="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint0Raw);
  DiagLog.print(F(" Calibrated="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint0Calibrated);
  DiagLog.print(F("ppm / Raw="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint1Raw);
  DiagLog.print(F(" Calibrated="));
  DiagLog.print(eepromSavedParametersStorage.MG811CalPoint1Calibrated);
  DiagLog.println(F("ppm"));
  DiagLog.print(F("MG811 filter: "));
  switch (eepromSavedParametersStorage.filterMG811) {
    case ADCFilter::OFF:
      DiagLog.println(F("off"));
      break;
    case ADCFilter::AVERAGE:
      DiagLog.println(F("moving average"));
      break;
    case ADCFilter::LOWPASS:
      DiagLog.print(F("low-pass, limit frequency: "));
      DiagLog.print(eepromSavedParametersStorage.filterMG811LowPassFrequency);
      DiagLog.println(F(" x 0.01 Hz"));
      break;
    default:
      DiagLog.print(F("unknown ("));
      DiagLog.print((int)eepromSavedParametersStorage.filterMG811, DEC);
      DiagLog.println(F(")"));
      break;
  }
  DiagLog.print(F("MG811 calibration mode: "));
  if (!eepromSavedParametersStorage.rejectCalibrationMG811)
    DiagLog.println(F("use calibration data"));
  else
    DiagLog.println(F("use uncalibrated value"));
  DiagLog.print(F("Sensor readings' serial output: "));
  if (!eepromSavedParametersStorage.sensorSerialOutput)
    DiagLog.println(F("off"));
  else
    DiagLog.println(F("on"));
}
