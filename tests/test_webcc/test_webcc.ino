#include <ESP8266WiFi.h>
#include "test.h"
#include "fakestream.h"
#include "fakediag.h"

#include "util_comm.h"
#include "webcc.h"

#ifndef ESP8266
#warning "Please select a ESP8266 board in Tools/Board"
#endif

class TestBufferedPrint {
  public:
    static const size_t testBufferSize = WIFICLIENT_MAX_PACKET_SIZE;
  public:
    static void write_stringShorterThanBuffer_expectStringSentAfterBufferedPrintOutOfScope(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut outputStreamMock;
      char testPrint[] = "1234567";
      char outBufferContent[2][sizeof(testPrint) + 1] = {0};//+1 char for null-terminator
      outputStreamMock.begin();
      //act
      do {
        uint8_t testBuffer[testBufferSize] = {};
        webcc::BufferedPrint testBufferedPrint(outputStreamMock, testBuffer, testBufferSize);
        testBufferedPrint.print(testPrint);
        strncpy(outBufferContent[0], outputStreamMock.getOutBufferContent(), sizeof(outBufferContent[0]));
      } while (0); //testBufferedPrint goes out of scope here
      strncpy(outBufferContent[1], outputStreamMock.getOutBufferContent(), sizeof(outBufferContent[1]));
      //assert
      TEST_ASSERT(!outputStreamMock.isOutBufferOverflow());
      TEST_ASSERT(!strcmp(outBufferContent[0], ""));
      TEST_ASSERT(!strcmp(outBufferContent[1], testPrint));
      TEST_FUNC_END();
    }
    static void write_stringLongerThanBuffer_expectStringSentAfterBufferIsFull(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut outputStreamMock;
      char testPrint[] = "0123456789";
      outputStreamMock.begin();
      uint8_t testBuffer[testBufferSize] = {};
      webcc::BufferedPrint testBufferedPrint(outputStreamMock, testBuffer, testBufferSize);
      //act
      for (int i = 0; i < (signed)(testBufferSize - strlen(testPrint)); i += strlen(testPrint)) {
        testBufferedPrint.print(testPrint);
      }
      boolean bufferBefore = !strcmp(outputStreamMock.getOutBufferContent(), "");
      testBufferedPrint.print(testPrint);
      boolean bufferAfter = true;
      const char * outBufferContent = outputStreamMock.getOutBufferContent();
      for (size_t i = 0; i < testBufferSize; i += strlen(testPrint)) {
        int comparisonResult = strncmp(testPrint, &outBufferContent[i], strlen(testPrint));
        bufferAfter &= (!comparisonResult);
      }
      //assert
      TEST_ASSERT(!outputStreamMock.isOutBufferOverflow());
      TEST_ASSERT(bufferBefore);
      TEST_ASSERT(bufferAfter);
      TEST_FUNC_END();
    }
  public:
    static void test_write(void) {
      write_stringShorterThanBuffer_expectStringSentAfterBufferedPrintOutOfScope();
      write_stringLongerThanBuffer_expectStringSentAfterBufferIsFull();
    }
  public:
    static void runTests() {
      test_write();
    }
};

