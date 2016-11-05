/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "http.h"
#include <ESP8266WiFi.h>

//////////////////////////////////////////////////////////////////////
// HTTPRequestHelper
//////////////////////////////////////////////////////////////////////

const char PROGMEM methodOptions[] = "OPTIONS";
const char PROGMEM methodGet[] = "GET";
const char PROGMEM methodHead[] = "HEAD";
const char PROGMEM methodPost[] = "POST";
const char PROGMEM methodPut[] = "PUT";
const char PROGMEM methodDelete[] = "DELETE";
const char PROGMEM methodTrace[] = "TRACE";
const char PROGMEM methodConnect[] = "CONNECT";

HTTPRequestMethod HTTPRequestHelper::getMethod(const char * method) {
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

int HTTPPercentCode::decodeHex(const char buffer[]) {
  if (!buffer) return (decodeError);
  const int RADIX_HEXADECIMAL = 16;
  int mostSignificant = decodeDigit(buffer[0]);
  if (mostSignificant == decodeError) return (decodeError);
  int leastSignificant = decodeDigit(buffer[1]);
  if (leastSignificant == decodeError) return (decodeError);
  return (mostSignificant * RADIX_HEXADECIMAL + leastSignificant);
}

int HTTPPercentCode::decodeDigit(char hexDigit) {
  if ((hexDigit >= '0') && (hexDigit <= '9')) return (hexDigit - '0');
  if ((hexDigit >= 'A') && (hexDigit <= 'F')) return (hexDigit - 'A' + 10);
  if ((hexDigit >= 'a') && (hexDigit <= 'f')) return (hexDigit - 'a' + 10);
  return (decodeError);
}

//////////////////////////////////////////////////////////////////////
// URL
//////////////////////////////////////////////////////////////////////

void URL::decode(char buffer[], size_t bufferSize) {
  if (!buffer || !bufferSize) return;
  size_t readPosition = 0;
  size_t writePosition = 0;
  while (buffer[readPosition] && (readPosition < (bufferSize - 1))) {
    char c = buffer[readPosition];
    if (c == '%') { //following 2 characters are percent hex code
      int hexValueDecoded = HTTPPercentCode::decodeHex(&buffer[readPosition + 1]);
      if (hexValueDecoded != HTTPPercentCode::decodeError) {
        buffer[writePosition++] = (char)hexValueDecoded;
        readPosition += HTTPPercentCode::size;
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

void HTTPResponseHeader::redirect(Print &client, const __FlashStringHelper * path) {
  statusLine(client, HTTPStatusCode::SEE_OTHER);
  client.print(F("Location: http://"));
  client.print(WiFi.softAPIP());//TODO
  if (pgm_read_byte(path) != '/') client.print('/');
  client.print(path);
}

void HTTPResponseHeader::statusLine(Print &client, HTTPStatusCode statusCode) {
  client.print(F("HTTP/1.1 "));
  client.print((int)statusCode);
  client.print(' ');
  client.print(statusCodeText(statusCode));
  client.print(F("\r\n"));
}

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

const __FlashStringHelper * HTTPResponseHeader::contentCharsetText(HTTPContentCharset charset) {
  switch (charset) {
    case HTTPContentCharset::UTF8:
      return (F("utf-8"));
    default:
      return (F(""));
  }
}


