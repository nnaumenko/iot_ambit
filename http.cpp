/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "http.h"
#include "diag.h"

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
  if (buffer) buffer[0] = 0;
  if (buffer && !bufferSize) return;
  if (!charsToFind) return;
  for (size_t bufferPos = 0; bufferPos < (bufferSize - 1); bufferPos++) {
    if (buffer) buffer[bufferPos] = 0;
    int i = this->peek();
    if (i == NOT_AVAILABLE) return;
    char c = (char) i;
    if (strchr(charsToFind, c)) return;
    if (buffer) buffer[bufferPos] = c;
    this->read();
  }
  if (buffer) buffer[bufferSize - 1] = 0;
}

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
// HTTPReqParser
//////////////////////////////////////////////////////////////////////

boolean HTTPReqParser::begin(HTTPStream & client) {
  this->client = &client;
  return (this->prepareToParse());
}

void HTTPReqParser::setHandler (HTTPReqPartHandler &handler) {
  this->reqPartHandler = &handler;
  if (this->reqPartHandler) this->reqPartHandler->begin(*this, *this->client);
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine::ParserTableBase
//////////////////////////////////////////////////////////////////////

HTTPReqParserStateMachine::ParserTableBase::ParserTableBase() {
  firstEntry();
}

size_t HTTPReqParserStateMachine::ParserTableBase::getTableIndex(void) {
  return (tableIndex);
}


boolean HTTPReqParserStateMachine::ParserTableBase::setTableIndex(size_t newIndex) {
  if (tableIndex >= (tableSize - 1)) return (false);
  tableIndex = newIndex;
  return (true);
}

void HTTPReqParserStateMachine::ParserTableBase::firstEntry(void) {
  tableIndex = 0;
}

boolean HTTPReqParserStateMachine::ParserTableBase::nextEntry(void) {
  if (tableIndex >= (tableSize - 1)) return (false);
  tableIndex++;
  return (true);
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine::ProcessingTable
//////////////////////////////////////////////////////////////////////

//Processing table defines operations performed by state machine at the particular state as follows
//Based on Current State, a Stream Operation is performed on the HTTP client stream
//Current State defines, which HTTP Request Part (e.g. method, path, version, fields, etc...) it corresponds to
const PROGMEM HTTPReqParserStateMachine::ProcessingTableEntry HTTPReqParserStateMachineProcessingTableInit[] = {
  //Current State                                                 Stream Operation                                         Request Part
  { HTTPReqParserStateMachine::ParserState::UNKNOWN,              HTTPReqParserStateMachine::StreamOperation::FLUSH,       HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::ERROR,                HTTPReqParserStateMachine::StreamOperation::FLUSH,       HTTPRequestPart::FINISH},
  { HTTPReqParserStateMachine::ParserState::BEGIN,                HTTPReqParserStateMachine::StreamOperation::DO_NOTHING,  HTTPRequestPart::BEGIN},
  { HTTPReqParserStateMachine::ParserState::METHOD,               HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::METHOD},
  { HTTPReqParserStateMachine::ParserState::PATH,                 HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::PATH},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::GET_QUERY_NAME},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::GET_QUERY_VALUE},
  { HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::VERSION},
  { HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  HTTPReqParserStateMachine::StreamOperation::PEEK,        HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::FIELD_NAME,           HTTPReqParserStateMachine::StreamOperation::SKIP,        HTTPRequestPart::FIELD_NAME},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          HTTPReqParserStateMachine::StreamOperation::SKIP,        HTTPRequestPart::FIELD_VALUE},
  { HTTPReqParserStateMachine::ParserState::HEADER_END,           HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    HTTPReqParserStateMachine::StreamOperation::PEEK,        HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::POST_QUERY_NAME},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::POST_QUERY_VALUE},
  { HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPReqParserStateMachine::StreamOperation::FLUSH,       HTTPRequestPart::FINISH},
};

