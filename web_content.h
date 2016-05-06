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

extern const char PROGMEM htmlConfigBodyBegin[];
extern const char PROGMEM htmlConfigBodyEnd[];

extern const char PROGMEM htmlConfigSectionBegin1[];
extern const char PROGMEM htmlConfigSectionBegin2[];
extern const char PROGMEM htmlConfigSectionEnd[];

extern const char PROGMEM htmlConfigSubsectionBegin1[];
extern const char PROGMEM htmlConfigSubsectionBegin2[];
extern const char PROGMEM htmlConfigSubsectionEnd[];

extern const char PROGMEM htmlConfigSave[];

extern const char PROGMEM htmlConfigTooltipBegin[];
extern const char PROGMEM htmlConfigTooltipEnd[];

extern const char PROGMEM htmlConfigParameterTextPart1[];
extern const char PROGMEM htmlConfigParameterTextPart2[];
extern const char PROGMEM htmlConfigParameterTextPart3[];
extern const char PROGMEM htmlConfigParameterTextPart4[];
extern const char PROGMEM htmlConfigParameterTextPart5[];

extern const char PROGMEM htmlConfigParameterSelectPart1[];
extern const char PROGMEM htmlConfigParameterSelectPart2[];
extern const char PROGMEM htmlConfigParameterSelectPart3[];
extern const char PROGMEM htmlConfigParameterSelectPart4[];
extern const char PROGMEM htmlConfigParameterSelectPart5[];

extern const char PROGMEM htmlConfigParameterSelectOptionPart1[];
extern const char PROGMEM htmlConfigParameterSelectOptionPart2[];
extern const char PROGMEM htmlConfigParameterSelectOptionPart2Selected[];
extern const char PROGMEM htmlConfigParameterSelectOptionPart3[];

#endif
