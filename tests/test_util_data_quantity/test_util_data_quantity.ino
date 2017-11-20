#include <ESP8266WiFi.h>

#include "test.h"

#include "util_data.h"

util::Value absDiff(util::Value v1, util::Value v2) {
  util::Value diff = v1 - v2;
  if (diff < util::Value(0)) diff *= util::Value(-1);
  return (diff);
}

class TestQuantityAndGeneric {
  public:
    static void test_validate_genericQuantity_expectTrue() {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      const char unit[] = "test";
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Generic testGenericQuantity(id, value, unit, timestamp, descriptionText);
      //act
      boolean validationResult = testGenericQuantity.validate();
      //assert
      TEST_ASSERT(validationResult);
      TEST_FUNC_END();
    }
    static void test_validate_noninitialisedQuantity_expectFalse() {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity testQuantity;
      //act
      boolean validationResult = testQuantity.validate();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_FUNC_END();
    }
    static void test_validate(void) {
      test_validate_genericQuantity_expectTrue();
      test_validate_noninitialisedQuantity_expectFalse();
    }
  public:
    static void test_accessors_getGenericQuantityData_expectCorrectValues() {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      const char unit[] = "test";
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Generic testGenericQuantity(id, value, unit, timestamp, descriptionText);
      //act
      boolean validationResult = testGenericQuantity.validate();
      util::quantity::QuantityDescriptionId resultId = testGenericQuantity.getDescriptionId();
      util::Timestamp resultTimestamp = testGenericQuantity.getTimestamp();
      util::Value resultValue = testGenericQuantity.getValue();
      const char * resultUnit = testGenericQuantity.getUnitText();
      const char * resultDescriptionText = testGenericQuantity.getDescriptionText();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(!strcmp(unit, resultUnit));
      TEST_ASSERT(!strcmp(descriptionText, resultDescriptionText));
      TEST_FUNC_END();
    }
    static void test_accessors_getNoninitialisedQuantityData_expectDefaultValues() {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity testQuantity;
      //act
      boolean validationResult = testQuantity.validate();
      util::quantity::QuantityDescriptionId resultId = testQuantity.getDescriptionId();
      util::Timestamp resultTimestamp = testQuantity.getTimestamp();
      util::Value resultValue = testQuantity.getValue();
      const char * resultUnit = testQuantity.getUnitText();
      const char * resultDescriptionText = testQuantity.getDescriptionText();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!resultId);
      TEST_ASSERT(!resultTimestamp);
      TEST_ASSERT(resultValue == util::Value());
      TEST_ASSERT(!strlen(resultUnit));
      TEST_ASSERT(!strlen(resultDescriptionText));
      TEST_FUNC_END();
    }
    static void test_accessors_getGenericAsQuantityData_expectCorrectValues() {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      const char unit[] = "test";
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Generic testGenericQuantity(id, value, unit, timestamp, descriptionText);
      util::quantity::Quantity * testQuantity = &testGenericQuantity;
      //act
      boolean validationResult = testQuantity->validate();
      util::quantity::QuantityDescriptionId resultId = testQuantity->getDescriptionId();
      util::Timestamp resultTimestamp = testQuantity->getTimestamp();
      util::Value resultValue = testQuantity->getValue();
      const char * resultUnit = testQuantity->getUnitText();
      const char * resultDescriptionText = testQuantity->getDescriptionText();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(!strcmp(unit, resultUnit));
      TEST_ASSERT(!strcmp(descriptionText, resultDescriptionText));
      TEST_FUNC_END();
    }
    static void test_accessors(void) {
      test_accessors_getGenericQuantityData_expectCorrectValues();
      test_accessors_getNoninitialisedQuantityData_expectDefaultValues();
      test_accessors_getGenericAsQuantityData_expectCorrectValues();
    }
  public:
    static void test_copyConstructor_copyFromGenericQuantity_expectSameValues() {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      const char unit[] = "test";
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Generic testGenericQuantitySource(id, value, unit, timestamp, descriptionText);
      //act
      util::quantity::Generic testGenericQuantity(testGenericQuantitySource);
      //assert
      boolean validationResultSrc = testGenericQuantitySource.validate();
      boolean validationResultDst = testGenericQuantity.validate();
      util::quantity::QuantityDescriptionId resultIdSrc = testGenericQuantitySource.getDescriptionId();
      util::quantity::QuantityDescriptionId resultIdDst = testGenericQuantity.getDescriptionId();
      util::Timestamp resultTimestampSrc = testGenericQuantitySource.getTimestamp();
      util::Timestamp resultTimestampDst = testGenericQuantity.getTimestamp();
      util::Value resultValueSrc = testGenericQuantitySource.getValue();
      util::Value resultValueDst = testGenericQuantity.getValue();
      const char * resultUnitSrc = testGenericQuantitySource.getUnitText();
      const char * resultUnitDst = testGenericQuantity.getUnitText();
      const char * resultDescriptionTextSrc = testGenericQuantitySource.getDescriptionText();
      const char * resultDescriptionTextDst = testGenericQuantity.getDescriptionText();
      TEST_ASSERT(validationResultSrc);
      TEST_ASSERT(validationResultDst);
      TEST_ASSERT(resultIdSrc == resultIdDst);
      TEST_ASSERT(resultTimestampSrc == resultTimestampDst);
      TEST_ASSERT(resultValueSrc == resultValueDst);
      TEST_ASSERT(!strcmp(resultUnitSrc, resultUnitDst));
      TEST_ASSERT(!strcmp(resultDescriptionTextSrc, resultDescriptionTextDst));
      TEST_FUNC_END();
    }
    static void test_copyConstructor_copyFromNoninitialisedQuantity_expectDefaultData() {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity testQuantitySource;
      //act
      util::quantity::Quantity testQuantity(testQuantitySource);
      //assert
      boolean validationResult = testQuantity.validate();
      util::quantity::QuantityDescriptionId resultId = testQuantity.getDescriptionId();
      util::Timestamp resultTimestamp = testQuantity.getTimestamp();
      util::Value resultValue = testQuantity.getValue();
      const char * resultUnit = testQuantity.getUnitText();
      const char * resultDescriptionText = testQuantity.getDescriptionText();
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!resultId);
      TEST_ASSERT(!resultTimestamp);
      TEST_ASSERT(resultValue == util::Value());
      TEST_ASSERT(!strlen(resultUnit));
      TEST_ASSERT(!strlen(resultDescriptionText));
      TEST_FUNC_END();
    }
    static void test_copyConstructor(void) {
      test_copyConstructor_copyFromGenericQuantity_expectSameValues();
      test_copyConstructor_copyFromNoninitialisedQuantity_expectDefaultData();
    }
  public:
    static void test_assignment_assignFromGenericToNoninitialisedQuantity_expectSameValues() {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      const char unit[] = "test";
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Generic testGenericQuantity(id, value, unit, timestamp, descriptionText);
      util::quantity::Quantity testQuantity;
      //act
      testQuantity = testGenericQuantity;
      //assert
      boolean validationResultSrc = testGenericQuantity.validate();
      boolean validationResultDst = testQuantity.validate();
      util::quantity::QuantityDescriptionId resultIdSrc = testGenericQuantity.getDescriptionId();
      util::quantity::QuantityDescriptionId resultIdDst = testQuantity.getDescriptionId();
      util::Timestamp resultTimestampSrc = testGenericQuantity.getTimestamp();
      util::Timestamp resultTimestampDst = testQuantity.getTimestamp();
      util::Value resultValueSrc = testGenericQuantity.getValue();
      util::Value resultValueDst = testQuantity.getValue();
      const char * resultUnitSrc = testGenericQuantity.getUnitText();
      const char * resultUnitDst = testQuantity.getUnitText();
      const char * resultDescriptionTextSrc = testGenericQuantity.getDescriptionText();
      const char * resultDescriptionTextDst = testQuantity.getDescriptionText();
      TEST_ASSERT(validationResultSrc);
      TEST_ASSERT(validationResultDst);
      TEST_ASSERT(resultIdSrc == resultIdDst);
      TEST_ASSERT(resultTimestampSrc == resultTimestampDst);
      TEST_ASSERT(resultValueSrc == resultValueDst);
      TEST_ASSERT(!strcmp(resultUnitSrc, resultUnitDst));
      TEST_ASSERT(!strcmp(resultDescriptionTextSrc, resultDescriptionTextDst));
      TEST_FUNC_END();
    }
    static void test_assignment_assignToAlreadyInitialisedQuantity_expectDataNotCopied() {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id1(3);
      util::Value value1(5);
      const char unit1[] = "test";
      util::Timestamp timestamp1 = util::getTimestamp();
      const char descriptionText1[] = "description";
      util::quantity::Generic testGenericQuantity1(id1, value1, unit1, timestamp1, descriptionText1);
      util::quantity::QuantityDescriptionId id2(4);
      util::Value value2(6);
      delay(2);//makes sure that testGenericQuantity1 and testGenericQuantity2 timestamps are not the same
      const char unit2[] = "unit";
      util::Timestamp timestamp2 = util::getTimestamp();
      const char descriptionText2[] = "descrText";
      util::quantity::Generic testGenericQuantity2(id2, value2, unit2, timestamp2, descriptionText2);
      //act
      testGenericQuantity1 = testGenericQuantity2;
      //assert
      boolean validationResult1 = testGenericQuantity1.validate();
      boolean validationResult2 = testGenericQuantity2.validate();
      util::quantity::QuantityDescriptionId resultId1 = testGenericQuantity1.getDescriptionId();
      util::Timestamp resultTimestamp1 = testGenericQuantity1.getTimestamp();
      util::Value resultValue1 = testGenericQuantity1.getValue();
      const char * resultUnit1 = testGenericQuantity1.getUnitText();
      const char * resultDescriptionText1 = testGenericQuantity1.getDescriptionText();
      //Make sure both Quantities are initialised
      TEST_ASSERT(validationResult1);
      TEST_ASSERT(validationResult2);
      //Make sure all data from two Quantities are different
      TEST_ASSERT(id1 != id2);
      TEST_ASSERT(timestamp1 != timestamp2);
      TEST_ASSERT(value1 != value2);
      TEST_ASSERT(strcmp(unit1, unit2));
      TEST_ASSERT(strcmp(descriptionText1, descriptionText2));
      //Make sure no data were copied during assignment
      TEST_ASSERT(resultId1 == id1);
      TEST_ASSERT(resultTimestamp1 == timestamp1);
      TEST_ASSERT(resultValue1 == value1);
      TEST_ASSERT(!strcmp(resultUnit1, unit1));
      TEST_ASSERT(!strcmp(resultDescriptionText1, descriptionText1));
      //
      TEST_FUNC_END();
    }
    static void test_assignment_assignFromNoninitialisedQuantity_expectDefaultData() {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity testQuantitySource;
      util::quantity::Quantity testQuantity;
      //act
      testQuantity = testQuantitySource;
      //assert
      boolean validationResult = testQuantity.validate();
      util::quantity::QuantityDescriptionId resultId = testQuantity.getDescriptionId();
      util::Timestamp resultTimestamp = testQuantity.getTimestamp();
      util::Value resultValue = testQuantity.getValue();
      const char * resultUnit = testQuantity.getUnitText();
      const char * resultDescriptionText = testQuantity.getDescriptionText();
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!resultId);
      TEST_ASSERT(!resultTimestamp);
      TEST_ASSERT(resultValue == util::Value());
      TEST_ASSERT(!strlen(resultUnit));
      TEST_ASSERT(!strlen(resultDescriptionText));
      TEST_FUNC_END();
    }
    static void test_assignment_assignFromNoninitialisedQuantityToAlreadyInitialisedQuantity_expectDataNotCopied() {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      const char unit[] = "test";
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Generic testGenericQuantitySource(id, value, unit, timestamp, descriptionText);
      util::quantity::Quantity * testGenericQuantity = &testGenericQuantitySource;
      util::quantity::Quantity testNonititialisedQuantity;
      //act
      *testGenericQuantity = testNonititialisedQuantity;
      //assert
      boolean validationResult = testGenericQuantity->validate();
      util::quantity::QuantityDescriptionId resultId = testGenericQuantity->getDescriptionId();
      util::Timestamp resultTimestamp = testGenericQuantity->getTimestamp();
      util::Value resultValue = testGenericQuantity->getValue();
      const char * resultUnit = testGenericQuantity->getUnitText();
      const char * resultDescriptionText = testGenericQuantity->getDescriptionText();
      TEST_ASSERT(validationResult);
      TEST_ASSERT(resultId == id);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(!strcmp(resultUnit, unit));
      TEST_ASSERT(!strcmp(resultDescriptionText, descriptionText));
      TEST_FUNC_END();
    }
    static void test_assignment(void) {
      test_assignment_assignFromGenericToNoninitialisedQuantity_expectSameValues();
      test_assignment_assignToAlreadyInitialisedQuantity_expectDataNotCopied();
      test_assignment_assignFromNoninitialisedQuantity_expectDefaultData();
      test_assignment_assignFromNoninitialisedQuantityToAlreadyInitialisedQuantity_expectDataNotCopied();
    }
  public:
    static void runTests(void) {
      test_validate();
      test_accessors();
      test_copyConstructor();
      test_assignment();
    }
};