HTTPReqParserStateMachine::ProcessingTable::ProcessingTable() {
  table = HTTPReqParserStateMachineProcessingTableInit;
  tableSize = sizeof (HTTPReqParserStateMachineProcessingTableInit) / tableEntrySize;
  this->isValidated = this->validate();
}

boolean HTTPReqParserStateMachine::ProcessingTable::begin (void) {
  firstEntry();
  return (this->isValidated);
}

boolean HTTPReqParserStateMachine::ProcessingTable::validate (void) {
  for (size_t i = 0; i < tableSize; i++) {
    uint32_t * statePointerI = (uint32_t *) &table[i].state;
    for (size_t j = i + 1; j < tableSize; j++) {
      //check that all states are unique
      uint32_t * statePointerJ = (uint32_t *) &table[j].state;
      if (pgm_read_dword(statePointerI) == pgm_read_dword(statePointerJ)) return (false);
    }
  }
  return (true);
  static_assert(sizeof(table[0].state) == 4, "table[].state is expected to be dword");
}

boolean HTTPReqParserStateMachine::ProcessingTable::find (ParserState state) {
  firstEntry();
  do {
    if (getState() == state) return (true);
  } while (nextEntry());
  return (false);
}

HTTPReqParserStateMachine::StreamOperation HTTPReqParserStateMachine::ProcessingTable::getStreamOperation(void) {
  uint32_t * streamOperationPointer = (uint32_t *)&table[tableIndex].operation;
  return ((StreamOperation)pgm_read_dword(streamOperationPointer));
  static_assert (sizeof(table[0].operation) == 4, "table[].operation is expected to be dword");
}


HTTPRequestPart HTTPReqParserStateMachine::ProcessingTable::getRequestPart(void) {
  uint32_t * requestPartPointer = (uint32_t *)&table[tableIndex].part;
  return ((HTTPRequestPart)pgm_read_dword(requestPartPointer));
  static_assert (sizeof(table[0].part) == 4, "table[].part is expected to be dword");
}


HTTPReqParserStateMachine::ParserState HTTPReqParserStateMachine::ProcessingTable::getState(void) {
  uint32_t * statePointer = (uint32_t *)&table[tableIndex].state;
  return ((ParserState)pgm_read_dword(statePointer));
  static_assert (sizeof(table[0].state) == 4, "table[].state is expected to be dword");
}


//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine::TransitionTable
//////////////////////////////////////////////////////////////////////

//Transition table defines state machine transitions as follows:
//Based on Current State and Next Character (the character where parser stopped reading from stream), a new state and a new HTTP status code are acquired
const PROGMEM HTTPReqParserStateMachine::TransitionTableEntry HTTPReqParserStateMachineTransitionTableInit[] = {
  //  Current state                                               Next char                                 New state                                                     New status code
  { HTTPReqParserStateMachine::ParserState::UNKNOWN,              HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::INTERNAL_SERVER_ERROR},
  { HTTPReqParserStateMachine::ParserState::ERROR,                HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::NO_CHANGE},
  { HTTPReqParserStateMachine::ParserState::BEGIN,                HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::METHOD,               HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::METHOD,               ' ',                                      HTTPReqParserStateMachine::ParserState::PATH,                 HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::METHOD,               HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::PATH,                 ' ',                                      HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::PATH,                 '?',                                      HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::PATH,                 HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       '=',                                      HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      '&',                                      HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      ' ',                                      HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         '\n',                                     HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  '\n',                                     HTTPReqParserStateMachine::ParserState::HEADER_END,           HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FIELD_NAME,           HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::FIELD_NAME,           ':',                                      HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::FIELD_NAME,           HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          '\n',                                     HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          HTTPReqParserStateMachine::CHAR_UNAVAIL,  HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::HEADER_END,           '\n',                                     HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::HEADER_END,           HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    HTTPReqParserStateMachine::CHAR_UNAVAIL,  HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      '=',                                      HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     '&',                                      HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      HTTPStatusCode::CONTINUE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     '\n',                                     HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     HTTPReqParserStateMachine::CHAR_UNAVAIL,  HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPStatusCode::OK},
  { HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPStatusCode::NO_CHANGE},
};

