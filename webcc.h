/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCC_H
#define WEBCC_H

#include <Arduino.h>
#include "http.h"
#include "stringmap.h"

void webConfigBegin(void);
void webConfigRun(void);

#endif
