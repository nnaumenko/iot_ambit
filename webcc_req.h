/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCC_REQ_H
#define WEBCC_REQ_H

#include <Arduino.h>
#include "http.h"

const size_t HTTP_REQUEST_PART_MAX_SIZE = 32;

#ifdef ESP8266
#include <ESP8266WiFi.h>
#define HTTPSTREAM_OUT_BUFFER_SIZE WIFICLIENT_MAX_PACKET_SIZE
//WIFICLIENT_MAX_PACKET_SIZE is 1460, see libraries/ESP8266WiFi/src/WiFiClient.h
//smaller value might be required when porting to Arduino due to memory constraints
#endif

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
  private:
    Stream * client = NULL;
  public:
    static const int NOT_AVAILABLE = -1;
  private:
    int previousChar = NOT_AVAILABLE;
  private:
    void sendOutputBuffer(void);
    int outputBufferPosition = 0;
    char outputBuffer[HTTPSTREAM_OUT_BUFFER_SIZE];
};

class HTTPStreamHelper {
  public:
    static void readUntil(Stream & client, const char * charsToFind, char * buffer, size_t bufferSize);
};

enum class HTTPRequestPart {
  NONE,
  BEGIN,
  FINISH,
  METHOD,
  PATH,
  URL_QUERY_NAME,
  URL_QUERY_VALUE,
  VERSION,
  FIELD_NAME,
  FIELD_VALUE,
  POST_QUERY_NAME,
  POST_QUERY_VALUE,
};

class HTTPReqParserStateMachine {
  public:
    boolean begin (Stream &client);
    void parse(void);
  public:
    boolean isFinished(void);
    boolean isError(void);
    void setStatusCode(HTTPStatusCode newStatusCode);
    HTTPStatusCode getStatusCode(void);
  public:
    const char * getReqPartValue(void);
    HTTPRequestPart getReqPart(void);
  private:
    Stream * client = NULL;
  private:
    const static size_t READ_BUFFER_SIZE = HTTP_REQUEST_PART_MAX_SIZE + 1 + 1; //+1 char for length limit detection and +1 char for null character
    char readBuffer[READ_BUFFER_SIZE];
  public:
    enum class ParserState {
      UNKNOWN,
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
  private:
    void transition(ParserState newState, HTTPStatusCode newStatusCode);
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
