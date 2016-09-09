/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCC_H
#define WEBCC_H

#include <Arduino.h>
#include "module.h"

#include "http.h"
#include "webcc_req.h"
#include "webcc_html.h"

namespace webcc {

#ifdef MODULE_TEXT
#undef MODULE_TEXT
#endif
#define MODULE_TEXT(name,value) const char name [sizeof(value)] = value

#ifdef PTEXT
#undef PTEXT
#endif
#define PTEXT(name) (__FlashStringHelper *)name

class Texts {
  public:
    MODULE_TEXT(moduleName, "WebConfigControl");

    MODULE_TEXT(rootPath, "/");

    MODULE_TEXT(serverNotInitialised, "WebCC WiFiServer not initialised");
    MODULE_TEXT(moreThanOneModuleAcceptedPath, "More than one moule accepted path ");
    MODULE_TEXT(webClientConnected, "Web client connected");

    MODULE_TEXT(beginParsing, "Begin parsing");
    MODULE_TEXT(endParsing, "End parsing");
    MODULE_TEXT(printMethod, "Method: ");
    MODULE_TEXT(printPath, "Path: ");
    MODULE_TEXT(printResult, "Result: ");
    MODULE_TEXT(ok, "OK");
    MODULE_TEXT(error, "Error");

} __attribute__((packed));

extern const Texts PROGMEM texts;

#undef MODULE_TEXT

template <class Diag, typename Parser, typename HelperStream, class... WebModules>
class WebConfigControl : public Module<WebConfigControl<Diag, Parser, HelperStream, WebModules...>> {
  public:
    void setServer(WiFiServer &server) {
      this->server = &server;
    }
    WiFiServer * getServer(void) {
      return (server);
    }
  private:
    WiFiServer * server = NULL;
  public:
    const char * PROGMEM moduleName (void) {
      return (texts.moduleName);
    }
    const char * PROGMEM getMainPath(void) {
      return (texts.rootPath);
    }
  public:
    void onBegin(void) {
      if (!server)
      {
        Diag::instance()->setMessageClass(Diag::CLASS_ERROR);
        Diag::instance()->println(PTEXT(texts.serverNotInitialised));
      }
    }
  public:
    void onRun(void) {
      Diag * diagLog = Diag::instance();
      if (!server) return;
      WiFiClient primaryClient = this->server->available();
      if (!primaryClient) return;
      diagLog->setMessageClass(Diag::CLASS_DEBUG);
      diagLog->timestamp();
      diagLog->println(PTEXT(texts.webClientConnected));
      while (!primaryClient.available()) {
        delay(1);
        yield();
      }
      HelperStream client(primaryClient);
      Parser parser;
      parser.begin(client);
      diagLog->setMessageClass(Diag::CLASS_DEBUG);
      diagLog->timestamp();
      diagLog->println(PTEXT(texts.beginParsing));
      char tempBuffer[HTTP_REQUEST_PART_MAX_SIZE + 1 + 1];
      tempBuffer[0] = '\0';
      int indexModuleAccepted = WEBMODULES_CALL_NONE_ACCEPTED;

      do {
        parser.parse();
        yield();
        switch (parser.getReqPart()) {
          case HTTPRequestPart::BEGIN:
            callWebModulesOnStart();
            break;
          case HTTPRequestPart::METHOD:
            diagLog->setMessageClass(Diag::CLASS_DEBUG);
            diagLog->print(PTEXT(texts.printMethod));
            diagLog->println(parser.getReqPartValue());
            strncpy(tempBuffer, parser.getReqPartValue(), sizeof(tempBuffer));
            break;
          case HTTPRequestPart::PATH:
            diagLog->setMessageClass(Diag::CLASS_DEBUG);
            diagLog->print(PTEXT(texts.printPath));
            diagLog->println(parser.getReqPartValue());
            indexModuleAccepted = callWebModulesOnPath(parser.getReqPartValue());
            if (indexModuleAccepted == WEBMODULES_CALL_NONE_ACCEPTED) {
              parser.setStatusCode(HTTPStatusCode::NOT_FOUND);
              break;
            }
            if (indexModuleAccepted == WEBMODULES_CALL_MULTIPLE_ACCEPTED) {
              parser.setStatusCode(HTTPStatusCode::INTERNAL_SERVER_ERROR);
              diagLog->setMessageClass(Diag::CLASS_ERROR);
              diagLog->print(PTEXT(texts.moreThanOneModuleAcceptedPath));
              diagLog->println(parser.getReqPartValue());
              //TODO: print list of the modules which accepted the same path
              break;
            }
            if (!callWebModulesOnMethod(indexModuleAccepted, tempBuffer)) { //tempBuffer at this point contains method
              parser.setStatusCode(HTTPStatusCode::NOT_IMPLEMENTED);
              break;
            }
            tempBuffer[0] = '\0';
          case HTTPRequestPart::URL_QUERY_NAME:
            strncpy(tempBuffer, parser.getReqPartValue(), sizeof(tempBuffer));
            break;
          case HTTPRequestPart::URL_QUERY_VALUE:
            callWebModulesOnURLQuery(indexModuleAccepted, tempBuffer, parser.getReqPartValue());
            break;
          case HTTPRequestPart::POST_QUERY_NAME:
            strncpy(tempBuffer, parser.getReqPartValue(), sizeof(tempBuffer));
            break;
          case HTTPRequestPart::POST_QUERY_VALUE:
            callWebModulesOnPOSTQuery(indexModuleAccepted, tempBuffer, parser.getReqPartValue());
            break;
          default:
            break;
        }
      } while (!parser.isFinished());
      diagLog->setMessageClass(Diag::CLASS_DEBUG);
      diagLog->timestamp();
      diagLog->println(PTEXT(texts.endParsing));
      diagLog->print(PTEXT(texts.printResult));
      if (!parser.isError()) {
        diagLog->println(PTEXT(texts.ok));
        callWebModulesOnRespond(indexModuleAccepted, client);
      }
      else {
        diagLog->println(PTEXT(texts.error));
        diagLog->println((int)parser.getStatusCode());
        HTTPResponseHeader::statusLine(client, parser.getStatusCode());
      }
      callWebModulesOnEnd(indexModuleAccepted, parser.isError());
      client.flush();
    }
  private:
    static const int WEBMODULES_CALL_THIS_ACCEPTED = -1;
    static const int WEBMODULES_CALL_NONE_ACCEPTED = -2;
    static const int WEBMODULES_CALL_MULTIPLE_ACCEPTED = -3;

