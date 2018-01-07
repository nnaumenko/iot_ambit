/*
 * Copyright (C) 2016-2018 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
* @file
* @brief WiFi module.
*
* WiFi module connects to access point or creates one.
*/

#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include "module.h"

namespace wifi {

#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value

/// String literals used internally by WiFi module
class Texts {
    MODULE_TEXT(moduleName, "WiFi");
} __attribute__((packed));

/// String literals used in UI and visible to user
class TextsUI {
    MODULE_TEXT(placeholder, " ");
} __attribute__((packed));

extern const Texts PROGMEM texts;
extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

template <class Diag>
class WiFiModule : public Module <WiFiModule<Diag>> {
  public:
    inline const char * PROGMEM moduleName (void);
    inline ModuleId moduleId(void);
};

template <class Diag>
const char * PROGMEM WiFiModule<Diag>::moduleName (void) {
  /// @return Human-readable module name
  return (texts.moduleName);
}

template <class Diag>
ModuleId WiFiModule<Diag>::moduleId (void) {
  /// @return Numeric module identifier
  return (ModuleIdWiFi);
}

} //namespace wifi

#endif
