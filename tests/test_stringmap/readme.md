# Running the tests

Copy here all necessary files from main project directory.

The files to be copied are as follows:

* stringmap.cpp
* stringmap.h 

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: static void testQuickStringMap::getDefaultKey_ExpectCorrectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findChar_Found_ExpectValidKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findChar_NotFound_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::testQuickStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findChar_CaseSensitive_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findChar_EmptyString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findChar_NULLString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::isProgmem_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::isProgmem_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::isProgmem_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findStringMapKey_Found_ExpectValidString(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findStringMapKey_NotFound_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::findStringMapKey_FindDefault_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsChar_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsChar_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsChar_CaseSensitive_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsChar_EmptyString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsChar_NULLString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsStringMapKey_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsStringMapKey_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testQuickStringMap::constainsStringMapKey_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testStringMapIteratorWithQuickStringMap::first_expectFirstItem(), file test_stringmap.ino
    PASSED
    Test function started: static void testStringMapIteratorWithQuickStringMap::next_expectSecondItem(), file test_stringmap.ino
    PASSED
    Test function started: static void testStringMapIteratorWithQuickStringMap::isDone_expectCorrectNumberOfIterations(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::getDefaultKey_ExpectCorrectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findChar_Found_ExpectValidKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findChar_NotFound_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::testProgmemStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findChar_CaseSensitive_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findChar_EmptyString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findChar_NULLString_ExpectDefaultKey(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::isProgmem_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::isProgmem_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::isProgmem_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findStringMapKey_Found_ExpectValidString(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findStringMapKey_NotFound_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::findStringMapKey_FindDefault_ExpectNULL(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsChar_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsChar_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsChar_CaseSensitive_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsChar_EmptyString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsChar_NULLString_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsStringMapKey_Found_ExpectTrue(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsStringMapKey_NotFound_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testProgmemStringMap::constainsStringMapKey_DefaultKey_ExpectFalse(), file test_stringmap.ino
    PASSED
    Test function started: static void testStringMapIteratorWithProgmemStringMap::first_expectFirstItem(), file test_stringmap.ino
    PASSED
    Test function started: static void testStringMapIteratorWithProgmemStringMap::next_expectSecondItem(), file test_stringmap.ino
    PASSED
    Test function started: static void testStringMapIteratorWithProgmemStringMap::isDone_expectCorrectNumberOfIterations(), file test_stringmap.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
