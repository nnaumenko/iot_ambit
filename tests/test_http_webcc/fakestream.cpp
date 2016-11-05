/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "test.h"
#include "fakestream.h"

void TestFakeStream::beginIn_inputInit_expectCorrectInputInBuffer(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamIn testStream;
  char testInBuffer[] = "123";
  //act
  testStream.begin(testInBuffer);
  //assert
  TEST_ASSERT(!strcmp(testStream.getInBufferContent(), testInBuffer));
  TEST_FUNC_END();
}

void TestFakeStream::beginInOut_inputInit_expectCorrectInputInBuffer(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamInOut testStream;
  char testInBuffer[] = "123";
  //act
  testStream.begin(testInBuffer);
  //assert
  TEST_ASSERT(!strcmp(testStream.getInBufferContent(), testInBuffer));
  TEST_FUNC_END();
}

void TestFakeStream::readIn_inputRead_expectCorrectCharsRead(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamIn testStream;
  char testInBuffer[] = "123";
  testStream.begin(testInBuffer);
  int available[3];
  int read[4];
  //act
  available[0] = testStream.available();
  read[0] = testStream.read();
  available[1] = testStream.available();
  read[1] = testStream.read();
  available[2] = testStream.available();
  read[2] = testStream.read();
  available[3] = testStream.available();
  //assert
  TEST_ASSERT(available[0] == 3);
  TEST_ASSERT(read[0] == (int)'1');
  TEST_ASSERT(available[1] == 2);
  TEST_ASSERT(read[1] == (int)'2');
  TEST_ASSERT(available[2] == 1);
  TEST_ASSERT(read[2] == (int)'3');
  TEST_ASSERT(available[3] == 0);
  TEST_FUNC_END();
}

void TestFakeStream::readInOut_inputRead_expectCorrectCharsRead(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamInOut testStream;
  char testInBuffer[] = "123";
  testStream.begin(testInBuffer);
  int available[3];
  int read[4];
  //act
  available[0] = testStream.available();
  read[0] = testStream.read();
  available[1] = testStream.available();
  read[1] = testStream.read();
  available[2] = testStream.available();
  read[2] = testStream.read();
  available[3] = testStream.available();
  //assert
  TEST_ASSERT(available[0] == 3);
  TEST_ASSERT(read[0] == (int)'1');
  TEST_ASSERT(available[1] == 2);
  TEST_ASSERT(read[1] == (int)'2');
  TEST_ASSERT(available[2] == 1);
  TEST_ASSERT(read[2] == (int)'3');
  TEST_ASSERT(available[3] == 0);
  TEST_FUNC_END();
}

void TestFakeStream::readIn_inputReadUnavailable_expectUnavailable(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamIn testStream;
  char testInBuffer[] = "1";
  testStream.begin(testInBuffer);
  int available[3];
  int read[3];
  //act
  available[0] = testStream.available();
  read[0] = testStream.read();
  available[1] = testStream.available();
  read[1] = testStream.read();
  available[2] = testStream.available();
  read[2] = testStream.read();
  //assert
  TEST_ASSERT(available[0] == 1);
  TEST_ASSERT(read[0] == '1');
  TEST_ASSERT(available[1] == 0);
  TEST_ASSERT(read[1] == -1);
  TEST_ASSERT(available[2] == 0);
  TEST_ASSERT(read[2] == -1);
  TEST_FUNC_END();
}

void TestFakeStream::readInOut_inputReadUnavailable_expectUnavailable(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamInOut testStream;
  char testInBuffer[] = "1";
  testStream.begin(testInBuffer);
  int available[3];
  int read[3];
  //act
  available[0] = testStream.available();
  read[0] = testStream.read();
  available[1] = testStream.available();
  read[1] = testStream.read();
  available[2] = testStream.available();
  read[2] = testStream.read();
  //assert
  TEST_ASSERT(available[0] == 1);
  TEST_ASSERT(read[0] == '1');
  TEST_ASSERT(available[1] == 0);
  TEST_ASSERT(read[1] == -1);
  TEST_ASSERT(available[2] == 0);
  TEST_ASSERT(read[2] == -1);
  TEST_FUNC_END();
}

void TestFakeStream::writeOut_printString_expectTheSameStringInTheOutBuffer(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamOut testStream;
  char testPrint[] = "1234567890qwertyuiopasdfghjklzxcvbnm";
  testStream.begin();
  //act
  testStream.print(testPrint);
  //assert
  TEST_ASSERT(!strcmp(testStream.getOutBufferContent(), testPrint));
  TEST_FUNC_END();
}

void TestFakeStream::writeInOut_printString_expectTheSameStringInTheOutBuffer(void) {
  TEST_FUNC_START();
  //arrange
  FakeStreamInOut testStream;
  char testPrint[] = "1234567890qwertyuiopasdfghjklzxcvbnm";
  testStream.begin("");
  //act
  testStream.print(testPrint);
  //assert
  TEST_ASSERT(!strcmp(testStream.getOutBufferContent(), testPrint));
  TEST_FUNC_END();
}

void TestFakeStream::runTests(void) {
  beginIn_inputInit_expectCorrectInputInBuffer();
  readIn_inputRead_expectCorrectCharsRead();
  readIn_inputReadUnavailable_expectUnavailable();
  writeOut_printString_expectTheSameStringInTheOutBuffer();
  beginInOut_inputInit_expectCorrectInputInBuffer();
  readInOut_inputRead_expectCorrectCharsRead();
  readInOut_inputReadUnavailable_expectUnavailable();
  writeInOut_printString_expectTheSameStringInTheOutBuffer();
}

