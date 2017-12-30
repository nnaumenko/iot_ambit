# Purpose of the tests

To confirm that any of the recent modifications of functions and classes in file util_data, namespace util did not introduce anomalies to their performance

# Running the tests

Copy here all necessary files from main project directory. The files to be copied are as follows:

* util_data.h
* util_data.cpp

Copy here all necessary files from tests/common directory. The files to be copied are as follows:

* fakestream.cpp
* fakestream.h
* test.h

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: static void TestRef<T>::constructorAccessor_initRamPointer_expectSameRamPointer() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::constructorAccessor_initProgmemPointer_expectSameProgmemPointer() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::constructorAccessor_initNullptrPointer_expectNullptrPointer() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nullptrRam_expectFalse() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nullptrProgmem_expectFalse() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nonNullptrRam_expectTrue() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nonNullptrProgmem_expectTrue() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::assignment_initRamPointer_expectSameRamPointer() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::assignment_initProgmemPointer_expectSameProgmemPointer() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::assignment_initNullptrPointer_expectNullptrPointer() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareSamePointerSameMemoryAreas_expectTrue() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareSamePointerDifferentMemoryAreas_expectFalse() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareDifferentPointerSameMemoryAreas_expectFalse() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareDifferentPointerDifferentMemoryAreas_expectFalse() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrsInRam_expectTrue() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrsInProgmem_expectTrue() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrsInDifferentMemoryAreas_expectTrue() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrToPointer_expectFalse() [with T = int], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::constructorAccessor_initRamPointer_expectSameRamPointer() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::constructorAccessor_initProgmemPointer_expectSameProgmemPointer() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::constructorAccessor_initNullptrPointer_expectNullptrPointer() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nullptrRam_expectFalse() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nullptrProgmem_expectFalse() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nonNullptrRam_expectTrue() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::operatorBool_nonNullptrProgmem_expectTrue() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::assignment_initRamPointer_expectSameRamPointer() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::assignment_initProgmemPointer_expectSameProgmemPointer() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::assignment_initNullptrPointer_expectNullptrPointer() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareSamePointerSameMemoryAreas_expectTrue() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareSamePointerDifferentMemoryAreas_expectFalse() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareDifferentPointerSameMemoryAreas_expectFalse() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareDifferentPointerDifferentMemoryAreas_expectFalse() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrsInRam_expectTrue() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrsInProgmem_expectTrue() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrsInDifferentMemoryAreas_expectTrue() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestRef<T>::equalOperator_compareNullptrToPointer_expectFalse() [with T = char], file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_constructor_initWithConstCharPointer_expectReferenceToCStringInRam(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_constructor_initWithFlashStringHelper_expectReferenceToCStringInProgmem(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_CStringInRam_expectCStringCopied(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_CStringInProgmem_expectCStringCopied(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_tooLongCStringInRam_expectCStringTruncated(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_tooLongCStringInProgmem_expectCStringTruncated(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_nullptrInRam_expectEmptyBuffer(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_nullptrInProgmem_expectEmptyBuffer(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_emptyString_expectEmptyBuffer(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_get_bufferSizeZero_expectBufferNotModified(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_print_printCStringInRam_expectCStringPrintedCorrectly(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_print_printCStringInProgmem_expectCStringPrintedCorrectly(), file test_util_data.ino
    PASSED
    Test function started: static void TestStrRef::strRef_print_printNullptr_expectNothingPrinted(), file test_util_data.ino
    PASSED
    sizeof(util::StrRef) 8
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 49
    Test functions failed: 0 

Note: class sizes are for reference only.

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
