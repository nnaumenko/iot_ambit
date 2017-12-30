#include <ESP8266WiFi.h>

#include "test.h"

#include "util_data.h"

util::quantity::Quantity::value_t absDiff(util::quantity::Quantity::value_t v1, util::quantity::Quantity::value_t v2) {
  util::quantity::Quantity::value_t diff = v1 - v2;
  if (diff < util::quantity::Quantity::value_t(0)) diff *= util::quantity::Quantity::value_t(-1);
  return (diff);
}

class TestQuantityAndGeneric {
  public:
    static void test_validate_genericQuantity_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Generic testGenericQuantity(id, idGroup, value, unit, timestamp);
      //act
      boolean validationResult = testGenericQuantity.validate();
      //assert
      TEST_ASSERT(validationResult);
      TEST_FUNC_END();
    }
    static void test_validate_genericQuantityInvalidValue_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      boolean valueValid = false;
      util::quantity::Generic testGenericQuantity(id, idGroup, value, unit, timestamp, valueValid);
      //act
      boolean validationResult = testGenericQuantity.validate();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_FUNC_END();
    }
    static void test_validate(void) {
      test_validate_genericQuantity_expectTrue();
      test_validate_genericQuantityInvalidValue_expectFalse();
    }
  public:
    static void test_accessors_getGenericQuantityData_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Quantity::value_t minRange(0);
      util::quantity::Quantity::value_t maxRange(100);
      const boolean valueValid = true;
      util::quantity::Generic testGenericQuantity(id, idGroup, value, unit, timestamp, valueValid, minRange, maxRange);
      //act
      boolean validationResult = testGenericQuantity.validate();
      util::quantity::Quantity::id_t resultId = testGenericQuantity.getId();
      util::quantity::Quantity::id_t resultIdGroup = testGenericQuantity.getIdGroup();
      util::quantity::Quantity::timestamp_t resultTimestamp = testGenericQuantity.getTimestamp();
      util::quantity::Quantity::value_t resultValue = testGenericQuantity.getValue();
      util::quantity::Quantity::text_t resultUnit = testGenericQuantity.getUnitText();
      util::quantity::Quantity::value_t resultMinRange = testGenericQuantity.getMinRange();
      util::quantity::Quantity::value_t resultMaxRange = testGenericQuantity.getMaxRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(idGroup == resultIdGroup);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(unit == resultUnit);
      TEST_ASSERT(resultMinRange == minRange);
      TEST_ASSERT(resultMaxRange == maxRange);
      TEST_FUNC_END();
    }
    static void test_accessors_getGenericAsQuantityData_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Quantity::value_t minRange(0);
      util::quantity::Quantity::value_t maxRange(100);
      const boolean valueValid = true;
      util::quantity::Generic testGenericQuantity(id, idGroup, value, unit, timestamp, valueValid, minRange, maxRange);
      util::quantity::Quantity * testQuantity = &testGenericQuantity;
      //act
      boolean validationResult = testQuantity->validate();
      util::quantity::Quantity::id_t resultId = testQuantity->getId();
      util::quantity::Quantity::id_t resultIdGroup = testQuantity->getIdGroup();
      util::quantity::Quantity::timestamp_t resultTimestamp = testQuantity->getTimestamp();
      util::quantity::Quantity::value_t resultValue = testQuantity->getValue();
      util::quantity::Quantity::text_t resultUnit = testQuantity->getUnitText();
      util::quantity::Quantity::value_t resultMinRange = testQuantity->getMinRange();
      util::quantity::Quantity::value_t resultMaxRange = testQuantity->getMaxRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(idGroup == resultIdGroup);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(unit == resultUnit);
      TEST_ASSERT(resultMinRange == minRange);
      TEST_ASSERT(resultMaxRange == maxRange);
      TEST_FUNC_END();
    }
    static void test_accessors(void) {
      test_accessors_getGenericQuantityData_expectCorrectValues();
      test_accessors_getGenericAsQuantityData_expectCorrectValues();
    }
  public:
    static void test_copyConstructor_copyFromGenericQuantity_expectSameValues(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Quantity::value_t minRange(0);
      util::quantity::Quantity::value_t maxRange(100);
      const boolean valueValid = true;
      util::quantity::Generic testGenericQuantitySource(id, idGroup, value, unit, timestamp, valueValid, minRange, maxRange);
      //act
      util::quantity::Generic testGenericQuantity(testGenericQuantitySource);
      //assert
      boolean validationResultSrc = testGenericQuantitySource.validate();
      boolean validationResultDst = testGenericQuantity.validate();
      util::quantity::Quantity::id_t resultIdSrc = testGenericQuantitySource.getId();
      util::quantity::Quantity::id_t resultIdDst = testGenericQuantity.getId();
      util::quantity::Quantity::id_t resultIdGroupSrc = testGenericQuantitySource.getIdGroup();
      util::quantity::Quantity::id_t resultIdGroupDst = testGenericQuantity.getIdGroup();
      util::quantity::Quantity::timestamp_t resultTimestampSrc = testGenericQuantitySource.getTimestamp();
      util::quantity::Quantity::timestamp_t resultTimestampDst = testGenericQuantity.getTimestamp();
      util::quantity::Quantity::value_t resultValueSrc = testGenericQuantitySource.getValue();
      util::quantity::Quantity::value_t resultValueDst = testGenericQuantity.getValue();
      util::quantity::Quantity::text_t resultUnitSrc = testGenericQuantitySource.getUnitText();
      util::quantity::Quantity::text_t resultUnitDst = testGenericQuantity.getUnitText();
      util::quantity::Quantity::value_t resultMinRangeSrc = testGenericQuantitySource.getMinRange();
      util::quantity::Quantity::value_t resultMinRangeDst = testGenericQuantity.getMinRange();
      util::quantity::Quantity::value_t resultMaxRangeSrc = testGenericQuantitySource.getMaxRange();
      util::quantity::Quantity::value_t resultMaxRangeDst = testGenericQuantity.getMaxRange();
      TEST_ASSERT(validationResultSrc);
      TEST_ASSERT(validationResultDst);
      TEST_ASSERT(resultIdSrc == resultIdDst);
      TEST_ASSERT(resultIdGroupSrc == resultIdGroupDst);
      TEST_ASSERT(resultTimestampSrc == resultTimestampDst);
      TEST_ASSERT(resultValueSrc == resultValueDst);
      TEST_ASSERT(resultUnitSrc == resultUnitDst);
      TEST_ASSERT(resultMinRangeSrc == resultMinRangeDst);
      TEST_ASSERT(resultMaxRangeSrc == resultMaxRangeDst);
      TEST_FUNC_END();
    }
    static void test_initFromParametersConstructor_unitTextNull_expectEmptyUnitText(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Quantity::text_t emptyUnitText;
      //act
      util::quantity::Generic testGenericQuantity(id, idGroup, value, emptyUnitText, timestamp);
      //assert
      TEST_ASSERT(testGenericQuantity.validate());
      TEST_ASSERT(!testGenericQuantity.getUnitText());
      TEST_FUNC_END();
    }
    static void test_initFromParametersConstructor_valueExceedsMinRange_expectValueLimitedToMinRange() {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Quantity::value_t minRange(10);
      util::quantity::Quantity::value_t maxRange(100);
      const boolean valueValid = true;
      util::quantity::Generic testGenericQuantity(id, idGroup, value, unit, timestamp, valueValid, minRange, maxRange);
      //act
      boolean validationResult = testGenericQuantity.validate();
      util::quantity::Quantity::value_t resultValue = testGenericQuantity.getValue();
      util::quantity::Quantity::value_t resultMinRange = testGenericQuantity.getMinRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(resultValue == resultMinRange);
      TEST_FUNC_END();
    }
    static void test_initFromParametersConstructor_valueExceedsMaxRange_expectValueLimitedToMaxRange() {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(105);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Quantity::value_t minRange(0);
      util::quantity::Quantity::value_t maxRange(100);
      const boolean valueValid = true;
      util::quantity::Generic testGenericQuantity(id, idGroup, value, unit, timestamp, valueValid, minRange, maxRange);
      //act
      boolean validationResult = testGenericQuantity.validate();
      util::quantity::Quantity::value_t resultValue = testGenericQuantity.getValue();
      util::quantity::Quantity::value_t resultMaxRange = testGenericQuantity.getMaxRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(resultValue == resultMaxRange);
      TEST_FUNC_END();
    }
    static void test_constructors(void) {
      test_copyConstructor_copyFromGenericQuantity_expectSameValues();
      test_initFromParametersConstructor_unitTextNull_expectEmptyUnitText();
      test_initFromParametersConstructor_valueExceedsMinRange_expectValueLimitedToMinRange();
      test_initFromParametersConstructor_valueExceedsMaxRange_expectValueLimitedToMaxRange();
    }
  public:
    static void test_assignment_assignFromGenericToGeneric_expectSameValues() {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Quantity::value_t minRange(1);
      util::quantity::Quantity::value_t maxRange(100);
      util::quantity::Quantity::text_t unit("test");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      boolean validity = true;
      util::quantity::Generic testSrc(id, idGroup, value, unit, timestamp, validity, minRange, maxRange);
      util::quantity::Generic testDst(1, 2, 3, util::StrRef("123"));
      //act
      testDst = testSrc;
      //assert
      boolean validationResultSrc = testSrc.validate();
      boolean validationResultDst = testDst.validate();
      util::quantity::Quantity::id_t resultIdSrc = testSrc.getId();
      util::quantity::Quantity::id_t resultIdDst = testDst.getId();
      util::quantity::Quantity::id_t resultIdGroupSrc = testSrc.getIdGroup();
      util::quantity::Quantity::id_t resultIdGroupDst = testDst.getIdGroup();
      util::quantity::Quantity::timestamp_t resultTimestampSrc = testSrc.getTimestamp();
      util::quantity::Quantity::timestamp_t resultTimestampDst = testDst.getTimestamp();
      util::quantity::Quantity::value_t resultValueSrc = testSrc.getValue();
      util::quantity::Quantity::value_t resultValueDst = testDst.getValue();
      util::quantity::Quantity::text_t resultUnitSrc = testSrc.getUnitText();
      util::quantity::Quantity::text_t resultUnitDst = testDst.getUnitText();
      util::quantity::Quantity::value_t resultMinRangeSrc = testSrc.getMinRange();
      util::quantity::Quantity::value_t resultMinRangeDst = testDst.getMinRange();
      util::quantity::Quantity::value_t resultMaxRangeSrc = testSrc.getMaxRange();
      util::quantity::Quantity::value_t resultMaxRangeDst = testDst.getMaxRange();
      TEST_ASSERT(validationResultSrc);
      TEST_ASSERT(validationResultDst);
      TEST_ASSERT(resultIdSrc == resultIdDst);
      TEST_ASSERT(resultIdGroupSrc == resultIdGroupDst);
      TEST_ASSERT(resultTimestampSrc == resultTimestampDst);
      TEST_ASSERT(resultValueSrc == resultValueDst);
      TEST_ASSERT(resultUnitSrc == resultUnitDst);
      TEST_ASSERT(resultMinRangeSrc == resultMinRangeDst);
      TEST_ASSERT(resultMaxRangeSrc == resultMaxRangeDst);
      TEST_FUNC_END();
    }
    static void test_assignment(void) {
      test_assignment_assignFromGenericToGeneric_expectSameValues();
    }
  public:
    static void runTests(void) {
      test_validate();
      test_accessors();
      test_constructors();
      test_assignment();
    }
};

