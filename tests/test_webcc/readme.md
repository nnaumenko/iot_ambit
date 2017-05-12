# Purpose of the tests

To confirm that any of the recent modification of module WebCC (webserver infrastructure) and its components did not introduce anomalies to module's performance

# Running the tests

Copy here all necessary files from main project directory. The files to be copied are as follows:

* module.h
* util_comm.cpp
* util_comm.h
* webcc.cpp
* webcc.h

Copy here all necessary files from tests/common directory. The files to be copied are as follows:

* fakediag.cpp
* fakediag.h
* fakestream.cpp
* fakestream.h
* test.h

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:


    ---------------- TESTING STARTED ----------------
    
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
    Test function started: static void TestBufferedPrint::write_stringShorterThanBuffer_expectStringSentAfterBufferedPrintOutOfScope(), file test_webcc.ino
    PASSED
    Test function started: static void TestBufferedPrint::write_stringLongerThanBuffer_expectStringSentAfterBufferIsFull(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::begin_validateInternals_expectTrue(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::begin_beginNotCalled_expectInternalError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalGetRequestWithNoFields_expectCorrespondingRequestParts(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalGetRequestWithNoFieldsAndWithSpecialChars_expectCorrespondingRequestParts(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalGetRequest_expectCorrespondingRequestParts(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalPostRequest_expectCorrespondingRequestParts(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_normalPostRequestWithUrlQuery_expectCorrespondingRequestParts(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenMethod_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenMethodAndTrailingSpace_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithMethodOnly_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithMethodAndTrailingSpace_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongMethod_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_NoMethod_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyMethod_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPath_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPathAndTrailingSpace_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithMethodAndPathOnly_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongPath_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPathWithQuery_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPathWithoutQuery_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery1_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery2_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery3_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery4_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery5_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery6_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery7_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery8_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery1NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery2NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery3NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery4NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery5NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery6NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery7NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery8NoVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery1FullHeader_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery3FullHeader_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery5FullHeader_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery6FullHeader_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenURLQuery7FullHeader_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongURLQueryName_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyURLQueryName_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongURLQueryValue_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyURLQueryValue_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenVersion_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongVersion_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyVersion_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithoutTrailingCRLF_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_headerWithOneTrailingCRLF_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenFieldName_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_fieldWithNameOnly_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_emptyFieldName_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongFieldName_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenFieldWithSeparator_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenFieldValue_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_emptyFieldValue_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongFieldValue_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_fieldFollowedBySingleCRLF_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery1_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery2_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery3_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery4_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery5_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery6_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery7_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery2FollowedByCRLF_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery4FollowedByCRLF_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_brokenPOSTQuery6FollowedByCRLF_expectStructureError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongPOSTQueryName_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPOSTQueryName_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_tooLongPOSTQueryValue_expectPartTooLongError(), file test_webcc.ino
    PASSED
    Test function started: static void TestHTTPReqParserStateMachine::parse_EmptyPOSTQueryValue_expectNoError(), file test_webcc.ino
    PASSED
    Test function started: static void TestFakeDiag::log_variousParameters_expectCorrectParametersStored(), file fakediag.cpp
    PASSED
    Test function started: static void TestFakeDiag::log_multipleEntries_expectCorrectEntriesStored(), file fakediag.cpp
    PASSED
    Test function started: static void TestFakeDiag::log_inputStringExceedsMaxLength_expectStoredStringTruncated(), file fakediag.cpp
    PASSED
    Test function started: static void TestFakeDiag::log_inputTooManyStrings_expectMaxStringsPerLimit(), file fakediag.cpp
    PASSED
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 87
    Test functions failed: 0

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
