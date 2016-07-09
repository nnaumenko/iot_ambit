/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCONFIG_H
#define WEBCONFIG_H

#include <Arduino.h>
#include "http.h"
#include "stringmap.h"
#include "web_content.h"

void webConfigBegin(void);
void webConfigRun(void);

#endif
