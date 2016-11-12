#include <ESP8266WiFi.h>
#include "test.h"
#include "fakestream.h"

#include "http.h"
#include "webcc.h"

#ifndef ESP8266
#warning "Please select a ESP8266 board in Tools/Board"
#endif

class TestHTTPRequestHelper {
  public:
    static void getMethod_OPTIONS_expectHTTPRequestMethodOPTIONS (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "OPTIONS";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::OPTIONS);
      TEST_FUNC_END();
    }
    static void getMethod_GET_expectHTTPRequestMethodGET (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "GET";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::GET);
      TEST_FUNC_END();
    }
    static void getMethod_HEAD_expectHTTPRequestMethodHEAD (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "HEAD";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::HEAD);
      TEST_FUNC_END();
    }
    static void getMethod_POST_expectHTTPRequestMethodPOST (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "POST";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::POST);
      TEST_FUNC_END();
    }
    static void getMethod_PUT_expectHTTPRequestMethodPUT (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "PUT";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::PUT);
      TEST_FUNC_END();
    }
    static void getMethod_DELETE_expectHTTPRequestMethodDELETE (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "DELETE";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::DELETE);
      TEST_FUNC_END();
    }
    static void getMethod_TRACE_expectHTTPRequestMethodTRACE (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "TRACE";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::TRACE);
      TEST_FUNC_END();
    }
    static void getMethod_CONNECT_expectHTTPRequestMethodCONNECT (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "CONNECT";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::CONNECT);
      TEST_FUNC_END();
    }
    static void getMethod_Unknown_expectHTTPRequestMethodUNKNOWN (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "ABCD";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::UNKNOWN);
      TEST_FUNC_END();
    }
    static void getMethod_CaseSensitive_expectHTTPRequestMethodUNKNOWN (void) {
      TEST_FUNC_START();
      //arrange
      char method[] = "options";
      //act
      HTTPRequestMethod result = HTTPRequestHelper::getMethod(method);
      //assert
      TEST_ASSERT(result == HTTPRequestMethod::UNKNOWN);
      TEST_FUNC_END();
    }
  public:
    static void test_getMethod(void) {
      getMethod_OPTIONS_expectHTTPRequestMethodOPTIONS();
      getMethod_GET_expectHTTPRequestMethodGET();
      getMethod_HEAD_expectHTTPRequestMethodHEAD();
      getMethod_POST_expectHTTPRequestMethodPOST();
      getMethod_PUT_expectHTTPRequestMethodPUT();
      getMethod_DELETE_expectHTTPRequestMethodDELETE();
      getMethod_TRACE_expectHTTPRequestMethodTRACE();
      getMethod_CONNECT_expectHTTPRequestMethodCONNECT();
      getMethod_Unknown_expectHTTPRequestMethodUNKNOWN();
      getMethod_CaseSensitive_expectHTTPRequestMethodUNKNOWN();
    }
  public:
    static void runTests(void) {
      test_getMethod();
    }
};

