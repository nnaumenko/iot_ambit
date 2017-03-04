/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
* @file
* @brief Diagnostic log.
*
* Diagnostic log processes, filters, stores and outputs messages.
*/

#ifndef DIAG_H
#define DIAG_H

#include <Arduino.h>
#include "module.h"

namespace diag {

#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value
#define FSH(name) reinterpret_cast<const __FlashStringHelper *>(name)

typedef uint32_t MessageNumber;
typedef uint32_t MessageTimestamp;

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

    MODULE_TEXT(messageNumberRollover, "Message number rollover");
} __attribute__((packed));

extern const Texts PROGMEM texts;

extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

class DiagLogStorageDummy;

template <class Storage = DiagLogStorageDummy>
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
    template <typename... MessageParts> MessageNumber log(Severity severity, const MessageParts... messageParts);
  public:
    inline Severity getSeverityFilter(void) const;
    inline void setSeverityFilter(Severity leastSeverityAllowed = Severity::DEBUG);
  public:
    inline void setPrintOutput(Print &output);
    inline void disablePrintOutput(void);
  public:
    const char * PROGMEM moduleName (void) {
      /// Returns human-readable module name as a c-string in PROGMEM.
      return (texts.moduleName);
    }
  private:
    Print * output = NULL;
    MessageNumber messageNumber = 0;
    template <typename CurrentPart> inline void printMessagePart(const CurrentPart currentPart);
    template <typename CurrentPart, typename... MessageParts> inline void printMessagePart(const CurrentPart currentPart, const MessageParts... messageParts);
  private:
    static const Severity minAllowedSeverityFilter = Severity::CRITICAL;
    Severity severityFilter = Severity::DEBUG;
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
  /// @param[in] severity Message severity.
  /// @param[in] messageParts Parameter pack which consists of all message parts.
  if (static_cast<uint8_t>(severity) > static_cast<uint8_t>(severityFilter)) return (messageNumber);
  MessageTimestamp timestamp = millis();
  if (output) {
    output->print('[');
    output->print(messageNumber);
    output->print(']');
    output->print('[');
    output->print(timestamp);
    output->print(']');
    output->print('[');
    output->print(severityString(severity));
    output->print(']');
  }
  printMessagePart(messageParts...);
  if (messageNumber == 0xFFFFFFFF) {
    messageNumber++;
    return (log(Severity::CRITICAL, FSH(textsUI.messageNumberRollover)));
  }
  return (messageNumber++);
}

/// @brief Sets message forwarding output.
/// @param[in] newOutput Print instance where messages will be forwarded.
template <class Storage>
void DiagLog<Storage>::setPrintOutput(Print &newOutput) {
  output = &newOutput;
}

/// @brief Disables message forwarding to Print output.
template <class Storage>
void DiagLog<Storage>::disablePrintOutput(void) {
  output = NULL;
}

/// @brief Get current setting of the message severity filter.
///
/// Messages with the severity exceeding (less severe than) severity
/// filter setting will be ignored. They will not be stored or forwarded
/// to the output.
///
/// Messages with severity exactly as specified by severity filter will be
/// stored and forwarded to the output.
/// @return Current setting of severity filter.
template <class Storage>
typename DiagLog<Storage>::Severity DiagLog<Storage>::getSeverityFilter(void) const {
  return (severityFilter);
}

/// @brief Set message severity filter.
///
/// Messages with the severity exceeding (less severe than) severity
/// filter setting will be ignored. They will not be stored or forwarded
/// to the output.
///
/// Messages with severity exactly as specified by severity filter will be
/// stored and forwarded to the output.
/// @param[in] leastSeverityAllowed Messages with severity exceeding this
/// value (less severe) will be ignored.
template <class Storage>
void DiagLog<Storage>::setSeverityFilter(DiagLog<Storage>::Severity leastSeverityAllowed) {
  severityFilter = leastSeverityAllowed;
  if (static_cast<uint8_t>(leastSeverityAllowed) < static_cast<uint8_t>(minAllowedSeverityFilter)) severityFilter = minAllowedSeverityFilter;
}

/// @brief Prints last part of message from variadic template
/// parameter pack.
///
/// Terminates the parameter pack expansion and prints last
/// part of the message from parameter pack.
/// @param[in] currentPart Part of the message to print.
template <class Storage>
template <typename CurrentPart> void DiagLog<Storage>::printMessagePart(const CurrentPart currentPart) {
  if (output) output->println(currentPart);
}

/// @brief Prints non-last part of message from variadic template parameter
/// pack.
///
/// Recursively expands the variadic template parameter pack and prints
/// the message parts one at a time.
/// @param[in] currentPart Current part of the message to be printed.
/// @param[in] messageParts... Rest of the message parts in a parameter pack.
template <class Storage>
template <typename CurrentPart, typename... MessageParts> void DiagLog<Storage>::printMessagePart(const CurrentPart currentPart, const MessageParts... messageParts) {
  if (output) output->print(currentPart);
  printMessagePart(messageParts...);
}

/// @brief Returns human-readable string for the specified message severity.
/// @param[in] severity Message severity.
/// @return C-string in PROGMEM message severity designation.
template <class Storage>
const __FlashStringHelper * DiagLog<Storage>::severityString(Severity severity) {
  switch (severity) {
    case Severity::EMERGENCY:
      return (FSH(textsUI.messageSeverityEmergency));
    case Severity::ALERT:
      return (FSH(textsUI.messageSeverityAlert));
    case Severity::CRITICAL:
      return (FSH(textsUI.messageSeverityCritical));
    case Severity::ERROR:
      return (FSH(textsUI.messageSeverityError));
    case Severity::WARNING:
      return (FSH(textsUI.messageSeverityWarning));
    case Severity::NOTICE:
      return (FSH(textsUI.messageSeverityNotice));
    case Severity::INFORMATIONAL:
      return (FSH(textsUI.messageSeverityInformational));
    case Severity::DEBUG:
      return (FSH(textsUI.messageSeverityDebug));
    default:
      return (FSH(textsUI.messageSeverityUndefined));
  }
}

template <size_t bufferSize = 4096>
class DiagLogStorage {
  public:
    DiagLogStorage();
    ~DiagLogStorage();
    boolean validate(void) const;
  public:
    Print * messageBegin(uint32_t number, uint32_t timestamp, uint8_t severity);
    void messageEnd(void);
  private:
    char * buffer = NULL;
  private:
};

template <size_t bufferSize>
DiagLogStorage<bufferSize>::DiagLogStorage() {
  buffer = malloc(bufferSize);
}

template <size_t bufferSize>
DiagLogStorage<bufferSize>::~DiagLogStorage() {
  free(bufferSize);
}

template <size_t bufferSize>
boolean DiagLogStorage<bufferSize>::validate(void) const {
  return (buffer != NULL);
}

#undef FSH

};

#endif
