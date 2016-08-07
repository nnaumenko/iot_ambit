/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef HTTP_H
#define HTTP_H

#include <Arduino.h>

const size_t HTTP_REQUEST_PART_MAX_SIZE = 32;

#ifdef ESP8266
#include <ESP8266WiFi.h>
#define HTTPSTREAM_OUT_BUFFER_SIZE WIFICLIENT_MAX_PACKET_SIZE
//WIFICLIENT_MAX_PACKET_SIZE is 1460, see libraries/ESP8266WiFi/src/WiFiClient.h
//smaller value might be required when porting to Arduino due to memory constraints
#endif

enum class HTTPStatusCode {
  UNKNOWN = 0,
  NO_CHANGE = 0,
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

class HTTPStream : public Stream {
  public:
    HTTPStream (Stream & client);
    ~HTTPStream();
  public:
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    virtual size_t write(uint8_t character);
  public:
    void readUntil(const char * charsToFind, char * buffer, size_t bufferSize);
  private:
    Stream * client = NULL;
  public:
    static const int NOT_AVAILABLE = -1;
  private:
    char previousChar = '\0';
  private:
    void sendOutputBuffer(void);
    int outputBufferPosition = 0;
    char outputBuffer[HTTPSTREAM_OUT_BUFFER_SIZE];
};
  
enum class HTTPRequestPart {
  NONE,
  BEGIN,
  FINISH,
  METHOD,
  PATH,
  GET_QUERY_NAME,
  GET_QUERY_VALUE,
  VERSION,
  FIELD_NAME,
  FIELD_VALUE,
  POST_QUERY_NAME,
  POST_QUERY_VALUE,
};

class HTTPReqParserBase;

class HTTPReqPartHandler {
  public:
    void begin (HTTPReqParserBase &parser, Print &client) {
      this->parser = &parser;
      this->client = &client;
    }
    virtual void handleReqPart(char * value, HTTPRequestPart part) = 0;
  protected:
    HTTPReqParserBase * parser = NULL;
    Print * client = NULL;
};

class HTTPReqParserBase {
  public:
    virtual boolean isFinished(void) = 0;
    virtual void setError(HTTPStatusCode errorStatusCode) = 0;
    virtual boolean isError(void) = 0;
    virtual HTTPStatusCode getStatusCode(void) = 0;
};

class HTTPReqParser : public HTTPReqParserBase {
  public:
    boolean begin (HTTPStream &client);
    void setHandler (HTTPReqPartHandler &handler);
  protected:
    HTTPStream * client = NULL;
    HTTPReqPartHandler * reqPartHandler = NULL;
  private:
    virtual boolean prepareToParse(void) = 0;
  public:
    virtual void parse(void) = 0;
};

class HTTPReqParserStateMachine : public HTTPReqParser {
  public:
    enum class ParserState {
      UNKNOWN,
      ERROR,
      BEGIN,
      METHOD,
      PATH,
      URL_QUERY_NAME,
      URL_QUERY_VALUE,
      HTTP_VERSION,
      FIELD_OR_HEADER_END,
      FIELD_NAME,
      FIELD_VALUE,
      HEADER_END,
      POST_QUERY_OR_END,
      POST_QUERY_NAME,
      POST_QUERY_VALUE,
      FINISHED,
    };
    enum class StreamOperation {
      FLUSH,       //Flush client stream and do not read
      DO_NOTHING,  //Do not read anything from stream
      READ,        //Read from stream to buffer until expected characters (see transition table) are found
      SKIP,        //Read from stream but do not put characters to buffer until expected characters are found
      PEEK,        //Do not read anything from stream and peek (rather than read) next character
    };
  public:
    static const char CHAR_OTHER = '\0';
    static const char CHAR_UNAVAIL = (char)HTTPStream::NOT_AVAILABLE;
  private:
    struct {
      HTTPStatusCode statusCode;
      HTTPRequestPart requestPart;
      char nextCharacter;
      ParserState currentState;
    } parserStatus;
  private:
    virtual boolean prepareToParse (void);
  public:
    virtual void parse(void);
    virtual boolean isFinished(void);
    virtual void setError(HTTPStatusCode errorStatusCode);
    virtual boolean isError(void);
    virtual HTTPStatusCode getStatusCode(void);
  private:
    void transition(ParserState newState, HTTPStatusCode newStatusCode);
  private:
    class ParserTableBase {
      public:
        ParserTableBase ();
      protected:
        size_t tableSize;
      protected:
        size_t tableIndex;
        size_t getTableIndex(void);
        boolean setTableIndex(size_t newIndex);
        void firstEntry(void);
        boolean nextEntry(void);
    };
  public:
    struct ProcessingTableEntry {
      ParserState state;
      StreamOperation operation;
      HTTPRequestPart part;
    };
  private:
    class ProcessingTable : ParserTableBase {
        const ProcessingTableEntry * table;
        static const size_t tableEntrySize = (sizeof(table[0]));
      public:
        ProcessingTable();
        boolean begin (void);
        boolean find (ParserState state);
        StreamOperation getStreamOperation(void);
        HTTPRequestPart getRequestPart(void);
      private:
        ParserState getState(void);
      private:
        boolean validate(void);
        boolean isValidated = false;
    } processingTable;
  public:
    struct TransitionTableEntry {
      ParserState state;
      char nextChar;
      ParserState newState;
      HTTPStatusCode newStatusCode;
    };
  private:
    class TransitionTable : ParserTableBase {
      private:
        const TransitionTableEntry * table;
        static const size_t tableEntrySize = (sizeof(table[0]));
      public:
        TransitionTable();
        boolean begin (void);
        boolean find (ParserState state, char nextChar);
        boolean enumerateNextChars(ParserState state, char * buffer, size_t bufferSize);
        ParserState getNewState();
        HTTPStatusCode getNewStatusCode();
      private:
        ParserState getState();
        char getNextChar();
        boolean validate(void);
        boolean isValidated = false;
    } transitionTable;
};

#endif
