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
  print(formMethod, progmemStrings, FSH(webccFormHTML.defaultFormMethod));
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
// HTTPReqParserStateMachine::InputStreamHelper
//////////////////////////////////////////////////////////////////////


HTTPReqParserStateMachine::ControlCharacter HTTPReqParserStateMachine::InputStreamHelper::readUntilControlCharacter(
  Stream &client,
  ControlCharacterSet controlChars,
  char * buffer,
  size_t bufferSize)
{
  ControlCharacter cc;
  size_t bufferPosition = 0;
  do {
    int i1 = client.read();
    int i2 = CC_UNAVAILABLE;
    if (i1 == CC_CR) i2 = client.read();
    cc = intToControlCharacter(controlChars, i1, i2);
    if (cc == ControlCharacter::OTHER) {
      buffer[bufferPosition++] = static_cast<char>(i1);
      if ((bufferPosition < (bufferSize - 1)) && i2 != CC_UNAVAILABLE)
        buffer[bufferPosition++] = static_cast<char>(i2);
    }
    else {
      break;
    }
  } while (bufferPosition < (bufferSize - 1));
  buffer[bufferPosition] = '\0';
  return (cc);
}

HTTPReqParserStateMachine::ControlCharacter HTTPReqParserStateMachine::InputStreamHelper::skipUntilControlCharacter(
  Stream &client,
  ControlCharacterSet controlChars)
{
  ControlCharacter cc;
  do {
    int i1 = client.read();
    int i2 = CC_UNAVAILABLE;
    if (i1 == CC_CR) i2 = client.read();
    cc = intToControlCharacter(controlChars, i1, i2);
  } while (cc == ControlCharacter::OTHER);
  return (cc);
}

HTTPReqParserStateMachine::ControlCharacter HTTPReqParserStateMachine::InputStreamHelper::read(
  Stream &client,
  ControlCharacterSet controlChars)
{
  int i1 = client.read();
  int i2 = CC_UNAVAILABLE;
  if (i1 == CC_CR) {
    client.read();
    i2 = client.read();
  }
  return (intToControlCharacter(controlChars, i1, i2));
}

HTTPReqParserStateMachine::ControlCharacter HTTPReqParserStateMachine::InputStreamHelper::readIfControlCharacter(
  Stream &client,
  ControlCharacterSet controlChars)
{
  int i1 = client.peek();
  int i2 = CC_UNAVAILABLE;
  if (i1 == CC_CR) {
    client.read();
    i2 = client.peek();
  }
  ControlCharacter cc = intToControlCharacter(controlChars, i1, i2);
  if (cc != ControlCharacter::OTHER) client.read();
  return (cc);
}

