# Purpose of the tests

To confirm that any of the recent modifications of utility functions and/or classes in file util_data, namespace util::checksum (checksum computation) did not introduce anomalies to function and/or class performance

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
    
    Test function started: static void TestCrc16::crc16_referenceData_expectCorrectChecksum(), file test_util_data_checksum.ino
    PASSED
    Test function started: static void TestCrc16::crc16_bufferNull_expectZero(), file test_util_data_checksum.ino
    PASSED
    Test function started: static void TestCrc16::crc16_bufferSizeZero_expectZero(), file test_util_data_checksum.ino
    PASSED
    Performance test function started: static void TestCrc16::testPerformanceCrc16(), file test_util_data_checksum.ino
    millis: 440 micros: 439970
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 3
    Test functions failed: 0

Note: performance figures are for reference only and might vary depending on actual hardware setup.

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
