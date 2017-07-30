# Purpose of the tests

To confirm that any of the recent modifications of classes derived from class TemplateFilter in file util_data, namespace util::dsp (digital signal processing and filtering) did not introduce anomalies to classes' performance

# Test data

Refer to testdata.ods for test data sets and related calculactions (Free LibreOffice Calc is reqired to open an .ods file).

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
    
    Test function started: static void TestAbstractFilter::filter_passInputValue_expectValuePassedToFilter(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestAbstractFilter::filter_passMultipleValues_expectValuesPassedToFilter(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestAbstractFilter::filter_getOutputValue_expectOutputValueReturned(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestAbstractFilter::getDeltaTime_firstCall_expectZero(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestAbstractFilter::getDeltaTime_secondCall_expectTimeDifference(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestAbstractFilter::filter_initErrors_expectFilterProcessNotCalled(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestAbstractFilter::filter_tooFewInput_expectFilterProcessNotCalled(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestAbstractFilter::filter_tooManyInput_expectWarningGeneratedIfFilterProcessReturnsOK(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestMovingAverage::testPulseStepResponsesAvg2(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestMovingAverage::testPulseStepResponsesAvg16(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestMovingAverage::testFrequenciesResponsesAvg2(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestMovingAverage::testFrequenciesResponsesAvg16(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestMovingAverage::testSumOverflow(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestLowPass::testPulseStepResponses20Hz(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestLowPass::testFrequenciesResponses20Hz(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestLowPass::testIncorrectFcValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestLinearScale::test_InitWithFactorAndOffset_expectCorrectValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestLinearScale::test_InitWithTwoPoints_expectCorrectValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestLinearScale::test_InitWithOffset_expectCorrectValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestLinearScale::test_IncorrectInitData_expectZero(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestSquareScale::test_InitWithThreePoints_expectCorrectValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestSquareScale::test_InitWithABC_expectCorrectValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestSquareScale::test_VertexWarning_expectWarningWhenParabolaVertexIsInRange(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestSquareScale::test_IncorrectInitData_expectZero(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestSplineScale::test_InitWithThreePoints_expectCorrectValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestSplineScale::test_InitWithUnsortedPoints_expectCorrectValues(), file test_util_data_dsp_filters.ino
    PASSED
    Test function started: static void TestSplineScale::test_IncorrectInitData_expectZero(), file test_util_data_dsp_filters.ino
    PASSED
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 27
    Test functions failed: 0 

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino


# Filter test data

Please refer to testdata.ods for a sets of filter input data and expected output data
