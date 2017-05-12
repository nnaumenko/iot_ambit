# Purpose of the tests

To confirm that any of the recent modifications of utility functions and/or classes in file util_comm, namespace util::json (JSON generation) did not introduce anomalies to function and/or class performance

# Running the tests

Copy here all necessary files from main project directory. The files to be copied are as follows:

* util_comm.cpp
* util_comm.h

Copy here all necessary files from tests/common directory. The files to be copied are as follows:

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
    Test function started: static void TestJSONOutput::value_addToObject_expectCorrectValuesAdded(), file test_util_comm_json.ino
    PASSED
    Test function started: static void TestJSONOutput::value_addToArray_expectCorrectValuesAddedNamesIgnored(), file test_util_comm_json.ino
    PASSED
    Test function started: static void TestJSONOutput::structure_nesting_expectCorrectJSON(), file test_util_comm_json.ino
    PASSED

    ---------------- TESTING FINISHED ---------------
    Test functions passed: 11
    Test functions failed: 0

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