class TestDimensionless {
  public:
    static void init_DimensionlessQuantity_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      util::quantity::Dimensionless::Unit unit = util::quantity::Dimensionless::Unit::NONE;
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      //act
      util::quantity::Dimensionless testDimensionlessQuantity(id, value, unit, timestamp, descriptionText);
      //assert
      boolean validationResult = testDimensionlessQuantity.validate();
      util::quantity::QuantityDescriptionId resultId = testDimensionlessQuantity.getDescriptionId();
      util::Timestamp resultTimestamp = testDimensionlessQuantity.getTimestamp();
      util::Value resultValue = testDimensionlessQuantity.getValue();
      const char * resultUnit = testDimensionlessQuantity.getUnitText();
      const char * resultDescriptionText = testDimensionlessQuantity.getDescriptionText();
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(!strcmp(util::quantity::Dimensionless::getUnitTextByUnit(unit), resultUnit));
      TEST_ASSERT(!strcmp(descriptionText, resultDescriptionText));
      TEST_FUNC_END();
    }
    static void test_init(void) {
      init_DimensionlessQuantity_expectCorrectData();
    }
  public:
    static void conversion_convertFromNoneToPercentAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(4);
      util::Value value(1);
      util::quantity::Dimensionless::Unit unit = util::quantity::Dimensionless::Unit::NONE;
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Dimensionless testDimensionlessQuantity(id, value, unit, timestamp, descriptionText);
      static const size_t testValuesCount = 3;
      util::Value result[testValuesCount] = {};
      static const size_t resultUnitSize = 8;
      char resultUnit[testValuesCount][resultUnitSize] = {};
      util::Value referenceResult[testValuesCount] = {util::Value(1), util::Value(100), util::Value(1)};
      const char referenceResultUnit[testValuesCount][resultUnitSize] = {"", "%", ""};
      size_t countResult = 0;
      size_t countResultUnit = 0;
      //act
      boolean validationResult = testDimensionlessQuantity.validate();
      result[countResult++] = testDimensionlessQuantity.getValue();
      strcpy(resultUnit[countResultUnit++], testDimensionlessQuantity.getUnitText());
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::PERCENT);
      result[countResult++] = testDimensionlessQuantity.getValue();
      strcpy(resultUnit[countResultUnit++], testDimensionlessQuantity.getUnitText());
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::NONE);
      result[countResult++] = testDimensionlessQuantity.getValue();
      strcpy(resultUnit[countResultUnit++], testDimensionlessQuantity.getUnitText());
      //assert
      static const size_t nullTerminatorSize = 1;
      TEST_ASSERT(resultUnitSize >= (util::quantity::Quantity::getUnitTextMaxSize() + nullTerminatorSize));
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(!strcmp(resultUnit[0], referenceResultUnit[0]));
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(!strcmp(resultUnit[1], referenceResultUnit[1]));
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(!strcmp(resultUnit[2], referenceResultUnit[2]));
      TEST_FUNC_END();
    }
    static void conversion_convertFromPercentToNoneAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(4);
      util::Value value(200);
      util::quantity::Dimensionless::Unit unit = util::quantity::Dimensionless::Unit::PERCENT;
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Dimensionless testDimensionlessQuantity(id, value, unit, timestamp, descriptionText);
      static const size_t testValuesCount = 3;
      util::Value result[testValuesCount] = {};
      static const size_t resultUnitSize = 8;
      char resultUnit[testValuesCount][resultUnitSize] = {};
      const util::Value referenceResult[testValuesCount] = {util::Value(200), util::Value(2), util::Value(200)};
      const char referenceResultUnit[testValuesCount][resultUnitSize] = {"%", "", "%"};
      size_t countResult = 0;
      size_t countResultUnit = 0;
      //act
      boolean validationResult = testDimensionlessQuantity.validate();
      result[countResult++] = testDimensionlessQuantity.getValue();
      strcpy(resultUnit[countResultUnit++], testDimensionlessQuantity.getUnitText());
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::NONE);
      result[countResult++] = testDimensionlessQuantity.getValue();
      strcpy(resultUnit[countResultUnit++], testDimensionlessQuantity.getUnitText());
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::PERCENT);
      result[countResult++] = testDimensionlessQuantity.getValue();
      strcpy(resultUnit[countResultUnit++], testDimensionlessQuantity.getUnitText());
      //assert
      static const size_t nullTerminatorSize = 1;
      TEST_ASSERT(resultUnitSize >= (util::quantity::Quantity::getUnitTextMaxSize() + nullTerminatorSize));
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(!strcmp(resultUnit[0], referenceResultUnit[0]));
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(!strcmp(resultUnit[1], referenceResultUnit[1]));
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(!strcmp(resultUnit[2], referenceResultUnit[2]));
      TEST_FUNC_END();
    }
    static void test_conversion(void) {
      conversion_convertFromNoneToPercentAndBack_expectCorrectData();
      conversion_convertFromPercentToNoneAndBack_expectCorrectData();
    }
  public:
    static void runTests(void) {
      test_init();
      test_conversion();
    }
};

