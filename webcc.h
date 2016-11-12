/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCC_H
#define WEBCC_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "module.h"

#include "http.h"

namespace webcc {

//////////////////////////////////////////////////////////////////////
// Texts & TextsUI
//////////////////////////////////////////////////////////////////////

#define MODULE_TEXT(name,value) public: const char name [sizeof(value)] = value
#define FSH(name) reinterpret_cast<const __FlashStringHelper *>(name)

class Texts {
    MODULE_TEXT(moduleName, "WebConfigControl");
    MODULE_TEXT(rootPath, "/");
    MODULE_TEXT(crlf, "\r\n");
} __attribute__((packed));

class TextsUI {
    MODULE_TEXT(errorNone, "No error");
    MODULE_TEXT(errorUnknown, "Unknown error");
    MODULE_TEXT(errorServerNotInitialised, "WiFiServer not initialised");
    MODULE_TEXT(errorParserError, "HTTP Request parser reported an error");
    MODULE_TEXT(errorMethodNotAccepted, "HTTP method not accepted");
    MODULE_TEXT(errorPathNotAccepted, "Path not found");
    MODULE_TEXT(errorPathConflict, "Module path conflict");

    MODULE_TEXT(parseErrorNone, "No HTTP request parser error");
    MODULE_TEXT(parseErrorUnknown, "Unknown HTTP request parser error");
    MODULE_TEXT(parseErrorInternal, "HTTP request parser internal error");
    MODULE_TEXT(parseErrorRequestPartTooLong, "Request part is too long");
    MODULE_TEXT(parseErrorRequestStructure, "Bad request");
    MODULE_TEXT(parseErrorRequestSemantics, "Request contains conflicting information");

    MODULE_TEXT(webClientConnected, "Web client connected");
    MODULE_TEXT(beginParsing, "Begin parsing request");
    MODULE_TEXT(endParsing, "End parsing request");
    MODULE_TEXT(printMethod, "Method: ");
    MODULE_TEXT(printPath, "Path: ");
    MODULE_TEXT(parsingError, "Error parsing request");
    MODULE_TEXT(httpStatusCode, "HTTP status code: ");
    MODULE_TEXT(redirectTo, "Redirecting to: ");
    MODULE_TEXT(sendModuleIndexBegin, "Sending module index");
    MODULE_TEXT(sendModuleIndexEnd, "Sending module sent");

