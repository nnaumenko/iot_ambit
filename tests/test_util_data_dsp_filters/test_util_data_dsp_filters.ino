#include <ESP8266WiFi.h>

#include "test.h"
#include "util_data.h"

#include "testdata.h"

class AnalogFilterTestSet {
  public:
    AnalogFilterTestSet(util::dsp::Filter::timestamp_t deltaTime, size_t decimals, const int32_t * testPointsProgmem, size_t testPointsNumber) {
      size = testPointsNumber;
      this->points = new util::dsp::Filter::value_t[size];
      this->deltaTime = deltaTime;
      for (size_t i = 0; i < size; i++) {
        int32_t tempPoint;
        memcpy_P (&tempPoint, &testPointsProgmem[i], sizeof(tempPoint));
        setPoint(i, util::dsp::Filter::value_t(tempPoint, decimals));
      }
      makeReadOnly();
    }
    AnalogFilterTestSet(size_t size) {
      this->size = size;
      this->points = new util::dsp::Filter::value_t[size];
      this->deltaTime = 0;
    }
    ~AnalogFilterTestSet() {
      if (points) delete(points);
    }
  public:
    void makeReadOnly(void) {
      readOnly = true;
    }
  public:
    size_t getSize(void) const {
      return (size);
    }
    util::dsp::Filter::timestamp_t getDeltaTime(void) const {
      return (deltaTime);
    }
    void setDeltaTime (util::dsp::Filter::timestamp_t deltaTime) {
      if (readOnly) return;
      this->deltaTime = deltaTime;
    }
    const util::dsp::Filter::value_t& getPoint(size_t index) const {
      static const util::dsp::Filter::value_t analogZero(0);
      if (index >= size) return (analogZero);
      return (points[index]);
    }
  public:
    boolean setPoint (size_t index, const util::dsp::Filter::value_t &value) {
      if (!points) return (false);
      if (readOnly) return (false);
      if (index >= size) return (false);
      points[index] = value;
      return (true);
    }
  private:
    size_t size = 0;
    util::dsp::Filter::value_t * points = nullptr;
    util::dsp::Filter::timestamp_t deltaTime = util::dsp::Filter::timestamp_t(0);
    boolean readOnly = false;
};

class AnalogFilterTester {
  public:
    static boolean feed(util::dsp::Filter &filter,
                        const AnalogFilterTestSet &inputSet,
                        AnalogFilterTestSet &outputSet) {
      if (inputSet.getSize() != outputSet.getSize()) return (false);
      if (inputSet.getDeltaTime() != outputSet.getDeltaTime()) return (false);
      util::dsp::Filter::timestamp_t currentTimestamp = inputSet.getDeltaTime();
      for (size_t i = 0; i < outputSet.getSize(); i++, currentTimestamp += inputSet.getDeltaTime()) {
        outputSet.setPoint(i, filter.filter(currentTimestamp, inputSet.getPoint(i)));
      }
      return (true);
    }
    static util::dsp::Filter::value_t compare (const AnalogFilterTestSet&outputSet,
                                     const AnalogFilterTestSet&referenceSet) {
      util::dsp::Filter::value_t total(0);
      if (referenceSet.getSize() != outputSet.getSize()) return (false);
      if (referenceSet.getDeltaTime() != outputSet.getDeltaTime()) return (false);
      for (size_t i = 0; i < outputSet.getSize(); i++) {
        util::dsp::Filter::value_t difference (referenceSet.getPoint(i) - outputSet.getPoint(i));
        total += difference * difference;
      }
      return (total);
    }
};

