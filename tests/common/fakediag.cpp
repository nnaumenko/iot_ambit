/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "test.h"
#include "fakediag.h"

void TestFakeDiag::TestFakeDiag::log_variousParameters_expectCorrectParametersStored(void) {
  TEST_FUNC_START();
  //arrange
  FakeDiag testDiag;
  static const char PROGMEM testProgmemString[] = "/progmem/";
  static const char testRamString[] = "/ram/";
  int testInt = -5;
  static const char PROGMEM testFinalString[] = "/progmem/ /ram/ -5";
  //act
  testDiag.log(FakeDiag::Severity::DEBUG, FPSTR(testProgmemString), ' ', testRamString, ' ', testInt);
  //assert
  TEST_ASSERT(testDiag.getSeverity(0) == FakeDiag::Severity::DEBUG);
  TEST_ASSERT(testDiag.getTimestamp(0));
  TEST_ASSERT(!strcmp_P(testDiag.getLogEntry(0), testFinalString));

  TEST_FUNC_END();
}

void TestFakeDiag::TestFakeDiag::log_multipleEntries_expectCorrectEntriesStored(void) {
  TEST_FUNC_START();
  //arrange
  FakeDiag testDiag;
  //act
  testDiag.log(FakeDiag::Severity::EMERGENCY, "0");
  testDiag.log(FakeDiag::Severity::ALERT, "1");
  testDiag.log(FakeDiag::Severity::CRITICAL, "2");
  testDiag.log(FakeDiag::Severity::ERROR, "3");
  testDiag.log(FakeDiag::Severity::WARNING, "4");
  testDiag.log(FakeDiag::Severity::NOTICE, "5");
  testDiag.log(FakeDiag::Severity::INFORMATIONAL, "6");
  testDiag.log(FakeDiag::Severity::DEBUG, "7");
  //assert
  TEST_ASSERT(testDiag.getSeverity(0) == FakeDiag::Severity::EMERGENCY);
  TEST_ASSERT(testDiag.getTimestamp(0));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(0), "0"));
  TEST_ASSERT(testDiag.getSeverity(1) == FakeDiag::Severity::ALERT);
  TEST_ASSERT(testDiag.getTimestamp(1));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(1), "1"));
  TEST_ASSERT(testDiag.getSeverity(2) == FakeDiag::Severity::CRITICAL);
  TEST_ASSERT(testDiag.getTimestamp(2));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(2), "2"));
  TEST_ASSERT(testDiag.getSeverity(3) == FakeDiag::Severity::ERROR);
  TEST_ASSERT(testDiag.getTimestamp(3));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(3), "3"));
  TEST_ASSERT(testDiag.getSeverity(4) == FakeDiag::Severity::WARNING);
  TEST_ASSERT(testDiag.getTimestamp(4));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(4), "4"));
  TEST_ASSERT(testDiag.getSeverity(5) == FakeDiag::Severity::NOTICE);
  TEST_ASSERT(testDiag.getTimestamp(5));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(5), "5"));
  TEST_ASSERT(testDiag.getSeverity(6) == FakeDiag::Severity::INFORMATIONAL);
  TEST_ASSERT(testDiag.getTimestamp(6));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(6), "6"));
  TEST_ASSERT(testDiag.getSeverity(7) == FakeDiag::Severity::DEBUG);
  TEST_ASSERT(testDiag.getTimestamp(7));
  TEST_ASSERT(!strcmp(testDiag.getLogEntry(7), "7"));
  TEST_FUNC_END();
}

void TestFakeDiag::log_inputStringExceedsMaxLength_expectStoredStringTruncated(void) {
  TEST_FUNC_START();
  //arrange
  FakeDiag testDiag;
  char veryLongString[FakeDiag::charsPerLogEntry + 3] = {};
  for (size_t i = 0; i < sizeof(veryLongString) - 1; i++)
    veryLongString[i] = i % 10;
  veryLongString[sizeof(veryLongString) - 1] = '\0';
  //act
  testDiag.log(FakeDiag::Severity::DEBUG, veryLongString);
  //assert
  TEST_ASSERT(testDiag.getSeverity(0) == FakeDiag::Severity::DEBUG);
  TEST_ASSERT(testDiag.getTimestamp(0));
  TEST_ASSERT(!strncmp_P(testDiag.getLogEntry(0), veryLongString, FakeDiag::charsPerLogEntry - 1)); //1 for NULL terminator
  TEST_FUNC_END();
}

void TestFakeDiag::log_inputTooManyStrings_expectMaxStringsPerLimit(void) {
  TEST_FUNC_START();
  //arrange
  FakeDiag testDiag;
  //act
  for (size_t i = 0; i < FakeDiag::logEntries + 1; i++)
    testDiag.log(FakeDiag::Severity::DEBUG, i);

  //assert
  for (size_t i = 0; i < FakeDiag::logEntries; i++) {
    TEST_ASSERT(testDiag.getSeverity(i) == FakeDiag::Severity::DEBUG);
    TEST_ASSERT(testDiag.getTimestamp(i));
    TEST_ASSERT(atoi(testDiag.getLogEntry(i)) == (int)i);
  }
  TEST_FUNC_END();
}

void TestFakeDiag::runTests(void) {
  log_variousParameters_expectCorrectParametersStored();
  log_multipleEntries_expectCorrectEntriesStored();
  log_inputStringExceedsMaxLength_expectStoredStringTruncated();
  log_inputTooManyStrings_expectMaxStringsPerLimit();
}
