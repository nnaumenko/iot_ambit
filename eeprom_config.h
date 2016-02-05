/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H

#include <Arduino.h>

#define CONFIG_TEXT_LENGTH 32 //NOT including trailing 0 character

extern char authToken[CONFIG_TEXT_LENGTH + 1];
extern char wifiSsid[CONFIG_TEXT_LENGTH + 1];
extern char wifiPassword[CONFIG_TEXT_LENGTH + 1];

#define MG811_CAL_POINTS 2
#define MG811_NUMBERS_PER_CAL_POINT 2
#define MG811_CAL_POINT_PPM 0
#define MG811_CAL_POINT_ADC 1

extern unsigned int calPointsMG811 [MG811_CAL_POINTS][MG811_NUMBERS_PER_CAL_POINT];

void saveConfig(void);
void loadConfig(void);

#endif
