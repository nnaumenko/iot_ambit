# Running the tests

Copy here all necessary files from main project directory.

The files to be copied are as follows:

* diag.h
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
    Test function started: static void TestBufferedPrint::write_stringShorterThanBuffer_expectStringSentAfterBufferedPrintOutOfScope(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestBufferedPrint::write_stringLongerThanBuffer_expectStringSentAfterBufferIsFull(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::begin_validateInternals_expectTrue(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::begin_beginNotCalled_expectInternalError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalGetRequestWithNoFields_expectCorrespondingRequestParts(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalGetRequestWithNoFieldsAndWithSpecialChars_expectCorrespondingRequestParts(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalGetRequest_expectCorrespondingRequestParts(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalPostRequest_expectCorrespondingRequestParts(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalPostRequestWithUrlQuery_expectCorrespondingRequestParts(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenMethod_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenMethodAndTrailingSpace_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithMethodOnly_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithMethodAndTrailingSpace_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongMethod_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_NoMethod_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyMethod_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPath_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPathAndTrailingSpace_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithMethodAndPathOnly_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongPath_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPathWithQuery_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPathWithoutQuery_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery1_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery2_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery3_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery4_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery5_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery6_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery7_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery8_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery1NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery2NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery3NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery4NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery5NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery6NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery7NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery8NoVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery1FullHeader_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery3FullHeader_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery5FullHeader_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery6FullHeader_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery7FullHeader_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongURLQueryName_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyURLQueryName_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongURLQueryValue_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyURLQueryValue_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenVersion_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongVersion_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyVersion_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithoutTrailingCRLF_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithOneTrailingCRLF_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenFieldName_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_fieldWithNameOnly_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_emptyFieldName_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongFieldName_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenFieldWithSeparator_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenFieldValue_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_emptyFieldValue_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongFieldValue_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_fieldFollowedBySingleCRLF_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery1_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery2_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery3_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery4_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery5_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery6_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery7_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery2FollowedByCRLF_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery4FollowedByCRLF_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery6FollowedByCRLF_expectStructureError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongPOSTQueryName_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPOSTQueryName_expectNoError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongPOSTQueryValue_expectPartTooLongError(), file test_http_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPOSTQueryValue_expectNoError(), file test_http_webcc.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
