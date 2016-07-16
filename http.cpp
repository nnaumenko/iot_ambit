/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "http.h"

//////////////////////////////////////////////////////////////////////
// HttpStream
//////////////////////////////////////////////////////////////////////

HttpStream::HttpStream (Stream & client) {
  this->client = &client;
}

HttpStream::~HttpStream() {
  this->sendOutputBuffer();
}

int HttpStream::available() {
  return (this->client->available());
}

int HttpStream::read() {
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

int HttpStream::peek() {
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

void HttpStream::flush() {
  this->sendOutputBuffer();
  return (this->client->flush());
}

size_t HttpStream::write(uint8_t character) {
  outputBuffer[outputBufferPosition++] = (char)character;
  if (outputBufferPosition >= HTTPSTREAM_OUT_BUFFER_SIZE) sendOutputBuffer();
  return (sizeof(character));
}

void HttpStream::sendOutputBuffer(void) {
  this->client->write((char *)this->outputBuffer, outputBufferPosition);
  outputBufferPosition = 0;
}

void HttpStream::readUntil(const char * charsToFind, char * buffer, size_t bufferSize) {
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
// HttpPercentCode
//////////////////////////////////////////////////////////////////////

int HttpPercentCode::decode(char buffer[]) {
  const int RADIX_HEXADECIMAL = 16;
  int mostSignificant = decodeDigit(buffer[0]);
  if (mostSignificant == decodeError) return (decodeError);
  int leastSignificant = decodeDigit(buffer[1]);
  if (leastSignificant == decodeError) return (decodeError);
  return (mostSignificant * RADIX_HEXADECIMAL + leastSignificant);
}

int HttpPercentCode::decodeDigit(char hexDigit) {
  if ((hexDigit >= '0') && (hexDigit <= '9')) return (hexDigit - '0');
  if ((hexDigit >= 'A') && (hexDigit <= 'F')) return (hexDigit - 'A' + 10);
  if ((hexDigit >= 'a') && (hexDigit <= 'f')) return (hexDigit - 'a' + 10);
  return (decodeError);
}

//////////////////////////////////////////////////////////////////////
// HttpURL
//////////////////////////////////////////////////////////////////////

void HttpURL::decode(char buffer[], size_t bufferSize) {
  if (!buffer[0]) return;
  size_t  readPosition = 0;
  size_t  writePosition = 0;
  do {
    char c = buffer[readPosition];
    if (c == '%') { //following 2 characters are percent hex code
      int hexValueDecoded = HttpPercentCode::decode(&buffer[readPosition + 1]);
      if (hexValueDecoded != HttpPercentCode::decodeError) {
        buffer[writePosition++] = (char)hexValueDecoded;
      }
      readPosition += HttpPercentCode::size;
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
// HttpReqParser::ParserTableBase
//////////////////////////////////////////////////////////////////////

HttpReqParser::ParserTableBase::ParserTableBase() {
  firstEntry();
}

size_t HttpReqParser::ParserTableBase::getTableIndex(void) {
  return (tableIndex);
}


boolean HttpReqParser::ParserTableBase::setTableIndex(size_t newIndex) {
  if (tableIndex >= (tableSize - 1)) return (false);
}

void HttpReqParser::ParserTableBase::firstEntry(void) {
  tableIndex = 0;
}

boolean HttpReqParser::ParserTableBase::nextEntry(void) {
  if (tableIndex >= (tableSize - 1)) return (false);
  tableIndex++;
  return (true);
}

//////////////////////////////////////////////////////////////////////
// HttpReqParser::ProcessingTable
//////////////////////////////////////////////////////////////////////

//Processing table defines operations performed by state machine at the particular state as follows
//Based on Current State, a Stream Operation is performed on the HTTP client stream
//Current State defines, which HTTP Request Part (e.g. method, path, version, fields, etc...) it corresponds to
PROGMEM HttpReqParser::ProcessingTableEntry HttpReqParserProcessingTableInit[] = {
  //Current State                                     Stream Operation                             Request Part
  { HttpReqParser::ParserState::UNKNOWN,              HttpReqParser::StreamOperation::FLUSH,       HttpRequestPart::NONE},
  { HttpReqParser::ParserState::ERROR,                HttpReqParser::StreamOperation::FLUSH,       HttpRequestPart::NONE},
  { HttpReqParser::ParserState::BEGIN,                HttpReqParser::StreamOperation::DO_NOTHING,  HttpRequestPart::NONE},
  { HttpReqParser::ParserState::METHOD,               HttpReqParser::StreamOperation::READ,        HttpRequestPart::METHOD},
  { HttpReqParser::ParserState::PATH,                 HttpReqParser::StreamOperation::READ,        HttpRequestPart::PATH},
  { HttpReqParser::ParserState::URL_QUERY_NAME,       HttpReqParser::StreamOperation::READ,        HttpRequestPart::GET_QUERY_NAME},
  { HttpReqParser::ParserState::URL_QUERY_VALUE,      HttpReqParser::StreamOperation::READ,        HttpRequestPart::GET_QUERY_VALUE},
  { HttpReqParser::ParserState::HTTP_VERSION,         HttpReqParser::StreamOperation::READ,        HttpRequestPart::VERSION},
  { HttpReqParser::ParserState::FIELD_OR_HEADER_END,  HttpReqParser::StreamOperation::PEEK,        HttpRequestPart::NONE},
  { HttpReqParser::ParserState::FIELD_NAME,           HttpReqParser::StreamOperation::SKIP,        HttpRequestPart::FIELD_NAME},
  { HttpReqParser::ParserState::FIELD_VALUE,          HttpReqParser::StreamOperation::SKIP,        HttpRequestPart::FIELD_VALUE},
  { HttpReqParser::ParserState::HEADER_END,           HttpReqParser::StreamOperation::READ,        HttpRequestPart::NONE},
  { HttpReqParser::ParserState::POST_QUERY_OR_END,    HttpReqParser::StreamOperation::PEEK,        HttpRequestPart::NONE},
  { HttpReqParser::ParserState::POST_QUERY_NAME,      HttpReqParser::StreamOperation::READ,        HttpRequestPart::POST_QUERY_NAME},
  { HttpReqParser::ParserState::POST_QUERY_VALUE,     HttpReqParser::StreamOperation::READ,        HttpRequestPart::POST_QUERY_VALUE},
  { HttpReqParser::ParserState::FINISHED,             HttpReqParser::StreamOperation::FLUSH,       HttpRequestPart::NONE},
};

boolean HttpReqParser::ProcessingTable::begin (void) {
  table = HttpReqParserProcessingTableInit;
  tableSize = sizeof (HttpReqParserProcessingTableInit) / tableEntrySize;
  firstEntry();
  //TODO: validate table
  //check that all states are unique
  return (true);
}

boolean HttpReqParser::ProcessingTable::find (ParserState state) {
  firstEntry();
  do {
    if (getState() == state) return (true);
  } while (nextEntry());
  return (false);
}

HttpReqParser::StreamOperation HttpReqParser::ProcessingTable::getStreamOperation(void) {
  return ((StreamOperation)pgm_read_word(&table[tableIndex].operation));
}

HttpRequestPart HttpReqParser::ProcessingTable::getRequestPart(void) {
  return ((HttpRequestPart)pgm_read_word(&table[tableIndex].part));
}

HttpReqParser::ParserState HttpReqParser::ProcessingTable::getState(void) {
  return ((ParserState)pgm_read_word(&table[tableIndex].state));
}


//////////////////////////////////////////////////////////////////////
// HttpReqParser::TransitionTable
//////////////////////////////////////////////////////////////////////

//Transition table defines state machine transitions as follows:
//Based on Current State and Next Character (the character where parser stopped reading from stream), a new state and a new HTTP status code are acquired
PROGMEM HttpReqParser::TransitionTableEntry HttpReqParserTransitionTableInit[] = {
  //  Current state                                   Next char                     New state                   New status code
  { HttpReqParser::ParserState::UNKNOWN,              HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::INTERNAL_SERVER_ERROR},
  { HttpReqParser::ParserState::ERROR,                HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::NO_CHANGE},
  { HttpReqParser::ParserState::BEGIN,                HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::METHOD,               HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::METHOD,               ' ',                          HttpReqParser::ParserState::PATH,                 HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::METHOD,               HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::PATH,                 ' ',                          HttpReqParser::ParserState::HTTP_VERSION,         HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::PATH,                 '?',                          HttpReqParser::ParserState::URL_QUERY_NAME,       HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::PATH,                 HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::URL_QUERY_NAME,       '=',                          HttpReqParser::ParserState::URL_QUERY_VALUE,      HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::URL_QUERY_NAME,       HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::URL_QUERY_VALUE,      '&',                          HttpReqParser::ParserState::URL_QUERY_NAME,       HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::URL_QUERY_VALUE,      ' ',                          HttpReqParser::ParserState::HTTP_VERSION,         HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::URL_QUERY_VALUE,      HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::HTTP_VERSION,         '\n',                         HttpReqParser::ParserState::FIELD_OR_HEADER_END,  HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::HTTP_VERSION,         HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::FIELD_OR_HEADER_END,  '\n',                         HttpReqParser::ParserState::HEADER_END,           HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::FIELD_OR_HEADER_END,  HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::FIELD_NAME,           HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::FIELD_NAME,           ':',                          HttpReqParser::ParserState::FIELD_VALUE,          HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::FIELD_NAME,           HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::FIELD_VALUE,          '\n',                         HttpReqParser::ParserState::FIELD_OR_HEADER_END,  HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::FIELD_VALUE,          HttpReqParser::CHAR_UNAVAIL,  HttpReqParser::ParserState::FINISHED,             HttpStatusCode::OK},
  { HttpReqParser::ParserState::FIELD_VALUE,          HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::HEADER_END,           '\n',                         HttpReqParser::ParserState::POST_QUERY_OR_END,    HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::HEADER_END,           HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::POST_QUERY_OR_END,    HttpReqParser::CHAR_UNAVAIL,  HttpReqParser::ParserState::FINISHED,             HttpStatusCode::OK},
  { HttpReqParser::ParserState::POST_QUERY_OR_END,    HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::POST_QUERY_NAME,     HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::POST_QUERY_NAME,      '=',                          HttpReqParser::ParserState::POST_QUERY_VALUE,     HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::POST_QUERY_NAME,      HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST},
  { HttpReqParser::ParserState::POST_QUERY_VALUE,     '&',                          HttpReqParser::ParserState::POST_QUERY_NAME,      HttpStatusCode::CONTINUE},
  { HttpReqParser::ParserState::POST_QUERY_VALUE,     '\n',                         HttpReqParser::ParserState::FINISHED,             HttpStatusCode::OK},
  { HttpReqParser::ParserState::POST_QUERY_VALUE,     HttpReqParser::CHAR_UNAVAIL,  HttpReqParser::ParserState::FINISHED,             HttpStatusCode::OK},
  { HttpReqParser::ParserState::URL_QUERY_VALUE,      HttpReqParser::CHAR_OTHER,    HttpReqParser::ParserState::ERROR,                HttpStatusCode::BAD_REQUEST}
};

boolean HttpReqParser::TransitionTable::begin (void) {
  table = HttpReqParserTransitionTableInit;
  tableSize = sizeof (HttpReqParserTransitionTableInit) / tableEntrySize;
  firstEntry();
  //TODO: validate table
  //Check that all state/char combinations are unique
  //Check that all states have OTHER_CHAR
  return (true);
}

void HttpReqParser::TransitionTable::find (ParserState state, char nextChar) {
  firstEntry();
  do {
    if ((getState() == state) && ((getNextChar() == nextChar) || (getNextChar() == CHAR_OTHER))) return;
  } while (nextEntry());
  //Assuming here that every state has OTHER_CHAR entry
}

boolean HttpReqParser::TransitionTable::enumerateNextChars(ParserState state, char * buffer, size_t bufferSize) {
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

HttpReqParser::ParserState HttpReqParser::TransitionTable::getNewState(void) {
  return ((HttpReqParser::ParserState)pgm_read_word(&table[tableIndex].newState));
}

HttpStatusCode HttpReqParser::TransitionTable::getNewStatusCode(void) {
  return ((HttpStatusCode)pgm_read_word(&table[tableIndex].newStatusCode));
}

HttpReqParser::ParserState HttpReqParser::TransitionTable::getState(void) {
  return ((HttpReqParser::ParserState)pgm_read_word(&table[tableIndex].state));
}
char HttpReqParser::TransitionTable::getNextChar(void) {
  return ((char)pgm_read_byte(&table[tableIndex].nextChar));
}

//////////////////////////////////////////////////////////////////////
// HttpReqParser
//////////////////////////////////////////////////////////////////////

boolean HttpReqParser::begin(HttpStream & client) {
  this->client = &client;
  this->parserStatus.statusCode = HttpStatusCode::CONTINUE;
  this->parserStatus.requestPart = HttpRequestPart::NONE;
  this->parserStatus.currentState = ParserState::BEGIN;
  this->parserStatus.nextCharacter = '\0';
  this->processingTable.begin();
  this->transitionTable.begin();
  return (true);
}

void HttpReqParser::setHandler (HttpReqPartHandler &handler) {
  this->reqPartHandler = &handler;
  this->reqPartHandler->begin(*this);
}

void HttpReqParser::parse(void) {
  if (this->client == NULL) {
    this->setError(HttpStatusCode::INTERNAL_SERVER_ERROR);
    return;
  }
  const size_t NEXT_CHARS_SIZE = 5; //max 4 characters per parser state + null character
  char nextChars[NEXT_CHARS_SIZE];
  const static size_t READ_BUFFER_SIZE = HTTP_REQUEST_PART_MAX_SIZE + 1 + 1; //+1 char for length limit detection and +1 char for null character
  char readBuffer[READ_BUFFER_SIZE];
  readBuffer[0] = '\0';

  if (!this->processingTable.find(this->parserStatus.currentState)) setError(HttpStatusCode::INTERNAL_SERVER_ERROR);

  int nextCharAsInt;
  switch (this->processingTable.getStreamOperation()) {
    case StreamOperation::FLUSH:
      this->client->flush();
      nextCharAsInt = HttpStream::NOT_AVAILABLE;
      break;
    case StreamOperation::DO_NOTHING:
      nextCharAsInt = HttpStream::NOT_AVAILABLE;
      break;
    case StreamOperation::READ:
      this->transitionTable.enumerateNextChars(this->parserStatus.currentState, nextChars, NEXT_CHARS_SIZE);
      this->client->readUntil(nextChars, readBuffer, READ_BUFFER_SIZE);
      if (strlen(readBuffer) > HTTP_REQUEST_PART_MAX_SIZE) {
        this->setError(HttpStatusCode::REQUEST_URI_TOO_LONG);
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
      this->setError(HttpStatusCode::INTERNAL_SERVER_ERROR);
      break;
  }

  HttpURL::decode(readBuffer, READ_BUFFER_SIZE);

  this->parserStatus.nextCharacter = (char)nextCharAsInt;

  this->parserStatus.requestPart = this->processingTable.getRequestPart();

  if (this->reqPartHandler != NULL) {
    if (strlen(readBuffer) > 0) reqPartHandler->execute(readBuffer, this->parserStatus.requestPart);
  }

  this->transitionTable.find(this->parserStatus.currentState, this->parserStatus.nextCharacter);

  this->transition(this->transitionTable.getNewState(), this->transitionTable.getNewStatusCode());

}

void HttpReqParser::transition(HttpReqParser::ParserState newState, HttpStatusCode newStatusCode) {
  this->parserStatus.currentState = newState;
  //TODO:add check if the state exists in the processing and transition table
  if (newStatusCode != HttpStatusCode::NO_CHANGE) this->parserStatus.statusCode = newStatusCode;
}

boolean HttpReqParser::finished(void) {
  if (this->parserStatus.currentState == ParserState::FINISHED) return (true);
  if (this->parserStatus.currentState == ParserState::ERROR) return (true);
  return (false);
}

void HttpReqParser::setError(HttpStatusCode errorStatusCode) {
  this->transition(ParserState::ERROR, errorStatusCode);
}

boolean HttpReqParser::isError(void) {
  return (this->parserStatus.currentState == ParserState::ERROR);
}

HttpStatusCode HttpReqParser::getStatusCode(void) {
  return (this->parserStatus.statusCode);
}