class TestHTTPPercentCode {
  public:
    static void decodeDigit_0_expect0(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('0');
      //assert
      TEST_ASSERT(result == 0);
      TEST_FUNC_END();
    }
    static void decodeDigit_9_expect9(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('9');
      //assert
      TEST_ASSERT(result == 9);
      TEST_FUNC_END();
    }
    static void decodeDigit_a_expecta(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('a');
      //assert
      TEST_ASSERT(result == 10);
      TEST_FUNC_END();
    }
    static void decodeDigit_f_expectf(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('f');
      //assert
      TEST_ASSERT(result == 15);
      TEST_FUNC_END();
    }
    static void decodeDigit_A_expectA(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('A');
      //assert
      TEST_ASSERT(result == 10);
      TEST_FUNC_END();
    }
    static void decodeDigit_F_expectF(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('F');
      //assert
      TEST_ASSERT(result == 15);
      TEST_FUNC_END();
    }
    static void decodeDigit_NullChar_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('\0');
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeDigit_Space_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('\0');
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeDigit_Percent_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('\0');
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeDigit_Minus_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('-');
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeDigit_h_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeDigit('h');
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
  public:
    static void test_decodeDigit(void) {
      decodeDigit_0_expect0();
      decodeDigit_9_expect9();
      decodeDigit_a_expecta();
      decodeDigit_f_expectf();
      decodeDigit_A_expectA();
      decodeDigit_F_expectF();
      decodeDigit_NullChar_expectError();
      decodeDigit_Space_expectError();
      decodeDigit_Percent_expectError();
    }
  public:
    static void decodeHex_00_expect00(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("00");
      //assert
      TEST_ASSERT(result == 0x00);
      TEST_FUNC_END();
    }
    static void decodeHex_0A_expect0A(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("0A");
      //assert
      TEST_ASSERT(result == 0x0A);
      TEST_FUNC_END();
    }
    static void decodeHex_A0_expectA0(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("A0");
      //assert
      TEST_ASSERT(result == 0xA0);
      TEST_FUNC_END();
    }
    static void decodeHex_FF_expectFF(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("FF");
      //assert
      TEST_ASSERT(result == 0xFF);
      TEST_FUNC_END();
    }
    static void decodeHex_0Space_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("0 ");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_0Percent_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("0%");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_0Minus_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("0-");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_0h_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("0h");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_0x_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("0x");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_Space0_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex(" 0");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_Percent0_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("%0");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_Minus0_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("-0");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_h0_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("h0");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_x0_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("x0");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_23Space_expect23(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int resultOnlyTwoFirstDigitsConverted = HTTPPercentCode::decodeHex("23 ");
      //assert
      TEST_ASSERT(resultOnlyTwoFirstDigitsConverted == 0x23);
      TEST_FUNC_END();
    }
    static void decodeHex_EmptyString_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_SingleCharString_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex("0");
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
    static void decodeHex_NullString_expectError(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int result = HTTPPercentCode::decodeHex(NULL);
      //assert
      TEST_ASSERT(result == HTTPPercentCode::decodeError);
      TEST_FUNC_END();
    }
  public:
    static void test_decodeHex(void) {
      decodeHex_00_expect00();
      decodeHex_0A_expect0A();
      decodeHex_A0_expectA0();
      decodeHex_FF_expectFF();
      decodeHex_0Space_expectError();
      decodeHex_0Percent_expectError();
      decodeHex_0Minus_expectError();
      decodeHex_0h_expectError();
      decodeHex_0x_expectError();
      decodeHex_Space0_expectError();
      decodeHex_Percent0_expectError();
      decodeHex_Minus0_expectError();
      decodeHex_h0_expectError();
      decodeHex_x0_expectError();
      decodeHex_23Space_expect23();
      decodeHex_EmptyString_expectError();
      decodeHex_SingleCharString_expectError();
      decodeHex_NullString_expectError();
    }
  public:
    static void runTests(void) {
      test_decodeDigit();
      test_decodeHex();
    }
};

class TestURL {
  public:
    static void decode_plainURL_expectUnmodifiedURL(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b&c=d";
      //act
      URL::decode(testURLBuffer, sizeof(testURLBuffer));
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index.htm?a=b&c=d"));
      TEST_FUNC_END();
    }
    static void decode_URLwithPercentCode_expectPercentCodeDecoded(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b%26c=d";
      //act
      URL::decode(testURLBuffer, sizeof(testURLBuffer));
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index.htm?a=b&c=d"));
      TEST_FUNC_END();
    }
    static void decode_URLwithPlus_expectPlusSubstitutedWithSpace(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b+b&c=d";
      //act
      URL::decode(testURLBuffer, sizeof(testURLBuffer));
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index.htm?a=b b&c=d"));
      TEST_FUNC_END();
    }
    static void decode_URLwithInvalidPercentCode_expectPercentCodeNotDecodedUnmodifiedURL(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b%2hc=d";
      //act
      URL::decode(testURLBuffer, sizeof(testURLBuffer));
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index.htm?a=b%2hc=d"));
      TEST_FUNC_END();
    }
    static void decode_URLwithBrokenPercentCodeAtTheEnd_expectUnmodifiedURL(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b&c=d%2";
      //act
      URL::decode(testURLBuffer, sizeof(testURLBuffer));
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index.htm?a=b&c=d%2"));
      TEST_FUNC_END();
    }
    static void decode_URLwithPercentCharAtTheEnd_expectUnmodifiedURL(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b&c=d%";
      //act
      URL::decode(testURLBuffer, sizeof(testURLBuffer));
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index.htm?a=b&c=d%"));
      TEST_FUNC_END();
    }
    static void decode_emptyString_expectEmptyString(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "";
      //act
      URL::decode(testURLBuffer, 65535);
      //assert
      TEST_ASSERT(!strlen(testURLBuffer));
      TEST_FUNC_END();
    }
    static void decode_zeroLength_expectUnmodifiedURL(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b+b&c=d";
      //act
      URL::decode(testURLBuffer, 0);
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index.htm?a=b+b&c=d"));
      TEST_FUNC_END();
    }
    static void decode_lengthSmallerThanActual_expectTruncatedString(void) {
      TEST_FUNC_START();
      //arrange
      char testURLBuffer[] = "index.htm?a=b+b&c=d";
      //act
      URL::decode(testURLBuffer, 6);
      //assert
      TEST_ASSERT(!strcmp(testURLBuffer, "index"));
      TEST_FUNC_END();
    }
    static void decode_NULLString_expectMethodDoesNotCrash(void) {
      TEST_FUNC_START();
      //arrange
      //act
      URL::decode(NULL, 65535);
      //assert
      TEST_FUNC_END();
    }
  public:
    static void test_decode(void) {
      decode_plainURL_expectUnmodifiedURL();
      decode_URLwithPercentCode_expectPercentCodeDecoded();
      decode_URLwithPlus_expectPlusSubstitutedWithSpace();
      decode_URLwithInvalidPercentCode_expectPercentCodeNotDecodedUnmodifiedURL();
      decode_URLwithBrokenPercentCodeAtTheEnd_expectUnmodifiedURL();
      decode_URLwithPercentCharAtTheEnd_expectUnmodifiedURL();
      decode_emptyString_expectEmptyString();
      decode_zeroLength_expectUnmodifiedURL();
      decode_lengthSmallerThanActual_expectTruncatedString();
      decode_NULLString_expectMethodDoesNotCrash();
    }
  public:
    static void runTests(void) {
      test_decode();
    }
};