namespace util {
namespace dsp {
class FakeFilter : public Filter {
  public:
    FakeFilter(Filter::Status initStatus = Filter::Status::OK,
                       size_t minInputs = 0,
                       size_t maxInputs = 0) {
      Filter::setInitStatus(initStatus);
      if (minInputs && maxInputs) {
        Filter::setInputsNumber(minInputs, maxInputs);
      }
      for (size_t i = 0; i < inputStorageSize; i++) {
        for (size_t j = 0; j < maxInputs; j++)
          inputValues[i][j] = value_t(0);
        inputTimestamps[i] = timestamp_t(0);
        inputNumbers[i] = 0;
      }
    }
    virtual ~FakeFilter() {}
  private:
    virtual Filter::Status filterProcess(const value_t *inputs, size_t inputsNumber, value_t &output, Timestamp timestamp) {
      if (inputValuesStored >= inputStorageSize) return (status);
      inputNumbers[inputValuesStored] = inputsNumber <= maxInputs ? inputsNumber : maxInputs;
      for (size_t i = 0; i < inputNumbers[inputValuesStored]; i++) {
        inputValues[inputValuesStored][i] = inputs[i];
      }
      inputTimestamps[inputValuesStored] = timestamp;
      inputValuesStored++;
      output = outputValue;
      return (status);
    }
  public:
    void setStatus(Filter::Status status) {
      this->status = status;
    }
    timestamp_t getInputTimestamp(size_t index) const {
      if (index >= inputValuesStored) return (timestamp_t(0));
      return (inputTimestamps[index]);
    }
    size_t getInputValuesNumber(void) const {
      return (inputValuesStored);
    }
    const value_t& getInputValue(size_t index, size_t inputNo = 0) const {
      static const value_t tZero = value_t(0);
      if (index >= inputValuesStored) return (tZero);
      if (inputNo >= maxInputs) return (tZero);
      return (inputValues[index][inputNo]);
    }
    void setOutput(const value_t& value) {
      outputValue = value;
    }
    timestamp_t deltaTime(timestamp_t currentTime) {
      return (Filter::getDeltaTime(currentTime));
    }
  private:
    Filter::Status status = (Filter::Status::OK);
    static const size_t maxInputs = 4;
    static const size_t inputStorageSize = 4;
    size_t inputValuesStored = 0;
    Timestamp inputTimestamps[inputStorageSize];
    value_t inputValues[inputStorageSize][maxInputs];
    size_t inputNumbers[inputStorageSize];
    value_t outputValue = value_t(0);
};

}//namespace dsp
}//namespace util

