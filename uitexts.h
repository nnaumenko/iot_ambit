/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef UITEXTS_H
#define UITEXTS_H

#include <Arduino.h>
#include "stringmap.h"

extern ProgmemStringMap webccUISelectEnableDisable;
extern ProgmemStringMap webccUISelectADCFilter;

extern ProgmemStringMap webccUIParameterDisplayNames;
extern ProgmemStringMap webccUIParameterTooltips;

extern const char PROGMEM webccUIText_SectionDisplayName_Connection[];
extern const char PROGMEM webccUIText_SectionDisplayName_ADC[];
extern const char PROGMEM webccUIText_SectionDisplayName_Misc[];

extern const char PROGMEM webccUIText_SubsectionDisplayName_WiFi[];
extern const char PROGMEM webccUIText_SubsectionDisplayName_Blynk[];
extern const char PROGMEM webccUIText_SubsectionDisplayName_ADCCalibration[];
extern const char PROGMEM webccUIText_SubsectionDisplayName_Filter[];
extern const char PROGMEM webccUIText_SubsectionDisplayName_SerialPort[];

#endif
