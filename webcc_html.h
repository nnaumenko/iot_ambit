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

class WebccHTML {
  public:
    WebccHTML(Print &client);
    ~WebccHTML();
  private:
    Print * client = NULL;
  public:
    void bodyBegin(char * caption, boolean isCaptionProgmem = true);
    void bodyEnd(void);
  private:
    boolean isWithinBody = false;
  public:
    void sectionBegin(const char * displayName, boolean isDisplayNameProgmem = true);
    void sectionEnd(void);
    void subsectionBegin(const char * displayName, boolean isDisplayNameProgmem = true);
    void subsectionEnd(void);
  private:
    boolean isWithinSection = false;
    boolean isWithinSubsection = false;
  public:
    void textParameter (const char * displayName, boolean isDisplayNameProgmem,
                        const char * internalName, boolean isInternalNameProgmem,
                        const char * value = NULL,
                        const char * tooltipText = NULL, boolean isTooltipTextProgmem = true);
    void textParameter (const char * displayName, boolean isDisplayNameProgmem,
                        const char * internalName, boolean isInternalNameProgmem,
                        long value,
                        const char * tooltipText = NULL, boolean isTooltipTextProgmem = true);
    void textParameter (const char * displayName,
                        const char * internalName,
                        const char * value = NULL,
                        const char * tooltipText = NULL);
    void textParameter (const char * displayName,
                        const char * internalName,
                        long value,
                        const char * tooltipText = NULL);
  public:
    void selectParameterBegin(const char * displayName, boolean isDisplayNameProgmem,
                              const char * internalName, boolean isInternalNameProgmem,
                              const char * tooltipText = NULL, boolean isTooltipTextProgmem = true);
    void selectParameterBegin(const char * displayName,
                              const char * internalName,
                              const char * tooltipText = NULL);
    void selectParameterOption(const char * displayName, boolean isDisplayNameProgmem,
                               long optionValue,
                               long actualValue);
    void selectParameterOption(const char * displayName,
                               long optionValue,
                               long actualValue);
    void selectParameterEnd (void);
  public:
    void checkBoxParameter(const char * displayName, boolean isDisplayNameProgmem,
                           const char * internalName, boolean isInternalNameProgmem,
                           boolean value,
                           const char * tooltipText = NULL, boolean isTooltipTextProgmem = true);
    void checkBoxParameter(const char * displayName, boolean isDisplayNameProgmem,
                           const char * internalName, boolean isInternalNameProgmem,
                           long value,
                           const char * tooltipText = NULL, boolean isTooltipTextProgmem = true);
    void checkBoxParameter(const char * displayName,
                           const char * internalName,
                           boolean value,
                           const char * tooltipText = NULL);
    void checkBoxParameter(const char * displayName,
                           const char * internalName,
                           long value,
                           const char * tooltipText = NULL);
};

class WebccHTMLStringMaps : protected WebccHTML {
  public:
    WebccHTMLStringMaps (Print & client, const StringMap & sections, const StringMap & displayNames, const StringMap & internalNames, const StringMap & tooltips) : WebccHTML (client) {
      this->sections = &sections;
      this->displayNames = &displayNames;
      this->internalNames = &internalNames;
      this->tooltips = &tooltips;
    }
  private:
    const StringMap * sections = NULL;
    const StringMap * displayNames = NULL;
    const StringMap * internalNames = NULL;
    const StringMap * tooltips = NULL;
  public:
    void begin (StringMapKey sectionCaption);
    void end (void);
    void section(StringMapKey section);
    void subsection(StringMapKey subsection);
    void textParameter (StringMapKey parameter, long value);
    void textParameter (StringMapKey parameter, const char * value = NULL);
    void selectParameter(StringMapKey parameter, long value, const StringMap & options);
    void checkBoxParameter(StringMapKey parameter, boolean value);
    void checkBoxParameter(StringMapKey parameter, long value);
};

#endif