class TestAbstractFilter {
  public:
    static void filter_passInputValue_expectValuePassedToFilter(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFakeFilter(util::dsp::Filter::Status::OK, 1, 3);
      util::dsp::Filter::value_t testValue0 = 7;
      util::dsp::Filter::value_t testValue1 = 10;
      util::dsp::Filter::value_t testValue2 = 3;
      util::dsp::Filter::timestamp_t testTimestamp0 = 100;
      util::dsp::Filter::timestamp_t testTimestamp1 = 130;
      util::dsp::Filter::timestamp_t testTimestamp2 = 500;
      //act
      testFakeFilter.filter(testTimestamp0, testValue0);
      testFakeFilter.filter(testTimestamp1, testValue1);
      testFakeFilter.filter(testTimestamp2, testValue2);
      //assert
      TEST_ASSERT(testFakeFilter.getInputValue(0) == testValue0);
      TEST_ASSERT(testFakeFilter.getInputValue(1) == testValue1);
      TEST_ASSERT(testFakeFilter.getInputValue(2) == testValue2);
      TEST_ASSERT(testFakeFilter.getInputTimestamp(0) == testTimestamp0);
      TEST_ASSERT(testFakeFilter.getInputTimestamp(1) == testTimestamp1);
      TEST_ASSERT(testFakeFilter.getInputTimestamp(2) == testTimestamp2);
      TEST_FUNC_END();
    }
    static void filter_passMultipleValues_expectValuesPassedToFilter(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFakeFilter(util::dsp::Filter::Status::OK, 1, 3);
      util::dsp::Filter::value_t testValue0 = 7;
      util::dsp::Filter::value_t testValue1 = 10;
      util::dsp::Filter::value_t testValue2 = 3;
      util::dsp::Filter::timestamp_t testTimestamp = 100;
      //act
      testFakeFilter.filter(testTimestamp, testValue0, testValue1, testValue2);
      //assert
      TEST_ASSERT(testFakeFilter.getInputValue(0, 0) == testValue0);
      TEST_ASSERT(testFakeFilter.getInputValue(0, 1) == testValue1);
      TEST_ASSERT(testFakeFilter.getInputValue(0, 2) == testValue2);
      TEST_ASSERT(testFakeFilter.getInputValue(0, 3) == static_cast<util::dsp::Filter::value_t>(0));
      TEST_ASSERT(testFakeFilter.getInputTimestamp(0) == testTimestamp);
      TEST_FUNC_END();
    }
    static void filter_getOutputValue_expectOutputValueReturned(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFakeFilter;
      util::dsp::Filter::value_t testInputValue(3);
      util::dsp::Filter::value_t testOutputValue(11);
      testFakeFilter.setOutput(testOutputValue);
      //act
      util::dsp::Filter::value_t result = testFakeFilter.filter(0, testInputValue);
      //assert
      TEST_ASSERT(result == testOutputValue);
      TEST_FUNC_END();
    }
    static void getDeltaTime_firstCall_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFakeFilter;
      util::dsp::Filter::timestamp_t testTimestamp = 100;
      //act
      util::dsp::Filter::timestamp_t resultDeltaTime = testFakeFilter.deltaTime(testTimestamp);
      //assert
      TEST_ASSERT(!resultDeltaTime);
      TEST_FUNC_END();
    }
    static void getDeltaTime_secondCall_expectTimeDifference(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFakeFilter;
      util::dsp::Filter::value_t testValue = 4;
      util::dsp::Filter::timestamp_t testTimestamp0 = 100;
      util::dsp::Filter::timestamp_t testTimestamp1 = 200;
      //act
      testFakeFilter.filter(testTimestamp0, testValue);
      util::dsp::Filter::timestamp_t resultDeltaTime = testFakeFilter.deltaTime(testTimestamp1);
      //assert
      TEST_ASSERT(resultDeltaTime == (testTimestamp1 - testTimestamp0));
      TEST_FUNC_END();
    }
    static void filter_initErrors_expectFilterProcessNotCalled(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFilter1(util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      util::dsp::FakeFilter testFilter2(util::dsp::Filter::Status::ERROR_INIT_NOT_ENOUGH_MEMORY);
      util::dsp::FakeFilter testFilter3(util::dsp::Filter::Status::ERROR_INIT_FAILED);
      //act
      util::dsp::Filter::Status statusBefore1 = testFilter1.getStatus();
      util::dsp::Filter::Status statusBefore2 = testFilter2.getStatus();
      util::dsp::Filter::Status statusBefore3 = testFilter3.getStatus();
      testFilter1.filter(0);
      testFilter2.filter(0);
      testFilter3.filter(0);
      util::dsp::Filter::Status statusAfter1 = testFilter1.getStatus();
      util::dsp::Filter::Status statusAfter2 = testFilter2.getStatus();
      util::dsp::Filter::Status statusAfter3 = testFilter3.getStatus();
      size_t inputs1 = testFilter1.getInputValuesNumber();
      size_t inputs2 = testFilter2.getInputValuesNumber();
      size_t inputs3 = testFilter3.getInputValuesNumber();
      //assert
      TEST_ASSERT(statusBefore1 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(statusBefore2 == util::dsp::Filter::Status::ERROR_INIT_NOT_ENOUGH_MEMORY);
      TEST_ASSERT(statusBefore3 == util::dsp::Filter::Status::ERROR_INIT_FAILED);
      TEST_ASSERT(statusAfter1 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(statusAfter2 == util::dsp::Filter::Status::ERROR_INIT_NOT_ENOUGH_MEMORY);
      TEST_ASSERT(statusAfter3 == util::dsp::Filter::Status::ERROR_INIT_FAILED);
      TEST_ASSERT(!inputs1);
      TEST_ASSERT(!inputs2);
      TEST_ASSERT(!inputs3);
      TEST_FUNC_END();
    }
    static void filter_tooFewInput_expectFilterProcessNotCalled(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFilter(util::dsp::Filter::Status::OK, 1, 2);
      //act
      util::dsp::Filter::Status statusBefore = testFilter.getStatus();
      testFilter.filter(0);
      util::dsp::Filter::Status statusAfter = testFilter.getStatus();
      //assert
      TEST_ASSERT(statusBefore == util::dsp::Filter::Status::OK);
      TEST_ASSERT(statusAfter == util::dsp::Filter::Status::ERROR_TOO_FEW_INPUTS);
      TEST_FUNC_END();
    }
    static void filter_tooManyInput_expectWarningGeneratedIfFilterProcessReturnsOK(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FakeFilter testFilter1(util::dsp::Filter::Status::OK, 1, 2);
      util::dsp::FakeFilter testFilter2(util::dsp::Filter::Status::OK, 1, 2);
      util::dsp::Filter::value_t testValue1 = 1;
      util::dsp::Filter::value_t testValue2 = 2;
      util::dsp::Filter::value_t testValue3 = 3;
      //act
      util::dsp::Filter::Status statusBefore1 = testFilter1.getStatus();
      util::dsp::Filter::Status statusBefore2 = testFilter2.getStatus();
      testFilter1.setStatus(util::dsp::Filter::Status::NONE);
      testFilter1.filter(0, testValue1, testValue2, testValue3);
      testFilter2.filter(0, testValue1, testValue2, testValue3);
      util::dsp::Filter::Status statusAfter1 = testFilter1.getStatus();
      util::dsp::Filter::Status statusAfter2 = testFilter2.getStatus();
      //assert
      TEST_ASSERT(statusBefore1 == util::dsp::Filter::Status::OK);
      TEST_ASSERT(statusBefore2 == util::dsp::Filter::Status::OK);
      TEST_ASSERT(statusAfter1 == util::dsp::Filter::Status::NONE);
      TEST_ASSERT(statusAfter2 == util::dsp::Filter::Status::WARNING_TOO_MANY_INPUTS);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      filter_passInputValue_expectValuePassedToFilter();
      filter_passMultipleValues_expectValuesPassedToFilter();
      filter_getOutputValue_expectOutputValueReturned();
      getDeltaTime_firstCall_expectZero();
      getDeltaTime_secondCall_expectTimeDifference();
      filter_initErrors_expectFilterProcessNotCalled();
      filter_tooFewInput_expectFilterProcessNotCalled();
      filter_tooManyInput_expectWarningGeneratedIfFilterProcessReturnsOK();
    }
};

class TestMovingAverage {
  public:
    static void testPulseStepResponsesAvg2(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t avgPoints = MOVING_AVERAGE_A_POINTS;
      static const util::dsp::Filter::value_t margin(1, 2);//0.01
      util::dsp::Filter * testFilter = new util::dsp::MovingAverage(avgPoints);
      util::dsp::Filter::Status status = testFilter->getStatus();
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT1, DECIMALS_INPUT1, input1, SIZE_INPUT_1);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT1, DECIMALS_OUTPUT1_MOVING_AVERAGE_A, output1_movingAverage_a, SIZE_INPUT_1);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void testPulseStepResponsesAvg16(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t avgPoints = MOVING_AVERAGE_B_POINTS;
      static const util::dsp::Filter::value_t margin(1, 2); //0.01
      util::dsp::Filter * testFilter = new util::dsp::MovingAverage(avgPoints);
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT1, DECIMALS_INPUT1, input1, SIZE_INPUT_1);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT1, DECIMALS_OUTPUT1_MOVING_AVERAGE_B, output1_movingAverage_b, SIZE_INPUT_1);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void testFrequenciesResponsesAvg2(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t avgPoints = MOVING_AVERAGE_A_POINTS;
      static const util::dsp::Filter::value_t margin(1, 2);//0.1
      util::dsp::Filter * testFilter = new util::dsp::MovingAverage(avgPoints);
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT2, DECIMALS_INPUT2, input2, SIZE_INPUT_2);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT2, DECIMALS_OUTPUT2_MOVING_AVERAGE_A, output2_movingAverage_a, SIZE_INPUT_2);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void testFrequenciesResponsesAvg16(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t avgPoints = MOVING_AVERAGE_B_POINTS;
      static const util::dsp::Filter::value_t margin(1, 2);//0.1
      util::dsp::Filter * testFilter = new util::dsp::MovingAverage(avgPoints);
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT2, DECIMALS_INPUT2, input2, SIZE_INPUT_2);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT2, DECIMALS_OUTPUT2_MOVING_AVERAGE_B, output2_movingAverage_b, SIZE_INPUT_2);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void testSumOverflow(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t avgPoints = 4;
      util::dsp::Filter * testFilter1 = new util::dsp::MovingAverage(avgPoints);
      util::dsp::Filter * testFilter2 = new util::dsp::MovingAverage(avgPoints);
      util::dsp::Filter::value_t maxMinusOne = util::dsp::Filter::value_t::max - util::dsp::Filter::value_t(1);
      util::dsp::Filter::value_t minPlusOne = util::dsp::Filter::value_t::min + util::dsp::Filter::value_t(1);
      util::dsp::Filter::value_t zero = util::dsp::Filter::value_t(0);
      util::dsp::Filter::value_t referenceValue1 = maxMinusOne / util::dsp::Filter::value_t(4) + maxMinusOne / util::dsp::Filter::value_t(4);
      util::dsp::Filter::value_t referenceValue2 = minPlusOne / util::dsp::Filter::value_t(4) + minPlusOne / util::dsp::Filter::value_t(4);
      //act
      testFilter1->filter(1, maxMinusOne);
      testFilter1->filter(2, maxMinusOne);
      testFilter1->filter(3, zero);
      util::dsp::Filter::value_t result1 = testFilter1->filter(4, zero);
      util::dsp::Filter::Status status1 = testFilter1->getStatus();
      testFilter2->filter(1, minPlusOne);
      testFilter2->filter(2, minPlusOne);
      testFilter2->filter(3, zero);
      util::dsp::Filter::value_t result2 = testFilter2->filter(4, zero);
      util::dsp::Filter::Status status2 = testFilter2->getStatus();
      //assert
      TEST_ASSERT(status1 == util::dsp::Filter::Status::OK);
      TEST_ASSERT(result1 == referenceValue1);
      TEST_ASSERT(status2 == util::dsp::Filter::Status::OK);
      TEST_ASSERT(result2 == referenceValue2);
      //cleanup
      delete(testFilter1);
      delete(testFilter2);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      testPulseStepResponsesAvg2();
      testPulseStepResponsesAvg16();
      testFrequenciesResponsesAvg2();
      testFrequenciesResponsesAvg16();
      testSumOverflow();
    }
};

