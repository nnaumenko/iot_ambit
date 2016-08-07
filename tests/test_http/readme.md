# Running the tests

Copy here all necessary files from main project directory.

The files to be copied are as follows:

* http.cpp
* http.h 

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: static void testHTTPPercentCode::decodeDigit_0_expect0(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_9_expect9(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_a_expecta(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_f_expectf(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_A_expectA(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_F_expectF(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_NullChar_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_Space_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeDigit_Percent_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_00_expect00(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_0A_expect0A(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_A0_expectA0(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_FF_expectFF(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_0Space_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_0Percent_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_0Minus_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_0h_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_0x_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_Space0_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_Percent0_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_Minus0_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_h0_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_x0_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_23Space_expect23(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_EmptyString_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_SingleCharString_expectError(), file test_http.ino
    PASSED
    Test function started: static void testHTTPPercentCode::decodeHex_NullString_expectError(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_plainURL_expectUnmodifiedURL(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_URLwithPercentCode_expectPercentCodeDecoded(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_URLwithPlus_expectPlusSubstitutedWithSpace(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_URLwithInvalidPercentCode_expectPercentCodeNotDecodedUnmodifiedURL(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_URLwithBrokenPercentCodeAtTheEnd_expectUnmodifiedURL(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_URLwithPercentCharAtTheEnd_expectUnmodifiedURL(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_emptyString_expectEmptyString(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_zeroLength_expectUnmodifiedURL(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_lengthSmallerThanActual_expectTruncatedString(), file test_http.ino
    PASSED
    Test function started: static void testURL::decode_NULLString_expectMethodDoesNotCrash(), file test_http.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino

