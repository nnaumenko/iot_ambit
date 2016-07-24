/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "http.h"

//////////////////////////////////////////////////////////////////////
// HTTPStream
//////////////////////////////////////////////////////////////////////

HTTPStream::HTTPStream (Stream & client) {
  this->client = &client;
}

HTTPStream::~HTTPStream() {
  this->sendOutputBuffer();
}

int HTTPStream::available() {
  return (this->client->available());
}

int HTTPStream::read() {
  int i = this->client->read();
  if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
  char c = (char)i;
  if (((c == '\r') && (previousChar == '\n')) ||
      ((c == '\n') && (previousChar == '\r'))) {
    i = this->client->read();
    if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
    c = (char)i;
    this->previousChar = '\0'; //make sure next char will not be treated as part of sequence
    if (c == '\r') c = '\n';
    return (c);
  }
  this->previousChar = c;
  if (c == '\r') c = '\n';
  return (c);
}

int HTTPStream::peek() {
  int i = this->client->peek();
  if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
  char c = (char)i;
  if (((c == '\r') && (previousChar == '\n')) ||
      ((c == '\n') && (previousChar == '\r'))) {
    this->client->read();
    i = this->client->peek();
    if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
    this->previousChar = '\0'; //make sure next char will not be treated as part of sequence
    c = (char)i;
    if (c == '\r') c = '\n';
    return (c);
  }
  this->previousChar = c;
  if (c == '\r') c = '\n';
  return (c);
}

void HTTPStream::flush() {
  this->sendOutputBuffer();
  return (this->client->flush());
}

size_t HTTPStream::write(uint8_t character) {
  outputBuffer[outputBufferPosition++] = (char)character;
  if (outputBufferPosition >= HTTPSTREAM_OUT_BUFFER_SIZE) sendOutputBuffer();
  return (sizeof(character));
}

void HTTPStream::sendOutputBuffer(void) {
  this->client->write((char *)this->outputBuffer, outputBufferPosition);
  outputBufferPosition = 0;
}

void HTTPStream::readUntil(const char * charsToFind, char * buffer, size_t bufferSize) {
  if (buffer != NULL) buffer[0] = 0;
  if ((buffer != NULL) && (!bufferSize)) return;
  if (charsToFind == NULL) return;
  for (size_t bufferPos = 0; bufferPos < (bufferSize - 1); bufferPos++) {
    if (buffer != NULL) buffer[bufferPos] = 0;
    int i = this->peek();
    if (i == NOT_AVAILABLE) return;
    char c = (char) i;
    if (strchr(charsToFind, c)) return;
    if (buffer != NULL) buffer[bufferPos] = c;
    this->read();
  }

  if (buffer != NULL) buffer[bufferSize - 1] = 0;
}

//////////////////////////////////////////////////////////////////////
// HTTPPercentCode
//////////////////////////////////////////////////////////////////////

