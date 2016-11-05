/*
* Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
* All rights reserved
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef STREAM_MOCK_H
#define STREAM_MOCK_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#ifdef ESP8266
#define STREAM_MOCK_OUT_BUFFER_SIZE WIFICLIENT_MAX_PACKET_SIZE+16
#endif

class FakeStreamIn : virtual public Stream {
  public:
    virtual void begin(const char * inputBuffer) {
      this->inBuffer = inputBuffer;
      this->inBufferPosition = 0;
    }
  public:
    virtual int available(void) {
      if (!this->inBuffer) return (this->readNotAvailable);
      return (strlen(&this->inBuffer[this->inBufferPosition]));
    }
    virtual int read(void) {
      if (!this->available()) return (this->readNotAvailable);
      return ((int)inBuffer[inBufferPosition++]);
    }
    virtual int peek(void) {
      if (!this->available()) return (this->readNotAvailable);
      return ((int)inBuffer[inBufferPosition]);
    }
    virtual void flush(void) {
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    virtual size_t write(uint8_t character) {
      return (0);
    }
#pragma GCC diagnostic pop
  public:
    const char * getInBufferContent(void) {
      if (!this->inBuffer) return (NULL);
      return (&this->inBuffer[this->inBufferPosition]);
    }
  private:
    const char * inBuffer = NULL;
    size_t inBufferPosition = 0;
    static const int readNotAvailable = -1;
};

class FakeStreamOut : virtual public Stream {
  public:
    virtual void begin(void) {
      memset(this->outBuffer, '\0', sizeof(this->outBuffer));
      this->outBufferPosition = 0;
    }
  public:
    virtual int available(void) {
      return (0);
    }
    virtual int read(void) {
      return (readNotAvailable);
    }
    virtual int peek(void) {
      return (readNotAvailable);
    }
    virtual void flush(void) {
    }
    virtual size_t write(uint8_t character) {
      if (!this->isOutBufferOverflow()) {
        this->outBuffer[this->outBufferPosition++] = character;
        return (sizeof(character));
      }
      return (0);
    }
  public:
    const char * getOutBufferContent(void) {
      return (this->outBuffer);
    }
    boolean isOutBufferOverflow(void) {
      return (this->outBufferPosition >= (STREAM_MOCK_OUT_BUFFER_SIZE - 1));
    }
    size_t getOutBufferDataSize(void) {
      return (this->outBufferPosition);
    }
  private:
    char outBuffer[STREAM_MOCK_OUT_BUFFER_SIZE] = {0};
    size_t outBufferPosition = 0;
    static const int readNotAvailable = -1;
};

class FakeStreamInOut : public FakeStreamIn, public FakeStreamOut {
  public:
    virtual void begin(const char * inputBuffer) {
      FakeStreamIn::begin(inputBuffer);
      FakeStreamOut::begin();
    }
  public:
    virtual int available(void) {
      return (FakeStreamIn::available());
    }
    virtual int read(void) {
      return (FakeStreamIn::read());
    }
    virtual int peek(void) {
      return (FakeStreamIn::peek());
    }
    virtual void flush(void) {
      FakeStreamIn::flush();
      FakeStreamOut::flush();
    }
    virtual size_t write(uint8_t character) {
      return (FakeStreamOut::write(character));
    }
};

class TestFakeStream {
  public:
    static void beginIn_inputInit_expectCorrectInputInBuffer(void);
    static void readIn_inputRead_expectCorrectCharsRead(void);
    static void readIn_inputReadUnavailable_expectUnavailable(void);
    static void writeOut_printString_expectTheSameStringInTheOutBuffer(void);
    static void beginInOut_inputInit_expectCorrectInputInBuffer(void);
    static void readInOut_inputRead_expectCorrectCharsRead(void);
    static void readInOut_inputReadUnavailable_expectUnavailable(void);
    static void writeInOut_printString_expectTheSameStringInTheOutBuffer(void);
  public:
    static void runTests(void);
};

#endif
