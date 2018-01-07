/*
 * Copyright (C) 2016-2018 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file
 * @brief Communication protocols and formats (e.g. HTTP, JSON, etc.)
 */

#ifndef UTIL_COMM_H
#define UTIL_COMM_H

#include <arduino.h>

namespace util {

namespace http {

/// HTTP status codes for HTTP Response
enum class HTTPStatusCode {
  UNKNOWN = 0,                            ///< Reserved for the case when status code other than defined here is encountered
  CONTINUE = 100,                         ///< 100 CONTINUE
  SWITCHING_PROTOCOLS = 101,              ///< 101 SWITCHING PROTOCOLS
  OK = 200,                               ///< 200 OK
  CREATED = 201,                          ///< 201 CREATED
  ACCEPTED = 202,                         ///< 202 ACCEPTED
  NON_AUTHORITATIVE_INFORMATION = 203,    ///< 203 NON AUTHORITATIVE INFORMATION
  NO_CONTENT = 204,                       ///< 204 NO CONTENT
  RESET_CONTENT = 205,                    ///< 205 RESET CONTENT
  PARTIAL_CONTENT = 206,                  ///< 206 PARTIAL CONTENT
  MULTIPLE_CHOISES = 300,                 ///< 300 MULTIPLE CHOISES
  MOVED_PERMANENTLY = 301,                ///< 301 MOVED PERMANENTLY
  FOUND = 302,                            ///< 302 FOUNT
  SEE_OTHER = 303,                        ///< 303 SEE OTHER
  NOT_MODIFIED = 304,                     ///< 304 NOT MODIFIED
  USE_PROXY = 305,                        ///< 305 USE PROXY
  SWITCH_PROXY = 306,                     ///< 306 SWITCH PROXY
  TEMPORARY_REDIRECT = 307,               ///< 307 TEMPORARY REDIRECT
  PERMANENT_REDIRECT = 308,               ///< 308 PERMANENT REDIRECT
  BAD_REQUEST = 400,                      ///< 400 BAD REQUEST
  UNAUTHORIZED = 401,                     ///< 401 UNAUTHORIZED
  FORBIDDEN = 403,                        ///< 403 FORBIDDEN
  NOT_FOUND = 404,                        ///< 404 NOT FOUND
  METHOD_NOT_ALLOWED = 405,               ///< 405 METHOD NOT ALLOWED
  NOT_ACCEPTABLE = 406,                   ///< 406 NOT ACCEPTABLE
  PROXY_AUTHENTICATION_REQUIRED = 407,    ///< 407 PROXY AUTHENTICATION FAILED
  REQUEST_TIMEOUT = 408,                  ///< 408 REQUEST TIMEOUT
  CONFLICT = 409,                         ///< 409 CONFLICT
  GONE = 410,                             ///< 410 GONE
  LENGTH_REQUIRED = 411,                  ///< 411 LENGTH REQUIRED
  PRECONDITION_FAILED = 412,              ///< 412 PRECONDITION FAILED
  PAYLOAD_TOO_LARGE = 413,                ///< 413 PAYLOAD TOO LARGE
  REQUEST_URI_TOO_LONG = 414,             ///< 414 REQUEST URI TOO LONG
  UNSUPPORTED_MEDIA_TYPE = 415,           ///< 415 UNSUPPORTED MEDIA TYPE
  RANGE_NOT_SATISFIABLE = 416,            ///< 416 RANGE NOT SATISFIABLE
  EXPECTATION_FAILED = 417,               ///< 417 EXPECTATION FAILED
  I_M_A_TEAPOT = 418,                     ///< 418 I'M A TEAPOT
  MISDIRECTED_REQUEST = 421,              ///< 419 MISDIRECTED REQUEST
  UPGRADE_REQUIRED = 426,                 ///< 426 UPGRADE REQUIRED
  PRECONDITION_REQUIRED = 428,            ///< 428 PRECONDITION REQUIRED
  TOO_MANY_REQUESTS = 429,                ///< 429 TOO MANY REQUESTS
  REQUEST_HEADER_FIELDS_TOO_LARGE = 431,  ///< 431 REQUEST HEADER FIELDS TOO LARGE
  INTERNAL_SERVER_ERROR = 500,            ///< 500 INTERNAL SERVER ERROR
  NOT_IMPLEMENTED = 501,                  ///< 501 NOT IMPLEMENTED
  BAD_GATEWAY = 502,                      ///< 502 BAD GATEWAY
  SERVICE_UNAVAILABLE = 503,              ///< 503 SERVICE UNAVAILABLE
  GATEWAY_TIMEOUT = 504,                  ///< 504 GATEWAY TIMEOUT
  HTTP_VERSION_NOT_SUPPORTED = 505,       ///< 505 HTTP VERSION NOT SUPPORTED
  VARIANT_ALSO_NEGOTIATES = 506,          ///< 506 VARIANT ALSO NEGOTIATES
  INSUFFICIENT_STORAGE = 507,             ///< 507 INSUFFICIENT STORAGE
  LOOP_DETECTED = 508,                    ///< 508 LOOP DETECTED
  NOT_EXTENDED = 510,                     ///< 510 NOT EXTENDED
  NETWORK_AUTHENTICATION_REQUIRED = 511,  ///< 511 NETWORK AUTHENTICATION REQUIRED
};

/// HTTP request methods
enum class HTTPRequestMethod {
  UNKNOWN = -1, ///< Reserved for the case when method other than defined here is encountered
  OPTIONS = 0,  ///< HTTP request method OPTIONS
  GET = 1,      ///< HTTP request method GET
  HEAD = 2,     ///< HTTP request method HEAD
  POST = 3,     ///< HTTP request method POST
  PUT = 4,      ///< HTTP request method PUT
  DELETE = 5,   ///< HTTP request method DELETE
  TRACE = 6,    ///< HTTP request method TRACE
  CONNECT = 7   ///< HTTP request method CONNECT
};

/// Content-types used in this application
enum class HTTPContentType {
  UNKNOWN = -1,   ///< Reserved for the case when content-type other than defined here is encountered
  HTML = 0,       ///< text/html
  PLAINTEXT = 1,  ///< text/plain
  CSV = 2,        ///< text/csv
  JSON = 3,       ///< application/json
  XML = 4         ///< text/xml
};

/// Charset for content-type used in this application
enum class HTTPContentCharset {
  UNKNOWN = -1,   ///< Reserved for the case when charset other than defined here is encountered
  UTF8 = 0,       ///< UTF-8
};

/// Organises miscellaneous functions which process parts of HTTP requests.
class HTTPRequestHelper {
  public:
    static HTTPRequestMethod getMethod(const char * method);
};

/// Converts two-digit ASCII hexadecimal codes (used in HTTP percent codes)
class HTTPHexCode {
  public:
    static int decodeHex(const char buffer[]);
    static int decodeDigit(char hexDigit);
    static const int decodeError = -1;    ///< This value is returned when decoding error occurs.
    static const size_t size = 2;         ///< Percent code size: 2 hex characters.
};

/// Processes URL strings
class URL {
  public:
    static void decode(char buffer[], size_t bufferSize);
};

/// Composes typical HTTP Response Headers and their parts
class HTTPResponseHeader {
  public:
    static void contentHeader(Print &client, HTTPContentType type, HTTPContentCharset charset = HTTPContentCharset::UTF8);
    static void redirect(Print &client, const __FlashStringHelper * path);
    static void statusLine(Print &client, HTTPStatusCode statusCode);
  public:
    static const __FlashStringHelper * statusCodeText(HTTPStatusCode statusCode);
    static const __FlashStringHelper * contentTypeText(HTTPContentType contentType);
    static const __FlashStringHelper * contentCharsetText(HTTPContentCharset contentCharset);
};

}; //namespace http

namespace json {

/// @brief Generates a JSON and sends to Print type client
/// @details All JSON nested structures are automatically 
class JSONOutput {
  public:
    JSONOutput(Print & client);
    ~JSONOutput();
    void beginArray(const char * name = NULL);
    void beginArray(const __FlashStringHelper * name);
    void beginObject(const char * name = NULL);
    void beginObject(const __FlashStringHelper * name);
    void finish(void);
    void value(const char * name, const char * value = NULL);
    void value(const char * name, const __FlashStringHelper * value);
    void value(const char * name, long value);
    void value(const char * name, unsigned long value);
    void value(const char * name, boolean value);
    void value(const __FlashStringHelper * name, const char * value = NULL);
    void value(const __FlashStringHelper * name, const __FlashStringHelper * value);
    void value(const __FlashStringHelper * name, long value);
    void value(const __FlashStringHelper * name, unsigned long value);
    void value(const __FlashStringHelper * name, boolean value);
  private:
    Print * client = NULL;
  private:
    static const char charLbrace = '{';
    static const char charRbrace = '}';
    static const char charQuote = '"';
    static const char charLbracket = '[';
    static const char charRbracket = ']';
    static const char charEqual = '=';
    static const char charColon = ':';
    static const char charComma = ',';
  private:
    typedef uint8_t Context;
    static const Context contextNone = 0;
    static const Context contextObject = 1;
    static const Context contextArray = 2;
    Context currentContext = contextNone;
    static const uint8_t nestingStackSize = 16;
    Context nestingStack[nestingStackSize];
    uint8_t nestingStackCounter = 0;
    boolean firstValue = true;
    inline boolean pushContext(Context context);
    inline void popContext(void);
  private:
    inline void printName(const char * name);
    inline void printName(const __FlashStringHelper * name);
    inline void printString(const char * string);
    inline void printString(const __FlashStringHelper * string);
};

/// Print json object name from RAM string
inline void JSONOutput::printName(const char * name) {
  if (!firstValue)
    client->print(charComma);
  else
    firstValue = false;
  if (currentContext == contextObject) {
    client->print(charQuote);
    if (name) client->print(name);
    client->print(charQuote);
    client->print(charColon);
  }
}

/// Print json object name from PROGMEM string
inline void JSONOutput::printName(const __FlashStringHelper * name) {
  if (!firstValue)
    client->print(charComma);
  else
    firstValue = false;
  if (currentContext == contextObject) {
    client->print(charQuote);
    if (name) client->print(name);
    client->print(charQuote);
    client->print(charColon);
  }
}

/// Print json object value string from RAM
inline void JSONOutput::printString(const char * string) {
  static const char nullString[] = "null";
  if (currentContext != contextNone) {
    if (string) {
      client->print(charQuote);
      client->print(string);
      client->print(charQuote);
    }
    else {
      client->print(FPSTR(nullString));
    }
  }
}

/// Print json object value string from PROGMEM
inline void JSONOutput::printString(const __FlashStringHelper * string) {
  static const char nullString[] = "null";
  if (currentContext != contextNone) {
    if (string) {
      client->print(charQuote);
      client->print(string);
      client->print(charQuote);
    }
    else {
      client->print(FPSTR(nullString));
    }
  }
}

/// Go one nesting level deeper
inline boolean JSONOutput::pushContext(Context context) {
  if (nestingStackCounter >= nestingStackSize) return (false);
  nestingStack[nestingStackCounter++] = currentContext;
  currentContext = context;
  return (true);
}

/// Get back from one nesting level
inline void JSONOutput::popContext(void) {
  if (nestingStackCounter) {
    currentContext = nestingStack[--nestingStackCounter];
  }
  else {
    currentContext = contextNone;
  }
}

}; //namespace json

}; //namespace util

#endif
