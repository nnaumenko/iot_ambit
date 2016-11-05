# Running the tests

Copy here all necessary files from main project directory.

The files to be copied are as follows:

* http.cpp
* http.h
* module.h
* webcc.cpp
* webcc.h

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: static void TestHTTPRequestHelper::getMethod_OPTIONS_expectHTTPRequestMethodOPTIONS(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_GET_expectHTTPRequestMethodGET(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_HEAD_expectHTTPRequestMethodHEAD(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_POST_expectHTTPRequestMethodPOST(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_PUT_expectHTTPRequestMethodPUT(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_DELETE_expectHTTPRequestMethodDELETE(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_TRACE_expectHTTPRequestMethodTRACE(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_CONNECT_expectHTTPRequestMethodCONNECT(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_Unknown_expectHTTPRequestMethodUNKNOWN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_CaseSensitive_expectHTTPRequestMethodUNKNOWN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_0_expect0(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_9_expect9(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_a_expecta(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_f_expectf(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_A_expectA(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_F_expectF(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_NullChar_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_Space_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeDigit_Percent_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_00_expect00(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_0A_expect0A(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_A0_expectA0(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_FF_expectFF(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_0Space_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_0Percent_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_0Minus_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_0h_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_0x_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_Space0_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_Percent0_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_Minus0_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_h0_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_x0_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_23Space_expect23(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_EmptyString_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_SingleCharString_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPPercentCode::decodeHex_NullString_expectError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_plainURL_expectUnmodifiedURL(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithPercentCode_expectPercentCodeDecoded(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithPlus_expectPlusSubstitutedWithSpace(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithInvalidPercentCode_expectPercentCodeNotDecodedUnmodifiedURL(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithBrokenPercentCodeAtTheEnd_expectUnmodifiedURL(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithPercentCharAtTheEnd_expectUnmodifiedURL(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_emptyString_expectEmptyString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_zeroLength_expectUnmodifiedURL(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_lengthSmallerThanActual_expectTruncatedString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestURL::decode_NULLString_expectMethodDoesNotCrash(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestFakeStream::beginIn_inputInit_expectCorrectInputInBuffer(), file fakestream.cpp
    PASSED
    Test function started: static void TestFakeStream::readIn_inputRead_expectCorrectCharsRead(), file fakestream.cpp
    PASSED
    Test function started: static void TestFakeStream::readIn_inputReadUnavailable_expectUnavailable(), file fakestream.cpp
    PASSED
    Test function started: static void TestFakeStream::writeOut_printString_expectTheSameStringInTheOutBuffer(), file fakestream.cpp
    PASSED
    Test function started: static void TestFakeStream::beginInOut_inputInit_expectCorrectInputInBuffer(), file fakestream.cpp
    PASSED
    Test function started: static void TestFakeStream::readInOut_inputRead_expectCorrectCharsRead(), file fakestream.cpp
    PASSED
    Test function started: static void TestFakeStream::readInOut_inputReadUnavailable_expectUnavailable(), file fakestream.cpp
    PASSED
    Test function started: static void TestFakeStream::writeInOut_printString_expectTheSameStringInTheOutBuffer(), file fakestream.cpp
    PASSED
    Test function started: static void TestParserInputStream::read_StringWithoutCRLF_expectUnmodifiedString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NotAvailable_expectNotAvailable(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_RN_expectN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NR_expectN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NN_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_RR_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_RNR_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NRN_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NRRN_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NRNR_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_RNZ_expectNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NRZ_expectNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NNZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_RRZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_RNRZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NRNZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NRRNZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::read_NRNRZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_StringWithoutCRLF_expectUnmodifiedString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NotAvailable_expectNotAvailable(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_RN_expectN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NR_expectN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NN_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_RR_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_RNR_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NRN_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NRRN_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NRNR_expectNN(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_RNZ_expectNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NRZ_expectNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NNZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_RRZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_RNRZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NRNZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NRRNZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::peek_NRNRZ_expectNNZ(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_readUntilCharacter_expectPartOfInputString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_readUntilFirstCharacter_expectEmptyString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_characterNotFound_expectSameString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_charNotFoundBecauseCaseSensitive_expectSameString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_stringDoesNotFitInBuffer_expectTruncatedString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_notAvailable_expectEmptyString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_charsToFindNULL_expectEmptyString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_zeroLengthBuffer_expectEmptyString(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_readBufferNULL_expectNoCrash(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_clientNULL_expectNoCrash(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::readUntil_clientReset_expectNoCrash(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::skipUntil_skipUntilCharacter_expectInputCharsSkipped(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::skipUntil_characterNotFound_expectInputStreamFullySkipped(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::skipUntil_charNotFoundBecauseCaseSensitive_expectInputStreamFullySkipped(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::skipUntil_notAvailable_expectMethodDoesNothing(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::skipUntil_charsToFindNULL_expectMethodDoesNothing(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::skipUntil_clientNULL_expectNoCrash(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestParserInputStream::skipUntil_clientReset_expectNoCrash(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestBufferedPrint::write_stringShorterThanBuffer_expectStringSentAfterBufferedPrintOutOfScope(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestBufferedPrint::write_stringLongerThanBuffer_expectStringSentAfterBufferIsFull(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::begin_validateInternals_expectTrue(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::begin_beginNotCalled_expectNoClientError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_parseRequest_expectCorrespondingRequestParts(), file test_http_webcc.ino
    PASSED
    
    ---------------- TESTING FINISHED --------------- 

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
