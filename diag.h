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
#include "util_data.h"
#include "util_comm.h"
#include "webcc.h"

namespace diag {

#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value

/// String literals used internally by Diag module
class Texts {
    MODULE_TEXT(moduleName, "DiagLog");
    MODULE_TEXT(diagLogConsolePath, "/diag");
    MODULE_TEXT(diagLogJsonPath, "/diag.json");

    MODULE_TEXT(jsonParTimestamp, "timestamp");
    MODULE_TEXT(jsonParMessageNumber, "msgNo");
    MODULE_TEXT(jsonParLog, "log");

    MODULE_TEXT(jsonParMsgNo, "no");
    MODULE_TEXT(jsonParMsgTime, "time");
    MODULE_TEXT(jsonParMsgSev, "sev");
    MODULE_TEXT(jsonParMsgText, "msg");

} __attribute__((packed));

/// String literals used in UI and visible to user
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

    MODULE_TEXT(sendingJson,                  "Sending log in JSON format");
    MODULE_TEXT(sendingConsole,               "Sending console page");
    MODULE_TEXT(sendingFinished,              "Finished sending");

    MODULE_TEXT(consoleCaption,               "Diagnostic Log");

} __attribute__((packed));

extern const Texts PROGMEM texts;
extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

// Interval (in milliseconds) when Diagnostic Log Console sends HTTP
// requests in order to receive JSON with messages
#define DIAG_CONSOLE_UPDATE_DELAY 5000

// If defined, will produce HTML UI form with formatting, i.e. with
// tabs, line breaks and comments. Formatting increases size of the
// produced HTML UI form but simplifies debugging
#define HTML_FORMATTING

#ifdef HTML_FORMATTING
#define TAB "\t"
#define CRLF "\r\n"
#define CSSCOMMENT(comment) "/* " comment " */"
#define JSCOMMENT(comment) "// " comment
#else
#define TAB ""
#define CRLF ""
#define CSSCOMMENT(comment) ""
#define JSCOMMENT(comment) ""
#endif

#define HTML_CODE(name,value) const char name [sizeof(value)] = value

#define STRINGISATION(a) #a
#define STRINGISATION_WRAP(a) STRINGISATION(a)

