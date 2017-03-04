/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file
 * @brief Diagnostic log
 *
 * Diagnostic log processes, filters, stores and outputs messages
 */

#ifndef DIAG_H
#define DIAG_H

#include <Arduino.h>
#include "module.h"

namespace diag {

#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value

class Texts {
    MODULE_TEXT(moduleName, "DiagLog");
} __attribute__((packed));

class TextsUI {
    MODULE_TEXT(messageSeverityEmergency,     "Emergency");
    MODULE_TEXT(messageSeverityAlert,         "Alert");
    MODULE_TEXT(messageSeverityCritical,      "Critical");
    MODULE_TEXT(messageSeverityError,         "Error");
    MODULE_TEXT(messageSeverityWarning,       "Warning");
    MODULE_TEXT(messageSeverityNotice,        "Notice");
    MODULE_TEXT(messageSeverityInformational, "Informational");
    MODULE_TEXT(messageSeverityDebug,         "Debug");
    MODULE_TEXT(messageSeverityUndefined,     "Undefined Message Severity");
} __attribute__((packed));

extern const Texts PROGMEM texts;

extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

class DiagLogStorage;

template <class Storage = DiagLogStorage>
class DiagLog : public Module<DiagLog<Storage>> {
  public:
    ///Message severity
    enum class Severity {
      EMERGENCY = 0,    ///<System is unusable
      ALERT = 1,        ///<Should be corrected immediately
      CRITICAL = 2,     ///<Critical conditions
      ERROR = 3,        ///<Error conditions
      WARNING = 4,      ///<Warning conditions; error might occur if no corrective action taken
      NOTICE = 5,       ///<Normal but significant condition; unusual events but no error
      INFORMATIONAL = 6,///<Informational messages; related to normal operation
      DEBUG = 7         ///<Debug-level messages
    };
  public:
    template <typename... MessageParts> uint32_t log(Severity severity, const MessageParts... messageParts);
  public:
    inline void setPrintOutput(Print &output);
    inline void setPrintOutput(void);
  public:
    const char * PROGMEM moduleName (void) {
      return (texts.moduleName);
    }
  private:
    Print * output = NULL;
    uint32_t messageNo = 0;
    template <typename CurrentPart> inline void printMessagePart(const CurrentPart currentPart);
    template <typename CurrentPart, typename... MessageParts> inline void printMessagePart(const CurrentPart currentPart, const MessageParts... messageParts);
  private:
    const __FlashStringHelper * severityString(Severity severity);
};

template <class Storage>
template <typename... MessageParts> uint32_t DiagLog<Storage>::log(Severity severity, const MessageParts... messageParts) {
  /// @brief Logs the message.
  ///
  /// This method is used by other modules to send messages to Diagnostic Log.
  ///
  /// Usage example:
  ///
  ///     diag::DiagLog::instance()->log(diag::DiagLog::Severity::DEBUG, "My message");
  ///
  /// Message may also consist of multiple parts:
  ///
  ///     int myValue = 0;
  ///     diag::DiagLog::instance()->log(diag::DiagLog::Severity::DEBUG, "Int value: ", myValue, F(" float value:"), 1.55);
  ///
  /// Message is checked against severity filter setting and discarded if its
  /// severity exceeds (less severe than) severity filter setting.
  ///
  /// Message number and timestamp (current millis() value) are automatically
  /// appended to the message.
  ///
  /// Then message is printed to the output and sent to storage.
  ///
  /// If the message number is about to roll over (i.e. incremented from 0xFFFFFFFF
  /// to 0x00000000), this is automatically noted in a message with severity CRITICAL.
  ///
  /// @param severity Message severity.
  /// @param messageParts Parameter pack which consists of all message parts.
  const uint32_t timestamp = millis();
  if (output) {
    output->print('[');
    output->print(messageNo);
    output->print(']');
    output->print('[');
    output->print(timestamp);
    output->print(']');
    output->print('[');
    output->print(severityString(severity));
    output->print(']');
  }
  printMessagePart(messageParts...);
  return (messageNo++);
}

template <class Storage>
void DiagLog<Storage>::setPrintOutput(Print &newOutput) {
  output = &newOutput;
}

template <class Storage>
void DiagLog<Storage>::setPrintOutput(void) {
  output = NULL;
}

template <class Storage>
template <typename CurrentPart> void DiagLog<Storage>::printMessagePart(const CurrentPart currentPart) {
  if (output) output->println(currentPart);
}

template <class Storage>
template <typename CurrentPart, typename... MessageParts> void DiagLog<Storage>::printMessagePart(const CurrentPart currentPart, const MessageParts... messageParts) {
  if (output) output->print(currentPart);
  printMessagePart(messageParts...);
}

template <class Storage>
const __FlashStringHelper * DiagLog<Storage>::severityString(Severity severity) {
  switch (severity) {
    case Severity::EMERGENCY:
      return (FPSTR(textsUI.messageSeverityEmergency));
    case Severity::ALERT:
      return (FPSTR(textsUI.messageSeverityAlert));
    case Severity::CRITICAL:
      return (FPSTR(textsUI.messageSeverityCritical));
    case Severity::ERROR:
      return (FPSTR(textsUI.messageSeverityError));
    case Severity::WARNING:
      return (FPSTR(textsUI.messageSeverityWarning));
    case Severity::NOTICE:
      return (FPSTR(textsUI.messageSeverityNotice));
    case Severity::INFORMATIONAL:
      return (FPSTR(textsUI.messageSeverityInformational));
    case Severity::DEBUG:
      return (FPSTR(textsUI.messageSeverityDebug));
    default:
      return (FPSTR(textsUI.messageSeverityUndefined));
  }
}

class DiagLogStorage {
    //TODO
};

};

#endif
