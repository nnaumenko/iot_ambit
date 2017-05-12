# Purpose of the tests

To confirm that any of the recent modification of module Diag (diagnostic log) and its components did not introduce anomalies to module's performance

# Running the tests

Copy here all necessary files from main project directory. The files to be copied are as follows:

* diag.h
* diag.cpp
* module.h
* util_comm.h
* util_comm.cpp
* util_data.h
* util_data.cpp

Copy here all necessary files from tests/common directory. The files to be copied are as follows:

* test.h
* fakestream.h
* fakestream.cpp


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
    Test function started: static void TestDiagLog::log_disablePrintOutput_expectNoCrash(), file test_diaglog.ino
    PASSED
    Test function started: static void TestDiagLog::log_messageSinglePart_expectTheSameString(), file test_diaglog.ino
    PASSED
    Test function started: static void TestDiagLog::log_messageMultipleParts_expectConcatenatedParts(), file test_diaglog.ino
    PASSED
    Test function started: static void TestDiagLog::log_messageSeverities_expectCorrectSeverityTexts(), file test_diaglog.ino
    PASSED
    Test function started: static void TestDiagLog::log_twoSequentialMessages_expectMessageNumberIncreases(), file test_diaglog.ino
    PASSED
    Test function started: static void TestDiagLog::log_severityFilter_expectNoFilteredMessages(), file test_diaglog.ino
    PASSED
    Test function started: static void TestDiagLog::setSeverityFilter_severityEmergencyAlertCritical_expectCritical(), file test_diaglog.ino
    PASSED
    Test function started: static void TestDiagLog::setSeverityFilter_severityErrorWarningNoticeInformationalDebug_expectSameSeverity(), file test_diaglog.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 16
    Test functions failed: 0 

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
