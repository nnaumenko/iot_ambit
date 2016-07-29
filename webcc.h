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
#include "http.h"

class WebConfigControl {
  public:
    WebConfigControl(WiFiServer &server, HTTPReqParser &parser, HTTPReqPartHandler &handler);
  private:
    WiFiServer * server = NULL;
    HTTPReqParser * parser = NULL;
    HTTPReqPartHandler * reqPartHandler = NULL;
  public:
    boolean begin(void);
    void execute(void);
};

#endif
