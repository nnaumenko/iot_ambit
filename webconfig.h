/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef WEBCONFIG_H
#define WEBCONFIG_H

#include <Arduino.h>

#define BUFFERED_PRINT_SIZE WIFICLIENT_MAX_PACKET_SIZE
//WIFICLIENT_MAX_PACKET_SIZE is 1460, see libraries/ESP8266WiFi/src/WiFiClient.h
//smaller value might be required when porting to Arduino due to memory constraints

class BufferedPrint : public Print {
  public:
    BufferedPrint(Print * destination);
    ~BufferedPrint();
    virtual size_t write(uint8_t);
  private:
    void printBuffer(void);
    Print * destination = NULL;
    int bufferPosition = 0;
    uint8_t buffer[BUFFERED_PRINT_SIZE];
};

class WebPage {
  public:
    WebPage(Print * client) {
      this->client = client;
    }
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

extern const char PROGMEM webServerParameterInternalNameWifiSsid[];
extern const char PROGMEM webServerParameterInternalNameWifiPassword[];
extern const char PROGMEM webServerParameterInternalNameBlynkAuth[];
extern const char PROGMEM webServerParameterInternalNameAdcCalPoint0Calibrated[];
extern const char PROGMEM webServerParameterInternalNameAdcCalPoint1Calibrated[];
extern const char PROGMEM webServerParameterInternalNameAdcCalPoint0Raw[];
extern const char PROGMEM webServerParameterInternalNameAdcCalPoint1Raw[];
extern const char PROGMEM webServerParameterInternalNameAdcRejectCal[];
extern const char PROGMEM webServerParameterInternalNameAdcFilter[];
extern const char PROGMEM webServerParameterInternalNameAdcFilterFreq[];
extern const char PROGMEM webServerParameterInternalNameMiscSensorSerialOutput[];

void webServerBegin(void);
void webServerRun(void);

#endif
