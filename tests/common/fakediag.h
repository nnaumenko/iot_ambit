/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef FAKEDIAG_H
#define FAKEDIAG_H

#include <Arduino.h>
#include "module.h"

class FakeDiag {
  public:
    ///Message severity, copied from diag.h with no changes
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
    const char * getLogEntry (size_t entry) {
      if (entry > messageNo) return (NULL);
      return (storedLog[entry]);
    }
    Severity getSeverity(size_t entry) {
      if (entry > messageNo) return (Severity::CRITICAL);
      return (storedLogSeverities[entry]);
    }
    uint32_t getTimestamp(size_t entry) {
      if (entry > messageNo) return (0);
      return (storedLogTimestamps[entry]);
    }
  public:
    static const size_t logEntries = 10; //how many log entries to store
    static const size_t charsPerLogEntry = 200; //how many chars to store per log entry
  private:
    size_t messageNo = 0;
    template <typename CurrentPart> inline void printMessagePart(const CurrentPart currentPart);
    template <typename CurrentPart, typename... MessageParts> inline void printMessagePart(const CurrentPart currentPart, const MessageParts... messageParts);
  private:
    char storedLog[logEntries][charsPerLogEntry] = {};
    Severity storedLogSeverities[logEntries] = {};
    uint32_t storedLogTimestamps[logEntries] = {};
  private:
    class PrintToBuffer : public Print {
      public:
        virtual size_t write(uint8_t character) {
          if (!buffer || !bufferSize) return 0;
          static const size_t NULL_TERMINATOR_SIZE = 1;
          static const char NULL_TERMINATOR = '\0';
          if (bufferPosition < (bufferSize - NULL_TERMINATOR_SIZE)) {
            buffer[bufferPosition++] = character;
            buffer[bufferPosition] = NULL_TERMINATOR;
          }
          return (1);
        }
      public:
        void setBuffer(char * setBuffer, size_t setBufferSize) {
          if (setBuffer && setBufferSize) {
            buffer = reinterpret_cast<uint8_t*>(setBuffer);
            bufferSize = setBufferSize;
            bufferPosition = 0;
          }
        }
      private:
        size_t bufferSize = 0;
        size_t bufferPosition = 0;
        uint8_t * buffer = NULL;
    };
    PrintToBuffer printToBuffer;
};

template <typename... MessageParts> uint32_t FakeDiag::log(Severity severity, const MessageParts... messageParts) {
  if (messageNo >= logEntries) return (messageNo);
  const uint32_t timestamp = millis();
  storedLogSeverities[messageNo] = severity;
  storedLogTimestamps[messageNo] = timestamp;
  printToBuffer.setBuffer(storedLog[messageNo], charsPerLogEntry);
  printMessagePart(messageParts...);
  return (messageNo++);
}

template <typename CurrentPart> void FakeDiag::printMessagePart(const CurrentPart currentPart) {
  printToBuffer.print(currentPart);
}

template <typename CurrentPart, typename... MessageParts> void FakeDiag::printMessagePart(const CurrentPart currentPart, const MessageParts... messageParts) {
  printToBuffer.print(currentPart);
  printMessagePart(messageParts...);
}

class TestFakeDiag {
  public:
    static void log_variousParameters_expectCorrectParametersStored(void);
    static void log_multipleEntries_expectCorrectEntriesStored(void);
    static void log_inputStringExceedsMaxLength_expectStoredStringTruncated(void);
    static void log_inputTooManyStrings_expectMaxStringsPerLimit(void);
  public:
    static void runTests(void);
};

#endif
