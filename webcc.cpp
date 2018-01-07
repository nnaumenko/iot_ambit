/*
 * Copyright (C) 2017-2018 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "webcc.h"

namespace webcc {

const Texts PROGMEM texts;
const TextsUI PROGMEM textsUI;

const WebccFormHTML PROGMEM webccFormHTML;

//////////////////////////////////////////////////////////////////////
// WebccForm
//////////////////////////////////////////////////////////////////////

WebccForm::WebccForm(Print &client) {
  /// @brief Initialises user interface HTML form generator
  /// @param client Destination to send HTML form to
  this->client = &client;
}

WebccForm::~WebccForm() {
  /// Automatically completes incomplete forms, sections and subsections
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  if (isWithinBody) bodyEnd();
}

/// @brief Begins HTML form, i.e. outputs form's beginning HTML/CSS/JS code
/// @param formURL Cstring which represents URL processing this form
/// when submitted (= form action)
/// @param caption Form's HTML page caption
/// @param submitButtonText Text to be displayed on form's submit 
/// button
/// @param formMethod Cstring which represents HTML form's HTTP method, 
/// typically "GET" or "POST"
/// @param progmemStrings If true cstring arguments formURL, caption, 
/// submitButtonText, formMethod and tooltipText are assumed to be 
/// located in PROGMEM, if false these cstring are assumed to be 
/// located in RAM
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
  client->print(FPSTR(webccFormHTML.bodyBegin1));
  print(caption, progmemStrings);
  client->print(FPSTR(webccFormHTML.bodyBegin2));
  print(formMethod, progmemStrings, FPSTR(webccFormHTML.defaultFormMethod));
  client->print(FPSTR(webccFormHTML.bodyBegin3));
  print(formURL, progmemStrings);
  client->print(FPSTR(webccFormHTML.bodyBegin4));
  print(caption, progmemStrings);
  client->print(FPSTR(webccFormHTML.bodyBegin5));
}

/// @brief Finalises HTML form
/// @details Finalises Section and/or Subsection if needed
/// automatically
void WebccForm::bodyEnd(void) {
  if (!client) return;
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  isWithinBody = false;
  client->print(FPSTR(webccFormHTML.bodyEnd));
}

/// @brief Begins a HTML form Section
/// @param displayName Section name visible in UI
/// @param progmemStrings If true displayName is assumed to be located 
/// in PROGMEM, if false sting is assumed to be located in RAM
void WebccForm::sectionBegin(const char * displayName, boolean progmemStrings) {
  if (!client) return;
  if (isWithinSubsection) subsectionEnd();
  if (isWithinSection) sectionEnd();
  isWithinSection = true;
  client->print(FPSTR(webccFormHTML.sectionBegin1));
  print(displayName, progmemStrings);
  client->print(FPSTR(webccFormHTML.sectionBegin2));
}

/// @brief Finalises HTML form Section
/// @details Also automatically finalised subsection if needed
void WebccForm::sectionEnd(void) {
  if (!client) return;
  if (isWithinSubsection) subsectionEnd();
  isWithinSection = false;
  if (submitButtonText) {
    client->print(FPSTR(webccFormHTML.submitButton1));
    print(submitButtonText, progmemSubmitButtonText);
    client->print(FPSTR(webccFormHTML.submitButton2));
  }
  client->print(FPSTR(webccFormHTML.sectionEnd));
}

/// @brief Begins a HTML form Subsection
/// @param displayName Subsection name visible in UI
/// @param progmemStrings If true displayName is assumed to be located 
/// in PROGMEM, if false sting is assumed to be located in RAM
void WebccForm::subsectionBegin(const char * displayName, boolean progmemStrings) {
  if (!client) return;
  if (isWithinSubsection) subsectionEnd();
  isWithinSubsection = true;
  client->print(FPSTR(webccFormHTML.subsectionBegin1));
  print(displayName, progmemStrings);
  client->print(FPSTR(webccFormHTML.subsectionBegin2));
}

/// @brief Finalises HTML form Subsection
void WebccForm::subsectionEnd(void) {
  if (!client) return;
  isWithinSubsection = false;
  client->print(FPSTR(webccFormHTML.subsectionEnd));
}

/// @brief Generates text parameter
/// @param displayName CString which contains parameter's name visible for user in
/// the UI. Can be located in RAM or PROGMEM (depending on value of progmemStrings
/// parameter)
/// @param internalName CString which contains parameter's name referred internally
/// in POST-requests, etc. Can be located in RAM or PROGMEM (depending on value of
/// progmemStrings parameter)
/// @param value Parameter's current value
/// @param tooltipText CString which contains parameter's tooltip text (or NULL if
/// no tooltip required). Can be located in RAM or PROGMEM (depending on value of
/// progmemStrings parameter).
/// @param progmemStrings If true cstring arguments displayName, internalName and
/// tooltipText are assumed to be located in PROGMEM, if false these cstring
/// arguments are assumed to be located in RAM
void WebccForm::textParameter (const char * displayName,
                               const char * internalName,
                               const char * value,
                               const char * tooltipText,
                               boolean progmemStrings) {
  if (!client) return;
  client->print(FPSTR(webccFormHTML.textParameter1));
  print(displayName, progmemStrings);
  client->print(FPSTR(webccFormHTML.textParameter2));
  tooltip(tooltipText, progmemStrings);
  client->print(FPSTR(webccFormHTML.textParameter3));
  print(internalName, progmemStrings);
  client->print(FPSTR(webccFormHTML.textParameter4));
  print(value, false); //works with (progmemStrings)
  client->print(FPSTR(webccFormHTML.textParameter5));
}

/// @brief Generates plain text in the HTML form
/// @details Plain text is only displayed in the HTML form and does not represent
/// an user-configurable parameter
/// @param displayName CString which will be displayed in the column of parameter 
/// names in the HTML form. Can be located in RAM or PROGMEM (depending on value of 
/// progmemStrings parameter)
/// @param value CString which will be displayed in the column of parameter values
/// in the HTML form
/// @param tooltipText CString which contains parameter's tooltip text (or NULL if
/// no tooltip required). Can be located in RAM or PROGMEM (depending on value of
/// progmemStrings parameter)
/// @param progmemStrings If true cstring arguments displayName and tooltipText 
/// are assumed to be located in PROGMEM, if false these cstring
/// arguments are assumed to be located in RAM
void WebccForm::plaintext (const char * displayName,
                           const char * value,
                           const char * tooltipText,
                           boolean progmemStrings) {
  if (!client) return;
  client->print(FPSTR(webccFormHTML.plaintext1));
  print(displayName, progmemStrings);
  client->print(FPSTR(webccFormHTML.plaintext2));
  tooltip(tooltipText, progmemStrings);
  client->print(FPSTR(webccFormHTML.plaintext3));
  print(value, progmemStrings);
  client->print(FPSTR(webccFormHTML.plaintext4));
}

/// @brief Generates link in the HTML form
/// @details Link is only displayed in the HTML form and does not represent an 
/// user-configurable parameter
/// @param displayName CString which will be displayed in the column of parameter 
/// names in the HTML form. Can be located in RAM or PROGMEM (depending on value 
/// of progmemStrings parameter)
/// @param linkURL CString which contains link URL to forward the user to. 
/// Can be located in RAM or PROGMEM (depending on value of progmemStrings parameter)
/// @param linkText CString which contains link text (located between <A> and </A> 
/// in HTML) which will be displayed in the column of parameter values in the HTML form
/// @param tooltipText CString which contains parameter's tooltip text (or NULL if
/// no tooltip required). Can be located in RAM or PROGMEM (depending on value of
/// progmemStrings parameter)
/// @param progmemStrings If true cstring arguments displayName and tooltipText 
/// are assumed to be located in PROGMEM, if false these cstring
/// arguments are assumed to be located in RAM
void WebccForm::link (const char * displayName,
                      const char * linkURL,
                      const char * linkText,
                      const char * tooltipText,
                      boolean progmemStrings) {
  if (!client) return;
  if (!linkURL) {
    plaintext(displayName, linkText, tooltipText, progmemStrings);
    return;
  }
  client->print(FPSTR(webccFormHTML.link1));
  print(displayName, progmemStrings);
  client->print(FPSTR(webccFormHTML.link2));
  tooltip(tooltipText, progmemStrings);
  client->print(FPSTR(webccFormHTML.link3));
  print(linkURL, progmemStrings);
  client->print(FPSTR(webccFormHTML.link4));
  print(linkText, progmemStrings);
  client->print(FPSTR(webccFormHTML.link5));
}

/// @brief Generates check box parameter in the HTML form
/// @param displayName CString which contains parameter's name visible for user in
/// the UI. Can be located in RAM or PROGMEM (depending on value of progmemStrings
/// parameter)
/// @param internalName CString which contains parameter's name referred internally
/// in POST-requests, etc. Can be located in RAM or PROGMEM (depending on value of
/// progmemStrings parameter)
/// @param value Parameter's current value
/// @param tooltipText CString which contains parameter's tooltip text (or NULL if
/// no tooltip required). Can be located in RAM or PROGMEM (depending on value of
/// progmemStrings parameter)
/// @param progmemStrings If true cstring arguments displayName, internalName and
/// tooltipText are assumed to be located in PROGMEM, if false these cstring
/// arguments are assumed to be located in RAM

void WebccForm::checkboxParameter(const char * displayName,
                                  const char * internalName,
                                  boolean value,
                                  const char * tooltipText,
                                  boolean progmemStrings) {
  if (!client) return;
  client->print(FPSTR(webccFormHTML.checkboxParameter1));
  if (displayName) client->print(FPSTR(displayName));
  client->print(FPSTR(webccFormHTML.checkboxParameter2));
  tooltip(tooltipText, progmemStrings);
  client->print(FPSTR(webccFormHTML.checkboxParameter3));
  if (internalName) client->print(FPSTR(internalName));
  if (value)
    client->print(FPSTR(webccFormHTML.checkboxParameter4Checked));
  else
    client->print(FPSTR(webccFormHTML.checkboxParameter4));
}

//////////////////////////////////////////////////////////////////////
// BufferedPrint
//////////////////////////////////////////////////////////////////////

/// Saves incoming data into internal buffer and sends buffer contents to
/// client if buffer overflows
size_t BufferedPrint::write (uint8_t character) {
  if (!client) return (0);
  buffer[bufferPosition++] = character;
  if (bufferPosition >= bufferSize) sendBuffer();
  return (sizeof(character));
}

/// Sends buffer contents to client and clears buffer
/// @warning Buffer memory is nor filled with zeros after data is sent
/// and buffer is cleared
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
  static const size_t stateTableSize = sizeof(stateTable) / sizeof(StateTableEntry);
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
  static const size_t transitionTableSize = sizeof(transitionTable) / sizeof(TransitionTableEntry);
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
    util::http::URL::decode(buffer, bufferSize);
    transition(ParserTables::getNextState(currentState, nextCharacter, defaultTransition));
  } while ((reqPart == HTTPRequestPart::NONE || !buffer[0]) && !error() && !finished());
}

}; //namespace webcc