    MODULE_TEXT(rootCaption, "WebConfigControl");
    MODULE_TEXT(moduleIndex, "Modules");
    MODULE_TEXT(moduleIndexNameCaption, "Name");
    MODULE_TEXT(moduleIndexLinkCaption, "Link");

} __attribute__((packed));

extern const Texts PROGMEM texts;
extern const TextsUI PROGMEM textsUI;

#undef MODULE_TEXT

const size_t HTTP_REQUEST_PART_MAX_SIZE = 32;

//////////////////////////////////////////////////////////////////////
// HTTPRequestPart
//////////////////////////////////////////////////////////////////////

enum class HTTPRequestPart {
  NONE,
  METHOD,
  PATH,
  URL_QUERY_NAME,
  URL_QUERY_VALUE,
  HTTP_VERSION,
  FIELD_NAME,
  FIELD_VALUE_PART1,
  FIELD_VALUE_PART2,
  POST_QUERY_NAME,
  POST_QUERY_VALUE,
};

//////////////////////////////////////////////////////////////////////
// ParseError
//////////////////////////////////////////////////////////////////////

enum class ParseError {
  NONE,                   //No error
  INTERNAL_ERROR,         //Parser's internal error
  REQUEST_PART_TOO_LONG,  //Request part is too long to fit into buffer
  REQUEST_STRUCTURE,      //Malformed request
  REQUEST_SEMANTICS       //Request information is semantically invalid
};

//////////////////////////////////////////////////////////////////////
// WebConfigControl
//////////////////////////////////////////////////////////////////////

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
class WebConfigControl : public Module<WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>> {
  public:
    inline void setServer(WiFiServer &server);
    inline WiFiServer * getServer(void);
  public:
    inline const char * PROGMEM moduleName (void);
    inline const char * PROGMEM getMainPath(void);
  public:
    void onBegin(void);
    void onRun(void);
  public:
    inline void setRootRedirect(const char * redirectURL);
    inline char * PROGMEM getRootRedirect(void);
  public:
    boolean onHTTPReqPath(const char * path);
    boolean onHTTPReqMethod(const char * method);
    boolean onRespond(Print &client);
  private:
    WiFiServer * server = NULL;
  private:
    inline void callWebModulesOnStart(void);
    inline int callWebModulesOnPath(const char * path);
    inline boolean callWebModulesOnMethod(size_t index, const char *method);
    inline boolean callWebModulesOnURLQuery(size_t index, const char * name, const char * value);
    inline boolean callWebModulesOnPOSTQuery(size_t index, const char * name, const char * value);
    inline boolean callWebModulesOnRespond(size_t index, Print &client);
    inline boolean callWebModulesOnEnd(size_t index, boolean error);
  private:
    enum class WebccError {
      NONE,
      SERVER_NOT_INITIALISED,
      PARSER_ERROR,
      METHOD_NOT_ACCEPTED,
      PATH_NOT_ACCEPTED,
      PATH_CONFLICT
    };
    const __FlashStringHelper * getErrorMessage(WebccError error);
    const __FlashStringHelper * getParseErrorMessage(ParseError error);
    HTTPStatusCode getHTTPStatusCode(WebccError error, ParseError parseError);
    void handleErrors(OutputStream &output, WebccError error, const Parser &parser);
  private:
    static const int WEBMODULES_CALL_THIS_ACCEPTED = -1;
    static const int WEBMODULES_CALL_NONE_ACCEPTED = -2;
    static const int WEBMODULES_CALL_MULTIPLE_ACCEPTED = -3;
  private:
    const char * rootRedirect = NULL;
};


template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
void WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::setServer(WiFiServer &server) {
  this->server = &server;
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
WiFiServer * WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::getServer(void) {
  return (server);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
const char * PROGMEM WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::moduleName (void) {
  return (texts.moduleName);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
const char * PROGMEM WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::getMainPath (void) {
  return (texts.rootPath);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
void WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::onBegin(void) {
  if (!server)
  {
    Diag::instance()->setMessageClass(Diag::CLASS_ERROR);
    Diag::instance()->print(getErrorMessage(WebccError::SERVER_NOT_INITIALISED));
  }
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
void WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::onRun(void) {
  Diag * diagLog = Diag::instance();
  if (!server) return;
  WiFiClient client = this->server->available();
  if (!client) return;
  diagLog->setMessageClass(Diag::CLASS_DEBUG);
  diagLog->timestamp();
  diagLog->println(FSH(textsUI.webClientConnected));
  while (!client.available()) {
    delay(1);
    yield();
  }
  OutputStream outputClient(client);
  Parser parser;
  parser.begin(client);
  diagLog->setMessageClass(Diag::CLASS_DEBUG);
  diagLog->timestamp();
  diagLog->println(FSH(textsUI.beginParsing));
  char tempBuffer[HTTP_REQUEST_PART_MAX_SIZE + 1];//+1 char for \0
  tempBuffer[0] = '\0';
  int indexModuleAccepted = WEBMODULES_CALL_NONE_ACCEPTED;
  WebccError error = WebccError::NONE;
  callWebModulesOnStart();
  do {
    char readBuffer[HTTP_REQUEST_PART_MAX_SIZE + 1];//+1 char for \0
    readBuffer[0] = '\0';
    HTTPRequestPart reqPart = HTTPRequestPart::NONE;
    parser.parse(readBuffer, sizeof(readBuffer), &reqPart);
    yield();
    if (parser.error()) {
      error = WebccError::PARSER_ERROR;
      break;
    }
    switch (reqPart) {
      case HTTPRequestPart::METHOD:
        diagLog->setMessageClass(Diag::CLASS_DEBUG);
        diagLog->print(FSH(textsUI.printMethod));
        diagLog->println(readBuffer);
        strncpy(tempBuffer, readBuffer, sizeof(tempBuffer));
        break;
      case HTTPRequestPart::PATH:
        //In case of empty path control will not be passed here, thus path check is needed in subsequent parts
        diagLog->setMessageClass(Diag::CLASS_DEBUG);
        diagLog->print(FSH(textsUI.printPath));
        diagLog->println(readBuffer);
        indexModuleAccepted = callWebModulesOnPath(readBuffer);
        if (indexModuleAccepted == WEBMODULES_CALL_NONE_ACCEPTED) {
          error = WebccError::PATH_NOT_ACCEPTED;
          break;
        }
        if (indexModuleAccepted == WEBMODULES_CALL_MULTIPLE_ACCEPTED) {
          error = WebccError::PATH_CONFLICT;
          diagLog->setMessageClass(Diag::CLASS_ERROR);
          diagLog->print(getErrorMessage(error));
          diagLog->print(' ');
          diagLog->println(readBuffer);
          //TODO: print list of the modules which accepted the same path
          break;
        }
        if (!callWebModulesOnMethod(indexModuleAccepted, tempBuffer)) { //tempBuffer at this point contains method
          error = WebccError::METHOD_NOT_ACCEPTED;
          break;
        }
        tempBuffer[0] = '\0';
      case HTTPRequestPart::URL_QUERY_NAME:
        if (indexModuleAccepted == WEBMODULES_CALL_NONE_ACCEPTED) {
          error = WebccError::PATH_NOT_ACCEPTED;
          break;
        }
        strncpy(tempBuffer, readBuffer, sizeof(tempBuffer));
        break;
      case HTTPRequestPart::URL_QUERY_VALUE:
        if (indexModuleAccepted == WEBMODULES_CALL_NONE_ACCEPTED) {
          error = WebccError::PATH_NOT_ACCEPTED;
          break;
        }
        callWebModulesOnURLQuery(indexModuleAccepted, tempBuffer, readBuffer);
        break;
      case HTTPRequestPart::POST_QUERY_NAME:
        if (indexModuleAccepted == WEBMODULES_CALL_NONE_ACCEPTED) {
          error = WebccError::PATH_NOT_ACCEPTED;
          break;
        }
        strncpy(tempBuffer, readBuffer, sizeof(tempBuffer));
        break;
      case HTTPRequestPart::POST_QUERY_VALUE:
        if (indexModuleAccepted == WEBMODULES_CALL_NONE_ACCEPTED) {
          error = WebccError::PATH_NOT_ACCEPTED;
          break;
        }
        callWebModulesOnPOSTQuery(indexModuleAccepted, tempBuffer, readBuffer);
        break;
      default:
        break;
    }
  } while (!parser.finished());
  diagLog->setMessageClass(Diag::CLASS_DEBUG);
  diagLog->timestamp();
  diagLog->println(FSH(textsUI.endParsing));
  if (error == WebccError::NONE) {
    callWebModulesOnRespond(indexModuleAccepted, outputClient);
  }
  else {
    diagLog->println(FSH(textsUI.parsingError));
    handleErrors(outputClient, error, parser);
  }
  callWebModulesOnEnd(indexModuleAccepted, parser.error());
  client.flush();
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
void WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::callWebModulesOnStart(void) {
  this->onHTTPReqStart();
  boolean callResult[] = { WebModules::instance()->onHTTPReqStart()... };
  (void)callResult;
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
int WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::callWebModulesOnPath(const char * path) {
  boolean thisModule = this->onHTTPReqPath(path);
  boolean callResult[] = { WebModules::instance()->onHTTPReqPath(path)... };
  int acceptCount = 0, acceptedModuleIndex = WEBMODULES_CALL_NONE_ACCEPTED;
  for (size_t i = 0; i < sizeof(callResult); i++) {
    if (callResult[i]) {
      acceptCount++;
      acceptedModuleIndex = i;
    }
  }
  if (acceptCount > 1) return (WEBMODULES_CALL_MULTIPLE_ACCEPTED);
  if (thisModule) return (WEBMODULES_CALL_THIS_ACCEPTED);
  return (acceptedModuleIndex);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::callWebModulesOnMethod(size_t index, const char *method) {
  if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
    return (this->onHTTPReqMethod(method));
  }
  if (index < 0) return (false);
  size_t i = 0;
  int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqMethod(method) : false)... };
  return (callResult[index]);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::callWebModulesOnURLQuery(size_t index, const char * name, const char * value) {
  if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
    return (this->onHTTPReqURLQuery(name, value));
  }
  if (index < 0) return (false);
  int i = 0;
  int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqURLQuery(name, value) : false)... };
  return (callResult[index]);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::callWebModulesOnPOSTQuery(size_t index, const char * name, const char * value) {
  if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
    return (this->onHTTPReqPOSTQuery(name, value));
  }
  if (index < 0) return (false);
  int i = 0;
  int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqPOSTQuery(name, value) : false)... };
  return (callResult[index]);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::callWebModulesOnRespond(size_t index, Print &client) {
  if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
    return (this->onRespond(client));
  }
  if (index < 0) return (false);
  int i = 0;
  int callResult[] = { ((i++ == index) ? WebModules::instance()->onRespond(client) : false)... };
  return (callResult[index]);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::callWebModulesOnEnd(size_t index, boolean error) {
  if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
    return (this->onHTTPReqEnd(error));
  }
  if (index < 0) return (false);
  int i = 0;
  int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqEnd(error) : false)... };
  return (callResult[index]);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
void WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::setRootRedirect(const char * redirectURL) {
  rootRedirect = redirectURL;
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
char * PROGMEM WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::getRootRedirect(void) {
  return (rootRedirect);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::onHTTPReqPath(const char * path) {
  if (!strcmp_P(path, texts.rootPath)) return (true);
  return (false);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::onHTTPReqMethod(const char * method) {
  if (HTTPRequestHelper::getMethod(method) == HTTPRequestMethod::GET) return (true);
  return (false);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
boolean WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::onRespond(Print &client) {
  Diag * diagLog = Diag::instance();
  if (rootRedirect) {
    HTTPResponseHeader::redirect(client, (__FlashStringHelper *)rootRedirect);
    diagLog->print(FSH(textsUI.redirectTo));
    diagLog->println((__FlashStringHelper *)rootRedirect);
    return (true);
  }
  diagLog->setMessageClass(Diag::CLASS_DEBUG);
  diagLog->timestamp();
  diagLog->println(FSH(textsUI.sendModuleIndexBegin));
  const char * paths[] = { (WebModules::instance()->getMainPath())... };
  const char * names[] = { (WebModules::instance()->moduleName())... };
  int moduleCount = sizeof...(WebModules);
  HTTPResponseHeader::contentHeader(client, HTTPContentType::HTML);
  WebForm index(client);
  index.bodyBegin(NULL, textsUI.rootCaption);
  index.sectionBegin(textsUI.moduleIndex);
  index.subsectionBegin(textsUI.moduleIndex);
  index.plaintext(textsUI.moduleIndexNameCaption, textsUI.moduleIndexLinkCaption);
  index.link(this->moduleName(), this->getMainPath(), this->getMainPath());
  for (int i = 0; i < moduleCount; i++) {
    index.link(names[i], paths[i], paths[i]);
  }
  index.bodyEnd();
  diagLog->setMessageClass(Diag::CLASS_DEBUG);
  diagLog->timestamp();
  diagLog->print(FSH(textsUI.sendModuleIndexEnd));
  return (true);
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
void WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::handleErrors(OutputStream &output, WebccError error, const Parser &parser) {
  Diag * diagLog = Diag::instance();
  diagLog->println(FSH(textsUI.httpStatusCode));
  diagLog->println((int)getHTTPStatusCode(error, parser.getError()));
  diagLog->println(getErrorMessage(error));
  if (error == WebccError::PARSER_ERROR) {
    diagLog->println(getParseErrorMessage(parser.getError()));
  }
  diagLog->println();

  HTTPResponseHeader::statusLine(output, getHTTPStatusCode(error, parser.getError()));
  output.print(FSH(texts.crlf));
  output.print(getErrorMessage(error));
  output.print(FSH(texts.crlf));
  if (error == WebccError::PARSER_ERROR) {
    output.print(getParseErrorMessage(parser.getError()));
    output.print(FSH(texts.crlf));
  }
  output.print(FSH(texts.crlf));
  output.print(FSH(texts.crlf));
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
const __FlashStringHelper * WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::getErrorMessage(WebccError error) {
  switch (error) {
    case WebccError::NONE:
      return (FSH(textsUI.errorNone));
    case WebccError::SERVER_NOT_INITIALISED:
      return (FSH(textsUI.errorServerNotInitialised));
    case WebccError::PARSER_ERROR:
      return (FSH(textsUI.errorParserError));
    case WebccError::METHOD_NOT_ACCEPTED:
      return (FSH(textsUI.errorMethodNotAccepted));
    case WebccError::PATH_NOT_ACCEPTED:
      return (FSH(textsUI.errorPathNotAccepted));
    case WebccError::PATH_CONFLICT:
      return (FSH(textsUI.errorPathConflict));
    default:
      return (FSH(textsUI.errorUnknown));
  }
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
const __FlashStringHelper * WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::getParseErrorMessage(ParseError error) {
  switch (error) {
    case ParseError::NONE:
      return (FSH(textsUI.parseErrorNone));
    case ParseError::INTERNAL_ERROR:
      return (FSH(textsUI.parseErrorInternal));
    case ParseError::REQUEST_PART_TOO_LONG:
      return (FSH(textsUI.parseErrorRequestPartTooLong));
    case ParseError::REQUEST_STRUCTURE:
      return (FSH(textsUI.parseErrorRequestStructure));
    case ParseError::REQUEST_SEMANTICS:
      return (FSH(textsUI.parseErrorRequestSemantics));
    default:
      return (FSH(textsUI.parseErrorUnknown));
  }
}

template <class Diag, class Parser, class OutputStream, class WebForm, class... WebModules>
HTTPStatusCode WebConfigControl<Diag, Parser, OutputStream, WebForm, WebModules...>::getHTTPStatusCode(WebccError error, ParseError parseError) {
  switch (error) {
    case WebccError::NONE:
      return (HTTPStatusCode::OK);
    case WebccError::SERVER_NOT_INITIALISED:
      return (HTTPStatusCode::INTERNAL_SERVER_ERROR);
    case WebccError::PARSER_ERROR:
      switch (parseError) {
        case ParseError::INTERNAL_ERROR:
          return (HTTPStatusCode::INTERNAL_SERVER_ERROR);
        case ParseError::REQUEST_PART_TOO_LONG:
          return (HTTPStatusCode::PAYLOAD_TOO_LARGE);
        case ParseError::REQUEST_STRUCTURE:
          return (HTTPStatusCode::BAD_REQUEST);
        case ParseError::REQUEST_SEMANTICS:
          return (HTTPStatusCode::CONFLICT);
        default:
          return (HTTPStatusCode::INTERNAL_SERVER_ERROR);
      }
      return (HTTPStatusCode::INTERNAL_SERVER_ERROR);
    case WebccError::METHOD_NOT_ACCEPTED:
      return (HTTPStatusCode::NOT_IMPLEMENTED);
    case WebccError::PATH_NOT_ACCEPTED:
      return (HTTPStatusCode::NOT_FOUND);
    case WebccError::PATH_CONFLICT:
      return (HTTPStatusCode::INTERNAL_SERVER_ERROR);
    default:
      return (HTTPStatusCode::INTERNAL_SERVER_ERROR);
  }
}

//////////////////////////////////////////////////////////////////////
// WebccFormHTML
//////////////////////////////////////////////////////////////////////

#define HTML_FORMATTING

#ifdef HTML_FORMATTING
#define TAB "\t"
#define CRLF "\r\n"
#define CSSCOMMENT(comment) "/* " comment " */"
#define JSCOMMENT(comment) "// " comment
#else
#define TAB ""
#define CRLF ""
#define CSSCOMMENT(comment) ""
#define JSCOMMENT(comment) ""
#endif

#define FORM_PART(name,value) const char name [sizeof(value)] = value

class WebccFormHTML {
  public:
    FORM_PART(bodyBegin1,
              "<!DOCTYPE html>" CRLF
              "<html>" CRLF
              "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" CRLF
              "<head>" CRLF
              "<title>"
             );
    FORM_PART(bodyBegin2,
              "</title>" CRLF
              "<style type=\"text/css\" media=\"all\">" CRLF
              "html{overflow-y:scroll;}" CRLF
              "body{font-family:Arial;background-color:#D0FFD0;color:#000000;font-size:85%;}" CRLF
              "h1{text-align:center;font-size:130%;font-weight:bold;}" CRLF
              "h2{text-align:center;font-size:120%;font-weight:bold;margin-top:2em;}" CRLF
              "h3{text-align:center;font-size:110%;font-weight:bold;}" CRLF
              "h3:first-child{margin-top:-.25em;}" CRLF
              "input, select {display:block;text-align:left;float:right;width:12.5em;}" CRLF
              "input[type='checkbox'] {width:1em;}" CRLF
              "input[type='submit'] {width:auto;}" CRLF
              "label {text-align:left;float:left;max-width:11em;}"
              ".plaintext {text-align:left;float:right;width:12.5em;}" CRLF
              ".parameter {overflow:auto;padding-bottom:0.25em;line-height:1.65em;}" CRLF
              ".subsection {text-align:left;" CRLF
              TAB "margin:auto;" CRLF
              TAB "margin-bottom:1em;" CRLF
              TAB "width:25em;" CRLF
              TAB "padding:1em;" CRLF
              TAB "background-color:#66FF66;" CRLF
              TAB "border-style:solid;" CRLF
              TAB "border-color:black;" CRLF
              TAB "border-width:2px;" CRLF
              TAB "overflow:auto;" CRLF
              "}" CRLF
              ".submit {" CRLF
              TAB "text-align:left;" CRLF
              TAB "margin:auto;" CRLF
              TAB "margin-bottom:1em;" CRLF
              TAB "width:25em;" CRLF
              TAB "padding:1em;" CRLF
              TAB "background-color:#AAFFAA;" CRLF
              TAB "border-style:solid;" CRLF
              TAB "border-color:black;" CRLF
              TAB "border-width:2px;" CRLF
              TAB "overflow:auto;" CRLF
              TAB "}" CRLF
              CSSCOMMENT ("Tooltip styles") CRLF
              ".tooltip_ref:hover .tooltip_text {display:block;}" CRLF
              ".tooltip_ref {color:#006000;font-weight:bold;font-size:60%;}" CRLF
              ".tooltip_text {" CRLF
              TAB "display:none;" CRLF
              TAB "background:#A0FFA0;" CRLF
              TAB "margin-left:1em;" CRLF
              TAB "padding:1em;" CRLF
              TAB "position:absolute;" CRLF
              TAB "color:black;" CRLF
              TAB "font-size:75%;" CRLF
              TAB "tmax-width:20em;" CRLF
              TAB "line-height:100%;" CRLF
              "}" CRLF
              CSSCOMMENT ("Tabs styles") CRLF
              ".tab-content {border-top:1px solid #A0A0A0;margin:auto;width:37em;}" CRLF
              ".tab-header {position:relative;left:0.75em;height:3em;width:37em;margin:auto;margin-bottom:0px;padding-bottom:0px;}" CRLF
              ".tab-header ul.tab-list {margin:0;padding:0;position:absolute;bottom:-1px;width:37em;}" CRLF
              ".tab-header ul.tab-list li {display:inline;list-style:none;margin:0;}" CRLF
              ".tab-header ul.tab-list a,.tab-header ul.tab-list span,.tab-header ul.tab-list a.tab-active {" CRLF
              TAB "width:8em;" CRLF
              TAB "display:block;" CRLF
              TAB "float:left;" CRLF
              TAB "padding:4px 0;" CRLF
              TAB "margin:1px 2px 0 0;" CRLF
              TAB "text-align:center;" CRLF
              TAB "font-size:85%;" CRLF
              TAB "text-decoration:none;" CRLF
              "}" CRLF
              ".tab-header ul.tab-list span,.tab-header ul.tab-list a.tab-active {" CRLF
              TAB "border:1px solid #606060;" CRLF
              TAB "border-bottom:none;" CRLF
              TAB "background:#66FF66;" CRLF
              TAB "padding-bottom:7px;" CRLF
              TAB "margin-top:0;" CRLF
              "}" CRLF
              ".tab-header ul.tab-list a {" CRLF
              TAB "background:#D0D0D0;"
              TAB "border-top:1px solid #A0A0A0;" CRLF
              TAB "border-left:1px solid #A0A0A0;" CRLF
              TAB "border-right:1px solid #A0A0A0;" CRLF
              TAB "border-bottom:none;" CRLF
              TAB "color:black;" CRLF
              "}" CRLF
              ".tab {display: none;}" CRLF
              "</style>" CRLF
              "<script type=\"text/javascript\">" CRLF
              "/*<![CDATA[*/" CRLF
              JSCOMMENT ("HTML Tabs begin") CRLF
              CRLF
              "function getChildElementsByClassName(parentElement,className){"  CRLF
              TAB "var i,childElements,pattern,result;" CRLF
              TAB "result=new Array();" CRLF
              TAB "pattern=new RegExp(\"\\\\b\"+className+\"\\\\b\");" CRLF
              CRLF
              TAB "childElements=parentElement.getElementsByTagName('*');" CRLF
              TAB "for(i=0;i<childElements.length;i++){" CRLF
              TAB TAB "if(childElements[i].className.search(pattern)!=-1){" CRLF
              TAB TAB TAB "result[result.length]=childElements[i];" CRLF
              TAB TAB "}" CRLF
              TAB "}" CRLF
              TAB "return result;" CRLF
              "}" CRLF
              CRLF
              CRLF
              "function BuildTabs(containerId){" CRLF
              TAB "var i,tabContainer,tabContents,tabHeading,title,tabElement;" CRLF
              TAB "var divElement,ulElement,liElement,tabLink,linkText;" CRLF
              CRLF
              TAB JSCOMMENT("assume that if document.getElementById exists, then this will work...") CRLF
              TAB "if(!eval('document.getElementById'))return;" CRLF
              TAB "tabContainer=document.getElementById(containerId);" CRLF
              TAB "if(tabContainer==null)return;" CRLF
              TAB "tabContents=getChildElementsByClassName(tabContainer,'tab-content');" CRLF
              TAB "if(tabContents.length==0)return;" CRLF
              CRLF
              TAB "divElement=document.createElement(\"div\");" CRLF
              TAB "divElement.className='tab-header';" CRLF
              TAB "divElement.id=containerId+'-header';" CRLF
              TAB "ulElement=document.createElement(\"ul\");" CRLF
              TAB "ulElement.className='tab-list';" CRLF
              CRLF
              TAB "tabContainer.insertBefore(divElement,tabContents[0]);" CRLF
              TAB "divElement.appendChild(ulElement);" CRLF
              CRLF
              TAB "for(i=0;i<tabContents.length;i++){" CRLF
              TAB TAB "tabHeading=getChildElementsByClassName(tabContents[i],'tab');" CRLF
              TAB TAB "title=tabHeading[0].childNodes[0].nodeValue;" CRLF
              CRLF
              TAB TAB JSCOMMENT("create the tabs as an unsigned list") CRLF
              TAB TAB "liElement=document.createElement(\"li\");" CRLF
              TAB TAB "liElement.id=containerId+'-tab-'+i;" CRLF
              CRLF
              TAB TAB "tabLink=document.createElement(\"a\");" CRLF
              TAB TAB "linkText=document.createTextNode(title);" CRLF
              CRLF
              TAB TAB "tabLink.className=\"tab-item\";" CRLF
              CRLF
              TAB TAB "tabLink.setAttribute(\"href\",\"javascript://\");" CRLF
              TAB TAB "tabLink.onclick=new Function (\"ActivateTab('\"+containerId+\"', \"+i+\")\");" CRLF
              CRLF
              TAB TAB "ulElement.appendChild(liElement);" CRLF
              TAB TAB "liElement.appendChild(tabLink);" CRLF
              TAB TAB "tabLink.appendChild(linkText);" CRLF
              CRLF
              TAB TAB JSCOMMENT ("remove the H1") CRLF
              TAB "tabContents[i].removeChild;" CRLF
              TAB "}" CRLF
              "}" CRLF
              CRLF
              CRLF
              "function ActivateTab(containerId,activeTabIndex){" CRLF
              TAB "var i,tabContainer,tabContents;" CRLF
              CRLF
              TAB "tabContainer=document.getElementById(containerId);" CRLF
              TAB "if(tabContainer==null) return;" CRLF
              CRLF
              TAB "tabContents=getChildElementsByClassName(tabContainer, 'tab-content');" CRLF
              TAB "if(tabContents.length>0) {" CRLF
              TAB TAB "for(i=0;i<tabContents.length;i++) {" CRLF
              /*  TAB TAB TAB "//tabContents[i].className = \"tab-content\";" CRLF*/
              TAB TAB TAB "tabContents[i].style.display=\"none\";" CRLF
              TAB TAB "}" CRLF
              CRLF
              TAB TAB "tabContents[activeTabIndex].style.display=\"block\";" CRLF
              CRLF
              TAB TAB "tabList=document.getElementById(containerId+'-list');" CRLF
              TAB TAB "tabs=getChildElementsByClassName(tabContainer,'tab-item');" CRLF
              TAB TAB "if(tabs.length>0) {" CRLF
              TAB TAB TAB "for(i=0;i<tabs.length;i++){" CRLF
              TAB TAB TAB TAB "tabs[i].className=\"tab-item\";" CRLF
              TAB TAB TAB "}" CRLF
              CRLF
              TAB TAB TAB "tabs[activeTabIndex].className=\"tab-item tab-active\";" CRLF
              TAB TAB TAB "tabs[activeTabIndex].blur();" CRLF
              TAB TAB "}" CRLF
              TAB "}" CRLF
              "}" CRLF
              CRLF
              "function InitTabs(){" CRLF
              TAB "BuildTabs('tab-container');" CRLF
              TAB "ActivateTab('tab-container',0);" CRLF
              "}" CRLF
              JSCOMMENT ("HTML Tabs end") CRLF
              "/*]]>*/" CRLF
              "</script>" CRLF
              "</head>" CRLF
              "<body onload=\"InitTabs();\">" CRLF
              "<form method=\""
             );
    FORM_PART(bodyBegin3,
              "\" action=\""
             );
    FORM_PART (bodyBegin4,
               "\">" CRLF
               "<h1>"
              );
    FORM_PART(bodyBegin5,
              "</h1>" CRLF
              "<div id='tab-container'>" CRLF
             );
    FORM_PART(bodyEnd,
              "</div>" CRLF
              "</form>" CRLF
              "</body>" CRLF
              "</html>" CRLF
             );
    FORM_PART(defaultFormMethod, "POST");
  public:
    FORM_PART(sectionBegin1,
              "<div class=\"tab-content\">" CRLF
              "<h2 class=\"tab\">"
             );
    FORM_PART(sectionBegin2,
              "</h2><br>" CRLF
             );
    FORM_PART(sectionEnd,
              "</div>" CRLF
             );
  public:
    FORM_PART(submitButton1,
              "<div class=\"submit\">" CRLF
              "<input type=\"submit\" value=\""
             );
    FORM_PART(submitButton2,
              "\"><br>" CRLF
              "</div>" CRLF
             );
  public:
    FORM_PART(subsectionBegin1,
              "<div class=\"subsection\">" CRLF
              "<h3>"
             );
    FORM_PART(subsectionBegin2,
              "</h3>" CRLF
             );
    FORM_PART(subsectionEnd,
              "</div>" CRLF
             );
  public:
    FORM_PART(tooltipBegin, "&nbsp;<span class=\"tooltip_ref\">[?]<span class=\"tooltip_text\">");
    FORM_PART(tooltipEnd, "</span></span>");
  public:
    FORM_PART(textParameter1, "<div class=\"parameter\"><label>");
    FORM_PART(textParameter2, "</label>");
    FORM_PART(textParameter3, "<input type=\"text\" name=\"");
    FORM_PART(textParameter4, "\" value=\"");
    FORM_PART(textParameter5, "\"></div>" CRLF);
  public:
    FORM_PART(plaintext1, "<div class=\"parameter\"><label>");
    FORM_PART(plaintext2, "</label>");
    FORM_PART(plaintext3, "<div class=\"plaintext\">");
    FORM_PART(plaintext4, "</div></div>" CRLF);
  public:
    FORM_PART(link1, "<div class=\"parameter\"><label>");
    FORM_PART(link2, "</label>");
    FORM_PART(link3, "<div class=\"plaintext\"><a href=\"");
    FORM_PART(link4, "\">");
    FORM_PART(link5, "</a></div></div>" CRLF);
  public:
    FORM_PART(selectParameter1, "<div class=\"parameter\"><label>");
    FORM_PART(selectParameter2, "</label>" CRLF);
    FORM_PART(selectParameter3, "<select name=\"");
    FORM_PART(selectParameter4, "\">" CRLF);
    FORM_PART(selectParameter5, "</select></div>" CRLF);
  public:
    FORM_PART(selectParameterOption1, "<option value=\"");
    FORM_PART(selectParameterOption2, "\">");
    FORM_PART(selectParameterOption2Selected, "\" selected>");
    FORM_PART(selectParameterOption3, "</option>" CRLF);
  public:
    FORM_PART(checkboxParameter1, "<div class=\"parameter\"><label>");
    FORM_PART(checkboxParameter2, "</label>");
    FORM_PART(checkboxParameter3, "<input type=\"checkbox\" name=\"");
    FORM_PART(checkboxParameter4, "\"></div>");
    FORM_PART(checkboxParameter4Checked, "\" checked></div>" CRLF);
}  __attribute__((packed));

#undef HTML_FORMATTING

#undef TAB
#undef CRLF
#undef CSSCOMMENT
#undef JSCOMMENT

#undef FORM_PART

extern const WebccFormHTML PROGMEM webccFormHTML;

//////////////////////////////////////////////////////////////////////
// WebccForm
//////////////////////////////////////////////////////////////////////

class WebccForm {
  public:
    WebccForm(Print &client);
    ~WebccForm();
  public:
    void bodyBegin(const char * formURL = NULL,
                   const char * caption = NULL,
                   const char * submitButtonText = NULL,
                   const char * formMethod = NULL,
                   boolean progmemStrings = true);
    void bodyEnd(void);
  public:
    void sectionBegin(const char * displayName,
                      boolean progmemStrings = true);
    void sectionEnd(void);
    void subsectionBegin(const char * displayName,
                         boolean progmemStrings = true);
    void subsectionEnd(void);
  public:
    void textParameter (const char * displayName,
                        const char * internalName,
                        const char * value = NULL,
                        const char * tooltipText = NULL,
                        boolean progmemStrings = true);
    inline void textParameter (const char * displayName,
                               const char * internalName,
                               long value,
                               const char * tooltipText = NULL,
                               boolean progmemStrings = true);
    void plaintext (const char *displayName,
                    const char * value = NULL,
                    const char * tooltipText = NULL,
                    boolean progmemStrings = true);
    inline void plaintext (const char * displayName,
                           long value,
                           const char * tooltipText = NULL,
                           boolean progmemStrings = true);
    void link (const char * displayName,
               const char * linkURL,
               const char * value,
               const char * tooltipText = NULL,
               boolean progmemStrings = true);
    void checkboxParameter(const char * displayName,
                           const char * internalName,
                           boolean value,
                           const char * tooltipText = NULL,
                           boolean progmemStrings = true);
    template <class... Options>
    void selectParameter(const char * displayName,
                         const char * internalName,
                         long value,
                         const char * tooltipText,
                         boolean progmemStrings,
                         Options... options);
  private:
    inline void print(const char * string, boolean progmemString, const __FlashStringHelper * defaultString = NULL);
  private:
    template <class OptionValue, class OptionName, class... MoreOptions>
    inline void selectParameterExpand(long value, boolean progmemStrings, OptionValue optionValue, OptionName optionName, MoreOptions... moreOptions);
    inline void selectParameterExpand(long value, boolean progmemStrings);
  private:
    inline void selectParameterBegin(const char * displayName,
                                     const char * internalName,
                                     const char * tooltipText,
                                     boolean progmemStrings);
    inline void selectParameterOption(const char * displayName,
                                      long optionValue,
                                      long actualValue,
                                      boolean progmemStrings);
    inline void selectParameterEnd (void);
  private:
    inline void tooltip(const char *tooltiptext, boolean progmemStrings);
  private:
    Print * client = NULL;
  private:
    const char * submitButtonText = NULL;
    boolean progmemSubmitButtonText;
  private:
    boolean isWithinBody = false;
    boolean isWithinSection = false;
    boolean isWithinSubsection = false;
};

void WebccForm::print(const char * string, boolean progmemString, const __FlashStringHelper * defaultString) {
  if (!string) {
    if (defaultString) client->print(defaultString);
    return;
  }
  progmemString ? client->print(FSH(string)) : client->print(string);
}

template <class... Options>
void WebccForm::selectParameter(const char * displayName,
                                const char * internalName,
                                long value,
                                const char * tooltipText,
                                boolean progmemStrings,
                                Options... options) {
  selectParameterBegin(displayName, internalName, tooltipText, progmemStrings);
  selectParameterExpand(value, progmemStrings, options...);
  selectParameterEnd();
}

template <class OptionValue, class OptionName, class... MoreOptions>
void WebccForm::selectParameterExpand(long value, boolean progmemStrings, OptionValue optionValue, OptionName optionName, MoreOptions... moreOptions) {
  selectParameterOption(optionName, optionValue, value, progmemStrings);
  selectParameterExpand(value, progmemStrings, moreOptions...);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void WebccForm::selectParameterExpand(long value, boolean progmemStrings) {}
#pragma GCC diagnostic pop

void WebccForm::textParameter (const char * displayName,
                               const char * internalName,
                               long value,
                               const char * tooltipText,
                               boolean progmemStrings) {
  const size_t MAX_CHARS_PER_LONG = 12; //max 10 digits per long + optional minus sign + null character
  const int RADIX_DECIMAL = 10;
  char valueAsText[MAX_CHARS_PER_LONG] = {0};
  ltoa (value, valueAsText, RADIX_DECIMAL);
  textParameter(displayName, internalName, valueAsText, tooltipText, progmemStrings);
}

void WebccForm::plaintext (const char * displayName,
                           long value,
                           const char * tooltipText,
                           boolean progmemStrings) {
  const size_t MAX_CHARS_PER_LONG = 12; //max 10 digits per long + optional minus sign + null character
  const int RADIX_DECIMAL = 10;
  char valueAsText[MAX_CHARS_PER_LONG] = {0};
  ltoa (value, valueAsText, RADIX_DECIMAL);
  plaintext(displayName, valueAsText, tooltipText, progmemStrings);
}

void WebccForm::selectParameterBegin(const char * displayName,
                                     const char * internalName,
                                     const char * tooltipText,
                                     boolean progmemStrings) {
  if (!client) return;
  client->print(FSH(webccFormHTML.selectParameter1));
  print(displayName, progmemStrings);
  client->print(FSH(webccFormHTML.selectParameter2));
  tooltip(tooltipText, progmemStrings);
  client->print(FSH(webccFormHTML.selectParameter3));
  print(internalName, progmemStrings);
  client->print(FSH(webccFormHTML.selectParameter4));
}

void WebccForm::selectParameterOption(const char * displayName,
                                      long optionValue,
                                      long actualValue,
                                      boolean progmemStrings) {
  if (!client) return;
  client->print(FSH(webccFormHTML.selectParameterOption1));
  client->print(optionValue, DEC);
  if (optionValue == actualValue)
    client->print(FSH(webccFormHTML.selectParameterOption2Selected));
  else
    client->print(FSH(webccFormHTML.selectParameterOption2));
  print(displayName, progmemStrings);
  client->print(FSH(webccFormHTML.selectParameterOption3));
}

void WebccForm::selectParameterEnd (void) {
  if (!client) return;
  client->print(FSH(webccFormHTML.selectParameter5));
}

void WebccForm::tooltip(const char * tooltipText, boolean progmemStrings) {
  if (!client) return;
  if (!tooltipText) return;
  client->print(FSH(webccFormHTML.tooltipBegin));
  print(tooltipText, progmemStrings);
  client->print(FSH(webccFormHTML.tooltipEnd));
}

//////////////////////////////////////////////////////////////////////
// BufferedPrint
//////////////////////////////////////////////////////////////////////

//WIFICLIENT_MAX_PACKET_SIZE is 1460, see libraries/ESP8266WiFi/src/WiFiClient.h
//smaller value might be required when porting to Arduino due to memory constraints
class BufferedPrint : public Print {
  public:
    inline BufferedPrint(Print & client);
    inline ~BufferedPrint();
  public:
    virtual size_t write(uint8_t character);
  public:
    inline static size_t getBufferSize(void);
  private:
    Print * client = NULL;
  private:
    void sendBuffer(void);
    size_t bufferPosition = 0;
    const static size_t bufferSize = WIFICLIENT_MAX_PACKET_SIZE;
    uint8_t buffer[bufferSize];
};

BufferedPrint::BufferedPrint (Print & client) {
  this->client = &client;
}

BufferedPrint::~BufferedPrint () {
  sendBuffer();
}

inline size_t BufferedPrint::getBufferSize(void) {
  return (bufferSize);
}

//////////////////////////////////////////////////////////////////////
// HTTPReqParserStateMachine
//////////////////////////////////////////////////////////////////////

class HTTPReqParserStateMachine {
  public:
    inline boolean begin(Stream &client);
    void parse(char * buffer, size_t bufferSize, HTTPRequestPart * part);
    inline boolean finished(void) const;
    inline boolean error(void) const;
    inline ParseError getError(void) const;
  private:
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
      FIELD_VALUE_PART1,
      FIELD_VALUE_PART2,
      POST_QUERY_OR_END,
      POST_QUERY_NAME,
      POST_QUERY_VALUE,
      FINISHED,
      ERROR_INTERNAL,
      ERROR_REQUEST_PART_TOO_LONG,
      ERROR_REQUEST_STRUCTURE,
      ERROR_REQUEST_SEMANTICS,
    };
    enum class ControlCharacter {
      OTHER,       //Character other than listed below
      SPACE,       //Space (0x20)
      QUESTION,    //Question mark (0x3F)
      AMPERSAND,   //Ampersand (0x26)
      EQUAL,       //Equal sign (0x3D)
      COLON,       //Colon (0x3A)
      CRLF,        //\r\n (0x10 & 0x13)
      SEMICOLON,   //Semicolon (0x3B)
      UNAVAILABLE, //see STREAM_UNAVAILABLE (-1)
    };
    enum class ControlCharacterSet {
      ALL,        //All control characters used
      FIELD_VALUE //Only UNAVAILABLE, SEMICOLON, EQUAL and CRLF characters are used
    };
    enum class StreamOperation {
      DO_NOTHING,         //Do not read anything from stream
      READ_UNTIL,         //Read from stream to buffer until one of control characters is found or buffer is full
      SKIP,               //Read from stream until control character are found
      READ_SINGLE,        //Read single character
      READ_IF_CC          //Do not read anything from stream and peek (rather than read) next character
    };
  private:
    inline void transition(ParserState newState);
    inline void setError(ParseError error);
    inline void setInternalError(size_t codeLine = 0);
  private:
    ParserState currentState = ParserState::BEGIN;
    Stream * inputStream = NULL;
    size_t internalErrorCodeLine = 0;
  private:
    class InputStreamHelper {
      public:
        static ControlCharacter readUntilControlCharacter(Stream &client, ControlCharacterSet controlChars, char *buffer, size_t bufferSize);
        static ControlCharacter skipUntilControlCharacter(Stream &client, ControlCharacterSet controlChars);
        static ControlCharacter read(Stream &client, ControlCharacterSet controlChars);
        static ControlCharacter readIfControlCharacter(Stream &client, ControlCharacterSet controlChars);
      private:
        static const int CC_UNAVAILABLE = -1; //returned by Stream::read and Stream::peek if no more data available
        static const int CC_CR          = static_cast<int>('\r');
        static const int CC_LF          = static_cast<int>('\n');
        static const int CC_SPACE       = static_cast<int>(' ');
        static const int CC_QUESTION    = static_cast<int>('?');
        static const int CC_AMPERSAND   = static_cast<int>('&');
        static const int CC_EQUAL       = static_cast<int>('=');
        static const int CC_COLON       = static_cast<int>(':');
        static const int CC_SEMICOLON   = static_cast<int>(';');
      private:
        static inline boolean isControlCharacter(int i1, int i2 = CC_UNAVAILABLE);
        static inline boolean isFieldValueControlCharacter(int i1, int i2 = CC_UNAVAILABLE);
        static inline boolean isControlCharacterInSet(ControlCharacterSet cc, int i1, int i2 = CC_UNAVAILABLE);
        static ControlCharacter intToControlCharacter(ControlCharacterSet cc, int first, int second = CC_UNAVAILABLE);
    };
    class ParserTables {
      public:
        static boolean getStateProperties(ParserState state,
                                          StreamOperation *streamOp,
                                          HTTPRequestPart *reqPart,
                                          ParserState *defaultTransition,
                                          ControlCharacterSet *controlChars);
        static ParserState getNextState(ParserState currentState,
                                        ControlCharacter nextCharacter,
                                        ParserState defaultTransition);
      private:
        struct StateTableEntry {
          ParserState state;
          StreamOperation streamOp;
          HTTPRequestPart reqPart;
          ParserState defaultTransition;
          ControlCharacterSet controlChars;
        };
        struct TransitionTableEntry {
          ParserState state;
          ControlCharacter nextCharacter;
          ParserState newState;
        };
    };
};

boolean HTTPReqParserStateMachine::begin(Stream & client) {
  inputStream = &client;
  currentState = ParserState::BEGIN;
  internalErrorCodeLine = 0;
  return (true);
}

boolean HTTPReqParserStateMachine::finished(void) const {
  return (error() || (currentState == ParserState::FINISHED));
}

boolean HTTPReqParserStateMachine::error(void) const {
  return ((currentState == ParserState::ERROR_INTERNAL) ||
          (currentState == ParserState::ERROR_REQUEST_PART_TOO_LONG) ||
          (currentState == ParserState::ERROR_REQUEST_STRUCTURE) ||
          (currentState == ParserState::ERROR_REQUEST_SEMANTICS));
}

ParseError HTTPReqParserStateMachine::getError(void) const {
  switch (currentState) {
    case ParserState::ERROR_INTERNAL:
      return (ParseError::INTERNAL_ERROR);
    case ParserState::ERROR_REQUEST_PART_TOO_LONG:
      return (ParseError::REQUEST_PART_TOO_LONG);
    case ParserState::ERROR_REQUEST_STRUCTURE:
      return (ParseError::REQUEST_STRUCTURE);
    case ParserState::ERROR_REQUEST_SEMANTICS:
      return (ParseError::REQUEST_SEMANTICS);
    default:
      return (ParseError::NONE);
  }
}

void HTTPReqParserStateMachine::transition(HTTPReqParserStateMachine::ParserState newState) {
  currentState = newState;
}

void HTTPReqParserStateMachine::setError(ParseError error) {
  switch (error) {
    case ParseError::INTERNAL_ERROR:
      transition(ParserState::ERROR_INTERNAL);
      return;
    case ParseError::REQUEST_PART_TOO_LONG:
      transition(ParserState::ERROR_REQUEST_PART_TOO_LONG);
      return;
    case ParseError::REQUEST_STRUCTURE:
      transition(ParserState::ERROR_REQUEST_STRUCTURE);
      return;
    case ParseError::REQUEST_SEMANTICS:
      transition(ParserState::ERROR_REQUEST_SEMANTICS);
      return;
    default:
      return;
  }
}

void HTTPReqParserStateMachine::setInternalError(size_t codeLine) {
  internalErrorCodeLine = codeLine;
  setError(ParseError::INTERNAL_ERROR);
}

boolean HTTPReqParserStateMachine::InputStreamHelper::isControlCharacter(int i1, int i2) {
  if ((i1 == CC_CR) && (i2 == CC_LF)) return (true);
  return ((i1 == CC_UNAVAILABLE) ||
          (i1 == CC_SPACE) ||
          (i1 == CC_QUESTION) ||
          (i1 == CC_AMPERSAND) ||
          (i1 == CC_EQUAL) ||
          (i1 == CC_COLON) ||
          (i1 == CC_SEMICOLON));
}

boolean HTTPReqParserStateMachine::InputStreamHelper::isFieldValueControlCharacter(int i1, int i2) {
  if ((i1 == CC_CR) && (i2 == CC_LF)) return (true);
  return ((i1 == CC_UNAVAILABLE) ||
          (i1 == CC_EQUAL) ||
          (i1 == CC_SEMICOLON));
}

boolean HTTPReqParserStateMachine::InputStreamHelper::isControlCharacterInSet(ControlCharacterSet cc, int i1, int i2) {
  switch (cc) {
    case ControlCharacterSet::ALL:
      return (isControlCharacter(i1, i2));
    case ControlCharacterSet::FIELD_VALUE:
      return (isFieldValueControlCharacter(i1, i2));
  }
  return (false);
}

#undef FSH

};

#endif
