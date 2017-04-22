#include <ESP8266WiFi.h>

#include "test.h"
#include "fakestream.h"

#include "diag.h"

using DiagLog = diag::DiagLog<>;

const char PROGMEM testFlashString[] = "testFlashString_";

class TestDiagLog {
  public:
    struct DiagLogPrintOutputContent {
      static const size_t entries = 8;
      static const size_t messageSize = 32;
      static const size_t severitySize = 32;
      size_t messageCount = 0;
      uint32_t messageNumber[entries] = {};
      uint32_t timestamp[entries] = {};
      char severity[entries][severitySize] = {};
      char message[entries][messageSize] = {};
    };
    static boolean parseDiagLogOutput(const char * output, DiagLogPrintOutputContent * result) {
      result->messageCount = 0;
      const char delimiters[] = "|\r\n";
      const size_t tokenisedOutputSize = 512;
      char tokenisedOutput[tokenisedOutputSize] = {};
      if (tokenisedOutputSize < strlen(output) + 1) return (false);
      strncpy(tokenisedOutput, output, sizeof(tokenisedOutput) - 1);
      char * currentOutputPos = strtok (tokenisedOutput, delimiters);
      if (!currentOutputPos) return (false);
      //atol function might cause issues if message number or timestamp (unsigned) is outside long int (signed) range
      while (currentOutputPos) {
        if (result->messageCount > result->entries) return (false);
        result->messageNumber[result->messageCount] = atol(currentOutputPos);
        yield();
        currentOutputPos = strtok (NULL, delimiters);
        if (!currentOutputPos) return (false);
        result->timestamp[result->messageCount] = atol(currentOutputPos);
        yield();
        currentOutputPos = strtok (NULL, delimiters);
        if (!currentOutputPos) return (false);
        strncpy(result->severity[result->messageCount], currentOutputPos, result->severitySize);
        yield();
        currentOutputPos = strtok (NULL, delimiters);
        if (!currentOutputPos) return (false);
        strncpy(result->message[result->messageCount], currentOutputPos, result->messageSize);
        yield();
        result->messageCount++;
        currentOutputPos = strtok (NULL, delimiters);
      }
      return (true);
    }
  public:
    static void log_disablePrintOutput_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      DiagLog::instance()->disablePrintOutput();
      DiagLog::instance()->setSeverityFilter();
      //act
      DiagLog::instance()->log(DiagLog::Severity::DEBUG, "test");
      //assert
      TEST_ASSERT(true);
      TEST_FUNC_END();
    }
    static void log_messageSinglePart_expectTheSameString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut testOutput;
      testOutput.begin();
      DiagLog::instance()->setPrintOutput(testOutput);
      DiagLog::instance()->setSeverityFilter();
      //act
      uint32_t logResult = DiagLog::instance()->log(DiagLog::Severity::DEBUG, "test");
      DiagLogPrintOutputContent result;
      boolean outputParsingResult = parseDiagLogOutput(testOutput.getOutBufferContent(), &result);
      //assert
      TEST_ASSERT(!testOutput.isOutBufferOverflow());
      TEST_ASSERT(outputParsingResult);
      TEST_ASSERT(result.messageCount == 1);
      TEST_ASSERT(result.messageNumber[0] == logResult);
      TEST_ASSERT(!strcmp(result.severity[0], "Debug"));
      TEST_ASSERT(!strcmp(result.message[0], "test"));
      //cleanup
      DiagLog::instance()->disablePrintOutput();
      TEST_FUNC_END();
    }
    static void log_messageMultipleParts_expectConcatenatedParts(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut testOutput;
      testOutput.begin();
      DiagLog::instance()->setPrintOutput(testOutput);
      DiagLog::instance()->setSeverityFilter();
      const __FlashStringHelper * testFSH = reinterpret_cast<const __FlashStringHelper *>(testFlashString);
      //act
      uint32_t logResult = DiagLog::instance()->log(DiagLog::Severity::DEBUG, "testRAMString_", testFSH, '3');
      DiagLogPrintOutputContent result;
      boolean outputParsingResult = parseDiagLogOutput(testOutput.getOutBufferContent(), &result);
      //assert
      TEST_ASSERT(!testOutput.isOutBufferOverflow());
      TEST_ASSERT(outputParsingResult);
      TEST_ASSERT(result.messageCount == 1);
      TEST_ASSERT(result.messageNumber[0] == logResult);
      TEST_ASSERT(!strcmp(result.severity[0], "Debug"));
      TEST_ASSERT(!strcmp(result.message[0], "testRAMString_testFlashString_3"));
      //cleanup
      DiagLog::instance()->disablePrintOutput();
      TEST_FUNC_END();
    }
    static void log_messageSeverities_expectCorrectSeverityTexts(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut testOutput;
      testOutput.begin();
      DiagLog::instance()->setPrintOutput(testOutput);
      DiagLog::instance()->setSeverityFilter();
      //act
      DiagLog::instance()->log(DiagLog::Severity::EMERGENCY, "test");
      DiagLog::instance()->log(DiagLog::Severity::ALERT, "test");
      DiagLog::instance()->log(DiagLog::Severity::CRITICAL, "test");
      DiagLog::instance()->log(DiagLog::Severity::ERROR, "test");
      DiagLog::instance()->log(DiagLog::Severity::WARNING, "test");
      DiagLog::instance()->log(DiagLog::Severity::NOTICE, "test");
      DiagLog::instance()->log(DiagLog::Severity::INFORMATIONAL, "test");
      DiagLog::instance()->log(DiagLog::Severity::DEBUG, "test");
      DiagLogPrintOutputContent result;
      boolean outputParsingResult = parseDiagLogOutput(testOutput.getOutBufferContent(), &result);
      //assert
      TEST_ASSERT(!testOutput.isOutBufferOverflow());
      TEST_ASSERT(outputParsingResult);
      TEST_ASSERT(result.messageCount == 8);
      int i = 0;
      TEST_ASSERT(!strcmp(result.severity[i++], "Emergency"));
      TEST_ASSERT(!strcmp(result.severity[i++], "Alert"));
      TEST_ASSERT(!strcmp(result.severity[i++], "Critical"));
      TEST_ASSERT(!strcmp(result.severity[i++], "Error"));
      TEST_ASSERT(!strcmp(result.severity[i++], "Warning"));
      TEST_ASSERT(!strcmp(result.severity[i++], "Notice"));
      TEST_ASSERT(!strcmp(result.severity[i++], "Informational"));
      TEST_ASSERT(!strcmp(result.severity[i++], "Debug"));
      //cleanup
      DiagLog::instance()->disablePrintOutput();
      TEST_FUNC_END();
    }
    static void log_twoSequentialMessages_expectMessageNumberIncreases(void) {
      TEST_FUNC_START();
      //arrange
      DiagLog::instance()->setSeverityFilter();
      //act
      uint32_t messageNumber1 = DiagLog::instance()->log(DiagLog::Severity::DEBUG, "test");
      uint32_t messageNumber2 = DiagLog::instance()->log(DiagLog::Severity::DEBUG, "test");
      //assert
      TEST_ASSERT((messageNumber1 + 1) == messageNumber2);
      TEST_FUNC_END();
    }
    static void log_severityFilter_expectNoFilteredMessages(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut testOutput;
      testOutput.begin();
      DiagLog::instance()->setPrintOutput(testOutput);
      DiagLog::instance()->setSeverityFilter(DiagLog::Severity::ERROR);
      uint32_t logResult[3] = {};
      //act
      logResult[0] = DiagLog::instance()->log(DiagLog::Severity::WARNING, "testWarning");
      logResult[1] = DiagLog::instance()->log(DiagLog::Severity::ERROR, "testError");
      logResult[2] = DiagLog::instance()->log(DiagLog::Severity::CRITICAL, "testCritical");
      DiagLogPrintOutputContent result;
      boolean outputParsingResult = parseDiagLogOutput(testOutput.getOutBufferContent(), &result);
      //assert
      TEST_ASSERT(!testOutput.isOutBufferOverflow());
      TEST_ASSERT(outputParsingResult);
      TEST_ASSERT(result.messageCount == 2);
      TEST_ASSERT(logResult[0] = logResult[1]);
      int i = 0;
      TEST_ASSERT(result.messageNumber[i] == logResult[1]);
      TEST_ASSERT(!strcmp(result.severity[i], "Error"));
      TEST_ASSERT(!strcmp(result.message[i++], "testError"));
      TEST_ASSERT(result.messageNumber[i] == logResult[2]);
      TEST_ASSERT(!strcmp(result.severity[i], "Critical"));
      TEST_ASSERT(!strcmp(result.message[i++], "testCritical"));
      //cleanup
      DiagLog::instance()->disablePrintOutput();
      TEST_FUNC_END();
    }
  public:
    static void test_log(void) {
      log_disablePrintOutput_expectNoCrash();
      log_messageSinglePart_expectTheSameString();
      log_messageMultipleParts_expectConcatenatedParts();
      log_messageSeverities_expectCorrectSeverityTexts();
      log_twoSequentialMessages_expectMessageNumberIncreases();
      log_severityFilter_expectNoFilteredMessages();
    }
  public:
    static void setSeverityFilter_severityEmergencyAlertCritical_expectCritical(void) {
      TEST_FUNC_START();
      //arrange
      DiagLog::Severity getSeverityFilterResult[3] = {};
      //act
      int i = 0;
      DiagLog::instance()->setSeverityFilter(DiagLog::Severity::EMERGENCY);
      getSeverityFilterResult[i++] = DiagLog::instance()->getSeverityFilter();
      DiagLog::instance()->setSeverityFilter(DiagLog::Severity::ALERT);
      getSeverityFilterResult[i++] = DiagLog::instance()->getSeverityFilter();
      DiagLog::instance()->setSeverityFilter(DiagLog::Severity::CRITICAL);
      getSeverityFilterResult[i++] = DiagLog::instance()->getSeverityFilter();
      //assert
      i = 0;
      TEST_ASSERT(getSeverityFilterResult[i++] == DiagLog::Severity::CRITICAL);
      TEST_ASSERT(getSeverityFilterResult[i++] == DiagLog::Severity::CRITICAL);
      TEST_ASSERT(getSeverityFilterResult[i++] == DiagLog::Severity::CRITICAL);
      TEST_FUNC_END();
    }
    static void setSeverityFilter_severityErrorWarningNoticeInformationalDebug_expectSameSeverity(void) {
      TEST_FUNC_START();
      //arrange
      DiagLog::Severity getSeverityFilterResult[5] = {};
      //act
      int i = 0;
      DiagLog::instance()->setSeverityFilter(DiagLog::Severity::EMERGENCY);
      getSeverityFilterResult[i++] = DiagLog::instance()->getSeverityFilter();
      DiagLog::instance()->setSeverityFilter(DiagLog::Severity::ALERT);
      getSeverityFilterResult[i++] = DiagLog::instance()->getSeverityFilter();
      DiagLog::instance()->setSeverityFilter(DiagLog::Severity::CRITICAL);
      getSeverityFilterResult[i++] = DiagLog::instance()->getSeverityFilter();
      //assert
      i = 0;
      TEST_ASSERT(getSeverityFilterResult[i++] == DiagLog::Severity::CRITICAL);
      TEST_ASSERT(getSeverityFilterResult[i++] == DiagLog::Severity::CRITICAL);
      TEST_ASSERT(getSeverityFilterResult[i++] == DiagLog::Severity::CRITICAL);
      TEST_FUNC_END();
    }
  public:
    static void test_severityFilter(void) {
      setSeverityFilter_severityEmergencyAlertCritical_expectCritical();
      setSeverityFilter_severityErrorWarningNoticeInformationalDebug_expectSameSeverity();
    }
  public:
    static void runTests(void) {
      test_log();
      test_severityFilter();
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestFakeStream::runTests();
  TestDiagLog::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
