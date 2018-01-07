/*
 * Copyright (C) 2016-2018 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "util_comm.h"
#include <ESP8266WiFi.h>

namespace util {

namespace http {

//////////////////////////////////////////////////////////////////////
// HTTPRequestHelper
//////////////////////////////////////////////////////////////////////

/// Converts method c-string from HTTP request into enum HTTPRequestMethod
/// @param method C-string which contains method extracted from HTTP
/// request header
/// @return Corresponding value of enum HTTPRequestMethod
/// or HTTPRequestMethod::UNKNOWN if the method is other than specified in
/// HTTPRequestMethod
HTTPRequestMethod HTTPRequestHelper::getMethod(const char * method) {
  static const char PROGMEM methodOptions[] = "OPTIONS";
  static const char PROGMEM methodGet[] = "GET";
  static const char PROGMEM methodHead[] = "HEAD";
  static const char PROGMEM methodPost[] = "POST";
  static const char PROGMEM methodPut[] = "PUT";
  static const char PROGMEM methodDelete[] = "DELETE";
  static const char PROGMEM methodTrace[] = "TRACE";
  static const char PROGMEM methodConnect[] = "CONNECT";
  if (!strcmp_P(method, methodOptions)) return (HTTPRequestMethod::OPTIONS);
  if (!strcmp_P(method, methodGet)) return (HTTPRequestMethod::GET);
  if (!strcmp_P(method, methodHead)) return (HTTPRequestMethod::HEAD);
  if (!strcmp_P(method, methodPost)) return (HTTPRequestMethod::POST);
  if (!strcmp_P(method, methodPut)) return (HTTPRequestMethod::PUT);
  if (!strcmp_P(method, methodDelete)) return (HTTPRequestMethod::DELETE);
  if (!strcmp_P(method, methodTrace)) return (HTTPRequestMethod::TRACE);
  if (!strcmp_P(method, methodConnect))return (HTTPRequestMethod::CONNECT);
  return (HTTPRequestMethod::UNKNOWN);
};

//////////////////////////////////////////////////////////////////////
// HTTPPercentCode
//////////////////////////////////////////////////////////////////////

/// Converts 2 digit hexadecimal number into integer
/// @param buffer ASCII representation of hex code, minimum length 2
/// characters
/// @return Converted value in range 0..255 or decodeError if conversion
/// error occured
int HTTPHexCode::decodeHex(const char buffer[]) {
  if (!buffer) return (decodeError);
  static const int RADIX_HEXADECIMAL = 16;
  int mostSignificant = decodeDigit(buffer[0]);
  if (mostSignificant == decodeError) return (decodeError);
  int leastSignificant = decodeDigit(buffer[1]);
  if (leastSignificant == decodeError) return (decodeError);
  return (mostSignificant * RADIX_HEXADECIMAL + leastSignificant);
}

/// Converts single hexadecimal ASCII digit into integer
/// @param hexDigit character representing hexadecimal digit in
/// ASCII format
/// @return Converted value in range 0..15 or decodeError if conversion
/// error occured
int HTTPHexCode::decodeDigit(char hexDigit) {
  if ((hexDigit >= '0') && (hexDigit <= '9')) return (hexDigit - '0');
  if ((hexDigit >= 'A') && (hexDigit <= 'F')) return (hexDigit - 'A' + 10);
  if ((hexDigit >= 'a') && (hexDigit <= 'f')) return (hexDigit - 'a' + 10);
  return (decodeError);
}

//////////////////////////////////////////////////////////////////////
// URL
//////////////////////////////////////////////////////////////////////

/// @brief Converts URL-encoded string into plain text
/// @details The following operations are performed with the input string:
/// * All '+' characters are substituted with spaces (' ')
/// * All percent-codes are substituted with the corresponding charaters
/// @par If input string contains percent codes, it is possible for
/// string to become shorter after conversion
/// @warning This method modifies input string
/// @warning Only 2-digit percent codes are supported
/// @param buffer Contains input cstring to be decoded (this string will be
/// modified by conversion)
/// @param bufferSize maximum length of the string to be processed

void URL::decode(char buffer[], size_t bufferSize) {
  if (!buffer || !bufferSize) return;
  size_t readPosition = 0;
  size_t writePosition = 0;
  while ((readPosition < (bufferSize - 1)) && buffer[readPosition]) {
    char c = buffer[readPosition];
    if (c == '%') { //following characters are percent hex code
      int hexValueDecoded = HTTPHexCode::decodeHex(&buffer[readPosition + 1]);
      if (hexValueDecoded != HTTPHexCode::decodeError) {
        buffer[writePosition++] = (char)hexValueDecoded;
        readPosition += (HTTPHexCode::size + 1); //+1 character for a '%' char
      }
      else {
        writePosition++;
        readPosition++;
      }
    }
    else {
      if (c == '+') c = ' ';
      buffer[writePosition++] = c;
      readPosition ++;
    }
  }
  buffer[writePosition] = 0;
}

//////////////////////////////////////////////////////////////////////
// HTTPResponseHeader
//////////////////////////////////////////////////////////////////////

/// @brief Generates HTTP Response header with given Content-type and Charset
/// @details Also adds status line with HTTP Status Code "200 OK" to the
/// beginning of the response header
/// @param client Client to send response header to
/// @param type Content-Type to include in the response header
/// @param charset Charset to include in response header
void HTTPResponseHeader::contentHeader(Print &client, HTTPContentType type, HTTPContentCharset charset) {
  statusLine(client, HTTPStatusCode::OK);
  client.print(F("Cache-control: no-cache\r\nConnection: close\r\n"));
  client.print(F("Content-type: "));
  client.print(contentTypeText(type));
  client.print(';');
  client.print(' ');
  client.print(contentCharsetText(charset));
  client.print(F("\r\n\r\n"));
}

/// @brief Generates HTTP Response with redirect to other URL
/// (HTTP status code "303 See Other")
/// @param client Client to send generated response to
/// @param path Redirect destination
void HTTPResponseHeader::redirect(Print &client, const __FlashStringHelper * path) {
  statusLine(client, HTTPStatusCode::SEE_OTHER);
  client.print(F("Location: http://"));
  client.print(WiFi.softAPIP()); ///@todo remove ESP8266 dependency
  if (pgm_read_byte(path) != '/') client.print('/');
  client.print(path);
  client.print(F("\r\n\r\n"));
}

/// @brief Generates HTTP Response Status Line based on HTTP Status Code
/// @details HTTP version in the response is always "HTTP/1.1"
/// @param client Client to send generated Status Line to
/// @param statusCode HTTP Status Code to include in the Status Line
void HTTPResponseHeader::statusLine(Print &client, HTTPStatusCode statusCode) {
  client.print(F("HTTP/1.1 "));
  client.print((int)statusCode);
  client.print(' ');
  client.print(statusCodeText(statusCode));
  client.print(F("\r\n"));
}

/// @brief Provides human-readable description of HTTP Status Code (e.g.
/// "OK" for HTTP Status Code 200)
/// @param statusCode HTTP Status Code
/// @return Cstring in progmem with details of HTTP Status Code
const __FlashStringHelper * HTTPResponseHeader::statusCodeText(HTTPStatusCode statusCode) {
  switch (statusCode) {
    case HTTPStatusCode::CONTINUE:
      return (F("CONTINUE"));
    case HTTPStatusCode::SWITCHING_PROTOCOLS:
      return (F("SWITCHING PROTOCOLS"));
    case HTTPStatusCode::OK:
      return (F("OK"));
    case HTTPStatusCode::CREATED:
      return (F("CREATED"));
    case HTTPStatusCode::ACCEPTED:
      return (F("ACCEPTED"));
    case HTTPStatusCode::NON_AUTHORITATIVE_INFORMATION:
      return (F("NON-AUTHORITATIVE INFORMATION"));
    case HTTPStatusCode::NO_CONTENT:
      return (F("NO CONTENT"));
    case HTTPStatusCode::RESET_CONTENT:
      return (F("RESET CONTENT"));
    case HTTPStatusCode::PARTIAL_CONTENT:
      return (F("PARTIAL CONTENT"));
    case HTTPStatusCode::MULTIPLE_CHOISES:
      return (F("MULTIPLE CHOISES"));
    case HTTPStatusCode::MOVED_PERMANENTLY:
      return (F("MOVED PERMANENTLY"));
    case HTTPStatusCode::FOUND:
      return (F("FOUND"));
    case HTTPStatusCode::SEE_OTHER:
      return (F("SEE OTHER"));
    case HTTPStatusCode::NOT_MODIFIED:
      return (F("NOT MODIFIED"));
    case HTTPStatusCode::USE_PROXY:
      return (F("USE PROXY"));
    case HTTPStatusCode::SWITCH_PROXY:
      return (F("SWITCH PROXY"));
    case HTTPStatusCode::TEMPORARY_REDIRECT:
      return (F("TEMPORARY REDIRECT"));
    case HTTPStatusCode::PERMANENT_REDIRECT:
      return (F("PERMANENT REDIRECT"));
    case HTTPStatusCode::BAD_REQUEST:
      return (F("BAD REQUEST"));
    case HTTPStatusCode::UNAUTHORIZED:
      return (F("UNAUTHORIZED"));
      break;
    case HTTPStatusCode::FORBIDDEN:
      return (F("FORBIDDEN"));
    case HTTPStatusCode::NOT_FOUND:
      return (F("NOT FOUND"));
    case HTTPStatusCode::METHOD_NOT_ALLOWED:
      return (F("METHOD NOT ALLOWED"));
    case HTTPStatusCode::NOT_ACCEPTABLE:
      return (F("NOT ACCEPTABLE"));
    case HTTPStatusCode::PROXY_AUTHENTICATION_REQUIRED:
      return (F("PROXY AUTHENTICATION REQUIRED"));
    case HTTPStatusCode::REQUEST_TIMEOUT:
      return (F("REQUEST TIMEOUT"));
    case HTTPStatusCode::CONFLICT:
      return (F("CONFLICT"));
    case HTTPStatusCode::GONE:
      return (F("GONE"));
    case HTTPStatusCode::LENGTH_REQUIRED:
      return (F("LENGTH REQUIRED"));
    case HTTPStatusCode::PRECONDITION_FAILED:
      return (F("PRECONDITION FAILED"));
    case HTTPStatusCode::PAYLOAD_TOO_LARGE:
      return (F("PAYLOAD TOO LARGE"));
    case HTTPStatusCode::REQUEST_URI_TOO_LONG:
      return (F("REQUEST URI TOO LONG"));
    case HTTPStatusCode::UNSUPPORTED_MEDIA_TYPE:
      return (F("UNSUPPORTED MEDIA TYPE"));
    case HTTPStatusCode::RANGE_NOT_SATISFIABLE:
      return (F("RANGE NOT SATISFIABLE"));
    case HTTPStatusCode::EXPECTATION_FAILED:
      return (F("EXPECTATION FAILED"));
    case HTTPStatusCode::I_M_A_TEAPOT:
      return (F("I'M A TEAPOT"));
    case HTTPStatusCode::MISDIRECTED_REQUEST:
      return (F("MISDIRECTED REQUEST"));
    case HTTPStatusCode::UPGRADE_REQUIRED:
      return (F("UPGRADE REQUIRED"));
    case HTTPStatusCode::PRECONDITION_REQUIRED:
      return (F("PRECONDITION REQUIRED"));
    case HTTPStatusCode::TOO_MANY_REQUESTS:
      return (F("TOO MANY REQUESTS"));
    case HTTPStatusCode::REQUEST_HEADER_FIELDS_TOO_LARGE:
      return (F("REQUEST HEADER FIELDS TOO LARGE"));
    case HTTPStatusCode::INTERNAL_SERVER_ERROR:
      return (F("INTERNAL SERVER ERROR"));
    case HTTPStatusCode::NOT_IMPLEMENTED:
      return (F("NOT IMPLEMENTED"));
    case HTTPStatusCode::BAD_GATEWAY:
      return (F("BAD GATEWAY"));
    case HTTPStatusCode::SERVICE_UNAVAILABLE:
      return (F("SERVICE UNAVAILABLE"));
    case HTTPStatusCode::GATEWAY_TIMEOUT:
      return (F("GATEWAY TIMEOUT"));
    case HTTPStatusCode::HTTP_VERSION_NOT_SUPPORTED:
      return (F("HTTP VERSION NOT SUPPORTED"));
    case HTTPStatusCode::VARIANT_ALSO_NEGOTIATES:
      return (F("VARIANT ALSO NEGOTIATES"));
    case HTTPStatusCode::INSUFFICIENT_STORAGE:
      return (F("INSUFFICIENT STORAGE"));
    case HTTPStatusCode::LOOP_DETECTED:
      return (F("LOOP DETECTED"));
    case HTTPStatusCode::NOT_EXTENDED:
      return (F("NOT EXTENDED"));
    case HTTPStatusCode::NETWORK_AUTHENTICATION_REQUIRED:
      return (F("NETWORK AUTHENTICATION REQUIRED"));
    default:
      return (F(""));
  }
}

/// @brief Provides text of the Content-Type (e.g. "text/html")
/// @param type HTTP Content-type
/// @return Cstring in progmem with text of the Content-type
const __FlashStringHelper * HTTPResponseHeader::contentTypeText(HTTPContentType type) {
  switch (type) {
    case HTTPContentType::HTML:
      return (F("text/html"));
    case HTTPContentType::PLAINTEXT:
      return (F("text/plain"));
    case HTTPContentType::CSV:
      return (F("text/csv"));
    case HTTPContentType::JSON:
      return (F("application/json"));
    case HTTPContentType::XML:
      return (F("text/xml"));
    default:
      return (F(""));
  }
}

/// @brief Provides text of the Charset-encoding (e.g. "utf-8")
/// @param type HTTP Charset-encoding
/// @return Cstring in progmem with text of the Charset-encoding
const __FlashStringHelper * HTTPResponseHeader::contentCharsetText(HTTPContentCharset charset) {
  switch (charset) {
    case HTTPContentCharset::UTF8:
      return (F("utf-8"));
    default:
      return (F(""));
  }
}

}; //namespace http

namespace json {

//////////////////////////////////////////////////////////////////////
// JSONOutput
//////////////////////////////////////////////////////////////////////

/// @brief Initialises JSONOutput object
/// @param client A Print to output generated JSON to
JSONOutput::JSONOutput(Print & client) {
  this->client = &client;
  beginObject();
}

/// @brief Destroys JSONOutput object
/// @details Closes all braces and brackets in order to produce a valid JSON
JSONOutput::~JSONOutput() {
  while (currentContext != contextNone) {
    finish();
  }
}

/// @brief Starts new Array
/// @param name Name of the array as a RAM string
void JSONOutput::beginArray(const char * name) {
  printName(name);
  pushContext(contextArray);
  client->print(charLbracket);
  firstValue = true;
}

/// @brief Starts new Array
/// @param name Name of the array as a PROGMEM string
void JSONOutput::beginArray(const __FlashStringHelper * name) {
  printName(name);
  pushContext(contextArray);
  client->print(charLbracket);
  firstValue = true;
}

/// @brief Starts new Object
/// @param name Name of the object as a RAM string
void JSONOutput::beginObject(const char * name) {
  printName(name);
  pushContext(contextObject);
  client->print(charLbrace);
  firstValue = true;
}

/// @brief Starts new Object
/// @param name Name of the object as a PROGMEM string
void JSONOutput::beginObject(const __FlashStringHelper * name) {
  printName(name);
  pushContext(contextObject);
  client->print(charLbrace);
  firstValue = true;
}

/// @brief Finishes current object or array
/// @details Closes brace or bracket in order to produce a valid JSON
void JSONOutput::finish(void) {
  if (currentContext == contextArray) client->print(charRbracket);
  if (currentContext == contextObject) client->print(charRbrace);
  popContext();
  firstValue = false;
}

/// @brief Produces a Name = Value pair
/// @param name Name as a RAM string
/// @param value Value as a RAM string
void JSONOutput::value(const char * name, const char * value) {
  printName(name);
  printString(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a RAM string
/// @param value Value as a PROGMEM string
void JSONOutput::value(const char * name, const __FlashStringHelper * value) {
  printName(name);
  printString(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a RAM string
/// @param value Value as an integer
void JSONOutput::value(const char * name, long value) {
  printName(name);
  if (currentContext != contextNone) client->print(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a RAM string
/// @param value Value as an unsigned integer
void JSONOutput::value(const char * name, unsigned long value) {
  printName(name);
  if (currentContext != contextNone) client->print(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a RAM string
/// @param value Value as a boolean (output is true/false)
void JSONOutput::value(const char * name, boolean value) {
  static const char PROGMEM valueTrue[] = "true";
  static const char PROGMEM valueFalse[] = "false";
  printName(name);
  if (currentContext != contextNone) {
    if (value) {
      client->print(FPSTR(valueTrue));
    }
    else {
      client->print(FPSTR(valueFalse));
    }
  }
}

/// @brief Produces a Name = Value pair
/// @param name Name as a PROGMEM string
/// @param value Value as a RAM string
void JSONOutput::value(const __FlashStringHelper * name, const char * value) {
  printName(name);
  printString(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a PROGMEM string
/// @param value Value as a PROGMEM string
void JSONOutput::value(const __FlashStringHelper * name, const __FlashStringHelper * value) {
  printName(name);
  printString(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a PROGMEM string
/// @param value Value as an integer
void JSONOutput::value(const __FlashStringHelper * name, long value) {
  printName(name);
  if (currentContext != contextNone) client->print(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a PROGMEM string
/// @param value Value as an unsigned integer
void JSONOutput::value(const __FlashStringHelper * name, unsigned long value) {
  printName(name);
  if (currentContext != contextNone) client->print(value);
}

/// @brief Produces a Name = Value pair
/// @param name Name as a PROGMEM string
/// @param value Value as a boolean (output is true/false)
void JSONOutput::value(const __FlashStringHelper * name, boolean value) {
  static const char PROGMEM valueTrue[] = "true";
  static const char PROGMEM valueFalse[] = "false";
  printName(name);
  if (currentContext != contextNone) {
    if (value) {
      client->print(FPSTR(valueTrue));
    }
    else {
      client->print(FPSTR(valueFalse));
    }
  }
}

}; //namespace json

}; //namespace util