HTTPReqParserStateMachine::TransitionTable::TransitionTable() {
  table = HTTPReqParserStateMachineTransitionTableInit;
  tableSize = sizeof (HTTPReqParserStateMachineTransitionTableInit) / tableEntrySize;
  this->isValidated = this->validate();
}

boolean HTTPReqParserStateMachine::TransitionTable::begin (void) {
  firstEntry();
  return (this->isValidated);
}

boolean HTTPReqParserStateMachine::TransitionTable::validate(void) {
  for (size_t i = 0; i < tableSize; i++) {
    //check that all state/char combinations are unique
    uint32_t * statePointerI = (uint32_t *) &table[i].state;
    char * nextCharPointerI = (char *) &table[i].nextChar;
    for (size_t j = i + 1; j < tableSize; j++) {
      uint32_t * statePointerJ = (uint32_t *) &table[j].state;
      char * nextCharPointerJ = (char *) &table[j].nextChar;
      if ((pgm_read_dword(statePointerI) == pgm_read_dword(statePointerJ))
          && (pgm_read_byte(nextCharPointerI) == pgm_read_byte(nextCharPointerJ))) return (false);
    }
    //check that all states have OTHER_CHAR
    boolean otherCharFound = false;
    for (size_t j = 0; j < tableSize; j++) {
      uint32_t * statePointerJ = (uint32_t *) &table[j].state;
      char * nextCharPointerJ = (char *) &table[j].nextChar;
      if ((pgm_read_dword(statePointerI) == pgm_read_dword(statePointerJ)) &&
          (pgm_read_byte(nextCharPointerJ) == HTTPReqParserStateMachine::CHAR_OTHER)) {
        otherCharFound = true;
        break;
      }
    }
    if (!otherCharFound) return (false);
  }
  return (true);
  static_assert(sizeof(table[0].state) == 4, "table[].state is expected to be dword");
  static_assert(sizeof(table[0].nextChar) == 1, "table[].newChar is expected to be byte");
}

boolean HTTPReqParserStateMachine::TransitionTable::find (ParserState state, char nextChar) {
  firstEntry();
  do {
    if ((getState() == state) && ((getNextChar() == nextChar) || (getNextChar() == CHAR_OTHER))) return (true);
  } while (nextEntry());
  return (false);
}

boolean HTTPReqParserStateMachine::TransitionTable::enumerateNextChars(ParserState state, char * buffer, size_t bufferSize) {
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

HTTPReqParserStateMachine::ParserState HTTPReqParserStateMachine::TransitionTable::getNewState(void) {
  uint32_t * newStatePointer = (uint32_t *)&table[tableIndex].newState;
  return ((HTTPReqParserStateMachine::ParserState)pgm_read_word(newStatePointer));
  static_assert (sizeof(table[0].newState) == 4, "table[].newState is expected to be dword");
}

HTTPStatusCode HTTPReqParserStateMachine::TransitionTable::getNewStatusCode(void) {
  uint32_t * newStatusCodePointer = (uint32_t *)&table[tableIndex].newStatusCode;
  return ((HTTPStatusCode)pgm_read_word(newStatusCodePointer));
  static_assert (sizeof(table[0].newStatusCode) == 4, "table[].newStatusCode is expected to be dword");
}

HTTPReqParserStateMachine::ParserState HTTPReqParserStateMachine::TransitionTable::getState(void) {
  uint32_t * statePointer = (uint32_t *)&table[tableIndex].state;
  return ((HTTPReqParserStateMachine::ParserState)pgm_read_word(statePointer));
  static_assert (sizeof(table[0].state) == 4, "table[].operation is expected to be dword");
}
char HTTPReqParserStateMachine::TransitionTable::getNextChar(void) {
  char * nextCharPointer = (char *) &table[tableIndex].nextChar;
  return ((char)pgm_read_byte(nextCharPointer));
  static_assert (sizeof(table[0].nextChar) == 1, "table[].nextChar is expected to be byte");
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine
//////////////////////////////////////////////////////////////////////

boolean HTTPReqParserStateMachine::prepareToParse(void) {
  boolean returnValue = true;
  this->parserStatus.statusCode = HTTPStatusCode::CONTINUE;
  this->parserStatus.requestPart = HTTPRequestPart::NONE;
  this->parserStatus.currentState = ParserState::BEGIN;
  this->parserStatus.nextCharacter = '\0';
  if (!this->processingTable.begin()) {
    DiagLog.println(F("HTTP Parser Processing Table failed validation."));
    this->setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);
    returnValue = false;
  }
  if (!this->transitionTable.begin()) {
    DiagLog.println(F("HTTP Parser Transition Table failed validation."));
    this->setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);
    returnValue = false;
  }
  return (returnValue);
}