class TestHTTPReqParserStateMachine {
  public:
    static void begin_validateInternals_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      char testInBuffer[] = "";
      FakeStreamIn testInputStream;
      testInputStream.begin(testInBuffer);
      webcc::HTTPReqParserStateMachine testParser;
      //act
      boolean beginResult = testParser.begin(testInputStream);
      //assert
      TEST_ASSERT(beginResult);
      TEST_FUNC_END();
    }
    static void begin_beginNotCalled_expectInternalError(void) {
      TEST_FUNC_START();
      //arrange
      char testInBuffer[] = "GET / HTTP/1.1\r\n\r\n";
      FakeStreamIn testInputStream;
      testInputStream.begin(testInBuffer);
      webcc::HTTPReqParserStateMachine testParser;
      //act
      //testParser.begin(testInputStream); is NOT called deliberately
      const size_t bufferSize = 32;
      char buffer[bufferSize] = {};
      webcc::HTTPRequestPart reqPart;
      testParser.parse(buffer, bufferSize, &reqPart);
      //assert
      TEST_ASSERT(testParser.finished());
      TEST_ASSERT(testParser.error());
      TEST_ASSERT(testParser.getError() == webcc::ParseError::INTERNAL_ERROR);
      TEST_FUNC_END();
    }
  public:
    static void test_begin(void) {
      begin_validateInternals_expectTrue();
      begin_beginNotCalled_expectInternalError();
    }
  private:
    struct ParserTestResult {
      static const int resultEntries = 25;
      static const size_t RESULT_BUFFER_ENTRY_SIZE = 32;
      char parseResultValue[resultEntries][RESULT_BUFFER_ENTRY_SIZE] = {};
      webcc::HTTPRequestPart parseResultPart[resultEntries] = {};
      boolean beginResult = false;
      size_t requestPartCount = 0;
      boolean error = false;
      webcc::ParseError errorCode = webcc::ParseError::NONE;
    };
    static boolean testParseRequest(char * request, ParserTestResult * result) {
      if (!request || !result) return (false);
      FakeStreamIn testInputStream;
      testInputStream.begin(request);
      webcc::HTTPReqParserStateMachine testParser;
      result->beginResult = testParser.begin(testInputStream);
      result->requestPartCount = 0;
      do {
        testParser.parse(result->parseResultValue[result->requestPartCount],
                         sizeof(result->parseResultValue[result->requestPartCount]),
                         &result->parseResultPart[result->requestPartCount]);
        result->requestPartCount++;
        yield();
      } while (!testParser.finished());
      result->error = testParser.error();
      result->errorCode = testParser.getError();
      return (true);
    }
  public:
    static void parse_normalGetRequestWithNoFields_expectCorrespondingRequestParts(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_normalGetRequestWithNoFieldsAndWithSpecialChars_expectCorrespondingRequestParts(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET in+dex%2Ehtm HTTP/1.1\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "in dex.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_normalGetRequest_expectCorrespondingRequestParts(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "a"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "b"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "c"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "d"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " parser-test"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_normalPostRequest_expectCorrespondingRequestParts(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n\r\na=b&c=d\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "POST"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "a"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "b"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "c"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "d"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_normalPostRequestWithUrlQuery_expectCorrespondingRequestParts(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm?a=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\nHost: 192.168.4.1\r\nAccept-Encoding: gzip, deflate\r\n\r\ne=f+g&h=i\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "POST"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "a"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "b"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "c"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "d"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "Host"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " 192.168.4.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "Accept-Encoding"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " gzip, deflate"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "e"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "f g"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "h"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "i"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_normalRequests(void) {
      parse_normalGetRequestWithNoFields_expectCorrespondingRequestParts();
      parse_normalGetRequestWithNoFieldsAndWithSpecialChars_expectCorrespondingRequestParts();
      parse_normalGetRequest_expectCorrespondingRequestParts();
      parse_normalPostRequest_expectCorrespondingRequestParts();
      parse_normalPostRequestWithUrlQuery_expectCorrespondingRequestParts();
    }
  public:
    static void parse_brokenMethod_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GE";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenMethodAndTrailingSpace_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET ";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_headerWithMethodOnly_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_headerWithMethodAndTrailingSpace_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET \r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_tooLongMethod_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "01234567890123467890123456789012 index.htm?a=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_NoMethod_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "index.htm?a=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_EmptyMethod_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = " index.htm?a=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithMethodIssues(void) {
      parse_brokenMethod_expectStructureError();
      parse_brokenMethodAndTrailingSpace_expectStructureError();
      parse_headerWithMethodOnly_expectStructureError();
      parse_headerWithMethodAndTrailingSpace_expectStructureError();
      parse_tooLongMethod_expectPartTooLongError();
      parse_NoMethod_expectStructureError();
      parse_EmptyMethod_expectNoError();
    }
  public:
    static void parse_brokenPath_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPathAndTrailingSpace_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm ";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_headerWithMethodAndPathOnly_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_tooLongPath_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET 012345678901234567890123456789012?a=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_EmptyPathWithQuery_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET ?a=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "a"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_EmptyPathWithoutQuery_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET  HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithPathIssues(void) {
      parse_brokenPath_expectStructureError();
      parse_brokenPathAndTrailingSpace_expectStructureError();
      parse_headerWithMethodAndPathOnly_expectStructureError();
      parse_tooLongPath_expectPartTooLongError();
      parse_EmptyPathWithQuery_expectNoError();
      parse_EmptyPathWithoutQuery_expectNoError();
    }
  public:
    static void parse_brokenURLQuery1_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery2_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery3_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery4_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&c=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery5_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm&";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery6_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery7_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?&";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery8_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?=&=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery1NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery2NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery3NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery4NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&c=\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery5NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm&\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery6NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm=\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery7NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?&\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery8NoVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?=&=\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery1FullHeader_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm? HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery3FullHeader_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b& HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery5FullHeader_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm& HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery6FullHeader_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm= HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenURLQuery7FullHeader_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?& HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithURLQueryIssues(void) {
      parse_brokenURLQuery1_expectStructureError();
      parse_brokenURLQuery2_expectStructureError();
      parse_brokenURLQuery3_expectStructureError();
      parse_brokenURLQuery4_expectStructureError();
      parse_brokenURLQuery5_expectStructureError();
      parse_brokenURLQuery6_expectStructureError();
      parse_brokenURLQuery7_expectStructureError();
      parse_brokenURLQuery8_expectStructureError();
      parse_brokenURLQuery1NoVersion_expectStructureError();
      parse_brokenURLQuery2NoVersion_expectStructureError();
      parse_brokenURLQuery3NoVersion_expectStructureError();
      parse_brokenURLQuery4NoVersion_expectStructureError();
      parse_brokenURLQuery5NoVersion_expectStructureError();
      parse_brokenURLQuery6NoVersion_expectStructureError();
      parse_brokenURLQuery7NoVersion_expectStructureError();
      parse_brokenURLQuery8NoVersion_expectStructureError();
      parse_brokenURLQuery1FullHeader_expectStructureError();
      parse_brokenURLQuery3FullHeader_expectStructureError();
      parse_brokenURLQuery5FullHeader_expectStructureError();
      parse_brokenURLQuery6FullHeader_expectStructureError();
      parse_brokenURLQuery7FullHeader_expectStructureError();
    }
  public:
    static void parse_tooLongURLQueryName_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?012345678901234567890123456789012=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_EmptyURLQueryName_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?=b&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "b"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithURLQueryNameIssues(void) {
      parse_tooLongURLQueryName_expectPartTooLongError();
      parse_EmptyURLQueryName_expectNoError();
    }
  public:
    static void parse_tooLongURLQueryValue_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=012345678901234567890123456789012&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_EmptyURLQueryValue_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=&c=d HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "a"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "c"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithURLQueryValueIssues(void) {
      parse_tooLongURLQueryValue_expectPartTooLongError();
      parse_EmptyURLQueryValue_expectNoError();
    }
  public:
    static void parse_brokenVersion_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&c=d HTTP";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_tooLongVersion_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&c=d 012345678901234567890123456789012\r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_EmptyVersion_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm?a=b&c=d \r\nUser-agent: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "a"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "b"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "c"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "d"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "User-agent"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithVersionIssues(void) {
      parse_brokenVersion_expectStructureError();
      parse_tooLongVersion_expectPartTooLongError();
      parse_EmptyVersion_expectNoError();
    }
  public:
    static void parse_headerWithoutTrailingCRLF_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_headerWithOneTrailingCRLF_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithHeaderIssues(void) {
      parse_headerWithoutTrailingCRLF_expectStructureError();
      parse_headerWithOneTrailingCRLF_expectStructureError();
    }
  public:
    static void parse_brokenFieldName_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\nUser";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_fieldWithNameOnly_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\nUser\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_emptyFieldName_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\n: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " parser-test"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_tooLongFieldName_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\n012345678901234567890123456789012: parser-test\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_brokenFieldWithSeparator_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\nUser-agent:";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenFieldValue_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\nUser-agent: p";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_emptyFieldValue_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\nUser-agent:\r\nHost: 192.168.4.1\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "GET"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "Host"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_tooLongFieldValue_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\nUser-agent:012345678901234567890123456789012\r\n\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
    static void parse_fieldFollowedBySingleCRLF_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "GET index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithFieldIssues(void) {
      parse_brokenFieldName_expectStructureError();
      parse_fieldWithNameOnly_expectStructureError();
      parse_emptyFieldName_expectNoError();
      parse_tooLongFieldName_expectPartTooLongError();
      parse_brokenFieldWithSeparator_expectStructureError();
      parse_brokenFieldValue_expectStructureError();
      parse_emptyFieldValue_expectNoError();
      parse_tooLongFieldValue_expectNoError();
      parse_fieldFollowedBySingleCRLF_expectStructureError();
    }
  public:
    static void parse_brokenPOSTQuery1_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\na=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery2_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\na=b&";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery3_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\na=b&c=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery4_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n&";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery5_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery6_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n?&";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery7_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n=&=";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery2FollowedByCRLF_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\na=b&\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery4FollowedByCRLF_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n&\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
    static void parse_brokenPOSTQuery6FollowedByCRLF_expectStructureError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n?&\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_STRUCTURE);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithPostQueryIssues(void) {
      parse_brokenPOSTQuery1_expectStructureError();
      parse_brokenPOSTQuery2_expectStructureError();
      parse_brokenPOSTQuery3_expectStructureError();
      parse_brokenPOSTQuery4_expectStructureError();
      parse_brokenPOSTQuery5_expectStructureError();
      parse_brokenPOSTQuery6_expectStructureError();
      parse_brokenPOSTQuery7_expectStructureError();
      parse_brokenPOSTQuery2FollowedByCRLF_expectStructureError();
      parse_brokenPOSTQuery4FollowedByCRLF_expectStructureError();
      parse_brokenPOSTQuery6FollowedByCRLF_expectStructureError();
    }
  public:
    static void parse_tooLongPOSTQueryName_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n012345678901234567890123456789012=b&c=d\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_EmptyPOSTQueryName_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n\r\n=b&c=d\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "POST"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "b"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithPostQueryNameIssues(void) {
      parse_tooLongPOSTQueryName_expectPartTooLongError();
      parse_EmptyPOSTQueryName_expectNoError();
    }
  public:
    static void parse_tooLongPOSTQueryValue_expectPartTooLongError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n\r\na=012345678901234567890123456789012&c=d\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      TEST_ASSERT(testResult.error);
      TEST_ASSERT(testResult.errorCode == webcc::ParseError::REQUEST_PART_TOO_LONG);
      TEST_FUNC_END();
    }
    static void parse_EmptyPOSTQueryValue_expectNoError(void) {
      TEST_FUNC_START();
      //arrange
      char testRequest[] = "POST index.htm HTTP/1.1\r\nUser-agent: parser-test\r\n\r\na=&c=d\r\n";
      ParserTestResult testResult;
      //act
      boolean testPerformed = testParseRequest(testRequest, &testResult);
      //assert
      TEST_ASSERT(testPerformed);
      TEST_ASSERT(testResult.beginResult);
      int i = 0;
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "POST"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "a"));
      TEST_ASSERT(testResult.parseResultPart[i] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[i++], "c"));
      TEST_ASSERT(!testResult.error);
      TEST_FUNC_END();
    }
  public:
    static void test_parse_requestsWithPostQueryValueIssues(void) {
      parse_tooLongPOSTQueryValue_expectPartTooLongError();
      parse_EmptyPOSTQueryValue_expectNoError();
    }
  public:
    static void runTests(void) {
      test_begin();
      test_parse_normalRequests();
      test_parse_requestsWithMethodIssues();
      test_parse_requestsWithPathIssues();
      test_parse_requestsWithURLQueryIssues();
      test_parse_requestsWithURLQueryNameIssues();
      test_parse_requestsWithURLQueryValueIssues();
      test_parse_requestsWithVersionIssues();
      test_parse_requestsWithHeaderIssues();
      test_parse_requestsWithFieldIssues();
      test_parse_requestsWithPostQueryIssues();
      test_parse_requestsWithPostQueryNameIssues();
      test_parse_requestsWithPostQueryValueIssues();
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestFakeStream::runTests();
  TestBufferedPrint::runTests();
  TestHTTPReqParserStateMachine::runTests();
  TestFakeDiag::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