class TestTemperature {
  public:
    static void init_Temperature_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(3);
      util::Value value(5);
      util::quantity::Temperature::Unit unit = util::quantity::Temperature::Unit::CELSIUS;
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      //act
      util::quantity::Temperature testTemperature(id, value, unit, timestamp, descriptionText);
      //assert
      boolean validationResult = testTemperature.validate();
      util::quantity::QuantityDescriptionId resultId = testTemperature.getDescriptionId();
      util::Timestamp resultTimestamp = testTemperature.getTimestamp();
      util::Value resultValue = testTemperature.getValue();
      const char * resultUnit = testTemperature.getUnitText();
      const char * resultDescriptionText = testTemperature.getDescriptionText();
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(!strcmp(util::quantity::Temperature::getUnitTextByUnit(unit), resultUnit));
      TEST_ASSERT(!strcmp(descriptionText, resultDescriptionText));
      TEST_FUNC_END();
    }
    static void test_init(void) {
      init_Temperature_expectCorrectData();
    }
  public:
    static void conversion_convertFromCelsiusToFahrenheitAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(4);
      util::Value value(100);
      util::quantity::Temperature::Unit unit = util::quantity::Temperature::Unit::CELSIUS;
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Temperature testTemperature(id, value, unit, timestamp, descriptionText);
      static const size_t testValuesCount = 3;
      util::Value result[testValuesCount] = {};
      static const size_t resultUnitSize = 8;
      char resultUnit[testValuesCount][resultUnitSize] = {};
      const util::Value referenceResult[testValuesCount] = {util::Value(100), util::Value(212), util::Value(100)};
      const char referenceResultUnit[testValuesCount][resultUnitSize] = {"C", "F", "C"};
      const util::Value tolerance(1, 1); //0.1
      size_t countResult = 0;
      size_t countResultUnit = 0;
      //act
      boolean validationResult = testTemperature.validate();
      result[countResult++] = testTemperature.getValue();
      strcpy(resultUnit[countResultUnit++], testTemperature.getUnitText());
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::FAHRENHEIT);
      result[countResult++] = testTemperature.getValue();
      strcpy(resultUnit[countResultUnit++], testTemperature.getUnitText());
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::CELSIUS);
      result[countResult++] = testTemperature.getValue();
      strcpy(resultUnit[countResultUnit++], testTemperature.getUnitText());
      //assert
      static const size_t nullTerminatorSize = 1;
      TEST_ASSERT(resultUnitSize >= (util::quantity::Quantity::getUnitTextMaxSize() + nullTerminatorSize));
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(!strcmp(resultUnit[0], referenceResultUnit[0]));
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(!strcmp(resultUnit[1], referenceResultUnit[1]));
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(!strcmp(resultUnit[2], referenceResultUnit[2]));
      TEST_FUNC_END();
    }
    static void conversion_convertFromFahrenheitToCelsiusAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::QuantityDescriptionId id(4);
      util::Value value(86);
      util::quantity::Temperature::Unit unit = util::quantity::Temperature::Unit::FAHRENHEIT;
      util::Timestamp timestamp = util::getTimestamp();
      const char descriptionText[] = "description";
      util::quantity::Temperature testTemperature(id, value, unit, timestamp, descriptionText);
      static const size_t testValuesCount = 3;
      util::Value result[testValuesCount] = {};
      static const size_t resultUnitSize = 8;
      char resultUnit[testValuesCount][resultUnitSize] = {};
      const util::Value referenceResult[testValuesCount] = {util::Value(86), util::Value(30), util::Value(86)};
      const char referenceResultUnit[testValuesCount][resultUnitSize] = {"F", "C", "F"};
      size_t countResult = 0;
      size_t countResultUnit = 0;
      //act
      boolean validationResult = testTemperature.validate();
      result[countResult++] = testTemperature.getValue();
      strcpy(resultUnit[countResultUnit++], testTemperature.getUnitText());
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::CELSIUS);
      result[countResult++] = testTemperature.getValue();
      strcpy(resultUnit[countResultUnit++], testTemperature.getUnitText());
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::FAHRENHEIT);
      result[countResult++] = testTemperature.getValue();
      strcpy(resultUnit[countResultUnit++], testTemperature.getUnitText());
      //assert
      static const size_t nullTerminatorSize = 1;
      TEST_ASSERT(resultUnitSize >= (util::quantity::Quantity::getUnitTextMaxSize() + nullTerminatorSize));
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(!strcmp(resultUnit[0], referenceResultUnit[0]));
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(!strcmp(resultUnit[1], referenceResultUnit[1]));
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(!strcmp(resultUnit[2], referenceResultUnit[2]));
      TEST_FUNC_END();
    }
    static void test_conversion(void) {
      conversion_convertFromCelsiusToFahrenheitAndBack_expectCorrectData();
      conversion_convertFromFahrenheitToCelsiusAndBack_expectCorrectData();
    }
  public:
    static void runTests(void) {
      test_init();
      test_conversion();
    }
};

