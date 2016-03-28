/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef MG811_H
#define MG811_H

#include <Arduino.h>

enum MG811Filter {
  MG811_FILTER_OFF = 0,
  MG811_FILTER_AVERAGE = 1,
  MG811_FILTER_LOWPASS = 2,
};

#endif