HTTPReqParserStateMachine::ControlCharacter HTTPReqParserStateMachine::InputStreamHelper::intToControlCharacter(ControlCharacterSet cc, int first, int second) {
  if (!isControlCharacterInSet(cc, first, second)) return (ControlCharacter::OTHER);
  switch (first) {
    case CC_UNAVAILABLE:
      return (ControlCharacter::UNAVAILABLE);
    case CC_CR:
      if (second == CC_LF) return (ControlCharacter::CRLF);
      return (ControlCharacter::OTHER);
    case CC_SPACE:
      return (ControlCharacter::SPACE);
    case CC_QUESTION:
      return (ControlCharacter::QUESTION);
    case CC_AMPERSAND:
      return (ControlCharacter::AMPERSAND);
    case CC_EQUAL:
      return (ControlCharacter::EQUAL);
    case CC_COLON:
      return (ControlCharacter::COLON);
    case CC_SEMICOLON:
      return (ControlCharacter::SEMICOLON);
    default:
      return (ControlCharacter::OTHER);
  }
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine::ParserTables
//////////////////////////////////////////////////////////////////////

boolean HTTPReqParserStateMachine::ParserTables::getStateProperties(
  ParserState state,
  StreamOperation *streamOp,
  HTTPRequestPart *reqPart,
  ParserState *defaultTransition,
  ControlCharacterSet *controlChars)
{
  if (!streamOp || !reqPart || !defaultTransition) return (false);
  static const PROGMEM StateTableEntry stateTable[] = {
    //State                                     Stream operation                Request Part                        Default transition state                  Control char set
    {ParserState::UNKNOWN,                      StreamOperation::DO_NOTHING,    HTTPRequestPart::NONE,              ParserState::ERROR_INTERNAL,              ControlCharacterSet::ALL},
    {ParserState::BEGIN,                        StreamOperation::DO_NOTHING,    HTTPRequestPart::NONE,              ParserState::METHOD,                      ControlCharacterSet::ALL},
    {ParserState::METHOD,                       StreamOperation::READ_UNTIL,    HTTPRequestPart::METHOD,            ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::PATH,                         StreamOperation::READ_UNTIL,    HTTPRequestPart::PATH,              ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::URL_QUERY_NAME,               StreamOperation::READ_UNTIL,    HTTPRequestPart::URL_QUERY_NAME,    ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::URL_QUERY_VALUE,              StreamOperation::READ_UNTIL,    HTTPRequestPart::URL_QUERY_VALUE,   ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::HTTP_VERSION,                 StreamOperation::READ_UNTIL,    HTTPRequestPart::HTTP_VERSION,      ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::FIELD_OR_HEADER_END,          StreamOperation::READ_IF_CC,    HTTPRequestPart::NONE,              ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::FIELD_NAME,                   StreamOperation::READ_UNTIL,    HTTPRequestPart::FIELD_NAME,        ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::FIELD_VALUE_PART1,            StreamOperation::READ_UNTIL,    HTTPRequestPart::FIELD_VALUE_PART1, ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::FIELD_VALUE},
    {ParserState::FIELD_VALUE_PART2,            StreamOperation::READ_UNTIL,    HTTPRequestPart::FIELD_VALUE_PART2, ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::FIELD_VALUE},
    {ParserState::POST_QUERY_OR_END,            StreamOperation::READ_IF_CC,    HTTPRequestPart::NONE,              ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::POST_QUERY_NAME,              StreamOperation::READ_UNTIL,    HTTPRequestPart::POST_QUERY_NAME,   ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::POST_QUERY_VALUE,             StreamOperation::READ_UNTIL,    HTTPRequestPart::POST_QUERY_VALUE,  ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::FINISHED,                     StreamOperation::DO_NOTHING,    HTTPRequestPart::NONE,              ParserState::FINISHED,                    ControlCharacterSet::ALL},
    {ParserState::ERROR_INTERNAL,               StreamOperation::DO_NOTHING,    HTTPRequestPart::NONE,              ParserState::ERROR_INTERNAL,              ControlCharacterSet::ALL},
    {ParserState::ERROR_REQUEST_PART_TOO_LONG,  StreamOperation::DO_NOTHING,    HTTPRequestPart::NONE,              ParserState::ERROR_REQUEST_PART_TOO_LONG, ControlCharacterSet::ALL},
    {ParserState::ERROR_REQUEST_STRUCTURE,      StreamOperation::DO_NOTHING,    HTTPRequestPart::NONE,              ParserState::ERROR_REQUEST_STRUCTURE,     ControlCharacterSet::ALL},
    {ParserState::ERROR_REQUEST_SEMANTICS,      StreamOperation::DO_NOTHING,    HTTPRequestPart::NONE,              ParserState::ERROR_REQUEST_SEMANTICS,     ControlCharacterSet::ALL},
  };
  const size_t stateTableSize = sizeof(stateTable) / sizeof(StateTableEntry);
  for (size_t i = 0; i < stateTableSize; i++) {
    StateTableEntry tempEntry;
    memcpy_P(&tempEntry, &stateTable[i], sizeof (tempEntry));
    if (tempEntry.state == state) {
      *streamOp = tempEntry.streamOp;
      *reqPart = tempEntry.reqPart;
      *defaultTransition = tempEntry.defaultTransition;
      *controlChars = tempEntry.controlChars;
      return (true);
    }
  }
  return (false);
}

HTTPReqParserStateMachine::ParserState HTTPReqParserStateMachine::ParserTables::getNextState(
  ParserState currentState,
  ControlCharacter nextCharacter,
  ParserState defaultTransition)
{
  static const PROGMEM TransitionTableEntry transitionTable[] = {
    //Current state                     Next control character          New state
    {ParserState::METHOD,               ControlCharacter::SPACE,        ParserState::PATH},
    {ParserState::METHOD,               ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG},
    {ParserState::PATH,                 ControlCharacter::SPACE,        ParserState::HTTP_VERSION},
    {ParserState::PATH,                 ControlCharacter::QUESTION,     ParserState::URL_QUERY_NAME},
    {ParserState::PATH,                 ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG},
    {ParserState::URL_QUERY_NAME,       ControlCharacter::EQUAL,        ParserState::URL_QUERY_VALUE},
    {ParserState::URL_QUERY_NAME,       ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG},
    {ParserState::URL_QUERY_VALUE,      ControlCharacter::AMPERSAND,    ParserState::URL_QUERY_NAME},
    {ParserState::URL_QUERY_VALUE,      ControlCharacter::SPACE,        ParserState::HTTP_VERSION},
    {ParserState::URL_QUERY_VALUE,      ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG},
    {ParserState::HTTP_VERSION,         ControlCharacter::CRLF,         ParserState::FIELD_OR_HEADER_END},
    {ParserState::HTTP_VERSION,         ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG},
    {ParserState::FIELD_OR_HEADER_END,  ControlCharacter::CRLF,         ParserState::POST_QUERY_OR_END},
    {ParserState::FIELD_OR_HEADER_END,  ControlCharacter::OTHER,        ParserState::FIELD_NAME},
    {ParserState::FIELD_OR_HEADER_END,  ControlCharacter::COLON,        ParserState::FIELD_VALUE_PART1},
    {ParserState::FIELD_NAME,           ControlCharacter::COLON,        ParserState::FIELD_VALUE_PART1},
    {ParserState::FIELD_NAME,           ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG},
    {ParserState::FIELD_VALUE_PART1,    ControlCharacter::CRLF,         ParserState::FIELD_OR_HEADER_END},
    {ParserState::FIELD_VALUE_PART1,    ControlCharacter::EQUAL,        ParserState::FIELD_VALUE_PART2},
    {ParserState::FIELD_VALUE_PART1,    ControlCharacter::SEMICOLON,    ParserState::FIELD_VALUE_PART1},
    {ParserState::FIELD_VALUE_PART1,    ControlCharacter::OTHER,        ParserState::FIELD_VALUE_PART1},
    {ParserState::FIELD_VALUE_PART2,    ControlCharacter::CRLF,         ParserState::FIELD_OR_HEADER_END},
    {ParserState::FIELD_VALUE_PART2,    ControlCharacter::EQUAL,        ParserState::FIELD_VALUE_PART2},
    {ParserState::FIELD_VALUE_PART2,    ControlCharacter::SEMICOLON,    ParserState::FIELD_VALUE_PART1},
    {ParserState::FIELD_VALUE_PART2,    ControlCharacter::OTHER,        ParserState::FIELD_VALUE_PART2},
    {ParserState::POST_QUERY_OR_END,    ControlCharacter::UNAVAILABLE,  ParserState::FINISHED},
    {ParserState::POST_QUERY_OR_END,    ControlCharacter::OTHER,        ParserState::POST_QUERY_NAME},
    {ParserState::POST_QUERY_OR_END,    ControlCharacter::EQUAL,        ParserState::POST_QUERY_VALUE},
    {ParserState::POST_QUERY_NAME,      ControlCharacter::EQUAL,        ParserState::POST_QUERY_VALUE},
    {ParserState::POST_QUERY_NAME,      ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG},
    {ParserState::POST_QUERY_VALUE,     ControlCharacter::AMPERSAND,    ParserState::POST_QUERY_NAME},
    {ParserState::POST_QUERY_VALUE,     ControlCharacter::UNAVAILABLE,  ParserState::FINISHED},
    {ParserState::POST_QUERY_VALUE,     ControlCharacter::CRLF,         ParserState::FINISHED},
    {ParserState::POST_QUERY_VALUE,     ControlCharacter::OTHER,        ParserState::ERROR_REQUEST_PART_TOO_LONG}
  };
  const size_t transitionTableSize = sizeof(transitionTable) / sizeof(TransitionTableEntry);
  for (size_t i = 0; i < transitionTableSize; i++) {
    TransitionTableEntry tempEntry;
    memcpy_P(&tempEntry, &transitionTable[i], sizeof(tempEntry));
    if ((tempEntry.state == currentState) && (tempEntry.nextCharacter == nextCharacter)) return (tempEntry.newState);
  }
  return (defaultTransition);
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine
//////////////////////////////////////////////////////////////////////

void HTTPReqParserStateMachine::parse(char * buffer, size_t bufferSize, HTTPRequestPart * part) {
  if (!inputStream) {
    setInternalError(__LINE__);
    return;
  }
  if (!buffer) {
    setInternalError(__LINE__);
    return;
  }
  if (!bufferSize) {
    setInternalError(__LINE__);
    return;
  }
  HTTPRequestPart reqPart;
  do {
    buffer[0] = '\0';
    StreamOperation streamOp;
    ParserState defaultTransition;
    ControlCharacterSet ccSet;
    if (!ParserTables::getStateProperties(currentState, &streamOp, &reqPart, &defaultTransition, &ccSet)) {
      setInternalError(__LINE__);
      return;
    }
    *part = reqPart;
    ControlCharacter nextCharacter = ControlCharacter::UNAVAILABLE;
    switch (streamOp) {
      case StreamOperation::DO_NOTHING:
        break;
      case StreamOperation::READ_UNTIL:
        nextCharacter = InputStreamHelper::readUntilControlCharacter(*inputStream, ccSet, buffer, bufferSize);
        break;
      case StreamOperation::SKIP:
        nextCharacter = InputStreamHelper::skipUntilControlCharacter(*inputStream, ccSet);
        break;
      case StreamOperation::READ_SINGLE:
        nextCharacter = InputStreamHelper::read(*inputStream, ccSet);
        break;
      case StreamOperation::READ_IF_CC:
        nextCharacter = InputStreamHelper::readIfControlCharacter(*inputStream, ccSet);
        break;
      default:
        setInternalError(__LINE__);
        return;
    }
    URL::decode(buffer, bufferSize);
    transition(ParserTables::getNextState(currentState, nextCharacter, defaultTransition));
  } while ((reqPart == HTTPRequestPart::NONE || !buffer[0]) && !error() && !finished());
}

};