/// @brief Contaings console HTML/JS code which will be sent to client via webserver
class DiagLogConsoleHTML {
  public:
    HTML_CODE(diagLogConsole,
              "<!DOCTYPE html>" CRLF
              "<html>" CRLF
              "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" CRLF
              "<head>" CRLF
              "<title>Diagnostic Log</title>" CRLF
              "<style type=\"text/css\" media=\"all\">" CRLF
              "html{overflow-y:scroll;}" CRLF
              "body{font-family:Arial;background-color:#D0FFD0;color:#000000;font-size:85%;}" CRLF
              "h1{text-align:center;font-size:130%;font-weight:bold;}" CRLF
              "textarea{color:#000000;background-color:#F0FFF0;width:95%;resize:none;}" CRLF
              "</style>" CRLF
              "<script type=\"text/javascript\">" CRLF
              "/*<![CDATA[*/" CRLF
              "var currentMessageNumber = 0;" CRLF
              "function RefreshConsole(){" CRLF
              TAB "var xmlhttp = new XMLHttpRequest();" CRLF
              TAB "xmlhttp.open('GET', 'http://192.168.4.1/diag.json', true);" CRLF
              TAB "xmlhttp.send();" CRLF
              TAB "xmlhttp.onreadystatechange = ProcessRequest;" CRLF
              TAB "function ProcessRequest(e){" CRLF
              TAB TAB "if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {" CRLF
              TAB TAB TAB "var text = xmlhttp.responseText;" CRLF
              TAB TAB TAB "var jsonObj = JSON.parse(text);" CRLF
              TAB TAB TAB "var index;" CRLF
              TAB TAB TAB "for (index = 0; index < jsonObj.log.length; ++index) {" CRLF
              TAB TAB TAB TAB "if (!Number(jsonObj.log[index].no) && currentMessageNumber>=0xFFFFFFF0) currentMessageNumber = 0;" CRLF
              TAB TAB TAB TAB "if (Number(jsonObj.log[index].no) > currentMessageNumber) {" CRLF
              TAB TAB TAB TAB TAB "currentMessageNumber = Number(jsonObj.log[index].no);" CRLF
              TAB TAB TAB TAB TAB "document.getElementById(\"diaglog\").value += '['+jsonObj.log[index].no+\']'" CRLF
              TAB TAB TAB TAB TAB "document.getElementById(\"diaglog\").value += '['+jsonObj.log[index].time+']'" CRLF
              TAB TAB TAB TAB TAB "document.getElementById(\"diaglog\").value += '['+jsonObj.log[index].sev+']'" CRLF
              TAB TAB TAB TAB TAB "document.getElementById(\"diaglog\").value += jsonObj.log[index].msg+'\\r\\n';" CRLF
              TAB TAB TAB TAB "}" CRLF
              TAB TAB TAB "}" CRLF
              TAB TAB "setTimeout(RefreshConsole," STRINGISATION_WRAP(DIAG_CONSOLE_UPDATE_DELAY) ");" CRLF
              TAB TAB "}" CRLF
              TAB "}" CRLF
              "}" CRLF
              CRLF
              "function Init(){" CRLF
              TAB "document.getElementById(\"diaglog\").value=\"\";" CRLF
              TAB "RefreshConsole();" CRLF
              "}" CRLF
              "/*]]>*/" CRLF
              "</script >" CRLF
              "</head>" CRLF
              "<body onload=\"Init();\">" CRLF
              "<h1>Diagnostic Log</h1>" CRLF
              "<div align=center>" CRLF
              "<textarea id=\"diaglog\" rows=\"25\" cols=\"80\" readonly>" CRLF
              "</textarea>" CRLF
              "</div>" CRLF
              "</body>" CRLF
              "</html>" CRLF
             );
} __attribute__((packed));

extern const DiagLogConsoleHTML PROGMEM diagLogConsoleHTML;

#undef HTML_FORMATTING
#undef TAB
#undef CRLF
#undef CSSCOMMENT
#undef JSCOMMENT

#undef HTML_CODE

#undef STRINGISATION
#undef STRINGISATION_WRAP

typedef uint32_t MessageNumber;
typedef uint32_t MessageTimestamp;

template <size_t StorageBufferSize = 3200> class DiagLogStorage;

