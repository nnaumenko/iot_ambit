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
// HTTPReqParser
//////////////////////////////////////////////////////////////////////

boolean HTTPReqParser::begin(HTTPStream & client) {
  this->client = &client;
  return (this->prepareToParse());
}

void HTTPReqParser::setHandler (HTTPReqPartHandler &handler) {
  this->reqPartHandler = &handler;
  if (this->reqPartHandler != NULL) this->reqPartHandler->begin(*this, *this->client);
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
PROGMEM HTTPReqParserStateMachine::ProcessingTableEntry HTTPReqParserStateMachineProcessingTableInit[] = {
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

boolean HTTPReqParserStateMachine::ProcessingTable::begin (void) {
  table = HTTPReqParserStateMachineProcessingTableInit;
  tableSize = sizeof (HTTPReqParserStateMachineProcessingTableInit) / tableEntrySize;
  firstEntry();
  //TODO: validate table
  //check that all states are unique
  return (true);
}

boolean HTTPReqParserStateMachine::ProcessingTable::find (ParserState state) {
  firstEntry();
  do {
    if (getState() == state) return (true);
  } while (nextEntry());
  return (false);
}

HTTPReqParserStateMachine::StreamOperation HTTPReqParserStateMachine::ProcessingTable::getStreamOperation(void) {
  return ((StreamOperation)pgm_read_word(&table[tableIndex].operation));
}

HTTPRequestPart HTTPReqParserStateMachine::ProcessingTable::getRequestPart(void) {
  return ((HTTPRequestPart)pgm_read_word(&table[tableIndex].part));
}

HTTPReqParserStateMachine::ParserState HTTPReqParserStateMachine::ProcessingTable::getState(void) {
  return ((ParserState)pgm_read_word(&table[tableIndex].state));
}


//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine::TransitionTable
//////////////////////////////////////////////////////////////////////

//Transition table defines state machine transitions as follows:
//Based on Current State and Next Character (the character where parser stopped reading from stream), a new state and a new HTTP status code are acquired
PROGMEM HTTPReqParserStateMachine::TransitionTableEntry HTTPReqParserStateMachineTransitionTableInit[] = {
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
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::ERROR,                HTTPStatusCode::BAD_REQUEST},
  { HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPStatusCode::NO_CHANGE},
};

boolean HTTPReqParserStateMachine::TransitionTable::begin (void) {
  table = HTTPReqParserStateMachineTransitionTableInit;
  tableSize = sizeof (HTTPReqParserStateMachineTransitionTableInit) / tableEntrySize;
  //TODO: validate table
  //Check that all state/char combinations are unique
  //Check that all states have OTHER_CHAR
  firstEntry();
  return (true);
}

void HTTPReqParserStateMachine::TransitionTable::find (ParserState state, char nextChar) {
  firstEntry();
  do {
    if ((getState() == state) && ((getNextChar() == nextChar) || (getNextChar() == CHAR_OTHER))) return;
  } while (nextEntry());
  //Assuming here that every state has OTHER_CHAR entry
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
  return ((HTTPReqParserStateMachine::ParserState)pgm_read_word(&table[tableIndex].newState));
}

HTTPStatusCode HTTPReqParserStateMachine::TransitionTable::getNewStatusCode(void) {
  return ((HTTPStatusCode)pgm_read_word(&table[tableIndex].newStatusCode));
}

HTTPReqParserStateMachine::ParserState HTTPReqParserStateMachine::TransitionTable::getState(void) {
  return ((HTTPReqParserStateMachine::ParserState)pgm_read_word(&table[tableIndex].state));
}
char HTTPReqParserStateMachine::TransitionTable::getNextChar(void) {
  return ((char)pgm_read_byte(&table[tableIndex].nextChar));
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine
//////////////////////////////////////////////////////////////////////

boolean HTTPReqParserStateMachine::prepareToParse(void) {
  this->parserStatus.statusCode = HTTPStatusCode::CONTINUE;
  this->parserStatus.requestPart = HTTPRequestPart::NONE;
  this->parserStatus.currentState = ParserState::BEGIN;
  this->parserStatus.nextCharacter = '\0';
  this->processingTable.begin();
  this->transitionTable.begin();
  return (true);
}

void HTTPReqParserStateMachine::parse(void) {
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
    if (this->parserStatus.requestPart != HTTPRequestPart::NONE) reqPartHandler->handleReqPart(readBuffer, this->parserStatus.requestPart);
  }
  this->transitionTable.find(this->parserStatus.currentState, this->parserStatus.nextCharacter);
  this->transition(this->transitionTable.getNewState(), this->transitionTable.getNewStatusCode());
}

void HTTPReqParserStateMachine::transition(HTTPReqParserStateMachine::ParserState newState, HTTPStatusCode newStatusCode) {
  this->parserStatus.currentState = newState;
  //TODO:add check if the state exists in the processing and transition table
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
