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

class TestParserInputStream {
  public:
    static void read_StringWithoutCRLF_expectUnmodifiedString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "123456789";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      char readResult[sizeof(testInBuffer) + 1] = {0};
      //act
      int i = 0;
      while (testStream.available()) {
        readResult[i++] = testStream.read();
      }
      //assert
      TEST_ASSERT(!strcmp(testInBuffer, readResult));
      TEST_FUNC_END();
    }
    static void read_NotAvailable_expectNotAvailable(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int availableResult[2] = {0};
      int readResult[2] = {0};
      //act
      readResult[0] = testStream.read();
      availableResult[0] = testStream.available();
      readResult[1] = testStream.read();
      availableResult[1] = testStream.available();
      //assert
      TEST_ASSERT(readResult[0] == -1);
      TEST_ASSERT(availableResult[0] == 0);
      TEST_ASSERT(readResult[1] == -1);
      TEST_ASSERT(availableResult[1] == 0);
      TEST_FUNC_END();
    }
    static void read_RN_expectN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[2] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == -1);
      TEST_FUNC_END();
    }
    static void read_NR_expectN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[2] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == -1);
      TEST_FUNC_END();
    }
    static void read_NN_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == -1);
      TEST_FUNC_END();
    }
    static void read_RR_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == -1);
      TEST_FUNC_END();
    }
    static void read_RNR_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\n\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == -1);
      TEST_FUNC_END();
    }
    static void read_NRN_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == -1);
      TEST_FUNC_END();
    }
    static void read_NRRN_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\r\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == -1);
      TEST_FUNC_END();
    }
    static void read_NRNR_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\n\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == -1);
      TEST_FUNC_END();
    }
    static void read_RNZ_expectNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[2] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == 'z');
      TEST_FUNC_END();
    }
    static void read_NRZ_expectNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[2] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == 'z');
      TEST_FUNC_END();
    }
    static void read_NNZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void read_RRZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void read_RNRZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\n\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void read_NRNZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void read_NRRNZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\r\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void read_NRNRZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\n\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int readResult[3] = {0};
      //act
      readResult[0] = testStream.read();
      readResult[1] = testStream.read();
      readResult[2] = testStream.read();
      //assert
      TEST_ASSERT(readResult[0] == '\n');
      TEST_ASSERT(readResult[1] == '\n');
      TEST_ASSERT(readResult[2] == 'z');
      TEST_FUNC_END();
    }
  public:
    static void test_read(void) {
      read_StringWithoutCRLF_expectUnmodifiedString();
      read_NotAvailable_expectNotAvailable();
      read_RN_expectN();
      read_NR_expectN();
      read_NN_expectNN();
      read_RR_expectNN();
      read_RNR_expectNN();
      read_NRN_expectNN();
      read_NRRN_expectNN();
      read_NRNR_expectNN();
      read_RNZ_expectNZ();
      read_NRZ_expectNZ();
      read_NNZ_expectNNZ();
      read_RRZ_expectNNZ();
      read_RNRZ_expectNNZ();
      read_NRNZ_expectNNZ();
      read_NRRNZ_expectNNZ();
      read_NRNRZ_expectNNZ();
    }
  public:
    static void peek_StringWithoutCRLF_expectUnmodifiedString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "123456789";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      char peekResult[sizeof(testInBuffer) + 1] = {0};
      //act
      int i = 0;
      while (testStream.available()) {
        peekResult[i++] = testStream.peek();
        testStream.read();
      }
      //assert
      TEST_ASSERT(!strcmp(testInBuffer, peekResult));
      TEST_FUNC_END();
    }
    static void peek_NotAvailable_expectNotAvailable(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[2] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == -1);
      TEST_ASSERT(peekResult[1] == -1);
      TEST_FUNC_END();
    }
    static void peek_RN_expectN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[2] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == -1);
      TEST_FUNC_END();
    }
    static void peek_NR_expectN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[2] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == -1);
      TEST_FUNC_END();
    }
    static void peek_NN_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == -1);
      TEST_FUNC_END();
    }
    static void peek_RR_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == -1);
      TEST_FUNC_END();
    }
    static void peek_RNR_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\n\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == -1);
      TEST_FUNC_END();
    }
    static void peek_NRN_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == -1);
      TEST_FUNC_END();
    }
    static void peek_NRRN_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\r\n";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == -1);
      TEST_FUNC_END();
    }
    static void peek_NRNR_expectNN(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\n\r";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == -1);
      TEST_FUNC_END();
    }
    static void peek_RNZ_expectNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[2] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == 'z');
      TEST_FUNC_END();
    }
    static void peek_NRZ_expectNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[2] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == 'z');
      TEST_FUNC_END();
    }
    static void peek_NNZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void peek_RRZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void peek_RNRZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\r\n\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void peek_NRNZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void peek_NRRNZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\r\nz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == 'z');
      TEST_FUNC_END();
    }
    static void peek_NRNRZ_expectNNZ(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "\n\r\n\rz";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int peekResult[3] = {0};
      //act
      peekResult[0] = testStream.peek();
      testStream.read();
      peekResult[1] = testStream.peek();
      testStream.read();
      peekResult[2] = testStream.peek();
      //assert
      TEST_ASSERT(peekResult[0] == '\n');
      TEST_ASSERT(peekResult[1] == '\n');
      TEST_ASSERT(peekResult[2] == 'z');
      TEST_FUNC_END();
    }
  public:
    static void test_peek(void) {
      peek_StringWithoutCRLF_expectUnmodifiedString();
      peek_NotAvailable_expectNotAvailable();
      peek_RN_expectN();
      peek_NR_expectN();
      peek_NN_expectNN();
      peek_RR_expectNN();
      peek_RNR_expectNN();
      peek_NRN_expectNN();
      peek_NRRN_expectNN();
      peek_NRNR_expectNN();
      peek_RNZ_expectNZ();
      peek_NRZ_expectNZ();
      peek_NNZ_expectNNZ();
      peek_RRZ_expectNNZ();
      peek_RNRZ_expectNNZ();
      peek_NRNZ_expectNNZ();
      peek_NRRNZ_expectNNZ();
      peek_NRNRZ_expectNNZ();
    }
  public:
    static void readUntil_readUntilCharacter_expectPartOfInputString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      char resultBuffer[3][sizeof(testInBuffer)] = {0};
      char separatorChars[2] = {0};
      webcc::ParserInputStream::ReturnStatus returnStatus[3];
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus[0] = testStream.readUntil("a4", resultBuffer[0], sizeof(testInBuffer));
      separatorChars[0] = inputStreamStub.read();
      returnStatus[1] = testStream.readUntil("a4", resultBuffer[1], sizeof(testInBuffer));
      separatorChars[1] = inputStreamStub.read();
      returnStatus[2] = testStream.readUntil("a4", resultBuffer[2], sizeof(testInBuffer));
      //assert
      TEST_ASSERT(testStream.validate());
      TEST_ASSERT(!strcmp(resultBuffer[0], "0123"));
      TEST_ASSERT(separatorChars[0] == '4');
      TEST_ASSERT(returnStatus[0] == webcc::ParserInputStream::ReturnStatus::FOUND);
      TEST_ASSERT(!strcmp(resultBuffer[1], "56789"));
      TEST_ASSERT(separatorChars[1] == 'a');
      TEST_ASSERT(returnStatus[1] == webcc::ParserInputStream::ReturnStatus::FOUND);
      TEST_ASSERT(!strcmp(resultBuffer[2], "bcdefgh"));
      TEST_ASSERT(returnStatus[2] == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_FUNC_END();
    }
    static void readUntil_readUntilFirstCharacter_expectEmptyString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      char resultBuffer[sizeof(testInBuffer)] = {0};
      char separatorChar = '\0';
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.readUntil("0", resultBuffer, sizeof(resultBuffer));
      separatorChar = inputStreamStub.read();
      //assert
      TEST_ASSERT(!strcmp(resultBuffer, ""));
      TEST_ASSERT(separatorChar == '0');
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::FOUND);
      TEST_FUNC_END();
    }
    static void readUntil_characterNotFound_expectSameString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      char resultBuffer[sizeof(testInBuffer) + 2] = {0}; //+1 char for null-terminator and +1 char to make sure input stream is empty before buffer is full
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.readUntil("ijklmnopqrstuvwxyz+-=* ", resultBuffer, sizeof(resultBuffer));
      //assert
      TEST_ASSERT(!strcmp(resultBuffer, testInBuffer));
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_FUNC_END();
    }
    static void readUntil_charNotFoundBecauseCaseSensitive_expectSameString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      char resultBuffer[sizeof(testInBuffer) + 2] = {0}; //+1 char for null-terminator and +1 char to make sure input stream is empty before buffer is full
      webcc::ParserInputStream::ReturnStatus returnStatus;
      //act
      returnStatus = testStream.readUntil("A", resultBuffer, sizeof(resultBuffer));
      //assert
      TEST_ASSERT(!strcmp(resultBuffer, testInBuffer));
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_FUNC_END();
    }
    static void readUntil_stringDoesNotFitInBuffer_expectTruncatedString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      char resultBuffer[11 + 1] = {0}; //11 chars + null-terminator
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.readUntil(" ", resultBuffer, sizeof(resultBuffer)); //+1 = terminating /0 char
      //assert
      TEST_ASSERT(!strcmp(resultBuffer, "0123456789a"));
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::END_OF_BUFFER);
      TEST_FUNC_END();
    }
    static void readUntil_notAvailable_expectEmptyString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "";
      inputStreamStub.begin(testInBuffer);
      char resultBuffer[sizeof(testInBuffer) + 2] = {0}; //+1 char for null-terminator and +1 char to make sure input stream is empty before buffer is full
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.readUntil(" ", resultBuffer, sizeof(resultBuffer));
      //assert
      TEST_ASSERT(!strcmp(resultBuffer, ""));
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_FUNC_END();
    }
    static void readUntil_charsToFindNULL_expectEmptyString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "abcdef";
      inputStreamStub.begin(testInBuffer);
      char resultBuffer[sizeof(testInBuffer)] = {0};
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.readUntil(NULL, resultBuffer, sizeof(resultBuffer));
      //assert
      TEST_ASSERT(!strcmp(resultBuffer, ""));
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_CHARS_TO_FIND);
      TEST_FUNC_END();
    }
    static void readUntil_zeroLengthBuffer_expectEmptyString(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      char resultBuffer[sizeof(testInBuffer) + 1] = {0};
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.readUntil("a", resultBuffer, 0);
      //assert
      TEST_ASSERT(!strcmp(resultBuffer, ""));
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_BUFFER);
      TEST_FUNC_END();
    }
    static void readUntil_readBufferNULL_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.readUntil("a", NULL, 65535);
      //assert
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_BUFFER);
      TEST_FUNC_END();
    }
    static void readUntil_clientNULL_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      char resultBuffer[sizeof(testInBuffer) + 1] = {0};
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      //testStream.begin(inputStreamStub); is NOT called to leave client unitialised
      //act
      returnStatus = testStream.readUntil("a", resultBuffer, sizeof(resultBuffer));
      //assert
      TEST_ASSERT(!testStream.validate());
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_CLIENT);
      TEST_FUNC_END();
    }
    static void readUntil_clientReset_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      char resultBuffer[sizeof(testInBuffer) + 1] = {0};
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      boolean validationResult[2];
      //act
      validationResult[0] = testStream.validate();
      testStream.reset();
      validationResult[1] = testStream.validate();
      returnStatus = testStream.readUntil("a", resultBuffer, sizeof(resultBuffer));
      //assert
      TEST_ASSERT(validationResult[0]);
      TEST_ASSERT(!validationResult[1]);
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_CLIENT);
      TEST_FUNC_END();
    }
  public:
    static void test_readUntil(void) {
      readUntil_readUntilCharacter_expectPartOfInputString();
      readUntil_readUntilFirstCharacter_expectEmptyString();
      readUntil_characterNotFound_expectSameString();
      readUntil_charNotFoundBecauseCaseSensitive_expectSameString();
      readUntil_stringDoesNotFitInBuffer_expectTruncatedString();
      readUntil_notAvailable_expectEmptyString();
      readUntil_charsToFindNULL_expectEmptyString();
      readUntil_zeroLengthBuffer_expectEmptyString();
      readUntil_readBufferNULL_expectNoCrash();
      readUntil_clientNULL_expectNoCrash();
      readUntil_clientReset_expectNoCrash();
    }
  public:
    static void skipUntil_skipUntilCharacter_expectInputCharsSkipped(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      int separatorChars[3] = {0};
      webcc::ParserInputStream::ReturnStatus returnStatus[3];
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus[0] = testStream.skipUntil("a4");
      separatorChars[0] = inputStreamStub.read();
      returnStatus[1] = testStream.skipUntil("a4");
      separatorChars[1] = inputStreamStub.read();
      returnStatus[2] = testStream.skipUntil("a4");
      separatorChars[2] = inputStreamStub.read();
      //assert
      TEST_ASSERT(testStream.validate());
      TEST_ASSERT(separatorChars[0] == (int)'4');
      TEST_ASSERT(returnStatus[0] == webcc::ParserInputStream::ReturnStatus::FOUND);
      TEST_ASSERT(separatorChars[1] == (int)'a');
      TEST_ASSERT(returnStatus[1] == webcc::ParserInputStream::ReturnStatus::FOUND);
      TEST_ASSERT(separatorChars[2] == webcc::ParserInputStream::NOT_AVAILABLE);
      TEST_ASSERT(returnStatus[2] == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_FUNC_END();
    }
    static void skipUntil_characterNotFound_expectInputStreamFullySkipped(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int nextChar;
      //act
      returnStatus = testStream.skipUntil("ijklmnopqrstuvwxyz+-=* ");
      nextChar = testStream.read();
      //assert
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_ASSERT(nextChar == webcc::ParserInputStream::NOT_AVAILABLE);
      TEST_FUNC_END();
    }
    static void skipUntil_charNotFoundBecauseCaseSensitive_expectInputStreamFullySkipped(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      int nextChar;
      //act
      returnStatus = testStream.skipUntil("A");
      nextChar = testStream.read();
      //assert
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_ASSERT(nextChar == webcc::ParserInputStream::NOT_AVAILABLE);
      TEST_FUNC_END();
    }
    static void skipUntil_notAvailable_expectMethodDoesNothing(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      //act
      returnStatus = testStream.skipUntil(" ");
      //assert
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::END_OF_STREAM);
      TEST_FUNC_END();
    }
    static void skipUntil_charsToFindNULL_expectMethodDoesNothing(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "abcd";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      int nextChar;
      //act
      returnStatus = testStream.skipUntil(NULL);
      nextChar = testStream.read();
      //assert
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_CHARS_TO_FIND);
      TEST_ASSERT(nextChar == (int)'a');
      TEST_FUNC_END();
    }
    static void skipUntil_clientNULL_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      //testStream.begin(inputStreamStub); is NOT called to leave client unitialised
      //act
      returnStatus = testStream.skipUntil("a");
      //assert
      TEST_ASSERT(!testStream.validate());
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_CLIENT);
      TEST_FUNC_END();
    }
    static void skipUntil_clientReset_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamIn inputStreamStub;
      char testInBuffer[] = "0123456789abcdefgh";
      inputStreamStub.begin(testInBuffer);
      webcc::ParserInputStream::ReturnStatus returnStatus;
      webcc::ParserInputStream testStream;
      testStream.begin(inputStreamStub);
      boolean validationResult[2];
      //act
      validationResult[0] = testStream.validate();
      testStream.reset();
      validationResult[1] = testStream.validate();
      returnStatus = testStream.skipUntil("a");
      //assert
      TEST_ASSERT(validationResult[0]);
      TEST_ASSERT(!validationResult[1]);
      TEST_ASSERT(returnStatus == webcc::ParserInputStream::ReturnStatus::NO_CLIENT);
      TEST_FUNC_END();
    }
  public:
    static void test_skipUntil(void) {
      skipUntil_skipUntilCharacter_expectInputCharsSkipped();
      skipUntil_characterNotFound_expectInputStreamFullySkipped();
      skipUntil_charNotFoundBecauseCaseSensitive_expectInputStreamFullySkipped();
      skipUntil_notAvailable_expectMethodDoesNothing();
      skipUntil_charsToFindNULL_expectMethodDoesNothing();
      skipUntil_clientNULL_expectNoCrash();
      skipUntil_clientReset_expectNoCrash();
    }
  public:
    static void runTests(void) {
      test_read();
      test_peek();
      test_readUntil();
      test_skipUntil();
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
    static void begin_beginNotCalled_expectNoClientError(void) {
      TEST_FUNC_START();
      //arrange
      char testInBuffer[] = "GET / HTTP/1.1\r\n\r\n";
      FakeStreamIn testInputStream;
      testInputStream.begin(testInBuffer);
      webcc::HTTPReqParserStateMachine testParser;
      //act
      //testParser.begin(testInputStream); is NOT called deliberately
      testParser.parse();
      //assert
      TEST_ASSERT(testParser.finished());
      TEST_ASSERT(testParser.error());
      TEST_ASSERT(testParser.getError() == webcc::ParseError::INTERNAL_ERROR);
      TEST_FUNC_END();
    }
  public:
    static void testGeneralFunctionality(void) {
      begin_validateInternals_expectTrue();
      begin_beginNotCalled_expectNoClientError();
    }
  public:
      static void parse_parseRequest_expectCorrespondingRequestParts(void) {
        TEST_FUNC_START();
        //arrange
        char testInBuffer[] = "POST index%2bindex.htm?a=b&c=d HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: 192.168.4.1\r\nAccept-Encoding: gzip, deflate\r\n\r\ne=f+g&h=i\r\n";
        FakeStreamIn testInputStream;
        testInputStream.begin(testInBuffer);
        webcc::HTTPReqParserStateMachine testParser;
        const int resultEntries = 25;
        const size_t RESULT_BUFFER_ENTRY_SIZE = 64;
        char parseResultValue[resultEntries][RESULT_BUFFER_ENTRY_SIZE] = {0};
        webcc::HTTPRequestPart parseResultPart[resultEntries] = {};
        //act
        boolean beginResult = testParser.begin(testInputStream);
        int i = 0;
        do {
          testParser.parse();
          if ((i < resultEntries) && (testParser.getReqPart() != webcc::HTTPRequestPart::NONE)) {
            strncpy(parseResultValue[i], testParser.getReqPartValue(), sizeof(parseResultValue[i]) - 1);
            parseResultPart[i] = testParser.getReqPart();
            i++;
          }
          yield();
        } while (!testParser.finished());
        //assert
        TEST_ASSERT(RESULT_BUFFER_ENTRY_SIZE >= testParser.getBufferSize());
        TEST_ASSERT(i < resultEntries);
        TEST_ASSERT(beginResult);
        TEST_ASSERT(parseResultPart[0] == webcc::HTTPRequestPart::BEGIN);
        TEST_ASSERT(!strcmp(parseResultValue[0], ""));
        TEST_ASSERT(parseResultPart[1] == webcc::HTTPRequestPart::METHOD);
        TEST_ASSERT(!strcmp(parseResultValue[1], "POST"));
        TEST_ASSERT(parseResultPart[2] == webcc::HTTPRequestPart::PATH);
        TEST_ASSERT(!strcmp(parseResultValue[2], "index+index.htm"));
        TEST_ASSERT(parseResultPart[3] == webcc::HTTPRequestPart::URL_QUERY_NAME);
        TEST_ASSERT(!strcmp(parseResultValue[3], "a"));
        TEST_ASSERT(parseResultPart[4] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
        TEST_ASSERT(!strcmp(parseResultValue[4], "b"));
        TEST_ASSERT(parseResultPart[5] == webcc::HTTPRequestPart::URL_QUERY_NAME);
        TEST_ASSERT(!strcmp(parseResultValue[5], "c"));
        TEST_ASSERT(parseResultPart[6] == webcc::HTTPRequestPart::URL_QUERY_VALUE);
        TEST_ASSERT(!strcmp(parseResultValue[6], "d"));
        TEST_ASSERT(parseResultPart[7] == webcc::HTTPRequestPart::VERSION);
        TEST_ASSERT(!strcmp(parseResultValue[7], "HTTP/1.1"));
        TEST_ASSERT(parseResultPart[8] == webcc::HTTPRequestPart::FIELD_NAME);
        TEST_ASSERT(!strcmp(parseResultValue[8], ""));
        TEST_ASSERT(parseResultPart[9] == webcc::HTTPRequestPart::FIELD_VALUE);
        TEST_ASSERT(!strcmp(parseResultValue[9], ""));
        TEST_ASSERT(parseResultPart[10] == webcc::HTTPRequestPart::FIELD_NAME);
        TEST_ASSERT(!strcmp(parseResultValue[10], ""));
        TEST_ASSERT(parseResultPart[11] == webcc::HTTPRequestPart::FIELD_VALUE);
        TEST_ASSERT(!strcmp(parseResultValue[11], ""));
        TEST_ASSERT(parseResultPart[12] == webcc::HTTPRequestPart::FIELD_NAME);
        TEST_ASSERT(!strcmp(parseResultValue[12], ""));
        TEST_ASSERT(parseResultPart[13] == webcc::HTTPRequestPart::FIELD_VALUE);
        TEST_ASSERT(!strcmp(parseResultValue[13], ""));
        TEST_ASSERT(parseResultPart[14] == webcc::HTTPRequestPart::POST_QUERY_NAME);
        TEST_ASSERT(!strcmp(parseResultValue[14], "e"));
        TEST_ASSERT(parseResultPart[15] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
        TEST_ASSERT(!strcmp(parseResultValue[15], "f g"));
        TEST_ASSERT(parseResultPart[16] == webcc::HTTPRequestPart::POST_QUERY_NAME);
        TEST_ASSERT(!strcmp(parseResultValue[16], "h"));
        TEST_ASSERT(parseResultPart[17] == webcc::HTTPRequestPart::POST_QUERY_VALUE);
        TEST_ASSERT(!strcmp(parseResultValue[17], "i"));
        TEST_ASSERT(i == 18);
        TEST_ASSERT(!testParser.error());
        TEST_FUNC_END();
      }
  public:
    static void testParsing(void) {
      parse_parseRequest_expectCorrespondingRequestParts();
    }
  public:
    static void runTests(void) {
      testGeneralFunctionality();
      testParsing();
    }
};

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestHTTPRequestHelper::runTests();
  TestHTTPPercentCode::runTests();
  TestURL::runTests();
  TestFakeStream::runTests();
  TestParserInputStream::runTests();
  TestBufferedPrint::runTests();
  TestHTTPReqParserStateMachine::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