class TestLowPass {
  public:
    static void testPulseStepResponses20Hz(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t fc = static_cast<util::dsp::Filter::value_t>(LOWPASS_A_FREQUENCY_HZ);
      static const util::dsp::Filter::value_t timestampFactor = util::timestampPerSecond;
      static const util::dsp::Filter::value_t margin(1, 1);//0.1
      util::dsp::Filter * testFilter = new util::dsp::LowPass(fc, util::ValuePi, timestampFactor);
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT1, DECIMALS_INPUT1, input1, SIZE_INPUT_1);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT1, DECIMALS_OUTPUT1_LOWPASS_A, output1_lowpass_a, SIZE_INPUT_1);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void testFrequenciesResponses20Hz(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t fc = static_cast<util::dsp::Filter::value_t>(LOWPASS_A_FREQUENCY_HZ);
      static const util::dsp::Filter::value_t timestampFactor = util::timestampPerSecond;
      static const util::dsp::Filter::value_t margin(1, 1);//0.1
      util::dsp::Filter * testFilter = new util::dsp::LowPass(fc, util::ValuePi, timestampFactor);
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT2, DECIMALS_INPUT2, input2, SIZE_INPUT_2);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT2, DECIMALS_OUTPUT1_LOWPASS_A, output2_lowpass_a, SIZE_INPUT_2);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void testIncorrectFcValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t fc1(0);
      static const util::dsp::Filter::value_t fc2(-1);
      static const util::dsp::Filter::value_t testValue(10);
      util::dsp::Filter * testFilter1 = new util::dsp::LowPass(fc1, util::ValuePi);
      util::dsp::Filter * testFilter2 = new util::dsp::LowPass(fc2, util::ValuePi);
      //act
      testFilter1->filter(1, testValue);
      testFilter1->filter(2, testValue);
      util::dsp::Filter::value_t outputResult1 = testFilter1->filter(3, testValue);
      util::dsp::Filter::Status status1 = testFilter1->getStatus();
      testFilter2->filter(1, testValue);
      testFilter2->filter(2, testValue);
      util::dsp::Filter::value_t outputResult2 = testFilter2->filter(3, testValue);
      util::dsp::Filter::Status status2 = testFilter2->getStatus();
      //assert
      TEST_ASSERT(status1 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(outputResult1 == util::dsp::Filter::value_t(0));
      TEST_ASSERT(status2 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(outputResult2 == util::dsp::Filter::value_t(0));
      //cleanup
      delete(testFilter1);
      delete(testFilter2);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      testPulseStepResponses20Hz();
      testFrequenciesResponses20Hz();
      testIncorrectFcValues();
    }
};