template <typename T>
class Matrix {
  public:
    Matrix() {}
    Matrix(size_t rows, size_t columns) {
      this->rows = rows;
      this->columns = columns;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvla"
      matrix = new (T[rows * columns]);
#pragma GCC diagnostic pop
      T defaultValue = {};
      for (size_t i = 0; i < rows * columns; i++)
        matrix[i] = defaultValue;
    }
    ~Matrix() {
      delete(matrix);
    }
  public:
    inline boolean operator == (const Matrix<T> &rhs) const {
      if (rhs.rows != rows) return (false);
      if (rhs.columns != columns) return (false);
      if (!matrix || !rhs.matrix) return (false);
      for (size_t i = 0; i < rows * columns; i++)
        if (rhs.matrix[i] != matrix[i]) return (false);
      return (true);
    }
  public:
    inline size_t getRows(void) const {
      return (rows);
    }
    inline size_t getColumns(void) const {
      return (columns);
    }
    inline T getValue(size_t row, size_t column) const {
      if ((row > rows) || (column > columns) || !matrix) return (false);
      return (matrix[row * columns + column]);
    }
    inline void setValue(size_t row, size_t column, T value) {
      if ((row > rows) || (column > columns) || !matrix) return;
      matrix[row * columns + column] = value;
    }
    void print(Print &dst) {
      for (size_t i = 0; i < this->getRows(); i++) {
        for (size_t j = 0; j < this->getColumns(); j++) {
          dst.print(this->getValue(i, j));
          dst.print(' ');
        }
        dst.println();
      }

    }
  private:
    size_t rows = 0;
    size_t columns = 0;
    T * matrix = nullptr;
};

