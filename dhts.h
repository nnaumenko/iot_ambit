/*
 * Copyright (C) 2016-2018 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/*
 * This code is based on Arduino library for DHT11DHT22, etc Temp &
 * Humidity Sensors by Adafruit Industries, distributed under MIT license
 * https://github.com/adafruit/DHT-sensor-library
 */

/**
 * @file
 * @brief Module for DHT sensors (temperature & humidity)
 */

#ifndef DHTS_H
#define DHTS_H

#include <arduino.h>

#include "module.h"
#include "util_data.h"

namespace dhts {

#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value

/// String literals used internally by DHT module
class Texts {
    MODULE_TEXT(moduleName, "DHT");
} __attribute__((packed));

/// String literals used in UI and visible to user
class TextsUI {
    MODULE_TEXT(placeholder, " ");
} __attribute__((packed));

extern const Texts PROGMEM texts;
extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

template <class Diag>
class DHTModule : public Module <DHTModule<Diag>> {
  public:
    inline const char * PROGMEM moduleName (void);
    inline ModuleId moduleId(void);
};

template <class Diag>
const char * PROGMEM DHTModule<Diag>::moduleName (void) {
  /// @return Human-readable module name
  return (texts.moduleName);
}

template <class Diag>
ModuleId DHTModule<Diag>::moduleId (void) {
  /// @return Numeric module identifier
  return (ModuleIdDHT);
}

}//namespace dht

#endif
