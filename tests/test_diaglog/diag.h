/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
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
#include "util.h"

namespace diag {

#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value

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

    MODULE_TEXT(messageNumberRollover,        "Message number rollover");
} __attribute__((packed));

extern const Texts PROGMEM texts;

extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

template <size_t StorageBufferSize = 3200> class DiagLogStorage;

template <class Storage = DiagLogStorage<>, char LogSeparatorChar = '|'>
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
    template <typename CurrentPart> inline void printMessagePart(Print &destination, const CurrentPart currentPart);
    template <typename CurrentPart, typename... MessageParts> inline void printMessagePart(Print &destination, const CurrentPart currentPart, const MessageParts... messageParts);
  private:
    static const Severity minAllowedSeverityFilter = Severity::CRITICAL;
    Severity severityFilter = Severity::DEBUG;
  private:
    inline const __FlashStringHelper * severityString(Severity severity);
  private:
    Storage storage;
  private:
    static const size_t maxMessageSize = 256;
    char messageBuffer[maxMessageSize];
};

template <class Storage, char LogSeparatorChar>
template <typename... MessageParts> uint32_t DiagLog<Storage, LogSeparatorChar>::log(Severity severity, const MessageParts... messageParts) {
  /// @brief Logs the message
  /// @details This method is used by other modules to send messages to Diagnostic Log
  /// @details Usage example:
  /// @details     diag::DiagLog<>::instance()->log(diag::DiagLog<>::Severity::DEBUG, "My message");
  /// @details Message may also consist of multiple parts:
  /// @details
  ///     int myValue = 0;
  ///     diag::DiagLog<>::instance()->log(diag::DiagLog<>::Severity::DEBUG, "Int value: ", myValue, F(" float value:"), 1.55);
  /// @details Message is checked against severity filter setting and discarded if its
  /// severity exceeds (less severe than) severity filter setting.
  /// @details Message number and timestamp (current millis() value) are automatically
  /// appended to the message.
  /// @details Then message is printed to the output and sent to storage.
  /// @details If the message number is about to roll over (i.e. incremented from 0xFFFFFFFF
  /// to 0x00000000), this is automatically noted in a message with severity CRITICAL.
  /// @param severity Message severity.
  /// @param messageParts Parameter pack which consists of all message parts.
  if (static_cast<uint8_t>(severity) > static_cast<uint8_t>(severityFilter)) return (messageNumber);
  MessageTimestamp timestamp = millis();
  util::arrays::PrintToBuffer message(messageBuffer, maxMessageSize);
  message.print(messageNumber);
  message.print(LogSeparatorChar);
  message.print(timestamp);
  message.print(LogSeparatorChar);
  message.print(severityString(severity));
  message.print(LogSeparatorChar);
  //TODO: print/render message to buffer, then print whole message from buffer and send buffer contents to storage
  printMessagePart(message, messageParts...);
  if (output) {
    output->print(messageBuffer);
  }
  storage.store(messageBuffer);
  if (messageNumber == 0xFFFFFFFF) {
    messageNumber++;
    return (log(Severity::CRITICAL, FPSTR(textsUI.messageNumberRollover)));
  }
  return (messageNumber++);
}

/// @brief Sets message forwarding output.
/// @param newOutput Print instance where messages will be forwarded.
template <class Storage, char LogSeparatorChar>
void DiagLog<Storage, LogSeparatorChar>::setPrintOutput(Print &newOutput) {
  output = &newOutput;
}

/// @brief Disables message forwarding to Print output.
template <class Storage, char LogSeparatorChar>
void DiagLog<Storage, LogSeparatorChar>::disablePrintOutput(void) {
  output = NULL;
}

/// @brief Get current setting of the message severity filter.
/// @details Messages with the severity exceeding (less severe than)
/// severity filter setting will be ignored. They will not be stored
/// or forwarded to the output.
/// @details Messages with severity exactly as specified by severity
/// filter will be stored and forwarded to the output.
/// @return Current setting of severity filter.
template <class Storage, char LogSeparatorChar>
typename DiagLog<Storage, LogSeparatorChar>::Severity DiagLog<Storage, LogSeparatorChar>::getSeverityFilter(void) const {
  return (severityFilter);
}

/// @brief Set message severity filter.
/// @details Messages with the severity exceeding (less severe than)
/// severity filter setting will be ignored. They will not be stored
/// or forwarded to the output.
/// @details Messages with severity exactly as specified by severity
/// filter will be stored and forwarded to the output.
/// @param leastSeverityAllowed Messages with severity exceeding this
/// value (less severe) will be ignored.
template <class Storage, char LogSeparatorChar>
void DiagLog<Storage, LogSeparatorChar>::setSeverityFilter(DiagLog<Storage, LogSeparatorChar>::Severity leastSeverityAllowed) {
  severityFilter = leastSeverityAllowed;
  if (static_cast<uint8_t>(leastSeverityAllowed) < static_cast<uint8_t>(minAllowedSeverityFilter)) severityFilter = minAllowedSeverityFilter;
}

/// @brief Prints last part of message from variadic template
/// parameter pack.
/// @details Terminates the parameter pack expansion and prints last
/// part of the message from parameter pack.
/// @param currentPart Part of the message to print.
template <class Storage, char LogSeparatorChar>
template <typename CurrentPart>
void DiagLog<Storage, LogSeparatorChar>::printMessagePart(Print &destination, const CurrentPart currentPart) {
  destination.println(currentPart);
}

/// @brief Prints non-last part of message from variadic template parameter
/// pack.
/// @details Recursively expands the variadic template parameter pack and prints
/// the message parts one at a time.
/// @param currentPart Current part of the message to be printed.
/// @param messageParts... Rest of the message parts in a parameter pack.
template <class Storage, char LogSeparatorChar>
template <typename CurrentPart, typename... MessageParts>
void DiagLog<Storage, LogSeparatorChar>::printMessagePart(Print &destination, const CurrentPart currentPart, const MessageParts... messageParts) {
  destination.print(currentPart);
  printMessagePart(destination, messageParts...);
}

/// @brief Returns human-readable string for the specified message severity.
/// @param severity Message severity.
/// @return C-string in PROGMEM message severity designation.
template <class Storage, char LogSeparatorChar>
const __FlashStringHelper * DiagLog<Storage, LogSeparatorChar>::severityString(Severity severity) {
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

template <size_t StorageBufferSize>
class DiagLogStorage {
  public:
    inline DiagLogStorage();
  public:
    inline boolean store(char * message);
    inline size_t count(void);
    inline size_t recall(size_t index, char *buffer, size_t bufferSize);
  private:
    char storageBuffer[StorageBufferSize];
    util::arrays::CStrRingBuffer storageRingBuffer;
};

template <size_t StorageBufferSize>
DiagLogStorage<StorageBufferSize>::DiagLogStorage() : storageRingBuffer(storageBuffer, StorageBufferSize) {
}

template <size_t StorageBufferSize>
boolean DiagLogStorage<StorageBufferSize>::store(char * message) {
  return (storageRingBuffer.push(message));
}

template <size_t StorageBufferSize>
size_t DiagLogStorage<StorageBufferSize>::count(void) {
  return (storageRingBuffer.count());
}

template <size_t StorageBufferSize>
size_t DiagLogStorage<StorageBufferSize>::recall(size_t index, char *buffer, size_t bufferSize) {
  return (storageRingBuffer.get(index, buffer, bufferSize));
}

}; //namespace diag;
#endif