  private:

    inline void callWebModulesOnStart(void) {
      this->onHTTPReqStart();
      boolean callResult[] = { WebModules::instance()->onHTTPReqStart()... };
      (void)callResult;
    }

    inline int callWebModulesOnPath(const char * path) {
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

    inline boolean callWebModulesOnMethod(size_t index, const char *method) {
      if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
        return (this->onHTTPReqMethod(method));
      }
      if (index < 0) return (false);
      size_t i = 0;
      int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqMethod(method) : false)... };
      return (callResult[index]);
    }

    inline boolean callWebModulesOnURLQuery(size_t index, const char * name, const char * value) {
      if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
        return (this->onHTTPReqURLQuery(name, value));
      }
      if (index < 0) return (false);
      int i = 0;
      int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqURLQuery(name, value) : false)... };
      return (callResult[index]);
    }

    inline boolean callWebModulesOnPOSTQuery(size_t index, const char * name, const char * value) {
      if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
        return (this->onHTTPReqPOSTQuery(name, value));
      }
      if (index < 0) return (false);
      int i = 0;
      int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqPOSTQuery(name, value) : false)... };
      return (callResult[index]);
    }

    inline boolean callWebModulesOnRespond(size_t index, Print &client) {
      if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
        return (this->onRespond(client));
      }
      if (index < 0) return (false);
      int i = 0;
      int callResult[] = { ((i++ == index) ? WebModules::instance()->onRespond(client) : false)... };
      return (callResult[index]);
    }

    inline boolean callWebModulesOnEnd(size_t index, boolean error) {
      if (index == WEBMODULES_CALL_THIS_ACCEPTED) {
        return (this->onHTTPReqEnd(error));
      }
      if (index < 0) return (false);
      int i = 0;
      int callResult[] = { ((i++ == index) ? WebModules::instance()->onHTTPReqEnd(error) : false)... };
      return (callResult[index]);
    }

  public:
    inline void setRootRedirect(const char * redirectURL) {
      rootRedirect = redirectURL;
    }
    inline char * PROGMEM getRootRedirect(void) {
      return (rootRedirect);
    }
  private:
    const char * rootRedirect = NULL;
  public:
    boolean onHTTPReqPath(const char * path) {
      if (!strcmp_P(path, texts.rootPath)) return (true);
      return (false);
    }

    boolean onHTTPReqMethod(const char * method) {
      if (HTTPRequestHelper::getMethod(method) == HTTPRequestMethod::GET) return (true);
      return (false);
    }

    boolean onRespond(Print &client) {
      if (rootRedirect) {
        HTTPResponseHeader::redirect(client, (__FlashStringHelper *)rootRedirect);
        return (true);
      }
      const char * paths[] = { (WebModules::instance()->getMainPath())... };
      const char * names[] = { (WebModules::instance()->moduleName())... };
      int moduleCount = sizeof...(WebModules);
      HTTPResponseHeader::contentHeader(client, HTTPContentType::HTML);
      WebccHTMLIndex index(client);
      index.begin();
      index.index((const __FlashStringHelper *)this->moduleName(), (const __FlashStringHelper *)this->getMainPath());
      for (int i = 0; i < moduleCount; i++)
        index.index((const __FlashStringHelper *)names[i], (const __FlashStringHelper *)paths[i]);
      index.finish();
      return (true);
    }
};

#undef PTEXT

};

#endif
