/*
 * Copyright (C) Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "webcc.h"

namespace webcc {

#define FSH(name) reinterpret_cast<const __FlashStringHelper *>(name)

const Texts PROGMEM texts;
const TextsUI PROGMEM textsUI;

const WebccFormHTML PROGMEM webccFormHTML;

//////////////////////////////////////////////////////////////////////
// WebccForm
//////////////////////////////////////////////////////////////////////

WebccForm::WebccForm(Print &client) {
  this->client = &client;
}

WebccForm::~WebccForm() {
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  if (isWithinBody) bodyEnd();
}

void WebccForm::bodyBegin(const char * formURL,
                          const char * caption,
                          const char * submitButtonText,
                          const char * formMethod,
                          boolean progmemStrings) {
  if (!client) return;
  if (isWithinBody) return;
  isWithinBody = true;
  this->submitButtonText = submitButtonText;
  this->progmemSubmitButtonText = progmemStrings;
  client->print(FSH(webccFormHTML.bodyBegin1));
  print(caption, progmemStrings);
  client->print(FSH(webccFormHTML.bodyBegin2));
  print(formMethod, progmemStrings, F("POST"));
  client->print(FSH(webccFormHTML.bodyBegin3));
  print(formURL, progmemStrings);
  client->print(FSH(webccFormHTML.bodyBegin4));
  print(caption, progmemStrings);
  client->print(FSH(webccFormHTML.bodyBegin5));
}

void WebccForm::bodyEnd(void) {
  if (!client) return;
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  isWithinBody = false;
  client->print(FSH(webccFormHTML.bodyEnd));
}

void WebccForm::sectionBegin(const char * displayName, boolean progmemStrings) {
  if (!client) return;
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  isWithinSection = true;
  client->print(FSH(webccFormHTML.sectionBegin1));
  print(displayName, progmemStrings);
  client->print(FSH(webccFormHTML.sectionBegin2));
}

void WebccForm::sectionEnd(void) {
  if (!client) return;
  isWithinSection = false;
  if (submitButtonText) {
    client->print(FSH(webccFormHTML.submitButton1));
    print(submitButtonText, progmemSubmitButtonText);
    client->print(FSH(webccFormHTML.submitButton2));
  }
  client->print(FSH(webccFormHTML.sectionEnd));
}

void WebccForm::subsectionBegin(const char * displayName, boolean progmemStrings) {
  if (!client) return;
  if (isWithinSubsection) subsectionEnd();
  isWithinSubsection = true;
  client->print(FSH(webccFormHTML.subsectionBegin1));
  print(displayName, progmemStrings);
  client->print(FSH(webccFormHTML.subsectionBegin2));
}

void WebccForm::subsectionEnd(void) {
  if (!client) return;
  isWithinSubsection = false;
  client->print(FSH(webccFormHTML.subsectionEnd));
}

void WebccForm::textParameter (const char * displayName,
                               const char * internalName,
                               const char * value,
                               const char * tooltipText,
                               boolean progmemStrings) {
  if (!client) return;
  client->print(FSH(webccFormHTML.textParameter1));
  print(displayName, progmemStrings);
  client->print(FSH(webccFormHTML.textParameter2));
  tooltip(tooltipText, progmemStrings);
  client->print(FSH(webccFormHTML.textParameter3));
  print(internalName, progmemStrings);
  client->print(FSH(webccFormHTML.textParameter4));
  print(value, false); //works with (progmemStrings)
  client->print(FSH(webccFormHTML.textParameter5));
}

void WebccForm::plaintext (const char * displayName,
                           const char * value,
                           const char * tooltipText,
                           boolean progmemStrings) {
  if (!client) return;
  client->print(FSH(webccFormHTML.plaintext1));
  print(displayName, progmemStrings);
  client->print(FSH(webccFormHTML.plaintext2));
  tooltip(tooltipText, progmemStrings);
  client->print(FSH(webccFormHTML.plaintext3));
  print(value, progmemStrings);
  client->print(FSH(webccFormHTML.plaintext4));
}

void WebccForm::link (const char * displayName,
                      const char * linkURL,
                      const char * value,
                      const char * tooltipText,
                      boolean progmemStrings) {
  if (!client) return;
  client->print(FSH(webccFormHTML.link1));
  print(displayName, progmemStrings);
  client->print(FSH(webccFormHTML.link2));
  tooltip(tooltipText, progmemStrings);
  client->print(FSH(webccFormHTML.link3));
  print(linkURL, progmemStrings);
  client->print(FSH(webccFormHTML.link4));
  print(value, progmemStrings);
  client->print(FSH(webccFormHTML.link5));
}

void WebccForm::checkboxParameter(const char * displayName,
                                  const char * internalName,
                                  boolean value,
                                  const char * tooltipText,
                                  boolean progmemStrings) {
  if (!client) return;
  client->print(FSH(webccFormHTML.checkboxParameter1));
  if (displayName) client->print(FSH(displayName));
  client->print(FSH(webccFormHTML.checkboxParameter2));
  tooltip(tooltipText, progmemStrings);
  client->print(FSH(webccFormHTML.checkboxParameter3));
  if (internalName) client->print(FSH(internalName));
  if (value)
    client->print(FSH(webccFormHTML.checkboxParameter4Checked));
  else
    client->print(FSH(webccFormHTML.checkboxParameter4));
}

//////////////////////////////////////////////////////////////////////
// BufferedPrint
//////////////////////////////////////////////////////////////////////

size_t BufferedPrint::write (uint8_t character) {
  if (!client) return (0);
  buffer[bufferPosition++] = character;
  if (bufferPosition >= bufferSize) sendBuffer();
  return (sizeof(character));
}

void BufferedPrint::sendBuffer(void) {
  if (!client) return;
  if (!bufferPosition) return;
  client->write(buffer, bufferPosition);
  bufferPosition = 0;
}

//////////////////////////////////////////////////////////////////////
// ParserInputStream
//////////////////////////////////////////////////////////////////////

int ParserInputStream::read(void) {
  const int CR = (int)'\r';
  const int LF = (int)'\n';
  if (!client) return (NOT_AVAILABLE);
  int i = client->read();
  if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
  if (((i == CR) && (previousChar == LF)) || ((i == LF) && (previousChar == CR))) i = client->read();
  if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
  previousChar = i;
  if (i == CR) i = LF;
  return (i);
}

int ParserInputStream::peek(void) {
  const int CR = (int)'\r';
  const int LF = (int)'\n';
  if (!client) return (NOT_AVAILABLE);
  int i = client->peek();
  if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
  if (((i == CR) && (previousChar == LF)) || ((i == LF) && (previousChar == CR))) {
    client->read();
    i = client->peek();
  }
  if (i == NOT_AVAILABLE) return (NOT_AVAILABLE);
  previousChar = i;
  if (i == CR) i = LF;
  return (i);
}

ParserInputStream::ReturnStatus ParserInputStream::readUntil(const char * charsToFind, char * buffer, size_t bufferSize) {
  if (!buffer || !bufferSize) return (NO_BUFFER);
  if (!client) return (NO_CLIENT);
  buffer[0] = 0;
  if (!charsToFind || !strlen(charsToFind)) return (NO_CHARS_TO_FIND);
  for (size_t bufferPos = 0; bufferPos < (bufferSize - 1); bufferPos++) {
    buffer[bufferPos] = 0;
    int i = peek();
    if (i == NOT_AVAILABLE) return (END_OF_STREAM);
    if (strchr(charsToFind, (char)i)) return (FOUND);
    buffer[bufferPos] = (char)i;
    read();
  }
  buffer[bufferSize - 1] = 0;
  return (END_OF_BUFFER);
}

ParserInputStream::ReturnStatus ParserInputStream::skipUntil(const char * charsToFind) {
  int i;
  if (!client) return (NO_CLIENT);
  if (!charsToFind || !strlen(charsToFind)) return (NO_CHARS_TO_FIND);
  do {
    i = peek();
    if (strchr(charsToFind, (char)i)) return (FOUND);
    read();
  } while (i != NOT_AVAILABLE);
  return (END_OF_STREAM);
};



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
  { HTTPReqParserStateMachine::ParserState::UNKNOWN,              HTTPReqParserStateMachine::StreamOperation::DO_NOTHING,  HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::BEGIN,                HTTPReqParserStateMachine::StreamOperation::DO_NOTHING,  HTTPRequestPart::BEGIN},
  { HTTPReqParserStateMachine::ParserState::METHOD,               HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::METHOD},
  { HTTPReqParserStateMachine::ParserState::PATH,                 HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::PATH},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::URL_QUERY_NAME},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::URL_QUERY_VALUE},
  { HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::VERSION},
  { HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  HTTPReqParserStateMachine::StreamOperation::PEEK,        HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::FIELD_NAME,           HTTPReqParserStateMachine::StreamOperation::SKIP,        HTTPRequestPart::FIELD_NAME},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          HTTPReqParserStateMachine::StreamOperation::SKIP,        HTTPRequestPart::FIELD_VALUE},
  { HTTPReqParserStateMachine::ParserState::HEADER_END,           HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    HTTPReqParserStateMachine::StreamOperation::PEEK,        HTTPRequestPart::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::POST_QUERY_NAME},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     HTTPReqParserStateMachine::StreamOperation::READ,        HTTPRequestPart::POST_QUERY_VALUE},
  { HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPReqParserStateMachine::StreamOperation::DO_NOTHING,  HTTPRequestPart::FINISH},
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
  { HTTPReqParserStateMachine::ParserState::UNKNOWN,              HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::INTERNAL_ERROR},
  { HTTPReqParserStateMachine::ParserState::BEGIN,                HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::METHOD,               ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::METHOD,               ' ',                                      HTTPReqParserStateMachine::ParserState::PATH,                 ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::METHOD,               HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::PATH,                 ' ',                                      HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::PATH,                 '?',                                      HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::PATH,                 HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       '=',                                      HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      '&',                                      HTTPReqParserStateMachine::ParserState::URL_QUERY_NAME,       ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      ' ',                                      HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::URL_QUERY_VALUE,      HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         '\n',                                     HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::HTTP_VERSION,         HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  '\n',                                     HTTPReqParserStateMachine::ParserState::HEADER_END,           ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FIELD_NAME,           ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::FIELD_NAME,           ':',                                      HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::FIELD_NAME,           HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          '\n',                                     HTTPReqParserStateMachine::ParserState::FIELD_OR_HEADER_END,  ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          HTTPReqParserStateMachine::CHAR_UNAVAIL,  HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::FIELD_VALUE,          HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::HEADER_END,           '\n',                                     HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::HEADER_END,           HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    HTTPReqParserStateMachine::CHAR_UNAVAIL,  HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_OR_END,    HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      '=',                                      HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::REQUEST_STRUCTURE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     '&',                                      HTTPReqParserStateMachine::ParserState::POST_QUERY_NAME,      ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     '\n',                                     HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     HTTPReqParserStateMachine::CHAR_UNAVAIL,  HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::POST_QUERY_VALUE,     HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::NONE},
  { HTTPReqParserStateMachine::ParserState::FINISHED,             HTTPReqParserStateMachine::CHAR_OTHER,    HTTPReqParserStateMachine::ParserState::FINISHED,             ParseError::NONE},
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

ParseError HTTPReqParserStateMachine::TransitionTable::getError(void) {
  uint16_t * errorPointer = (uint16_t *)&table[tableIndex].error;
  return ((ParseError)pgm_read_word(errorPointer));
  static_assert (sizeof(table[0].error) == 4, "table[].error is expected to be dword");
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

boolean HTTPReqParserStateMachine::begin(Stream & client) {
  inputStream.begin(client);
  readBuffer[0] = '\0';
  parserStatus.requestPart = HTTPRequestPart::NONE;
  parserStatus.currentState = ParserState::BEGIN;
  parserStatus.nextCharacter = ParserInputStream::NOT_AVAILABLE;
  parserStatus.error = ParseError::NONE;

  boolean returnValue = true;
  if (!processingTable.begin()) {
    setInternalError(InternalError::PROCESSING_TABLE, __LINE__);
    returnValue = false;
  }
  if (!transitionTable.begin()) {
    setInternalError(InternalError::TRANSITION_TABLE, __LINE__);
    returnValue = false;
  }
  return (returnValue);
}

void HTTPReqParserStateMachine::parse(void) {
  if (!inputStream.validate()) {
    setInternalError(InternalError::CLIENT_NOT_INITIALISED, __LINE__);
    return;
  }
  if (!processingTable.find(parserStatus.currentState)) {
    setInternalError(InternalError::STATE_NOT_FOUND, __LINE__);
    return;
  }
  readBuffer[0] = '\0';
  int nextCharAsInt = ParserInputStream::NOT_AVAILABLE;
  const size_t NEXT_CHARS_SIZE = 5; //max 4 characters per parser state + null character
  char nextChars[NEXT_CHARS_SIZE];
  ParserInputStream::ReturnStatus inputStreamStatus;

  switch (this->processingTable.getStreamOperation()) {
    case StreamOperation::DO_NOTHING:
      break;
    case StreamOperation::READ:
      this->transitionTable.enumerateNextChars(this->parserStatus.currentState, nextChars, NEXT_CHARS_SIZE);
      inputStreamStatus = inputStream.readUntil(nextChars, readBuffer, READ_BUFFER_SIZE);
      if (inputStreamStatus == ParserInputStream::END_OF_BUFFER) {
        setError(ParseError::REQUEST_PART_TOO_LONG);
        return;
      }
      if ((inputStreamStatus == ParserInputStream::NO_BUFFER) || (inputStreamStatus == ParserInputStream::NO_CHARS_TO_FIND)) {
        setInternalError(InternalError::STREAM_READ_PARAMETERS, __LINE__);
      }
      nextCharAsInt = inputStream.read();
      break;
    case StreamOperation::SKIP:
      this->transitionTable.enumerateNextChars(this->parserStatus.currentState, nextChars, NEXT_CHARS_SIZE);
      inputStream.skipUntil(nextChars);
      nextCharAsInt = inputStream.read();
      break;
    case StreamOperation::PEEK:
      nextCharAsInt = inputStream.peek();
      break;
    default:
      setInternalError(InternalError::STREAM_OPERATION, __LINE__);
      return;
  }
  URL::decode(readBuffer, READ_BUFFER_SIZE);
  parserStatus.nextCharacter = (char)nextCharAsInt;
  parserStatus.requestPart = processingTable.getRequestPart();
  if (!transitionTable.find(parserStatus.currentState, parserStatus.nextCharacter)) {
    setInternalError(InternalError::STATE_NOT_FOUND, __LINE__);
    return;
  }
  if (transitionTable.getError() != ParseError::NONE) {
    setError(transitionTable.getError());
    if (transitionTable.getError() == ParseError::INTERNAL_ERROR) {
      setInternalError(InternalError::ERROR_BY_STATE, __LINE__);
    }
  }
  transition(transitionTable.getNewState());
}

void HTTPReqParserStateMachine::transition(HTTPReqParserStateMachine::ParserState newState) {
  parserStatus.currentState = newState;
  if (parserStatus.currentState == ParserState::FINISHED) inputStream.reset();
}

boolean HTTPReqParserStateMachine::finished(void) const {
  return (parserStatus.currentState == ParserState::FINISHED);
}

boolean HTTPReqParserStateMachine::error(void) const {
  return (parserStatus.error != ParseError::NONE);
}

const char * HTTPReqParserStateMachine::getReqPartValue(void) const {
  return (readBuffer);
}

size_t HTTPReqParserStateMachine::getBufferSize(void) const {
  return (sizeof(readBuffer));
}

HTTPRequestPart HTTPReqParserStateMachine::getReqPart(void) const {
  return (parserStatus.requestPart);
}

ParseError HTTPReqParserStateMachine::getError(void) const {
  return (parserStatus.error);
}

void HTTPReqParserStateMachine::setError(ParseError error) {
  if (error == ParseError::NONE) return;
  transition(ParserState::FINISHED);
  parserStatus.error = error;
  if (error == ParseError::INTERNAL_ERROR) {
    setInternalError();
  }
}

void HTTPReqParserStateMachine::setInternalError(InternalError error, size_t codeLine) {
  transition(ParserState::FINISHED);
  parserStatus.error = ParseError::INTERNAL_ERROR;
  internalErrorStatus.error = error;
  internalErrorStatus.state = parserStatus.currentState;
  internalErrorStatus.nextChar = parserStatus.nextCharacter;
  internalErrorStatus.codeLine = codeLine;
}


};
