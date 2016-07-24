/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef ADC_H
#define ADC_H

#include <Arduino.h>

enum class ADCFilter {
  OFF = 0,
  AVERAGE,
  LOWPASS,
};

#endif
