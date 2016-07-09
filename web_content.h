/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEB_CONTENT_H
#define WEB_CONTENT_H

#include <Arduino.h>

class HtmlPage {
  public:
    HtmlPage(Print &client);
    void bodyBegin(void);
    void bodyEnd(void);
    void sectionBegin(const __FlashStringHelper * displayName);
    void sectionEnd(void);
    void subsectionBegin(const __FlashStringHelper * displayName);
    void subsectionEnd(void);
    void textParameter (const __FlashStringHelper * displayName,
                        const __FlashStringHelper * internalName,
                        char * value = NULL,
                        const __FlashStringHelper * tooltipText = NULL);
    void textParameter (const __FlashStringHelper * displayName,
                        const __FlashStringHelper * internalName,
                        long value,
                        const __FlashStringHelper * tooltipText = NULL);
    void selectParameterBegin(const __FlashStringHelper * displayName,
                              const __FlashStringHelper * internalName,
                              const __FlashStringHelper * tooltipText = NULL);
    void selectParameterOption(const __FlashStringHelper * optionDisplayName,
                               int optionValue,
                               int actualValue);
    void selectParameterEnd (void);
  private:
    Print * client = NULL;
};

#endif