class TestLinearScale {
  public:
    static void test_InitWithFactorAndOffset_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t factor(5, 1);        //0.5
      static const util::dsp::Filter::value_t offset(-1);          //-1.0
      util::dsp::Filter * testFilter = new util::dsp::LinearScale(factor, offset);
      static const util::dsp::Filter::value_t testInput1(0);       //0.0
      static const util::dsp::Filter::value_t testInput2(1);       //1.0
      static const util::dsp::Filter::value_t testOutput1(-1);     //-1.0
      static const util::dsp::Filter::value_t testOutput2(-5, 1);  //-0.5
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      static const util::dsp::Filter::value_t result1 = testFilter->filter(1, testInput1);
      static const util::dsp::Filter::value_t result2 = testFilter->filter(2, testInput2);
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(result1 == testOutput1);
      TEST_ASSERT(result2 == testOutput2);
      //cleanup
      delete(testFilter);
      TEST_FUNC_END();
    }
    static void test_InitWithTwoPoints_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t point1x(2);          //2.0
      static const util::dsp::Filter::value_t point1y(0);          //0.0
      static const util::dsp::Filter::value_t point2x(6);          //6.0
      static const util::dsp::Filter::value_t point2y(2);          //2.0
      util::dsp::Filter * testFilter = new util::dsp::LinearScale(point1x, point1y, point2x, point2y);
      static const util::dsp::Filter::value_t testInput1(0);       //0.0
      static const util::dsp::Filter::value_t testInput2(1);       //1.0
      static const util::dsp::Filter::value_t testOutput1(-1);     //-1.0
      static const util::dsp::Filter::value_t testOutput2(-5, 1);  //-0.5
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      static const util::dsp::Filter::value_t result1 = testFilter->filter(1, testInput1);
      static const util::dsp::Filter::value_t result2 = testFilter->filter(2, testInput2);
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(result1 == testOutput1);
      TEST_ASSERT(result2 == testOutput2);
      //cleanup
      delete(testFilter);
      TEST_FUNC_END();
    }
    static void test_InitWithOffset_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t offset(-1);          //-1.0
      util::dsp::Filter * testFilter = new util::dsp::LinearScale(offset);
      static const util::dsp::Filter::value_t testInput1(0);       //0.0
      static const util::dsp::Filter::value_t testInput2(1);       //1.0
      static const util::dsp::Filter::value_t testOutput1(-1);     //-1.0
      static const util::dsp::Filter::value_t testOutput2(0);      //0.0
      //act
      util::dsp::Filter::Status status = testFilter->getStatus();
      static const util::dsp::Filter::value_t result1 = testFilter->filter(1, testInput1);
      static const util::dsp::Filter::value_t result2 = testFilter->filter(2, testInput2);
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(result1 == testOutput1);
      TEST_ASSERT(result2 == testOutput2);
      //cleanup
      delete(testFilter);
      TEST_FUNC_END();
    }
    static void test_IncorrectInitData_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t point1x(6);          //6.0
      static const util::dsp::Filter::value_t point1y(2);          //2.0
      static const util::dsp::Filter::value_t point2x(6);          //6.0
      static const util::dsp::Filter::value_t point2y(2);          //2.0
      util::dsp::Filter * testFilter = new util::dsp::LinearScale(point1x, point1y, point2x, point2y);
      static const util::dsp::Filter::value_t testInput(1);
      //act
      util::dsp::Filter::Status status1 = testFilter->getStatus();
      static const util::dsp::Filter::value_t result = testFilter->filter(1, testInput);
      util::dsp::Filter::Status status2 = testFilter->getStatus();
      //assert
      TEST_ASSERT(status1 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(status2 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(result == util::dsp::Filter::value_t(0));
      //cleanup
      delete(testFilter);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      test_InitWithFactorAndOffset_expectCorrectValues();
      test_InitWithTwoPoints_expectCorrectValues();
      test_InitWithOffset_expectCorrectValues();
      test_IncorrectInitData_expectZero();
    }
};