template <class Base, class... IntrospectedClasses>
class TestIntrospectionClassMatchMatrix : public Matrix<boolean> {
  public:
    TestIntrospectionClassMatchMatrix() :
      Matrix<boolean>(sizeof... (IntrospectedClasses), sizeof... (IntrospectedClasses))
    {
      decltype(INTROSPECT_CLASS(Base)) classTypes[] = {INTROSPECT_CLASS(IntrospectedClasses)...};
      for (size_t i = 0; i < sizeof... (IntrospectedClasses); i++)
        for (size_t j = 0; j < sizeof... (IntrospectedClasses); j++)
          setValue(i, j, classTypes[i] == classTypes[j]);
    }
};

template <class Base>
class TestIntrospectionObjectMatchMatrix : public Matrix<boolean> {
  public:
    template <class... TestObjects>TestIntrospectionObjectMatchMatrix(const TestObjects... testObjects) :
      Matrix<boolean>(sizeof... (testObjects), sizeof... (testObjects))
    {
      decltype(INTROSPECT_CLASS(Base)) objectTypes[] = {testObjects->INTROSPECT_OBJECT()...};
      for (size_t i = 0; i < sizeof... (testObjects); i++)
        for (size_t j = 0; j < sizeof... (testObjects); j++)
          setValue(i, j, (objectTypes[i] == objectTypes[j]));
    }
};