/// @brief Diagnostic log
/// @tparam Storage Storage for messages, see DiagLogStorage.
/// @tparam LogSeparatorChar Character used to separate parts of the message (timestamp, severity, etc)
/// @details Forwards, stores and outputs on request the diagnostic messages supplied by other modules
template <class Storage = DiagLogStorage<>, char LogSeparatorChar = '|'>
class DiagLog : public Module<DiagLog<Storage>> {
  public:
    /// Message severity
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
    inline const char * PROGMEM moduleName (void);
    inline const char * PROGMEM getMainPath(void);
  public:
    inline boolean onHTTPReqStart(void);
    inline boolean onHTTPReqPath(const char * path);
    inline boolean onHTTPReqMethod(const char * method);
    boolean onRespond(Print &client);
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
  private:
    boolean httpConsolePath = false;
    boolean httpJsonPath = false;
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
  if (output) output->println(messageBuffer);
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
  destination.print(currentPart);
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

/// @brief Returns human-readable module name as a c-string in PROGMEM.
template <class Storage, char LogSeparatorChar>
const char * PROGMEM DiagLog<Storage, LogSeparatorChar>::moduleName (void) {
  return (texts.moduleName);
}

/// @brief Returns default webserver path for this module, implements interface method ModuleWebServer::getMainPath().
template <class Storage, char LogSeparatorChar>
const char * PROGMEM DiagLog<Storage, LogSeparatorChar>::getMainPath (void) {
  return (texts.diagLogConsolePath);
}

/// @brief Interface to integrate into webserver, implements interface method ModuleWebServer::onHTTPReqStart()
template <class Storage, char LogSeparatorChar>
boolean DiagLog<Storage, LogSeparatorChar>::onHTTPReqStart(void) {
  httpConsolePath = false;
  httpJsonPath = false;
  return (true);
}

/// @brief Interface to integrate into webserver, implements interface method ModuleWebServer::onHTTPReqPath()
template <class Storage, char LogSeparatorChar>
boolean DiagLog<Storage, LogSeparatorChar>::onHTTPReqPath(const char * path) {
  httpConsolePath = !strcmp_P(path, texts.diagLogConsolePath);
  httpJsonPath = !strcmp_P(path, texts.diagLogJsonPath);
  return (httpConsolePath || httpJsonPath);
}

/// @brief Interface to integrate into webserver, implements interface method ModuleWebServer::onHTTPReqMethod()
template <class Storage, char LogSeparatorChar>
boolean DiagLog<Storage, LogSeparatorChar>::onHTTPReqMethod(const char * method) {
  return (util::http::HTTPRequestHelper::getMethod(method) == util::http::HTTPRequestMethod::GET);
}

/// @brief Interface to integrate into webserver, implements interface method ModuleWebServer::onRespond()
template <class Storage, char LogSeparatorChar>
boolean DiagLog<Storage, LogSeparatorChar>::onRespond(Print & client) {
  if (httpConsolePath) {
    util::http::HTTPResponseHeader::contentHeader(client, util::http::HTTPContentType::HTML);
    client.print(FPSTR(diagLogConsoleHTML.diagLogConsole));
    return (true);
  }
  if (httpJsonPath) {
    util::http::HTTPResponseHeader::contentHeader(client, util::http::HTTPContentType::JSON);
    util::json::JSONOutput json (client);
    json.value(FPSTR(texts.jsonParTimestamp), millis());
    json.value(FPSTR(texts.jsonParMessageNumber), static_cast<unsigned long>(messageNumber) - 1);
    json.beginArray(FPSTR(texts.jsonParLog));
    size_t numberOfMessages = storage.count();
    char tokenStr[2] = {};
    tokenStr[0] = LogSeparatorChar;
    for (size_t i = 0; i < numberOfMessages; i++) {
      storage.recall(i, messageBuffer, maxMessageSize);
      json.beginObject();
      char * messagePart = strtok(messageBuffer, tokenStr);
      json.value(FPSTR(texts.jsonParMsgNo), messagePart);
      messagePart = strtok(NULL, tokenStr);
      json.value(FPSTR(texts.jsonParMsgTime), messagePart);
      messagePart = strtok(NULL, tokenStr);
      json.value(FPSTR(texts.jsonParMsgSev), messagePart);
      messagePart = strtok(NULL, tokenStr);
      json.value(FPSTR(texts.jsonParMsgText), messagePart);
      json.finish();
    }
    json.~JSONOutput();
    return (true);
  }
  return (false);
}

/// @brief Storage for messages in diagnositic log
/// @details When there is not enought memory to store an incoming message,
/// the oldest stored messages are deleted to free enough memory for an
/// incoming message.
/// @tparam StorageBufferSize Size of buffer for storing messages, in chars
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

/// @brief Stores a message
/// @param message Cstring which contains message text
template <size_t StorageBufferSize>
boolean DiagLogStorage<StorageBufferSize>::store(char * message) {
  return (storageRingBuffer.push(message));
}

/// @brief Returns number of messages in storage
/// @return Number of messages currently in storage
template <size_t StorageBufferSize>
size_t DiagLogStorage<StorageBufferSize>::count(void) {
  return (storageRingBuffer.count());
}

/// @brief Copies particular message from storage into supplied buffer
/// @param index Index of message in the storage, in range 0 to count()
/// @param buffer Buffer to copy a message to
/// @param bufferSize Length of the buffer to copy message to, in chars
/// @return Number of chars copied into buffer
template <size_t StorageBufferSize>
size_t DiagLogStorage<StorageBufferSize>::recall(size_t index, char *buffer, size_t bufferSize) {
  return (storageRingBuffer.get(index, buffer, bufferSize));
}

}; //namespace diag

#endif