class TestSquareScale {
  public:
    static void test_InitWithThreePoints_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t x1(SQUARE_SCALE_A_X1, SQUARE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y1(SQUARE_SCALE_A_Y1, SQUARE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t x2(SQUARE_SCALE_A_X2, SQUARE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y2(SQUARE_SCALE_A_Y2, SQUARE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t x3(SQUARE_SCALE_A_X3, SQUARE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y3(SQUARE_SCALE_A_Y3, SQUARE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t margin(1, 0);//1.0
      util::dsp::Filter * testFilter = new util::dsp::SquareScale(x1, y1, x2, y2, x3, y3);
      util::dsp::Filter::Status status = testFilter->getStatus();
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_INPUT3, input3, SIZE_INPUT_3);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_OUTPUT3_SQUARE_SCALE_A, output3_squareScale_a, SIZE_INPUT_3);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::WARNING_INIT_DATA_INCORRECT);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void test_InitWithABC_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t a(SQUARE_SCALE_A_A, SQUARE_SCALE_A_FACTOR_DECIMALS);
      static const util::dsp::Filter::value_t b(SQUARE_SCALE_A_B, SQUARE_SCALE_A_FACTOR_DECIMALS);
      static const util::dsp::Filter::value_t c(SQUARE_SCALE_A_C, SQUARE_SCALE_A_FACTOR_DECIMALS);
      static const util::dsp::Filter::value_t margin(1, 0);//1.0
      util::dsp::Filter * testFilter = new util::dsp::SquareScale(a, b, c);
      util::dsp::Filter::Status status = testFilter->getStatus();
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_INPUT3, input3, SIZE_INPUT_3);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_OUTPUT3_SQUARE_SCALE_A, output3_squareScale_a, SIZE_INPUT_3);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(status == util::dsp::Filter::Status::OK);
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void test_VertexWarning_expectWarningWhenParabolaVertexIsInRange(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t point1x(1, 1);         //0.1
      static const util::dsp::Filter::value_t point1y(5, 0);         //5.0
      static const util::dsp::Filter::value_t point2x(5, 1);         //0.5
      static const util::dsp::Filter::value_t point2y1(75, 0);       //75.0
      static const util::dsp::Filter::value_t point2y2(60, 0);       //60.0
      static const util::dsp::Filter::value_t point2y3(50, 0);       //50.0
      static const util::dsp::Filter::value_t point3x(9, 1);         //0.9
      static const util::dsp::Filter::value_t point3y(90, 0);        //90.0
      util::dsp::Filter * testFilter1 = new util::dsp::SquareScale(point1x, point1y, point2x, point2y1, point3x, point3y); // Vertex inside range: warning
      util::dsp::Filter * testFilter2 = new util::dsp::SquareScale(point1x, point1y, point2x, point2y2, point3x, point3y); // Vertex outside range: OK
      util::dsp::Filter * testFilter3 = new util::dsp::SquareScale(point1x, point1y, point2x, point2y3, point3x, point3y); // Linear dependency: OK
      static const util::dsp::Filter::value_t testInput(1);
      static const util::dsp::Filter::value_t zero(0);
      //act
      util::dsp::Filter::Status status1 = testFilter1->getStatus();
      util::dsp::Filter::Status status2 = testFilter2->getStatus();
      util::dsp::Filter::Status status3 = testFilter3->getStatus();
      //assert
      TEST_ASSERT(status1 == util::dsp::Filter::Status::WARNING_INIT_DATA_INCORRECT);
      TEST_ASSERT(status2 == util::dsp::Filter::Status::OK);
      TEST_ASSERT(status3 == util::dsp::Filter::Status::OK);
      //cleanup
      delete(testFilter1);
      delete(testFilter2);
      delete(testFilter3);
      TEST_FUNC_END();
    }
    static void test_IncorrectInitData_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t point1x(6);          //6.0
      static const util::dsp::Filter::value_t point1y(2);          //2.0
      static const util::dsp::Filter::value_t point2x(6);          //6.0
      static const util::dsp::Filter::value_t point2y(3);          //3.0
      static const util::dsp::Filter::value_t point3x(7);          //7.0
      static const util::dsp::Filter::value_t point3y(4);          //4.0
      util::dsp::Filter * testFilter = new util::dsp::SquareScale(point1x, point1y, point2x, point2y, point3x, point3y);
      static const util::dsp::Filter::value_t testInput(1);
      static const util::dsp::Filter::value_t zero(0);
      //act
      util::dsp::Filter::Status status1 = testFilter->getStatus();
      static const util::dsp::Filter::value_t result = testFilter->filter(1, testInput);
      util::dsp::Filter::Status status2 = testFilter->getStatus();
      //assert
      TEST_ASSERT(status1 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(status2 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(result == zero);
      //cleanup
      delete(testFilter);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      test_InitWithThreePoints_expectCorrectValues();
      test_InitWithABC_expectCorrectValues();
      test_VertexWarning_expectWarningWhenParabolaVertexIsInRange();
      test_IncorrectInitData_expectZero();
    }
};

class TestSplineScale {
  public:
    static void test_InitWithThreePoints_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t x1(SPLINE_SCALE_A_X1, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y1(SPLINE_SCALE_A_Y1, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t x2(SPLINE_SCALE_A_X2, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y2(SPLINE_SCALE_A_Y2, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t x3(SPLINE_SCALE_A_X3, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y3(SPLINE_SCALE_A_Y3, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t margin(5, 1);//0.5
      util::dsp::Filter * testFilter = new util::dsp::SplineScale(x1, y1, x2, y2, x3, y3);
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_INPUT3, input3, SIZE_INPUT_3);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_OUTPUT3_SPLINE_SCALE_A, output3_splineScale_a, SIZE_INPUT_3);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void test_InitWithUnsortedPoints_expectCorrectValues(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t x1(SPLINE_SCALE_A_X1, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y1(SPLINE_SCALE_A_Y1, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t x2(SPLINE_SCALE_A_X2, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y2(SPLINE_SCALE_A_Y2, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t x3(SPLINE_SCALE_A_X3, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t y3(SPLINE_SCALE_A_Y3, SPLINE_SCALE_A_DECIMALS);
      static const util::dsp::Filter::value_t margin(5, 1);//0.5
      util::dsp::Filter * testFilter = new util::dsp::SplineScale(x2, y2, x1, y1, x3, y3);
      AnalogFilterTestSet * testInputSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_INPUT3, input3, SIZE_INPUT_3);
      AnalogFilterTestSet * testReferenceSet = new AnalogFilterTestSet (DELTA_TIME_MS_INPUT3, DECIMALS_OUTPUT3_SPLINE_SCALE_A, output3_splineScale_a, SIZE_INPUT_3);
      AnalogFilterTestSet * testOutputSet = new AnalogFilterTestSet (testInputSet->getSize());
      testOutputSet->setDeltaTime(testInputSet->getDeltaTime());
      //act
      boolean feedResult = AnalogFilterTester::feed(*testFilter, *testInputSet, *testOutputSet);
      testOutputSet->makeReadOnly();
      util::dsp::Filter::value_t comparisonResult(AnalogFilterTester::compare(*testOutputSet, *testReferenceSet));
      //assert
      TEST_ASSERT(feedResult);
      TEST_ASSERT(comparisonResult <= margin);
      //cleanup
      delete(testFilter);
      delete(testInputSet);
      delete(testOutputSet);
      delete(testReferenceSet);
      TEST_FUNC_END();
    }
    static void test_IncorrectInitData_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      static const util::dsp::Filter::value_t point1x(6);          //6.0
      static const util::dsp::Filter::value_t point1y(2);          //2.0
      static const util::dsp::Filter::value_t point2x(6);          //6.0
      static const util::dsp::Filter::value_t point2y(3);          //3.0
      static const util::dsp::Filter::value_t point3x(7);          //7.0
      static const util::dsp::Filter::value_t point3y(4);          //4.0
      util::dsp::Filter * testFilter = new util::dsp::SplineScale(point1x, point1y, point2x, point2y, point3x, point3y);
      static const util::dsp::Filter::value_t testInput(1);
      //act
      util::dsp::Filter::Status status1 = testFilter->getStatus();
      static const util::dsp::Filter::value_t result = testFilter->filter(1, testInput);
      util::dsp::Filter::Status status2 = testFilter->getStatus();
      //assert
      TEST_ASSERT(status1 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(status2 == util::dsp::Filter::Status::ERROR_INIT_DATA_INCORRECT);
      TEST_ASSERT(result == util::dsp::Filter::value_t(0));
      //cleanup
      delete(testFilter);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      test_InitWithThreePoints_expectCorrectValues();
      test_InitWithUnsortedPoints_expectCorrectValues();
      test_IncorrectInitData_expectZero();
    }
};

class TestAnalogChannel {
  public:
    static void runTests(void) {
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestAbstractFilter::runTests();
  TestMovingAverage::runTests();
  TestLowPass::runTests();
  TestLinearScale::runTests();
  TestSquareScale::runTests();
  TestSplineScale::runTests();
  TestAnalogChannel::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
