# Purpose of the tests

To confirm that any of the recent modifications of utility functions and/or classes in file util_data, namespace util::arrays (custom data structures) did not introduce anomalies to function and/or class performance

# Running the tests

Copy here all necessary files from main project directory. The files to be copied are as follows:

* util_data.h
* util_data.cpp

Copy here all necessary files from tests/common directory. The files to be copied are as follows:

* test.h

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: void RingBufferTester<T, testBufferSize>::test_testValues() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_correctInit_expectTrue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_notInitialised_expectFalse() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_singleValue_expectCorrectValue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_multipleValues_expectSameValues() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_fullArray_expectOldestValuesOverwritten() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_notInitialised_expectNoCrash() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_emptyRingBuffer_expectZero() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_singleValue_expectOne() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_fullRingBuffer_expectBufferSize() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_notInitialised_expectZero() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_singleValue_expectSameValue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrap_expectOldestValuesOverwritten() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrongIndex_expectDefaultValue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_notInitialised_expectDefaultValue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_singleValue_expectOldestValueRemoved() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_multipleValues_expectValuesRemoved() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_wrap_expectOlderValuesRemoved() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_emptyRingBuffer_expectNoActionPerformed() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_notInitialised_expectNoActionPerformed() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_emptyRingBuffer_expectTrue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_nonEmptyRingBuffer_expectFalse() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_notInitialised_expectTrue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_fullRingBuffer_expectTrue() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_nonFullRingBuffer_expectFalse() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_notInitialised_expectFalse() [with T = char; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = char; unsigned int testBufferSize = 256u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 5 micros: 4466
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = char; unsigned int testBufferSize = 256u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3851
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = char; unsigned int testBufferSize = 1024u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 4318
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = char; unsigned int testBufferSize = 1024u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3845
    Test function started: void RingBufferTester<T, testBufferSize>::test_testValues() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_correctInit_expectTrue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_notInitialised_expectFalse() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_singleValue_expectCorrectValue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_multipleValues_expectSameValues() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_fullArray_expectOldestValuesOverwritten() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_notInitialised_expectNoCrash() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_emptyRingBuffer_expectZero() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_singleValue_expectOne() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_fullRingBuffer_expectBufferSize() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_notInitialised_expectZero() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_singleValue_expectSameValue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrap_expectOldestValuesOverwritten() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrongIndex_expectDefaultValue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_notInitialised_expectDefaultValue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_singleValue_expectOldestValueRemoved() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_multipleValues_expectValuesRemoved() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_wrap_expectOlderValuesRemoved() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_emptyRingBuffer_expectNoActionPerformed() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_notInitialised_expectNoActionPerformed() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_emptyRingBuffer_expectTrue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_nonEmptyRingBuffer_expectFalse() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_notInitialised_expectTrue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_fullRingBuffer_expectTrue() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_nonFullRingBuffer_expectFalse() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_notInitialised_expectFalse() [with T = int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = int; unsigned int testBufferSize = 128u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 5 micros: 4435
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = int; unsigned int testBufferSize = 128u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3899
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = int; unsigned int testBufferSize = 400u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 4489
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = int; unsigned int testBufferSize = 400u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3899
    Test function started: void RingBufferTester<T, testBufferSize>::test_testValues() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_correctInit_expectTrue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_notInitialised_expectFalse() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_singleValue_expectCorrectValue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_multipleValues_expectSameValues() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_fullArray_expectOldestValuesOverwritten() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_notInitialised_expectNoCrash() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_emptyRingBuffer_expectZero() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_singleValue_expectOne() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_fullRingBuffer_expectBufferSize() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_notInitialised_expectZero() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_singleValue_expectSameValue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrap_expectOldestValuesOverwritten() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrongIndex_expectDefaultValue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_notInitialised_expectDefaultValue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_singleValue_expectOldestValueRemoved() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_multipleValues_expectValuesRemoved() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_wrap_expectOlderValuesRemoved() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_emptyRingBuffer_expectNoActionPerformed() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_notInitialised_expectNoActionPerformed() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_emptyRingBuffer_expectTrue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_nonEmptyRingBuffer_expectFalse() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_notInitialised_expectTrue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_fullRingBuffer_expectTrue() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_nonFullRingBuffer_expectFalse() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_notInitialised_expectFalse() [with T = long int; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = long int; unsigned int testBufferSize = 64u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 4243
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = long int; unsigned int testBufferSize = 64u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3849
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = long int; unsigned int testBufferSize = 320u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 4194
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = long int; unsigned int testBufferSize = 320u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3848
    Test function started: void RingBufferTester<T, testBufferSize>::test_testValues() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_correctInit_expectTrue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_notInitialised_expectFalse() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_singleValue_expectCorrectValue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_multipleValues_expectSameValues() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_fullArray_expectOldestValuesOverwritten() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_notInitialised_expectNoCrash() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_emptyRingBuffer_expectZero() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_singleValue_expectOne() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_fullRingBuffer_expectBufferSize() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_notInitialised_expectZero() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_singleValue_expectSameValue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrap_expectOldestValuesOverwritten() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrongIndex_expectDefaultValue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_notInitialised_expectDefaultValue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_singleValue_expectOldestValueRemoved() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_multipleValues_expectValuesRemoved() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_wrap_expectOlderValuesRemoved() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_emptyRingBuffer_expectNoActionPerformed() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_notInitialised_expectNoActionPerformed() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_emptyRingBuffer_expectTrue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_nonEmptyRingBuffer_expectFalse() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_notInitialised_expectTrue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_fullRingBuffer_expectTrue() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_nonFullRingBuffer_expectFalse() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_notInitialised_expectFalse() [with T = float; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = float; unsigned int testBufferSize = 64u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 5 micros: 4554
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = float; unsigned int testBufferSize = 64u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3849
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = float; unsigned int testBufferSize = 320u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 4505
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = float; unsigned int testBufferSize = 320u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 4 micros: 3849
    Test function started: void RingBufferTester<T, testBufferSize>::test_testValues() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_correctInit_expectTrue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_notInitialised_expectFalse() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_singleValue_expectCorrectValue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_multipleValues_expectSameValues() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_fullArray_expectOldestValuesOverwritten() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_notInitialised_expectNoCrash() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_emptyRingBuffer_expectZero() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_singleValue_expectOne() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_fullRingBuffer_expectBufferSize() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_notInitialised_expectZero() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_singleValue_expectSameValue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrap_expectOldestValuesOverwritten() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrongIndex_expectDefaultValue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_notInitialised_expectDefaultValue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_singleValue_expectOldestValueRemoved() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_multipleValues_expectValuesRemoved() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_wrap_expectOlderValuesRemoved() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_emptyRingBuffer_expectNoActionPerformed() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_notInitialised_expectNoActionPerformed() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_emptyRingBuffer_expectTrue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_nonEmptyRingBuffer_expectFalse() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_notInitialised_expectTrue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_fullRingBuffer_expectTrue() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_nonFullRingBuffer_expectFalse() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_notInitialised_expectFalse() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 32u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 5 micros: 4659
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 32u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 6 micros: 5538
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 128u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 5 micros: 4589
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = TestRingBuffer::TestStruct; unsigned int testBufferSize = 128u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 6 micros: 5539
    Test function started: void RingBufferTester<T, testBufferSize>::test_testValues() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_correctInit_expectTrue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::validate_notInitialised_expectFalse() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_singleValue_expectCorrectValue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_multipleValues_expectSameValues() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_fullArray_expectOldestValuesOverwritten() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::push_notInitialised_expectNoCrash() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_emptyRingBuffer_expectZero() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_singleValue_expectOne() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_fullRingBuffer_expectBufferSize() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::count_notInitialised_expectZero() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_singleValue_expectSameValue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrap_expectOldestValuesOverwritten() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_wrongIndex_expectDefaultValue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::subscriptOperator_notInitialised_expectDefaultValue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_singleValue_expectOldestValueRemoved() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_multipleValues_expectValuesRemoved() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_wrap_expectOlderValuesRemoved() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_emptyRingBuffer_expectNoActionPerformed() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::pop_notInitialised_expectNoActionPerformed() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_emptyRingBuffer_expectTrue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_nonEmptyRingBuffer_expectFalse() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::empty_notInitialised_expectTrue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_fullRingBuffer_expectTrue() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_nonFullRingBuffer_expectFalse() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: void RingBufferTester<T, testBufferSize>::full_notInitialised_expectFalse() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 4u], file test_util.ino
    PASSED
    Test function started: static void TestRingBuffer::checkClassConstrDestr_bufferNotFull_expectInstancesEqualsToPushCount(), file test_util.ino
    PASSED
    Test function started: static void TestRingBuffer::checkClassConstrDestr_pushPopSequence_expectInstancesAsDiffrenceBetweenPushAndPopCount(), file test_util.ino
    PASSED
    Test function started: static void TestRingBuffer::checkClassConstrDestr_bufferFull_expectInstancesEqualsToBufferSize(), file test_util.ino
    PASSED
    Test function started: static void TestRingBuffer::checkClassConstrDestr_bufferDestructor_expectInstancesAsBefore(), file test_util.ino
    PASSED
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 32u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 10 micros: 9620
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 32u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 5 micros: 4869
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_push() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 128u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 9 micros: 9572
    Performance test function started: static void RingBufferPerformanceTester<T, testBufferSize, numberOfOperations>::testPerformance_subscriptOperator() [with T = TestRingBuffer::TestClass; unsigned int testBufferSize = 128u; unsigned int numberOfOperations = 4096u], file test_util.ino
    millis: 5 micros: 4872
    Test function started: static void TestCstrRingBuffer::validate_correctInit_expectTrue(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::validate_bufferNull_expectFalse(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::validate_bufferSizeZero_expectFalse(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::push_get_multipleCStrings_expectCStringsStored(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::push_get_multipleCStringsBufferFull_expectOldestCStringOverwritten(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::push_get_cstringFitsExactlyIntoFreeSpace_expectCorrectCString(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::push_cstringTooLarge_expectNoAction(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::push_notInitialised_expectNoCrash(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::get_bufferSmallerThanStoredString_expectStringTruncated(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::get_bufferSmallerThanStoredFirstPart_expectStringTruncated(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::get_bufferSmallerThanStoredSecondPart_expectStringTruncated(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::get_emptybuffer_expectZeroLengthString(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::get_notInitialised_expectNoCrash(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::pop_noRingBufferRollover_expectOldestCStringRemoved(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::pop_ringBufferRollover_expectRolloverCStringRemovedCorrectly(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::pop_cstringFitsExactlyIntoFreeSpace_expectCStringRemovedCorrectly(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::pop_emptyRingBuffer_expectNoActionPerformed(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::pop_notInitialised_expectNoActionPerformed(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::count_emptyRingBuffer_expectZero(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::count_singleCString_expectOne(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::count_multipleCStrings_expectCorrectNumber(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::count_notInitialised_expectZero(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::full_fullRingBuffer_expectTrue(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::full_nonFullRingBuffer_expectFalse(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::full_cstringFitsExactlyIntoFreeSpace_expectFalse(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::full_zeroLengthInputStringFullBuffer_expectFalse(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::full_notInitialised_expectFalse(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::empty_emptyRingBuffer_expectTrue(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::empty_nonEmptyRingBuffer_expectFalse(), file test_util.ino
    PASSED
    Test function started: static void TestCstrRingBuffer::empty_notInitialised_expectTrue(), file test_util.ino
    PASSED
    Performance test function started: static void TestCstrRingBuffer::test_performance_push_shortStrings(), file test_util.ino
    millis: 20 micros: 20409
    Performance test function started: static void TestCstrRingBuffer::test_performance_push_longStrings(), file test_util.ino
    millis: 90 micros: 89867
    Performance test function started: static void TestCstrRingBuffer::test_performance_get_shortStrings(), file test_util.ino
    millis: 817 micros: 817262
    Performance test function started: static void TestCstrRingBuffer::test_performance_get_longStrings(), file test_util.ino
    millis: 263 micros: 263483
    Test function started: static void TestPrintToBuffer::writeSingle_writeSingleChar_expectCorrectCharInBuffer(), file test_util.ino
    PASSED
    Test function started: static void TestPrintToBuffer::writeSingle_bufferOverflow_expectExtraCharsIgnored(), file test_util.ino
    PASSED
    Test function started: static void TestPrintToBuffer::writeSingle_writeSingleCharToNonEmptyBuffer_expectSingleCharInBuffer(), file test_util.ino
    PASSED
    Test function started: static void TestPrintToBuffer::writeBuffer_writeCString_expectCorrectCStringWritten(), file test_util.ino
    PASSED
    Test function started: static void TestPrintToBuffer::writeBuffer_bufferOverflow_expectCStringTruncated(), file test_util.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 195
    Test functions failed: 0
        

Note: performance figures are for reference only and might vary depending on actual hardware setup.

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
