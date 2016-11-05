/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef HTTP_H
#define HTTP_H

#include <arduino.h>

enum class HTTPStatusCode {
  UNKNOWN = 0,
  CONTINUE = 100,
  SWITCHING_PROTOCOLS = 101,
  OK = 200,
  CREATED = 201,
  ACCEPTED = 202,
  NON_AUTHORITATIVE_INFORMATION = 203,
  NO_CONTENT = 204,
  RESET_CONTENT = 205,
  PARTIAL_CONTENT = 206,
  MULTIPLE_CHOISES = 300,
  MOVED_PERMANENTLY = 301,
  FOUND = 302,
  SEE_OTHER = 303,
  NOT_MODIFIED = 304,
  USE_PROXY = 305,
  SWITCH_PROXY = 306,
  TEMPORARY_REDIRECT = 307,
  PERMANENT_REDIRECT = 308,
  BAD_REQUEST = 400,
  UNAUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  NOT_ACCEPTABLE = 406,
  PROXY_AUTHENTICATION_REQUIRED = 407,
  REQUEST_TIMEOUT = 408,
  CONFLICT = 409,
  GONE = 410,
  LENGTH_REQUIRED = 411,
  PRECONDITION_FAILED = 412,
  PAYLOAD_TOO_LARGE = 413,
  REQUEST_URI_TOO_LONG = 414,
  UNSUPPORTED_MEDIA_TYPE = 415,
  RANGE_NOT_SATISFIABLE = 416,
  EXPECTATION_FAILED = 417,
  I_M_A_TEAPOT = 418,
  MISDIRECTED_REQUEST = 421,
  UPGRADE_REQUIRED = 426,
  PRECONDITION_REQUIRED = 428,
  TOO_MANY_REQUESTS = 429,
  REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
  INTERNAL_SERVER_ERROR = 500,
  NOT_IMPLEMENTED = 501,
  BAD_GATEWAY = 502,
  SERVICE_UNAVAILABLE = 503,
  GATEWAY_TIMEOUT = 504,
  HTTP_VERSION_NOT_SUPPORTED = 505,
  VARIANT_ALSO_NEGOTIATES = 506,
  INSUFFICIENT_STORAGE = 507,
  LOOP_DETECTED = 508,
  NOT_EXTENDED = 510,
  NETWORK_AUTHENTICATION_REQUIRED = 511,
};

enum class HTTPRequestMethod {
  UNKNOWN = -1,
  OPTIONS = 0,
  GET = 1,
  HEAD = 2,
  POST = 3,
  PUT = 4,
  DELETE = 5,
  TRACE = 6,
  CONNECT = 7
};

enum class HTTPContentType {
  UNKNOWN = -1,
  HTML = 0,
  PLAINTEXT = 1,
  CSV = 2,
  JSON = 3,
  XML = 4
};

enum class HTTPContentCharset {
  UNKNOWN = -1,
  UTF8 = 0,
};

class HTTPRequestHelper {
  public:
    static HTTPRequestMethod getMethod(const char * method);
};

class HTTPPercentCode {
  public:
    static int decodeHex(const char buffer[]);
    static int decodeDigit(char hexDigit);
    static const int decodeError = -1;
    static const size_t size = 3; //each percent code contains exacty 3 characters
};

class URL {
  public:
    static void decode(char buffer[], size_t bufferSize);
};

class HTTPResponseHeader {
  public:
    static void contentHeader(Print &client, HTTPContentType type, HTTPContentCharset charset = HTTPContentCharset::UTF8);
    static void redirect(Print &client, const __FlashStringHelper * path);
    static void statusLine(Print &client, HTTPStatusCode statusCode);
  public:
    static const __FlashStringHelper * statusCodeText(HTTPStatusCode statusCode);
    static const __FlashStringHelper * contentTypeText(HTTPContentType contentType);
    static const __FlashStringHelper * contentCharsetText(HTTPContentCharset contentCharset);
  private:
    Print * client = NULL;
};

#endif
