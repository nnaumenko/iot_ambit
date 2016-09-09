/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef UITEXTS_H
#define UITEXTS_H

#include <Arduino.h>

enum class webccUISections {
  UNKNOWN = 0,
  CAPTION,
  CONNECTION,
  WIFI,
  BLYNK,
  ADC,
  ADC_CALIBRATION,
  ADC_FILTER,
  MISC,
  SERIAL_PORT,
  STARTUP
};

class ProgmemStringMap;

extern ProgmemStringMap webccUIParameterSections;
extern ProgmemStringMap webccUIParameterDisplayNames;
extern ProgmemStringMap webccUIParameterTooltips;
extern ProgmemStringMap webccUISelectADCFilter;

#endif
