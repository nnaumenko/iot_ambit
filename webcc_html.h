/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCC_HTML_H
#define WEBCC_HTML_H

#include <Arduino.h>
#include "stringmap.h"
#include "eeprom_config.h"

class WebccHTML {
  public:
    WebccHTML(Print &client);
    ~WebccHTML();
  public:
    void bodyBegin(void);
    void bodyEnd(void);
    boolean isWithinBody = false;
  public:
    void sectionBegin(const char * displayName);
    void sectionEnd(void);
    void subsectionBegin(const char * displayName);
    void subsectionEnd(void);
  private:
    boolean isWithinSection = false;
    boolean isWithinSubsection = false;
  public:
    void textParameter (const char * displayName,
                        const char * internalName,
                        const char * value = NULL,
                        const char * tooltipText = NULL);
    void textParameter (const char * displayName,
                        const char * internalName,
                        long value,
                        const char * tooltipText = NULL);
  public:
    void selectParameter(const char * displayName,
                         const char * internalName,
                         const StringMap &options,
                         int value,
                         const char * tooltipText = NULL);
  private:
    void selectParameterBegin(const char * displayName,
                              const char * internalName,
                              const char * tooltipText = NULL);
    void selectParameterOption(const char * optionDisplayName,
                               int optionValue,
                               int actualValue);
    void selectParameterEnd (void);
  private:
    Print * client = NULL;
};

#endif
