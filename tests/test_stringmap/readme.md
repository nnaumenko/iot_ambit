# Running the tests

Copy here all necessary files from main project directory.

The files to be copied are as follows:

* stringmap.cpp
* stringmap.h 

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: static void TestQuickStringMap::getDefaultKey_ExpectCorrectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findChar_Found_ExpectValidKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findChar_NotFound_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::testQuickStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findChar_CaseSensitive_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findChar_EmptyString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findChar_NULLString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::isProgmem_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::isProgmem_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::isProgmem_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findStringMapKey_Found_ExpectValidString(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findStringMapKey_NotFound_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::findStringMapKey_FindDefault_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsChar_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsChar_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsChar_CaseSensitive_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsChar_EmptyString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsChar_NULLString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsStringMapKey_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsStringMapKey_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestQuickStringMap::constainsStringMapKey_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapIteratorWithQuickStringMap::first_expectFirstItem(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapIteratorWithQuickStringMap::next_expectSecondItem(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapIteratorWithQuickStringMap::isDone_expectCorrectNumberOfIterations(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::getDefaultKey_ExpectCorrectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findChar_Found_ExpectValidKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findChar_NotFound_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::testProgmemStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findChar_CaseSensitive_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findChar_EmptyString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findChar_NULLString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::isProgmem_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::isProgmem_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::isProgmem_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findStringMapKey_Found_ExpectValidString(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findStringMapKey_NotFound_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::findStringMapKey_FindDefault_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsChar_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsChar_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsChar_CaseSensitive_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsChar_EmptyString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsChar_NULLString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsStringMapKey_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsStringMapKey_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestProgmemStringMap::constainsStringMapKey_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapIteratorWithProgmemStringMap::first_expectFirstItem(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapIteratorWithProgmemStringMap::next_expectSecondItem(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapIteratorWithProgmemStringMap::isDone_expectCorrectNumberOfIterations(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::count_Expect3(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findChar_Found_ExpectValidKey(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findChar_NotFound_ExpectNotFound(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findChar_CaseSensitive_ExpectNotFound(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findChar_EmptyStrings_ExpectNotFound(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findChar_NULLStrings_ExpectNotFound(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findInt_Found_ExpectCorrespondingString(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findInt_NotFound_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findInt_EmptyStrings_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void TestStringMapLite::findInt_NULLStrings_ExpectNULL(), file test_stringmap.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------
# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
