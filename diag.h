/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef DIAG_H
#define DIAG_H

#include <Arduino.h>
#include "module.h"

namespace diag {

#ifdef MODULE_TEXT
#undef MODULE_TEXT
#endif
#define MODULE_TEXT(name,value) const char name [sizeof(value)] = value

#ifdef PTEXT
#undef PTEXT
#endif
#define PTEXT(name) (__FlashStringHelper *)name

class Texts {
  public:
    MODULE_TEXT(moduleName, "TempDiagLog");
} __attribute__((packed));

extern const Texts PROGMEM texts;

#undef MODULE_TEXT

class TempDiagLog : public Module <TempDiagLog>, public Print {
  public:
    void timestamp(void) {
      this->print('[');
      this->print(millis(), DEC);
      this->print(']');
    }
  public:
    virtual inline size_t write(uint8_t character) {
      if (!messageAccepted()) return (0);
      return (Serial.write(character));
    }
    virtual inline size_t write(const char * cstring) {
      if (!messageAccepted()) return (0);
      return (Serial.write(cstring));
    }
    virtual inline size_t write(const uint8_t *buffer, size_t size) {
      if (!messageAccepted()) return (0);
      return (Serial.write(buffer, size));
    }
  public:
    enum MessageClass {
      CLASS_CRITICAL = 1,
      CLASS_ERROR = 2,
      CLASS_VERBOSE = 3,
      CLASS_DEBUG = 4,
    };
    inline MessageClass getMessageClass (void) {
      return (currentMessageClass);
    }
    inline void setMessageClass(MessageClass newMessageClass) {
      currentMessageClass = newMessageClass;
    }
  private:
    inline boolean messageAccepted() {
      return (currentMessageClass <= logMessageClassAccepted);
    }
    MessageClass logMessageClassAccepted = CLASS_DEBUG;
    MessageClass currentMessageClass = CLASS_DEBUG;
  public:
    const char * PROGMEM moduleName (void) {
      return (texts.moduleName);
    }
};

};

#endif
