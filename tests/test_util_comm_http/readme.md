# Purpose of the tests

To confirm that any of the recent modifications of utility functions and/or classes in file util_comm, namespace util::http (HTTP protocol helpers) did not introduce anomalies to function and/or class performance

# Running the tests

Copy here all necessary files from main project directory. The files to be copied are as follows:

* util_comm.cpp
* util_comm.h

Copy here all necessary files from tests/common directory. The files to be copied are as follows:

* test.h

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:


    ---------------- TESTING STARTED ----------------
    
    Test function started: static void TestHTTPRequestHelper::getMethod_OPTIONS_expectHTTPRequestMethodOPTIONS(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_GET_expectHTTPRequestMethodGET(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_HEAD_expectHTTPRequestMethodHEAD(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_POST_expectHTTPRequestMethodPOST(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_PUT_expectHTTPRequestMethodPUT(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_DELETE_expectHTTPRequestMethodDELETE(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_TRACE_expectHTTPRequestMethodTRACE(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_CONNECT_expectHTTPRequestMethodCONNECT(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_Unknown_expectHTTPRequestMethodUNKNOWN(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPRequestHelper::getMethod_CaseSensitive_expectHTTPRequestMethodUNKNOWN(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_0_expect0(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_9_expect9(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_a_expecta(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_f_expectf(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_A_expectA(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_F_expectF(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_NullChar_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_Space_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeDigit_Percent_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_00_expect00(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_0A_expect0A(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_A0_expectA0(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_FF_expectFF(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_0Space_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_0Percent_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_0Minus_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_0h_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_0x_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_Space0_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_Percent0_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_Minus0_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_h0_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_x0_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_23Space_expect23(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_EmptyString_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_SingleCharString_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestHTTPHexCode::decodeHex_NullString_expectError(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_plainURL_expectUnmodifiedURL(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithPercentCode_expectPercentCodeDecoded(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithPlus_expectPlusSubstitutedWithSpace(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithInvalidPercentCode_expectPercentCodeNotDecodedUnmodifiedURL(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithBrokenPercentCodeAtTheEnd_expectUnmodifiedURL(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_URLwithPercentCharAtTheEnd_expectUnmodifiedURL(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_emptyString_expectEmptyString(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_zeroLength_expectUnmodifiedURL(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_lengthSmallerThanActual_expectTruncatedString(), file test_util_comm_http.ino
    PASSED
    Test function started: static void TestURL::decode_NULLString_expectMethodDoesNotCrash(), file test_util_comm_http.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 47
    Test functions failed: 0
    
# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