class TestDimensionless {
  public:
    static void init_DimensionlessQuantity_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Dimensionless::Unit unit = util::quantity::Dimensionless::Unit::NONE;
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Quantity::value_t minRange(1);
      util::quantity::Quantity::value_t maxRange(100);
      boolean validity = true;
      //act
      util::quantity::Dimensionless testDimensionlessQuantity(id, idGroup, value, unit, timestamp, validity, minRange, maxRange);
      //assert
      boolean validationResult = testDimensionlessQuantity.validate();
      util::quantity::Quantity::id_t resultId = testDimensionlessQuantity.getId();
      util::quantity::Quantity::id_t resultIdGroup = testDimensionlessQuantity.getIdGroup();
      util::quantity::Quantity::timestamp_t resultTimestamp = testDimensionlessQuantity.getTimestamp();
      util::quantity::Quantity::value_t resultValue = testDimensionlessQuantity.getValue();
      util::quantity::Quantity::text_t resultUnit = testDimensionlessQuantity.getUnitText();
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(idGroup == resultIdGroup);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(util::quantity::Dimensionless::getUnitTextByUnit(unit) == resultUnit);
      TEST_FUNC_END();
    }
    static void test_init(void) {
      init_DimensionlessQuantity_expectCorrectData();
    }
  public:
    static void conversion_convertFromNoneToPercentAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(4);
      util::quantity::Quantity::id_t idGroup(5);
      util::quantity::Quantity::value_t value(2);
      util::quantity::Dimensionless::Unit unit = util::quantity::Dimensionless::Unit::NONE;
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      boolean valid = true;
      util::quantity::Quantity::value_t minRange(1);
      util::quantity::Quantity::value_t maxRange(3);
      util::quantity::Dimensionless testDimensionlessQuantity(id, idGroup, value, unit, timestamp, valid, minRange, maxRange);
      static const size_t testValuesCount = 3;
      util::quantity::Quantity::value_t result[testValuesCount] = {};
      util::StrRef resultUnit[testValuesCount] = {};
      util::quantity::Quantity::value_t resultMinRange[testValuesCount] = {};
      util::quantity::Quantity::value_t resultMaxRange[testValuesCount] = {};
      util::quantity::Quantity::value_t referenceResult[testValuesCount] = {
        util::quantity::Quantity::value_t(2),
        util::quantity::Quantity::value_t(200),
        util::quantity::Quantity::value_t(2)
      };
      util::quantity::Quantity::value_t referenceMinRange[testValuesCount] = {
        util::quantity::Quantity::value_t(1),
        util::quantity::Quantity::value_t(100),
        util::quantity::Quantity::value_t(1)
      };
      util::quantity::Quantity::value_t referenceMaxRange[testValuesCount] = {
        util::quantity::Quantity::value_t(3),
        util::quantity::Quantity::value_t(300),
        util::quantity::Quantity::value_t(3)
      };
      util::StrRef referenceResultUnit[testValuesCount] = {
        util::quantity::Dimensionless::getUnitTextByUnit(util::quantity::Dimensionless::Unit::NONE),
        util::quantity::Dimensionless::getUnitTextByUnit(util::quantity::Dimensionless::Unit::PERCENT),
        util::quantity::Dimensionless::getUnitTextByUnit(util::quantity::Dimensionless::Unit::NONE)
      };
      //act
      boolean validationResult = testDimensionlessQuantity.validate();
      result[0] = testDimensionlessQuantity.getValue();
      resultUnit[0] = testDimensionlessQuantity.getUnitText();
      resultMinRange[0] = testDimensionlessQuantity.getMinRange();
      resultMaxRange[0] = testDimensionlessQuantity.getMaxRange();
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::PERCENT);
      result[1] = testDimensionlessQuantity.getValue();
      resultUnit[1] = testDimensionlessQuantity.getUnitText();
      resultMinRange[1] = testDimensionlessQuantity.getMinRange();
      resultMaxRange[1] = testDimensionlessQuantity.getMaxRange();
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::NONE);
      result[2] = testDimensionlessQuantity.getValue();
      resultUnit[2] = testDimensionlessQuantity.getUnitText();
      resultMinRange[2] = testDimensionlessQuantity.getMinRange();
      resultMaxRange[2] = testDimensionlessQuantity.getMaxRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(resultUnit[0] == referenceResultUnit[0]);
      TEST_ASSERT(resultMinRange[0] == referenceMinRange[0]);
      TEST_ASSERT(resultMaxRange[0] == referenceMaxRange[0]);
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(resultUnit[1] == referenceResultUnit[1]);
      TEST_ASSERT(resultMinRange[1] == referenceMinRange[1]);
      TEST_ASSERT(resultMaxRange[1] == referenceMaxRange[1]);
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(resultUnit[2] == referenceResultUnit[2]);
      TEST_ASSERT(resultMinRange[2] == referenceMinRange[2]);
      TEST_ASSERT(resultMaxRange[2] == referenceMaxRange[2]);
      TEST_FUNC_END();
    }
    static void conversion_convertFromPercentToNoneAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(4);
      util::quantity::Quantity::id_t idGroup(5);
      util::quantity::Quantity::value_t value(200);
      util::quantity::Dimensionless::Unit unit = util::quantity::Dimensionless::Unit::PERCENT;
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      boolean valid = true;
      util::quantity::Quantity::value_t minRange(100);
      util::quantity::Quantity::value_t maxRange(300);
      util::quantity::Dimensionless testDimensionlessQuantity(id, idGroup, value, unit, timestamp, valid, minRange, maxRange);
      static const size_t testValuesCount = 3;
      util::quantity::Quantity::value_t result[testValuesCount] = {};
      util::StrRef resultUnit[testValuesCount] = {};
      const util::quantity::Quantity::value_t referenceResult[testValuesCount] = {
        util::quantity::Quantity::value_t(200),
        util::quantity::Quantity::value_t(2),
        util::quantity::Quantity::value_t(200)
      };
      util::quantity::Quantity::value_t referenceMinRange[testValuesCount] = {
        util::quantity::Quantity::value_t(100),
        util::quantity::Quantity::value_t(1),
        util::quantity::Quantity::value_t(100)
      };
      util::quantity::Quantity::value_t referenceMaxRange[testValuesCount] = {
        util::quantity::Quantity::value_t(300),
        util::quantity::Quantity::value_t(3),
        util::quantity::Quantity::value_t(300)
      };
      util::StrRef referenceResultUnit[testValuesCount] = {
        util::quantity::Dimensionless::getUnitTextByUnit(util::quantity::Dimensionless::Unit::PERCENT),
        util::quantity::Dimensionless::getUnitTextByUnit(util::quantity::Dimensionless::Unit::NONE),
        util::quantity::Dimensionless::getUnitTextByUnit(util::quantity::Dimensionless::Unit::PERCENT)
      };
      util::quantity::Quantity::value_t resultMinRange[testValuesCount] = {};
      util::quantity::Quantity::value_t resultMaxRange[testValuesCount] = {};
      //act
      boolean validationResult = testDimensionlessQuantity.validate();
      result[0] = testDimensionlessQuantity.getValue();
      resultUnit[0] = testDimensionlessQuantity.getUnitText();
      resultMinRange[0] = testDimensionlessQuantity.getMinRange();
      resultMaxRange[0] = testDimensionlessQuantity.getMaxRange();
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::NONE);
      result[1] = testDimensionlessQuantity.getValue();
      resultUnit[1] = testDimensionlessQuantity.getUnitText();
      resultMinRange[1] = testDimensionlessQuantity.getMinRange();
      resultMaxRange[1] = testDimensionlessQuantity.getMaxRange();
      testDimensionlessQuantity.convertToUnit(util::quantity::Dimensionless::Unit::PERCENT);
      result[2] = testDimensionlessQuantity.getValue();
      resultUnit[2] = testDimensionlessQuantity.getUnitText();
      resultMinRange[2] = testDimensionlessQuantity.getMinRange();
      resultMaxRange[2] = testDimensionlessQuantity.getMaxRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(resultUnit[0] == referenceResultUnit[0]);
      TEST_ASSERT(resultMinRange[0] == referenceMinRange[0]);
      TEST_ASSERT(resultMaxRange[0] == referenceMaxRange[0]);
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(resultUnit[1] == referenceResultUnit[1]);
      TEST_ASSERT(resultMinRange[1] == referenceMinRange[1]);
      TEST_ASSERT(resultMaxRange[1] == referenceMaxRange[1]);
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(resultUnit[2] == referenceResultUnit[2]);
      TEST_ASSERT(resultMinRange[2] == referenceMinRange[2]);
      TEST_ASSERT(resultMaxRange[2] == referenceMaxRange[2]);
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
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Temperature::Unit unit = util::quantity::Temperature::Unit::CELSIUS;
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      //act
      util::quantity::Temperature testTemperature(id, idGroup, value, unit, timestamp);
      //assert
      boolean validationResult = testTemperature.validate();
      util::quantity::Quantity::id_t resultId = testTemperature.getId();
      util::quantity::Quantity::id_t resultIdGroup = testTemperature.getIdGroup();
      util::quantity::Quantity::timestamp_t resultTimestamp = testTemperature.getTimestamp();
      util::quantity::Quantity::value_t resultValue = testTemperature.getValue();
      util::quantity::Quantity::text_t resultUnit = testTemperature.getUnitText();
      TEST_ASSERT(validationResult);
      TEST_ASSERT(id == resultId);
      TEST_ASSERT(idGroup == resultIdGroup);
      TEST_ASSERT(resultTimestamp == timestamp);
      TEST_ASSERT(resultValue == value);
      TEST_ASSERT(util::quantity::Temperature::getUnitTextByUnit(unit) == resultUnit);
      TEST_FUNC_END();
    }
    static void test_init(void) {
      init_Temperature_expectCorrectData();
    }
  public:
    static void conversion_convertFromCelsiusToFahrenheitAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(4);
      util::quantity::Quantity::id_t idGroup(5);
      util::quantity::Quantity::value_t value(100);
      util::quantity::Temperature::Unit unit = util::quantity::Temperature::Unit::CELSIUS;
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      boolean valid = true;
      util::quantity::Quantity::value_t minRange(10);
      util::quantity::Quantity::value_t maxRange(200);
      util::quantity::Temperature testTemperature(id, idGroup, value, unit, timestamp, valid, minRange, maxRange);
      static const size_t testValuesCount = 3;
      util::quantity::Quantity::value_t result[testValuesCount] = {};
      util::quantity::Quantity::value_t resultMinRange[testValuesCount] = {};
      util::quantity::Quantity::value_t resultMaxRange[testValuesCount] = {};
      util::StrRef resultUnit[testValuesCount] = {};
      const util::quantity::Quantity::value_t referenceResult[testValuesCount] = {
        util::quantity::Quantity::value_t(100),
        util::quantity::Quantity::value_t(212),
        util::quantity::Quantity::value_t(100)
      };
      util::quantity::Quantity::value_t referenceMinRange[testValuesCount] = {
        util::quantity::Quantity::value_t(10),
        util::quantity::Quantity::value_t(50),
        util::quantity::Quantity::value_t(10)
      };
      util::quantity::Quantity::value_t referenceMaxRange[testValuesCount] = {
        util::quantity::Quantity::value_t(200),
        util::quantity::Quantity::value_t(392),
        util::quantity::Quantity::value_t(200)
      };
      util::StrRef referenceResultUnit[testValuesCount] = {
        util::quantity::Quantity::text_t(util::quantity::Temperature::getUnitTextByUnit(util::quantity::Temperature::Unit::CELSIUS)),
        util::quantity::Quantity::text_t(util::quantity::Temperature::getUnitTextByUnit(util::quantity::Temperature::Unit::FAHRENHEIT)),
        util::quantity::Quantity::text_t(util::quantity::Temperature::getUnitTextByUnit(util::quantity::Temperature::Unit::CELSIUS))
      };
      const util::quantity::Quantity::value_t tolerance(1, 1); //0.1
      //act
      boolean validationResult = testTemperature.validate();
      result[0] = testTemperature.getValue();
      resultUnit[0] = testTemperature.getUnitText();
      resultMinRange[0] = testTemperature.getMinRange();
      resultMaxRange[0] = testTemperature.getMaxRange();
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::FAHRENHEIT);
      result[1] = testTemperature.getValue();
      resultUnit[1] = testTemperature.getUnitText();
      resultMinRange[1] = testTemperature.getMinRange();
      resultMaxRange[1] = testTemperature.getMaxRange();
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::CELSIUS);
      result[2] = testTemperature.getValue();
      resultUnit[2] = testTemperature.getUnitText();
      resultMinRange[2] = testTemperature.getMinRange();
      resultMaxRange[2] = testTemperature.getMaxRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(resultUnit[0] == referenceResultUnit[0]);
      TEST_ASSERT(resultMinRange[0] == referenceMinRange[0]);
      TEST_ASSERT(resultMaxRange[0] == referenceMaxRange[0]);
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(resultUnit[1] == referenceResultUnit[1]);
      TEST_ASSERT(resultMinRange[1] == referenceMinRange[1]);
      TEST_ASSERT(resultMaxRange[1] == referenceMaxRange[1]);
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(resultUnit[2] == referenceResultUnit[2]);
      TEST_ASSERT(resultMinRange[2] == referenceMinRange[2]);
      TEST_ASSERT(resultMaxRange[2] == referenceMaxRange[2]);
      TEST_FUNC_END();
    }
    static void conversion_convertFromFahrenheitToCelsiusAndBack_expectCorrectData(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(4);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(86);
      util::quantity::Temperature::Unit unit = util::quantity::Temperature::Unit::FAHRENHEIT;
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      boolean valid = true;
      util::quantity::Quantity::value_t minRange(50);
      util::quantity::Quantity::value_t maxRange(392);
      util::quantity::Temperature testTemperature(id, idGroup, value, unit, timestamp, valid, minRange, maxRange);
      static const size_t testValuesCount = 3;
      util::quantity::Quantity::value_t result[testValuesCount] = {};
      util::StrRef resultUnit[testValuesCount] = {};
      util::quantity::Quantity::value_t resultMinRange[testValuesCount] = {};
      util::quantity::Quantity::value_t resultMaxRange[testValuesCount] = {};
      const util::quantity::Quantity::value_t referenceResult[testValuesCount] = {
        util::quantity::Quantity::value_t(86),
        util::quantity::Quantity::value_t(30),
        util::quantity::Quantity::value_t(86)
      };
      util::quantity::Quantity::value_t referenceMinRange[testValuesCount] = {
        util::quantity::Quantity::value_t(50),
        util::quantity::Quantity::value_t(10),
        util::quantity::Quantity::value_t(50)
      };
      util::quantity::Quantity::value_t referenceMaxRange[testValuesCount] = {
        util::quantity::Quantity::value_t(392),
        util::quantity::Quantity::value_t(200),
        util::quantity::Quantity::value_t(392)
      };
      util::StrRef referenceResultUnit[testValuesCount] = {
        util::quantity::Quantity::text_t(util::quantity::Temperature::getUnitTextByUnit(util::quantity::Temperature::Unit::FAHRENHEIT)),
        util::quantity::Quantity::text_t(util::quantity::Temperature::getUnitTextByUnit(util::quantity::Temperature::Unit::CELSIUS)),
        util::quantity::Quantity::text_t(util::quantity::Temperature::getUnitTextByUnit(util::quantity::Temperature::Unit::FAHRENHEIT))
      };
      //act
      boolean validationResult = testTemperature.validate();
      result[0] = testTemperature.getValue();
      resultUnit[0] = testTemperature.getUnitText();
      resultMinRange[0] = testTemperature.getMinRange();
      resultMaxRange[0] = testTemperature.getMaxRange();
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::CELSIUS);
      result[1] = testTemperature.getValue();
      resultUnit[1] = testTemperature.getUnitText();
      resultMinRange[1] = testTemperature.getMinRange();
      resultMaxRange[1] = testTemperature.getMaxRange();
      testTemperature.convertToUnit(util::quantity::Temperature::Unit::FAHRENHEIT);
      result[2] = testTemperature.getValue();
      resultUnit[2] = testTemperature.getUnitText();
      resultMinRange[2] = testTemperature.getMinRange();
      resultMaxRange[2] = testTemperature.getMaxRange();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result[0] == referenceResult[0]);
      TEST_ASSERT(resultUnit[0] == referenceResultUnit[0]);
      TEST_ASSERT(resultMinRange[0] == referenceMinRange[0]);
      TEST_ASSERT(resultMaxRange[0] == referenceMaxRange[0]);
      TEST_ASSERT(result[1] == referenceResult[1]);
      TEST_ASSERT(resultUnit[1] == referenceResultUnit[1]);
      TEST_ASSERT(resultMinRange[1] == referenceMinRange[1]);
      TEST_ASSERT(resultMaxRange[1] == referenceMaxRange[1]);
      TEST_ASSERT(result[2] == referenceResult[2]);
      TEST_ASSERT(resultUnit[2] == referenceResultUnit[2]);
      TEST_ASSERT(resultMinRange[2] == referenceMinRange[2]);
      TEST_ASSERT(resultMaxRange[2] == referenceMaxRange[2]);
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
      static const size_t numClasses = 3;
      Matrix<boolean> referenceClassMatchMatrix(numClasses, numClasses);
      for (size_t i = 0; i < numClasses; i++)
        for (size_t j = 0; j < numClasses; j++)
          referenceClassMatchMatrix.setValue(i, j, i == j ? true : false);
      //act
      const TestIntrospectionClassMatchMatrix<util::quantity::Quantity,
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
      static const size_t numObjects = 6;
      util::quantity::Generic g1(1, 2, 3, "unit1");
      util::quantity::Generic g2(4, 5, 6, "unit2");
      util::quantity::Dimensionless d1(1, 2, 3, util::quantity::Dimensionless::Unit::NONE);
      util::quantity::Dimensionless d2(4, 5, 6, util::quantity::Dimensionless::Unit::PERCENT);
      util::quantity::Temperature t1(1, 2, 3, util::quantity::Temperature::Unit::CELSIUS);
      util::quantity::Temperature t2(4, 5, 6, util::quantity::Temperature::Unit::FAHRENHEIT);
      util::quantity::Quantity * testObject1 = &g1;
      util::quantity::Quantity * testObject2 = &g2;
      util::quantity::Quantity * testObject3 = &d1;
      util::quantity::Quantity * testObject4 = &d2;
      util::quantity::Quantity * testObject5 = &t1;
      util::quantity::Quantity * testObject6 = &t2;

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
        testObject6);
      //assert
      TEST_ASSERT(testObjectMatchMatrix == referenceObjectMatchMatrix);
      TEST_FUNC_END();
    }
    static void testClassTypeAndObjectTypeMatch(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t numObjects = 6;
      static const size_t numClasses = 3;
      util::quantity::Generic g1(1, 2, 3, "unit1");
      util::quantity::Generic g2(4, 5, 6, "unit2");
      util::quantity::Dimensionless d1(1, 2, 3, util::quantity::Dimensionless::Unit::NONE);
      util::quantity::Dimensionless d2(4, 5, 6, util::quantity::Dimensionless::Unit::PERCENT);
      util::quantity::Temperature t1(1, 2, 3, util::quantity::Temperature::Unit::CELSIUS);
      util::quantity::Temperature t2(4, 5, 6, util::quantity::Temperature::Unit::FAHRENHEIT);
      util::quantity::Quantity * testObject1 = &g1;
      util::quantity::Quantity * testObject2 = &g2;
      util::quantity::Quantity * testObject3 = &d1;
      util::quantity::Quantity * testObject4 = &d2;
      util::quantity::Quantity * testObject5 = &t1;
      util::quantity::Quantity * testObject6 = &t2;
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
      //act
      const TestIntrospectionClassVsObjectMatchMatrix<util::quantity::Quantity,
            util::quantity::Generic,
            util::quantity::Dimensionless,
            util::quantity::Temperature
            > testClassVsObjectMatchMatrix (
              testObject1,
              testObject2,
              testObject3,
              testObject4,
              testObject5,
              testObject6
            );
      //assert
      TEST_ASSERT(testClassVsObjectMatchMatrix == referenceClassVsObjectMatchMatrix);
      TEST_FUNC_END();
    }
    static void testCopiedObjectTypeMatch(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Generic testGeneric(0, 1, 2, "test");
      util::quantity::Dimensionless testDimensionless(1, 2, 3, util::quantity::Dimensionless::Unit::NONE);
      util::quantity::Temperature testTemperature(2, 3, 4, util::quantity::Temperature::Unit::CELSIUS);
      util::quantity::Generic testGenericCopy(testGeneric);
      util::quantity::Dimensionless testDimensionlessCopy(testDimensionless);
      util::quantity::Temperature testTemperatureCopy(testTemperature);
      util::quantity::Quantity * testGenericRef = &testGeneric;
      util::quantity::Quantity * testDimensionlessRef = &testDimensionless;
      util::quantity::Quantity * testTemperatureRef = &testTemperature;
      util::quantity::Quantity testGenericQuantity(*testGenericRef);
      util::quantity::Quantity testDimensionlessQuantity(*testDimensionlessRef);
      util::quantity::Quantity testTemperatureQuantity(*testTemperatureRef);
      //act
      boolean genericCopyTest = (testGenericCopy.INTROSPECT_OBJECT() == INTROSPECT_CLASS(util::quantity::Generic));
      boolean dimensionlessCopyTest = (testDimensionlessCopy.INTROSPECT_OBJECT() == INTROSPECT_CLASS(util::quantity::Dimensionless));
      boolean temperatureCopyTest = (testTemperatureCopy.INTROSPECT_OBJECT() == INTROSPECT_CLASS(util::quantity::Temperature));
      boolean genericObjectTest = (testGenericQuantity.INTROSPECT_OBJECT() == INTROSPECT_CLASS(util::quantity::Generic));
      boolean dimensionlessObjectTest = (testDimensionlessQuantity.INTROSPECT_OBJECT() == INTROSPECT_CLASS(util::quantity::Dimensionless));
      boolean temperatureObjectTest = (testTemperatureQuantity.INTROSPECT_OBJECT() == INTROSPECT_CLASS(util::quantity::Temperature));
      //assert
      TEST_ASSERT(INTROSPECT_CLASS(util::quantity::Quantity) != INTROSPECT_CLASS(util::quantity::Generic));
      TEST_ASSERT(INTROSPECT_CLASS(util::quantity::Quantity) != INTROSPECT_CLASS(util::quantity::Dimensionless));
      TEST_ASSERT(INTROSPECT_CLASS(util::quantity::Generic) != INTROSPECT_CLASS(util::quantity::Dimensionless));
      TEST_ASSERT(genericCopyTest);
      TEST_ASSERT(dimensionlessCopyTest);
      TEST_ASSERT(temperatureCopyTest);
      TEST_ASSERT(genericObjectTest);
      TEST_ASSERT(dimensionlessObjectTest);
      TEST_ASSERT(temperatureObjectTest);
      TEST_FUNC_END();
    }
    static void testIntrospection(void) {
      testClassTypeMatch();
      testObjectTypeMatch();
      testClassTypeAndObjectTypeMatch();
      testCopiedObjectTypeMatch();
    }
  public:
    static void testReflectionGeneric(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::StrRef unit("gnrc");
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Generic testGeneric(id, idGroup, value, unit, timestamp);
      util::quantity::Quantity * testQuantity = &testGeneric;
      //act
      util::quantity::Generic testReflected = (*REFLECT_OBJECT (util::quantity::Generic, testQuantity));
      util::quantity::Dimensionless testReflectedOther1 = (*REFLECT_OBJECT (util::quantity::Dimensionless, testQuantity));
      util::quantity::Temperature   testReflectedOther2 = (*REFLECT_OBJECT (util::quantity::Temperature, testQuantity));
      //assert
      TEST_ASSERT(VALIDATE_OBJECT_TYPE(testReflected));
      TEST_ASSERT(testReflected.validate());
      TEST_ASSERT(testReflected.getId() == id);
      TEST_ASSERT(testReflected.getIdGroup() == idGroup);
      TEST_ASSERT(testReflected.getTimestamp() == timestamp);
      TEST_ASSERT(testReflected.getValue() == value);
      TEST_ASSERT(testReflected.getUnitText() == unit);
      TEST_ASSERT(!VALIDATE_OBJECT_TYPE(testReflectedOther1));
      TEST_ASSERT(!VALIDATE_OBJECT_TYPE(testReflectedOther2));
      TEST_FUNC_END();
    }
    static void testReflectionDimensionless(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Dimensionless::Unit unit = util::quantity::Dimensionless::Unit::NONE;
      util::quantity::Quantity::text_t referenceUnitText = util::quantity::Dimensionless::getUnitTextByUnit(unit);
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Dimensionless testDimensionless(id, idGroup, value, unit, timestamp);
      util::quantity::Quantity * testQuantity = &testDimensionless;
      //act
      util::quantity::Dimensionless testReflected       = (*REFLECT_OBJECT (util::quantity::Dimensionless, testQuantity));
      util::quantity::Generic       testReflectedOther1 = (*REFLECT_OBJECT (util::quantity::Generic, testQuantity));
      util::quantity::Temperature   testReflectedOther2 = (*REFLECT_OBJECT (util::quantity::Temperature, testQuantity));
      //assert
      TEST_ASSERT(VALIDATE_OBJECT_TYPE(testReflected));
      TEST_ASSERT(testReflected.validate());
      TEST_ASSERT(testReflected.getId() == id);
      TEST_ASSERT(testReflected.getIdGroup() == idGroup);
      TEST_ASSERT(testReflected.getTimestamp() == timestamp);
      TEST_ASSERT(testReflected.getValue() == value);
      TEST_ASSERT(testReflected.getUnitText() == referenceUnitText);
      TEST_ASSERT(!VALIDATE_OBJECT_TYPE(testReflectedOther1));
      TEST_ASSERT(!VALIDATE_OBJECT_TYPE(testReflectedOther2));
      TEST_FUNC_END();
    }
    static void testReflectionTemperature(void) {
      TEST_FUNC_START();
      //arrange
      util::quantity::Quantity::id_t id(3);
      util::quantity::Quantity::id_t idGroup(4);
      util::quantity::Quantity::value_t value(5);
      util::quantity::Temperature::Unit unit = util::quantity::Temperature::Unit::CELSIUS;
      util::quantity::Quantity::text_t referenceUnitText = util::quantity::Temperature::getUnitTextByUnit(unit);
      util::quantity::Quantity::timestamp_t timestamp = util::getTimestamp();
      util::quantity::Temperature testTemperature(id, idGroup, value, unit, timestamp);
      util::quantity::Quantity * testQuantity = &testTemperature;
      //act
      util::quantity::Temperature testReflected (*REFLECT_OBJECT (util::quantity::Temperature, testQuantity));
      util::quantity::Generic       testReflectedOther1 (*REFLECT_OBJECT (util::quantity::Generic, testQuantity));
      util::quantity::Dimensionless testReflectedOther2 (*REFLECT_OBJECT (util::quantity::Dimensionless, testQuantity));
      //assert
      TEST_ASSERT(VALIDATE_OBJECT_TYPE(testReflected));
      TEST_ASSERT(testReflected.getId() == id);
      TEST_ASSERT(testReflected.getIdGroup() == idGroup);
      TEST_ASSERT(testReflected.getTimestamp() == timestamp);
      TEST_ASSERT(testReflected.getValue() == value);
      TEST_ASSERT(testReflected.getUnitText() == referenceUnitText);
      TEST_ASSERT(!VALIDATE_OBJECT_TYPE(testReflectedOther1));
      TEST_ASSERT(!VALIDATE_OBJECT_TYPE(testReflectedOther2));
      TEST_FUNC_END();
    }
    static void testReflection(void) {
      testReflectionGeneric();
      testReflectionDimensionless();
      testReflectionTemperature();
    }

  public:
    static void runTests(void) {
      testIntrospection();
      testReflection();
    }
};

static void CheckClassSizes(void) {
  TEST_PRINTLN_DATA(sizeof(util::quantity::Quantity));
  TEST_PRINTLN_DATA(sizeof(util::quantity::Generic));
  TEST_PRINTLN_DATA(sizeof(util::quantity::Dimensionless));
  TEST_PRINTLN_DATA(sizeof(util::quantity::Temperature));
}

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestQuantityAndGeneric::runTests();
  TestDimensionless::runTests();
  TestTemperature::runTests();
  TestQuantityIntrospectionReflection::runTests();
  CheckClassSizes();
  TEST_END();
}

void loop() {
  delay(100);
}
