# Purpose of the tests

To confirm that any of the recent modifications of class Quantity and its child classes in file util_data, namespace util::quantity (physical quantities) did not introduce anomalies to class performance

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
    
    Test function started: static void TestQuantityAndGeneric::test_validate_genericQuantity_expectTrue(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_validate_genericQuantityInvalidValue_expectFalse(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_accessors_getGenericQuantityData_expectCorrectValues(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_accessors_getGenericAsQuantityData_expectCorrectValues(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_copyConstructor_copyFromGenericQuantity_expectSameValues(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_initFromParametersConstructor_unitTextNull_expectEmptyUnitText(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_initFromParametersConstructor_valueExceedsMinRange_expectValueLimitedToMinRange(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_initFromParametersConstructor_valueExceedsMaxRange_expectValueLimitedToMaxRange(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityAndGeneric::test_assignment_assignFromGenericToGeneric_expectSameValues(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestDimensionless::init_DimensionlessQuantity_expectCorrectData(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestDimensionless::conversion_convertFromNoneToPercentAndBack_expectCorrectData(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestDimensionless::conversion_convertFromPercentToNoneAndBack_expectCorrectData(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestTemperature::init_Temperature_expectCorrectData(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestTemperature::conversion_convertFromCelsiusToFahrenheitAndBack_expectCorrectData(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestTemperature::conversion_convertFromFahrenheitToCelsiusAndBack_expectCorrectData(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityIntrospectionReflection::testClassTypeMatch(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityIntrospectionReflection::testObjectTypeMatch(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityIntrospectionReflection::testClassTypeAndObjectTypeMatch(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityIntrospectionReflection::testCopiedObjectTypeMatch(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityIntrospectionReflection::testReflectionGeneric(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityIntrospectionReflection::testReflectionDimensionless(), file test_util_data_quantity.ino
    PASSED
    Test function started: static void TestQuantityIntrospectionReflection::testReflectionTemperature(), file test_util_data_quantity.ino
    PASSED
    sizeof(util::quantity::Quantity) 42
    sizeof(util::quantity::Generic) 42
    sizeof(util::quantity::Dimensionless) 42
    sizeof(util::quantity::Temperature) 42
    
    ---------------- TESTING FINISHED ---------------
    Test functions passed: 22
    Test functions failed: 0

Note: class sizes are for reference only.

# Failed tests

The following example illustrates failed tests reporting:

    Test function started: static void test(), file test.ino
    FAILED in line 1, file test.ino
