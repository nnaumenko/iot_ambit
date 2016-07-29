/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include "diag.h"
#include "http.h"
#include "webcc.h"
#include "webcc_html.h"
#include "uitexts.h"

WebConfigControl::WebConfigControl(WiFiServer &server, HTTPReqParser &parser, HTTPReqPartHandler &handler) {
  this->server = &server;
  this->parser = &parser;
  this->reqPartHandler = &handler;
}

boolean WebConfigControl::begin(void) {
  return (this->server && this->parser && this->reqPartHandler);
}

void WebConfigControl::execute(void) {
  WiFiClient primaryClient = this->server->available();
  if (!primaryClient) {
    return;
  }
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] web client connected"));
  while (!primaryClient.available()) {
    delay(1);
    yield();
  }
  HTTPStream client(primaryClient);
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] begin parsing."));
  this->parser->begin(client);
  this->parser->setHandler(*this->reqPartHandler);
  do {
    this->parser->parse();
    yield();
  } while (!this->parser->isFinished());
  DiagLog.print(F("["));
  DiagLog.print(millis());
  DiagLog.println(F("] end parsing."));
  DiagLog.print(F("Result: "));
  if (this->parser->isError()) DiagLog.println(F("error")); else DiagLog.println(F("OK"));
  client.flush();
}