template <class Base, class... IntrospectedClasses>
class TestIntrospectionClassVsObjectMatchMatrix : public Matrix<boolean> {
  public:
    template <class... TestObjects>TestIntrospectionClassVsObjectMatchMatrix(const TestObjects... testObjects) :
      Matrix<boolean> (sizeof... (IntrospectedClasses) , sizeof... (testObjects))
    {
      decltype(INTROSPECT_CLASS(Base)) objectTypes[] = {testObjects->INTROSPECT_OBJECT()...};
      decltype(INTROSPECT_CLASS(Base)) classTypes[] = {INTROSPECT_CLASS(IntrospectedClasses)...};
      for (size_t i = 0; i < sizeof... (IntrospectedClasses); i++)
        for (size_t j = 0; j < sizeof... (testObjects); j++)
          setValue(i, j, (classTypes[i] == objectTypes[j]));
    }
};

class TestQuantityIntrospectionReflection {
  public:
    static void testClassTypeMatch(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t numClasses = 4;
      Matrix<boolean> referenceClassMatchMatrix(numClasses, numClasses);
      for (size_t i = 0; i < numClasses; i++)
        for (size_t j = 0; j < numClasses; j++)
          referenceClassMatchMatrix.setValue(i, j, i == j ? true : false);
      //act
      const TestIntrospectionClassMatchMatrix<util::quantity::Quantity,
            util::quantity::Quantity,
            util::quantity::Generic,
            util::quantity::Dimensionless,
            util::quantity::Temperature> testClassMatchMatrix;
      //assert
      TEST_ASSERT(testClassMatchMatrix == referenceClassMatchMatrix);
      TEST_FUNC_END();
    }
    static void testObjectTypeMatch(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t numObjects = 8;
      util::quantity::Quantity q1;
      util::quantity::Quantity q2;
      util::quantity::Generic g1(1, 2, "unit1");
      util::quantity::Generic g2(3, 4, "unit2");
      util::quantity::Dimensionless d1(1, 2, util::quantity::Dimensionless::Unit::NONE);
      util::quantity::Dimensionless d2(3, 4, util::quantity::Dimensionless::Unit::PERCENT);
      util::quantity::Temperature t1(1, 2, util::quantity::Temperature::Unit::CELSIUS);
      util::quantity::Temperature t2(3, 4, util::quantity::Temperature::Unit::FAHRENHEIT);
      util::quantity::Quantity * testObject1 = &q1;
      util::quantity::Quantity * testObject2 = &q2;
      util::quantity::Quantity * testObject3 = &g1;
      util::quantity::Quantity * testObject4 = &g2;
      util::quantity::Quantity * testObject5 = &d1;
      util::quantity::Quantity * testObject6 = &d2;
      util::quantity::Quantity * testObject7 = &t1;
      util::quantity::Quantity * testObject8 = &t2;

      Matrix<boolean> referenceObjectMatchMatrix(numObjects, numObjects);
      for (size_t i = 0; i < numObjects; i++)
        for (size_t j = 0; j < numObjects; j++)
          referenceObjectMatchMatrix.setValue(i, j, ((i >> 1) == (j >> 1)) ? true : false);
      //act
      const TestIntrospectionObjectMatchMatrix<util::quantity::Quantity> testObjectMatchMatrix(
        testObject1,
        testObject2,
        testObject3,
        testObject4,
        testObject5,
        testObject6,
        testObject7,
        testObject8);
      //assert
      TEST_ASSERT(testObjectMatchMatrix == referenceObjectMatchMatrix);
      TEST_FUNC_END();
    }
    static void testClassTypeAndObjectTypeMatch(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t numObjects = 8;
      static const size_t numClasses = 4;
      util::quantity::Quantity q1;
      util::quantity::Quantity q2;
      util::quantity::Generic g1(1, 2, "unit1");
      util::quantity::Generic g2(3, 4, "unit2");
      util::quantity::Dimensionless d1(1, 2, util::quantity::Dimensionless::Unit::NONE);
      util::quantity::Dimensionless d2(3, 4, util::quantity::Dimensionless::Unit::PERCENT);
      util::quantity::Temperature t1(1, 2, util::quantity::Temperature::Unit::CELSIUS);
      util::quantity::Temperature t2(3, 4, util::quantity::Temperature::Unit::FAHRENHEIT);
      util::quantity::Quantity * testObject1 = &q1;
      util::quantity::Quantity * testObject2 = &q2;
      util::quantity::Quantity * testObject3 = &g1;
      util::quantity::Quantity * testObject4 = &g2;
      util::quantity::Quantity * testObject5 = &d1;
      util::quantity::Quantity * testObject6 = &d2;
      util::quantity::Quantity * testObject7 = &t1;
      util::quantity::Quantity * testObject8 = &t2;
      Matrix<boolean> referenceClassVsObjectMatchMatrix(numClasses, numObjects);
      for (size_t i = 0; i < referenceClassVsObjectMatchMatrix.getRows(); i++)
        for (size_t j = 0; j < referenceClassVsObjectMatchMatrix.getColumns(); j++)
          referenceClassVsObjectMatchMatrix.setValue(i, j, false);
      referenceClassVsObjectMatchMatrix.setValue(0, 0, true);
      referenceClassVsObjectMatchMatrix.setValue(0, 1, true);
      referenceClassVsObjectMatchMatrix.setValue(1, 2, true);
      referenceClassVsObjectMatchMatrix.setValue(1, 3, true);
      referenceClassVsObjectMatchMatrix.setValue(2, 4, true);
      referenceClassVsObjectMatchMatrix.setValue(2, 5, true);
      referenceClassVsObjectMatchMatrix.setValue(3, 6, true);
      referenceClassVsObjectMatchMatrix.setValue(3, 7, true);
      //act
      const TestIntrospectionClassVsObjectMatchMatrix<util::quantity::Quantity,
            util::quantity::Quantity,
            util::quantity::Generic,
            util::quantity::Dimensionless,
            util::quantity::Temperature
            > testClassVsObjectMatchMatrix (
              testObject1,
              testObject2,
              testObject3,
              testObject4,
              testObject5,
              testObject6,
              testObject7,
              testObject8
            );
      //assert
      TEST_ASSERT(testClassVsObjectMatchMatrix == referenceClassVsObjectMatchMatrix);
      TEST_FUNC_END();
    }
    static void testCopiedObjectTypeMatch(void) {//TODO
      TEST_FUNC_START();
      //arrange
      //act
      //assert
      TEST_ASSERT(true);
      TEST_FUNC_END();
    }
    static void testIntrospection(void) {
      testClassTypeMatch();
      testObjectTypeMatch();
      testClassTypeAndObjectTypeMatch();
      testCopiedObjectTypeMatch();
    }
  public:
    static void testConcreteClassReflection(void) {//TODO
      TEST_FUNC_START();
      //arrange
      //act
      //assert
      TEST_ASSERT(true);
      TEST_FUNC_END();
    }
    static void testAttemptWrongTypeReflection(void) {//TODO
      TEST_FUNC_START();
      //arrange
      //act
      //assert
      TEST_ASSERT(true);
      TEST_FUNC_END();
    }
    static void testReflection(void) {
      testConcreteClassReflection();
      testAttemptWrongTypeReflection();
    }

  public:
    static void runTests(void) {
      testIntrospection();
      testReflection();
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestQuantityAndGeneric::runTests();
  TestDimensionless::runTests();
  TestTemperature::runTests();
  TestQuantityIntrospectionReflection::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
