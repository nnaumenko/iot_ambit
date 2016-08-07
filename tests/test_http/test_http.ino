#include <ESP8266WiFi.h>
#include "test.h"
#include "http.h"

#ifndef ESP8266
#warning "Please select a ESP8266 board in Tools/Board"
#endif

class testHTTPPercentCode {
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

class testURL {
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

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  testHTTPPercentCode::runTests();
  testURL::runTests();
//  TestHTTPStream::runTests();
  //  testHTTPReqParserStateMachine();
  TEST_END();
}

void loop() {
  delay(100);
}
