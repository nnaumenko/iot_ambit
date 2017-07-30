# Purpose of the tests

To confirm that any of the recent modifications of class FixedPoint in file util_data, namespace util::dsp (digital signal processing) did not introduce anomalies to class performance

# Running the tests

Copy here all necessary files from main project directory. The files to be copied are as follows:

* util_data.h
* util_data.cpp

Copy here all necessary files from tests/common directory. The files to be copied are as follows:

* test.h

Open .ino file in IDE, compile, upload and open serial monitor to observe test results

# Expected test results

## Primary tests

Primary tests are designed to test the functionality required for this project

To run primary tests, before compiling and uploading make sure that the following line is not commented in the file test_util_data_dsp_FixedPoint.ino:

    #define PRIMARY_TEST

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    millis: 799 micros: 798976
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    millis: 1850 micros: 1850021
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = int; unsigned int FractionBits = 16u; U = long long int; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    millis: 325 micros: 325012
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    millis: 3900 micros: 3900016
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    millis: 5488 micros: 5487521
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = int; unsigned int FractionBits = 8u; U = float; T TMaxRange = 2147483647; T TMinRange = -2147483648], file test_util_data_dsp_FixedPoint.ino
    millis: 313 micros: 312508
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 8 of more fraction bits and 14 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    millis: 714 micros: 713838
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    millis: 938 micros: 937508
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = short int; unsigned int FractionBits = 4u; U = int; T TMaxRange = 32767; T TMinRange = -32768], file test_util_data_dsp_FixedPoint.ino
    millis: 325 micros: 325008
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 2 of more fraction bits and 8 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 8 of more fraction bits and 14 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 2 of more fraction bits and 9 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    millis: 675 micros: 675026
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    millis: 838 micros: 837508
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = signed char; unsigned int FractionBits = 2u; U = short int; T TMaxRange = 127; T TMinRange = -128], file test_util_data_dsp_FixedPoint.ino
    millis: 312 micros: 312512
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 120
    Test functions failed: 0

Note: performance figures are for reference only and might vary depending on actual hardware setup.

## Secondary tests

Secondary tests work with functionality not needed for this projects: unsigned base type or no range set. Many tests will be skipped.

To run secondary tests, before compiling and uploading make sure that the following line is commented in the file test_util_data_dsp_FixedPoint.ino:

    //#define PRIMARY_TEST

The following is expected in serial monitor window:

    ---------------- TESTING STARTED ----------------
    
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Range check is skipped because range not is set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Negative value testing skipped because T is unsigned type or range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 362 micros: 362512
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 1388 micros: 1387512
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 4294967295u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 312 micros: 312508
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Range check is skipped because range not is set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 8 of more fraction bits and 14 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Negative value testing skipped because T is unsigned type or range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 2 of more fraction bits and 9 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 363 micros: 362512
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 388 micros: 387508
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 65535u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 313 micros: 312508
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Range check is skipped because range not is set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Negative value testing skipped because T is unsigned type or range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 363 micros: 362508
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 1400 micros: 1400012
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = unsigned int; unsigned int FractionBits = 8u; U = long long unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 313 micros: 312507
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::defaultConstructor_noParameters_expectZero() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromT_expectSameValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Range check is skipped because range not is set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction2bits_expectCorrectValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 8 of more fraction bits and 14 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::copyConstructor_copyFromFixedPoint_expectSameValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_copyFromFixedPoint_expectSameValue() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::conversionToBasicType_rounding_expectRoundingRulesFollowed() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Negative value testing skipped because T is unsigned type or range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideValidNumberStrings_expectCorrectValues() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_provideInvalidNumberStrings_expectZero() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_null_expectZero() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_fixedPointValues_expectCorrectConvertedValues() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Testing function skipped because it was designed for 2 of more fraction bits and 9 or more integer bits
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::addition_addTwoNumbers_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::subtraction_subtractOneNumberFromAnother_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplication_multiplyTwoNumbers_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::division_divideOneNumberByAnother_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideMultiply_expectNoFractionsLoss() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::fractions_divideAddMultiply_expectNoFractionsLoss() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_noFractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_noFractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::equalAndNotEqual_fractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::greaterLesserOrEqual_fractions_expectCorrectResult() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::arithmeticOperators_overflowValues_expectValuesNotModified() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::assignment_assignToOverflownValue_exepectValueAssigned() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongInteger_expectOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::constructorFromString_tooLongFraction_expectNoOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_edgesOfAcceptableRange_expectNoOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::getValue_tooManyDecimals_expectOverflow() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    Test function skipped because range not set
    PASSED
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAddition() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 375 micros: 375012
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestMultiplication() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 375 micros: 375008
    Performance test function started: static void FixedPointTester<T, FractionBits, U, TMaxRange, TMinRange>::performanceTestAssignment() [with T = short unsigned int; unsigned int FractionBits = 8u; U = unsigned int; T TMaxRange = 0u; T TMinRange = 0u], file test_util_data_dsp_FixedPoint.ino
    millis: 313 micros: 312508
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 120
    Test functions failed: 0

Note: performance figures are for reference only and might vary depending on actual hardware setup.

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
