/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEB_CONTENT_H
#define WEB_CONTENT_H

#include <Arduino.h>

extern const char PROGMEM httpStatusCode200[];
extern const char PROGMEM httpStatusCode404[];
extern const char PROGMEM httpStatusCode405[];

extern const char PROGMEM webServerHeadBegin[];
extern const char PROGMEM webServerHeadBody[];
extern const char PROGMEM webServerBodyEnd[];

extern const char PROGMEM webServerFormBegin1[];
extern const char PROGMEM webServerFormBegin2[];
extern const char PROGMEM webServerFormInputBegin[];
extern const char PROGMEM webServerFormInputInput[];
extern const char PROGMEM webServerFormInputValue[];
extern const char PROGMEM webServerFormInputEnd[];
extern const char PROGMEM webServerFormEnd[];

extern const char PROGMEM webServerWifiFormCaption[];
extern const char PROGMEM webServerMG811FormCaption[];
extern const char PROGMEM webServerMiscFormCaption[];

extern const char PROGMEM webServerWifiFormParameterSsid[];
extern const char PROGMEM webServerWifiFormParameterPassword[];
extern const char PROGMEM webServerWifiFormParameterAuth[];

extern const char PROGMEM webServerWifiFormParameterDisplayNameSsid[];
extern const char PROGMEM webServerWifiFormParameterDisplayNamePassword[];
extern const char PROGMEM webServerWifiFormParameterDisplayNameAuth[];

extern const char PROGMEM webServerMG811FormParameterCalPoint0Ppm[];
extern const char PROGMEM webServerMG811FormParameterCalPoint0Adc[];
extern const char PROGMEM webServerMG811FormParameterCalPoint1Ppm[];
extern const char PROGMEM webServerMG811FormParameterCalPoint1Adc[];

extern const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint0Ppm[];
extern const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint0Adc[];
extern const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint1Ppm[];
extern const char PROGMEM webServerMG811FormParameterDisplayNameCalPoint1Adc[];

#endif