class TestBufferedPrint {
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
        webcc::BufferedPrint testBufferedPrint(outputStreamMock);
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
      webcc::BufferedPrint testBufferedPrint(outputStreamMock);
      //act
      for (int i = 0; i < (signed)(webcc::BufferedPrint::getBufferSize() - strlen(testPrint)); i += strlen(testPrint)) {
        testBufferedPrint.print(testPrint);
      }
      boolean bufferBefore = !strcmp(outputStreamMock.getOutBufferContent(), "");
      testBufferedPrint.print(testPrint);
      boolean bufferAfter = true;
      const char * outBufferContent = outputStreamMock.getOutBufferContent();
      for (size_t i = 0; i < webcc::BufferedPrint::getBufferSize(); i += strlen(testPrint)) {
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "HTTP/1.1"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "in dex.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "HTTP/1.1"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "a"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "b"));
      TEST_ASSERT(testResult.parseResultPart[4] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[4], "c"));
      TEST_ASSERT(testResult.parseResultPart[5] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[5], "d"));
      TEST_ASSERT(testResult.parseResultPart[6] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[6], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[7] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[7], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[8] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[8], " parser-test"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "POST"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[4] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[4], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[5] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[5], "a"));
      TEST_ASSERT(testResult.parseResultPart[6] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[6], "b"));
      TEST_ASSERT(testResult.parseResultPart[7] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[7], "c"));
      TEST_ASSERT(testResult.parseResultPart[8] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[8], "d"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "POST"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "a"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "b"));
      TEST_ASSERT(testResult.parseResultPart[4] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[4], "c"));
      TEST_ASSERT(testResult.parseResultPart[5] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[5], "d"));
      TEST_ASSERT(testResult.parseResultPart[6] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[6], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[7] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[7], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[8] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[8], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[9] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[9], "Host"));
      TEST_ASSERT(testResult.parseResultPart[10] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[10], " 192.168.4.1"));
      TEST_ASSERT(testResult.parseResultPart[11] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[11], "Accept-Encoding"));
      TEST_ASSERT(testResult.parseResultPart[12] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[12], " gzip, deflate"));
      TEST_ASSERT(testResult.parseResultPart[13] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[13], "e"));
      TEST_ASSERT(testResult.parseResultPart[14] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[14], "f g"));
      TEST_ASSERT(testResult.parseResultPart[15] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[15], "h"));
      TEST_ASSERT(testResult.parseResultPart[16] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[16], "i"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "index.htm"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "a"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "HTTP/1.1"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "b"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "a"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "c"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "a"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "b"));
      TEST_ASSERT(testResult.parseResultPart[4] == webcc::HTTPRequestPart::URL_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[4], "c"));
      TEST_ASSERT(testResult.parseResultPart[5] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[5], "d"));
      TEST_ASSERT(testResult.parseResultPart[6] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[6], "User-agent"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], " parser-test"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "GET"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[4] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[4], "Host"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "POST"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[4] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[4], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[5] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[5], "b"));
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
      TEST_ASSERT(testResult.parseResultPart[0] == webcc::HTTPRequestPart::METHOD);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[0], "POST"));
      TEST_ASSERT(testResult.parseResultPart[1] == webcc::HTTPRequestPart::PATH);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[1], "index.htm"));
      TEST_ASSERT(testResult.parseResultPart[2] == webcc::HTTPRequestPart::HTTP_VERSION);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[2], "HTTP/1.1"));
      TEST_ASSERT(testResult.parseResultPart[3] == webcc::HTTPRequestPart::FIELD_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[3], "User-agent"));
      TEST_ASSERT(testResult.parseResultPart[4] == webcc::HTTPRequestPart::FIELD_VALUE_PART1);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[4], " parser-test"));
      TEST_ASSERT(testResult.parseResultPart[5] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[5], "a"));
      TEST_ASSERT(testResult.parseResultPart[6] == webcc::HTTPRequestPart::POST_QUERY_NAME);
      TEST_ASSERT(!strcmp(testResult.parseResultValue[6], "c"));
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

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestHTTPRequestHelper::runTests();
  TestHTTPPercentCode::runTests();
  TestURL::runTests();
  TestFakeStream::runTests();
  TestBufferedPrint::runTests();
  TestHTTPReqParserStateMachine::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