int HTTPPercentCode::decode(char buffer[]) {
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
  if (!buffer[0]) return;
  size_t  readPosition = 0;
  size_t  writePosition = 0;
  do {
    char c = buffer[readPosition];
    if (c == '%') { //following 2 characters are percent hex code
      int hexValueDecoded = HTTPPercentCode::decode(&buffer[readPosition + 1]);
      if (hexValueDecoded != HTTPPercentCode::decodeError) {
        buffer[writePosition++] = (char)hexValueDecoded;
      }
      readPosition += HTTPPercentCode::size;
    }
    else {
      if (c == '+') c = ' ';
      buffer[writePosition++] = c;
      readPosition ++;
    }
  } while ((buffer[readPosition]) && (readPosition < (bufferSize - 1))); //assuming that writePosition <= readPosition
  buffer[writePosition] = 0;
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParser::ParserTableBase
//////////////////////////////////////////////////////////////////////

HTTPReqParser::ParserTableBase::ParserTableBase() {
  firstEntry();
}

size_t HTTPReqParser::ParserTableBase::getTableIndex(void) {
  return (tableIndex);
}


boolean HTTPReqParser::ParserTableBase::setTableIndex(size_t newIndex) {
  if (tableIndex >= (tableSize - 1)) return (false);
}

void HTTPReqParser::ParserTableBase::firstEntry(void) {
  tableIndex = 0;
}

boolean HTTPReqParser::ParserTableBase::nextEntry(void) {
  if (tableIndex >= (tableSize - 1)) return (false);
  tableIndex++;
  return (true);
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParser::ProcessingTable
//////////////////////////////////////////////////////////////////////

//Processing table defines operations performed by state machine at the particular state as follows
//Based on Current State, a Stream Operation is performed on the HTTP client stream
//Current State defines, which HTTP Request Part (e.g. method, path, version, fields, etc...) it corresponds to
PROGMEM HTTPReqParser::ProcessingTableEntry HTTPReqParserProcessingTableInit[] = {
  //Current State                                     Stream Operation                             Request Part
  { HTTPReqParser::ParserState::UNKNOWN,              HTTPReqParser::StreamOperation::FLUSH,       HTTPRequestPart::NONE},
  { HTTPReqParser::ParserState::ERROR,                HTTPReqParser::StreamOperation::FLUSH,       HTTPRequestPart::NONE},
  { HTTPReqParser::ParserState::BEGIN,                HTTPReqParser::StreamOperation::DO_NOTHING,  HTTPRequestPart::NONE},
  { HTTPReqParser::ParserState::METHOD,               HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::METHOD},
  { HTTPReqParser::ParserState::PATH,                 HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::PATH},
  { HTTPReqParser::ParserState::URL_QUERY_NAME,       HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::GET_QUERY_NAME},
  { HTTPReqParser::ParserState::URL_QUERY_VALUE,      HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::GET_QUERY_VALUE},
  { HTTPReqParser::ParserState::HTTP_VERSION,         HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::VERSION},
  { HTTPReqParser::ParserState::FIELD_OR_HEADER_END,  HTTPReqParser::StreamOperation::PEEK,        HTTPRequestPart::NONE},
  { HTTPReqParser::ParserState::FIELD_NAME,           HTTPReqParser::StreamOperation::SKIP,        HTTPRequestPart::FIELD_NAME},
  { HTTPReqParser::ParserState::FIELD_VALUE,          HTTPReqParser::StreamOperation::SKIP,        HTTPRequestPart::FIELD_VALUE},
  { HTTPReqParser::ParserState::HEADER_END,           HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::NONE},
  { HTTPReqParser::ParserState::POST_QUERY_OR_END,    HTTPReqParser::StreamOperation::PEEK,        HTTPRequestPart::NONE},
  { HTTPReqParser::ParserState::POST_QUERY_NAME,      HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::POST_QUERY_NAME},
  { HTTPReqParser::ParserState::POST_QUERY_VALUE,     HTTPReqParser::StreamOperation::READ,        HTTPRequestPart::POST_QUERY_VALUE},
  { HTTPReqParser::ParserState::FINISHED,             HTTPReqParser::StreamOperation::FLUSH,       HTTPRequestPart::NONE},
};

boolean HTTPReqParser::ProcessingTable::begin (void) {
  table = HTTPReqParserProcessingTableInit;
  tableSize = sizeof (HTTPReqParserProcessingTableInit) / tableEntrySize;
  firstEntry();
  //TODO: validate table
  //check that all states are unique
  return (true);
}

boolean HTTPReqParser::ProcessingTable::find (ParserState state) {
  firstEntry();
  do {
    if (getState() == state) return (true);
  } while (nextEntry());
  return (false);
}

HTTPReqParser::StreamOperation HTTPReqParser::ProcessingTable::getStreamOperation(void) {
  return ((StreamOperation)pgm_read_word(&table[tableIndex].operation));
}

HTTPRequestPart HTTPReqParser::ProcessingTable::getRequestPart(void) {
  return ((HTTPRequestPart)pgm_read_word(&table[tableIndex].part));
}

HTTPReqParser::ParserState HTTPReqParser::ProcessingTable::getState(void) {
  return ((ParserState)pgm_read_word(&table[tableIndex].state));
}


//////////////////////////////////////////////////////////////////////
// HTTPReqParser::TransitionTable
//////////////////////////////////////////////////////////////////////

//Transition table defines state machine transitions as follows:
//Based on Current State and Next Character (the character where parser stopped reading from stream), a new state and a new HTTP status code are acquired
PROGMEM HTTPReqParser::TransitionTableEntry HTTPReqParserTransitionTableInit[] = {
  //  Current state                                   Next char                     New state                   New status code
  { HTTPReqParser::ParserState::UNKNOWN,              HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::INTERNAL_SERVER_ERROR},
  { HTTPReqParser::ParserState::ERROR,                HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::NO_CHANGE},
  { HTTPReqParser::ParserState::BEGIN,                HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::METHOD,               HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::METHOD,               ' ',                          HTTPReqParser::ParserState::PATH,                 HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::METHOD,               HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::PATH,                 ' ',                          HTTPReqParser::ParserState::HTTP_VERSION,         HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::PATH,                 '?',                          HTTPReqParser::ParserState::URL_QUERY_NAME,       HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::PATH,                 HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::URL_QUERY_NAME,       '=',                          HTTPReqParser::ParserState::URL_QUERY_VALUE,      HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::URL_QUERY_NAME,       HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::URL_QUERY_VALUE,      '&',                          HTTPReqParser::ParserState::URL_QUERY_NAME,       HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::URL_QUERY_VALUE,      ' ',                          HTTPReqParser::ParserState::HTTP_VERSION,         HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::URL_QUERY_VALUE,      HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::HTTP_VERSION,         '\n',                         HTTPReqParser::ParserState::FIELD_OR_HEADER_END,  HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::HTTP_VERSION,         HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::FIELD_OR_HEADER_END,  '\n',                         HTTPReqParser::ParserState::HEADER_END,           HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::FIELD_OR_HEADER_END,  HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::FIELD_NAME,           HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::FIELD_NAME,           ':',                          HTTPReqParser::ParserState::FIELD_VALUE,          HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::FIELD_NAME,           HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::FIELD_VALUE,          '\n',                         HTTPReqParser::ParserState::FIELD_OR_HEADER_END,  HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::FIELD_VALUE,          HTTPReqParser::CHAR_UNAVAIL,  HTTPReqParser::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParser::ParserState::FIELD_VALUE,          HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::HEADER_END,           '\n',                         HTTPReqParser::ParserState::POST_QUERY_OR_END,    HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::HEADER_END,           HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::POST_QUERY_OR_END,    HTTPReqParser::CHAR_UNAVAIL,  HTTPReqParser::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParser::ParserState::POST_QUERY_OR_END,    HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::POST_QUERY_NAME,      HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::POST_QUERY_NAME,      '=',                          HTTPReqParser::ParserState::POST_QUERY_VALUE,     HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::POST_QUERY_NAME,      HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParser::ParserState::POST_QUERY_VALUE,     '&',                          HTTPReqParser::ParserState::POST_QUERY_NAME,      HTTPStatusCode::CONTINUE},
  { HTTPReqParser::ParserState::POST_QUERY_VALUE,     '\n',                         HTTPReqParser::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParser::ParserState::POST_QUERY_VALUE,     HTTPReqParser::CHAR_UNAVAIL,  HTTPReqParser::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParser::ParserState::URL_QUERY_VALUE,      HTTPReqParser::CHAR_OTHER,    HTTPReqParser::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST}
};

boolean HTTPReqParser::TransitionTable::begin (void) {
  table = HTTPReqParserTransitionTableInit;
  tableSize = sizeof (HTTPReqParserTransitionTableInit) / tableEntrySize;
  firstEntry();
  //TODO: validate table
  //Check that all state/char combinations are unique
  //Check that all states have OTHER_CHAR
  return (true);
}

void HTTPReqParser::TransitionTable::find (ParserState state, char nextChar) {
  firstEntry();
  do {
    if ((getState() == state) && ((getNextChar() == nextChar) || (getNextChar() == CHAR_OTHER))) return;
  } while (nextEntry());
  //Assuming here that every state has OTHER_CHAR entry
}

boolean HTTPReqParser::TransitionTable::enumerateNextChars(ParserState state, char * buffer, size_t bufferSize) {
  size_t currentIndex = getTableIndex();
  size_t bufferPos = 0;
  firstEntry();
  do {
    if ((getState() == state) && (getNextChar() != CHAR_OTHER)) {
      if (bufferPos < (bufferSize - 1))
        buffer[bufferPos++] = getNextChar();
      else
        return (false);
    }
  } while (nextEntry());
  buffer[bufferPos] = 0;
  setTableIndex(currentIndex);
  return (true);
}

HTTPReqParser::ParserState HTTPReqParser::TransitionTable::getNewState(void) {
  return ((HTTPReqParser::ParserState)pgm_read_word(&table[tableIndex].newState));
}

HTTPStatusCode HTTPReqParser::TransitionTable::getNewStatusCode(void) {
  return ((HTTPStatusCode)pgm_read_word(&table[tableIndex].newStatusCode));
}

HTTPReqParser::ParserState HTTPReqParser::TransitionTable::getState(void) {
  return ((HTTPReqParser::ParserState)pgm_read_word(&table[tableIndex].state));
}
char HTTPReqParser::TransitionTable::getNextChar(void) {
  return ((char)pgm_read_byte(&table[tableIndex].nextChar));
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParser
//////////////////////////////////////////////////////////////////////

boolean HTTPReqParser::begin(HTTPStream & client) {
  this->client = &client;
  this->parserStatus.statusCode = HTTPStatusCode::CONTINUE;
  this->parserStatus.requestPart = HTTPRequestPart::NONE;
  this->parserStatus.currentState = ParserState::BEGIN;
  this->parserStatus.nextCharacter = '\0';
  this->processingTable.begin();
  this->transitionTable.begin();
  return (true);
}

void HTTPReqParser::setHandler (HTTPReqPartHandler &handler) {
  this->reqPartHandler = &handler;
  this->reqPartHandler->begin(*this);
}

void HTTPReqParser::parse(void) {
  if (this->client == NULL) {
    this->setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);
    return;
  }
  const size_t NEXT_CHARS_SIZE = 5; //max 4 characters per parser state + null character
  char nextChars[NEXT_CHARS_SIZE];
  const static size_t READ_BUFFER_SIZE = HTTP_REQUEST_PART_MAX_SIZE + 1 + 1; //+1 char for length limit detection and +1 char for null character
  char readBuffer[READ_BUFFER_SIZE];
  readBuffer[0] = '\0';

  if (!this->processingTable.find(this->parserStatus.currentState)) setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);

  int nextCharAsInt;
  switch (this->processingTable.getStreamOperation()) {
    case StreamOperation::FLUSH:
      this->client->flush();
      nextCharAsInt = HTTPStream::NOT_AVAILABLE;
      break;
    case StreamOperation::DO_NOTHING:
      nextCharAsInt = HTTPStream::NOT_AVAILABLE;
      break;
    case StreamOperation::READ:
      this->transitionTable.enumerateNextChars(this->parserStatus.currentState, nextChars, NEXT_CHARS_SIZE);
      this->client->readUntil(nextChars, readBuffer, READ_BUFFER_SIZE);
      if (strlen(readBuffer) > HTTP_REQUEST_PART_MAX_SIZE) {
        this->setError(HTTPStatusCode::REQUEST_URI_TOO_LONG);
        return;
      }
      nextCharAsInt = this->client->read();
      break;
    case StreamOperation::SKIP:
      this->transitionTable.enumerateNextChars(this->parserStatus.currentState, nextChars, NEXT_CHARS_SIZE);
      this->client->readUntil(nextChars, NULL, 0);
      nextCharAsInt = this->client->read();
      break;
    case StreamOperation::PEEK:
      nextCharAsInt = this->client->peek();
      break;
    default:
      this->setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);
      break;
  }
  URL::decode(readBuffer, READ_BUFFER_SIZE);
  this->parserStatus.nextCharacter = (char)nextCharAsInt;
  this->parserStatus.requestPart = this->processingTable.getRequestPart();
  if (this->reqPartHandler != NULL) {
    if (strlen(readBuffer) > 0) reqPartHandler->execute(readBuffer, this->parserStatus.requestPart);
  }
  this->transitionTable.find(this->parserStatus.currentState, this->parserStatus.nextCharacter);
  this->transition(this->transitionTable.getNewState(), this->transitionTable.getNewStatusCode());
}

void HTTPReqParser::transition(HTTPReqParser::ParserState newState, HTTPStatusCode newStatusCode) {
  this->parserStatus.currentState = newState;
  //TODO:add check if the state exists in the processing and transition table
  if (newStatusCode != HTTPStatusCode::NO_CHANGE) this->parserStatus.statusCode = newStatusCode;
}

boolean HTTPReqParser::finished(void) {
  if (this->parserStatus.currentState == ParserState::FINISHED) return (true);
  if (this->parserStatus.currentState == ParserState::ERROR) return (true);
  return (false);
}

void HTTPReqParser::setError(HTTPStatusCode errorStatusCode) {
  this->transition(ParserState::ERROR, errorStatusCode);
}

boolean HTTPReqParser::isError(void) {
  return (this->parserStatus.currentState == ParserState::ERROR);
}

HTTPStatusCode HTTPReqParser::getStatusCode(void) {
  return (this->parserStatus.statusCode);
}