void HTTPReqParserStateMachine::parse(void) {
  if (!this->client) {
    DiagLog.println(F("HTTP parser's client not initialised."));
    this->setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);
    return;
  }
  const size_t NEXT_CHARS_SIZE = 5; //max 4 characters per parser state + null character
  char nextChars[NEXT_CHARS_SIZE];
  const static size_t READ_BUFFER_SIZE = HTTP_REQUEST_PART_MAX_SIZE + 1 + 1; //+1 char for length limit detection and +1 char for null character
  char readBuffer[READ_BUFFER_SIZE];
  readBuffer[0] = '\0';

  if (!this->processingTable.find(this->parserStatus.currentState)) {
    DiagLog.print(F("Could not find HTTP parser state in Processing Table: "));
    DiagLog.println((long)this->parserStatus.currentState);
    setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);
  }

  int nextCharAsInt = HTTPStream::NOT_AVAILABLE;
  switch (this->processingTable.getStreamOperation()) {
    case StreamOperation::FLUSH:
      this->client->flush();
      break;
    case StreamOperation::DO_NOTHING:
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
      DiagLog.print(F("Unknown HTTP Stream Operation: "));
      DiagLog.println((long)this->processingTable.getStreamOperation());
      break;
  }
  URL::decode(readBuffer, READ_BUFFER_SIZE);
  this->parserStatus.nextCharacter = (char)nextCharAsInt;
  this->parserStatus.requestPart = this->processingTable.getRequestPart();
  if (this->reqPartHandler) {
    if (this->parserStatus.requestPart != HTTPRequestPart::NONE) reqPartHandler->handleReqPart(readBuffer, this->parserStatus.requestPart);
  }
  if (!this->transitionTable.find(this->parserStatus.currentState, this->parserStatus.nextCharacter)) {
    this->setError(HTTPStatusCode::INTERNAL_SERVER_ERROR);
    DiagLog.print(F("Lookup in HTTP parser's transition table failed: state "));
    DiagLog.print((long)this->parserStatus.currentState);
    DiagLog.print(F(" nextChar 0x"));
    DiagLog.println((int)this->parserStatus.nextCharacter, HEX);
  }
  this->transition(this->transitionTable.getNewState(), this->transitionTable.getNewStatusCode());
}

void HTTPReqParserStateMachine::transition(HTTPReqParserStateMachine::ParserState newState, HTTPStatusCode newStatusCode) {
  this->parserStatus.currentState = newState;
  //TODO:add check if the state exists in the processing and transition tables
  if (newStatusCode != HTTPStatusCode::NO_CHANGE) this->parserStatus.statusCode = newStatusCode;
}

boolean HTTPReqParserStateMachine::isFinished(void) {
  return (this->parserStatus.requestPart == HTTPRequestPart::FINISH);
}

void HTTPReqParserStateMachine::setError(HTTPStatusCode errorStatusCode) {
  this->transition(ParserState::ERROR, errorStatusCode);
}

boolean HTTPReqParserStateMachine::isError(void) {
  return (this->parserStatus.currentState == ParserState::ERROR);
}

HTTPStatusCode HTTPReqParserStateMachine::getStatusCode(void) {
  return (this->parserStatus.statusCode);
}
