#include <ESP8266WiFi.h>

#include "test.h"

#include "util.h"

template <typename T, size_t testBufferSize>
class RingBufferTester {
  public:
    template <typename... Values> RingBufferTester (Values&&... values) {
      T valuesUnpack[] = {values...};
      size_t numberOfValues = sizeof...(values);
      testValuesCount = numberOfValues;
      testValues = (T**)malloc(numberOfValues * sizeof(T*));
      for (int i = 0; i < numberOfValues; i++) {
        testValues[i] = new T (valuesUnpack[i]);
      }
    }
    ~RingBufferTester() {
      for (int i = 0; i < testValuesCount; i++) {
        delete testValues[i];
      }
      free(testValues);
      testValues = NULL;
      testValuesCount = 0;
    }
  private:
    size_t testValuesCount = 0;
    T ** testValues = NULL;
    T defaultValue = T();
  public:
    void test_testValues(void) {
      TEST_FUNC_START();
      TEST_ASSERT(testValues);
      for (int i = 0; i < testValuesCount; i++) {
        TEST_ASSERT (testValues[i]);
      }
      TEST_ASSERT((testBufferSize + 2) <= testValuesCount);
      TEST_FUNC_END();
    }
  public:
    void validate_correctInit_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      //assert
      TEST_ASSERT(validationResult);
      TEST_FUNC_END();
    }
    void validate_notInitialised_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::RingBuffer<T> testRingBuffer;
      //act
      boolean validationResult = testRingBuffer.validate();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_FUNC_END();
    }
  public:
    void test_validate(void) {
      validate_correctInit_expectTrue();
      validate_notInitialised_expectFalse();
    }
  public:
    void push_singleValue_expectCorrectValue(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push(*testValues[0]);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(testBuffer[0] == *testValues[0]);
      TEST_FUNC_END();
    }
    void push_multipleValues_expectSameValues(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++) {
        testRingBuffer.push(*testValues[i]);
      }
      //assert
      TEST_ASSERT(validationResult);
      for (int i = 0; i < testBufferSize; i++) {
        TEST_ASSERT(*testValues[i] == testBuffer[i]);
      }
      TEST_FUNC_END();
    }
    void push_fullArray_expectOldestValuesOverwritten(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++) {
        testRingBuffer.push(*testValues[i]);
      }
      testRingBuffer.push(*testValues[testBufferSize]);
      testRingBuffer.push(*testValues[testBufferSize + 1]);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(testBuffer[0] == *testValues[testBufferSize]);
      TEST_ASSERT(testBuffer[1] == *testValues[testBufferSize + 1]);
      for (int i = 2; i < testBufferSize; i++) {
        TEST_ASSERT(*testValues[i] == testBuffer[i]);
      }
      TEST_FUNC_END();
    }
    void push_notInitialised_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::RingBuffer<T> testRingBuffer;
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push(defaultValue);
      //assert
      TEST_ASSERT(!validationResult);
      TEST_FUNC_END();
    }
  public:
    void test_push(void) {
      push_singleValue_expectCorrectValue();
      push_multipleValues_expectSameValues();
      push_fullArray_expectOldestValuesOverwritten();
      push_notInitialised_expectNoCrash();
    }
  public:
    void count_emptyRingBuffer_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!countResult);
      TEST_FUNC_END();
    }
    void count_singleValue_expectOne(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push(defaultValue);
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countResult == 1);
      TEST_FUNC_END();
    }
    void count_fullRingBuffer_expectBufferSize(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++) {
        testRingBuffer.push(*testValues[i]);
      }
      testRingBuffer.push(*testValues[testBufferSize]);
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countResult == testBufferSize);
      TEST_FUNC_END();
    }
    void count_notInitialised_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::RingBuffer<T> testRingBuffer;
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!countResult);
      TEST_FUNC_END();
    }
    void test_count(void) {
      count_emptyRingBuffer_expectZero();
      count_singleValue_expectOne();
      count_fullRingBuffer_expectBufferSize();
      count_notInitialised_expectZero();
    }
  public:
    void subscriptOperator_singleValue_expectSameValue(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push(*testValues[0]);
      T ringBufferValue = testRingBuffer[0];
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(ringBufferValue == *testValues[0]);
      TEST_FUNC_END();
    }
    void subscriptOperator_wrap_expectOldestValuesOverwritten(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++) {
        testRingBuffer.push(*testValues[i]);
      }
      testRingBuffer.push(*testValues[testBufferSize]);
      T ringBufferValues[testRingBuffer.count()];
      for (int i = 0; i < testRingBuffer.count(); i++) {
        ringBufferValues[i] = testRingBuffer[i];
      }
      //assert
      TEST_ASSERT(validationResult);
      for (int i = 0; i < testRingBuffer.count(); i++) {
        TEST_ASSERT(ringBufferValues[i] == *testValues[i + 1]);
      }
      TEST_FUNC_END();
    }
    void subscriptOperator_wrongIndex_expectDefaultValue(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push(*testValues[0]);
      T ringBufferValue = testRingBuffer[1];
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(ringBufferValue == defaultValue);
      TEST_FUNC_END();
    }
    void subscriptOperator_notInitialised_expectDefaultValue(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::RingBuffer<T> testRingBuffer;
      //act
      boolean validationResult = testRingBuffer.validate();
      T ringBufferValue = testRingBuffer[0];
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(ringBufferValue == defaultValue);
      TEST_FUNC_END();
    }
  public:
    void test_subscriptOperator(void) {
      subscriptOperator_singleValue_expectSameValue();
      subscriptOperator_wrap_expectOldestValuesOverwritten();
      subscriptOperator_wrongIndex_expectDefaultValue();
      subscriptOperator_notInitialised_expectDefaultValue();
    }
  public:
    void pop_singleValue_expectOldestValueRemoved(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++) {
        testRingBuffer.push(*testValues[i]);
      }
      testRingBuffer.pop();
      size_t count = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(count == testBufferSize - 1);
      for (int i = 0; i < count; i++) {
        TEST_ASSERT(*testValues[i + 1] == testRingBuffer[i]);
      }
      TEST_FUNC_END();
    }
    void pop_multipleValues_expectValuesRemoved(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++)
        testRingBuffer.push(*testValues[i]);
      for (int i = 0; i < testBufferSize; i++)
        testRingBuffer.pop();
      size_t count = testRingBuffer.count();
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!count);
      TEST_FUNC_END();
    }
    void pop_wrap_expectOlderValuesRemoved(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++) {
        testRingBuffer.push(*testValues[i]);
      }
      testRingBuffer.push(*testValues[testBufferSize]);
      testRingBuffer.push(*testValues[testBufferSize + 1]);
      do {
        testRingBuffer.pop();
      } while (testRingBuffer.count() > 3);
      size_t countBeforeWrap = testRingBuffer.count();
      T resultBeforeWrap[] = {testRingBuffer[0], testRingBuffer[1], testRingBuffer[2]};
      testRingBuffer.pop();
      testRingBuffer.pop();
      size_t countAfterWrap = testRingBuffer.count();
      T resultAfterWrap = testRingBuffer[0];
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countBeforeWrap == 3);
      TEST_ASSERT(resultBeforeWrap[0] == *testValues[testBufferSize - 1]);
      TEST_ASSERT(resultBeforeWrap[1] == *testValues[testBufferSize]);
      TEST_ASSERT(resultBeforeWrap[2] == *testValues[testBufferSize + 1]);
      TEST_ASSERT(countAfterWrap == 1);
      TEST_ASSERT(resultAfterWrap == *testValues[testBufferSize + 1]);
      TEST_FUNC_END();
    }
    void pop_emptyRingBuffer_expectNoActionPerformed(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t countBefore = testRingBuffer.count();
      testRingBuffer.pop();
      size_t countAfter = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!countBefore);
      TEST_ASSERT(!countAfter);
      TEST_FUNC_END();
    }
    void pop_notInitialised_expectNoActionPerformed(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::RingBuffer<T> testRingBuffer;
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.pop();
      size_t count = testRingBuffer.count();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!count);
      TEST_FUNC_END();
    }
    void test_pop(void) {
      pop_singleValue_expectOldestValueRemoved();
      pop_multipleValues_expectValuesRemoved();
      pop_wrap_expectOlderValuesRemoved();
      pop_emptyRingBuffer_expectNoActionPerformed();
      pop_notInitialised_expectNoActionPerformed();
    }
  public:
    void empty_emptyRingBuffer_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean result = testRingBuffer.empty();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    void empty_nonEmptyRingBuffer_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push(defaultValue);
      boolean result = testRingBuffer.empty();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    void empty_notInitialised_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::RingBuffer<T> testRingBuffer;
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean result = testRingBuffer.empty();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
  public:
    void test_empty() {
      empty_emptyRingBuffer_expectTrue();
      empty_nonEmptyRingBuffer_expectFalse();
      empty_notInitialised_expectTrue();
    }
  public:
    void full_fullRingBuffer_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++)
        testRingBuffer.push(defaultValue);
      boolean result = testRingBuffer.full();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    void full_nonFullRingBuffer_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (int i = 0; i < testBufferSize; i++)
        testRingBuffer.push(defaultValue);
      testRingBuffer.pop();
      boolean result = testRingBuffer.full();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    void full_notInitialised_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::RingBuffer<T> testRingBuffer;
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean result = testRingBuffer.full();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
  public:
    void test_full() {
      full_fullRingBuffer_expectTrue();
      full_nonFullRingBuffer_expectFalse();
      full_notInitialised_expectFalse();
    }
  public:
    void runTests(void) {
      test_testValues();
      test_validate();
      test_push();
      test_count();
      test_subscriptOperator();
      test_pop();
      test_empty();
      test_full();
    }
};

template <typename T, size_t testBufferSize, uint32_t numberOfOperations>
class RingBufferPerformanceTester {
#pragma GCC push_options
#pragma GCC optimize ("O0")
  public:
    static void testPerformance_push(void) {
      PERF_TEST_FUNC_START();
      const T defaultValue = T();
      T testBuffer[testBufferSize] = {};
      util::arrays::RingBuffer<T> ringBuffer(testBuffer, testBufferSize);
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++) {
        ringBuffer.push(defaultValue);
      }
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void testPerformance_subscriptOperator(void) {
      PERF_TEST_FUNC_START();
      const T defaultValue = T();
      T testBuffer[testBufferSize] = {};
      T testResultBuffer[testBufferSize];
      util::arrays::RingBuffer<T> ringBuffer(testBuffer, testBufferSize);
      for (uint32_t i = 0; i < testBufferSize; i++) {
        ringBuffer.push(defaultValue);
      }
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++) {
        new (&testResultBuffer[0]) T(ringBuffer[0]);
      }
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
#pragma GCC pop_options
  public:
    static void runTests(void) {
      testPerformance_push();
      testPerformance_subscriptOperator();
    }
};

class TestRingBuffer {
  public:
    static void testChar(void) {
      RingBufferTester<char, 4> ringBufferTester('a', 'b', 'c', 'd', 'e', 'f');
      ringBufferTester.runTests();
      RingBufferPerformanceTester<char, 256, 4096>::runTests();
      RingBufferPerformanceTester<char, 1024, 4096>::runTests();
    }
    static void testInt(void) {
      RingBufferTester<int, 4> ringBufferTester(1, 2, 3, 4, 5, 6);
      ringBufferTester.runTests();
      RingBufferPerformanceTester<int, 128, 4096>::runTests();
      RingBufferPerformanceTester<int, 400, 4096>::runTests();
    }
    static void testLong(void) {
      RingBufferTester<long, 4> ringBufferTester(1L, 2L, 3L, 4L, 5L, 6L);
      ringBufferTester.runTests();
      RingBufferPerformanceTester<long, 64, 4096>::runTests();
      RingBufferPerformanceTester<long, 320, 4096>::runTests();
    }
    static void testFloat(void) {
      RingBufferTester<float, 4> ringBufferTester(1.0f, 1.25f, 1.75f, 2.0f, 2.5f, 3.5f);
      ringBufferTester.runTests();
      RingBufferPerformanceTester<float, 64, 4096>::runTests();
      RingBufferPerformanceTester<float, 320, 4096>::runTests();
    }
  public:
    struct TestStruct {
      uint32_t a;
      uint16_t b;
      char c;
      TestStruct(uint32_t a, uint16_t b, char c) {
        this->a = a;
        this->b = b;
        this->c = c;
      }
      TestStruct() {
        a = 0;
        b = 1;
        c = '\2';
      }
      boolean operator==(const TestStruct& rhs) {
        if (a != rhs.a) return (false);
        if (b != rhs.b) return (false);
        if (c != rhs.c) return (false);
        return (true);
      }
    };
    static void testStruct(void) {
      TestStruct t1 (3, 4, 5);
      TestStruct t2 (6, 7, 8);
      TestStruct t3 (9, 10, 11);
      TestStruct t4 (12, 13, 14);
      TestStruct t5 (15, 16, 17);
      TestStruct t6 (18, 19, 20);
      RingBufferTester <TestStruct, 4> ringBufferTester(t1, t2, t3, t4, t5, t6);
      ringBufferTester.runTests();
      RingBufferPerformanceTester<TestStruct, 32, 4096>::runTests();
      RingBufferPerformanceTester<TestStruct, 128, 4096>::runTests();
    }
  public:
    class TestClass {
      public:
        inline TestClass() {
          value = 0;
          ref();
          //TEST_STREAM.println("Trivial constructor");
          //yield();
        }
        inline TestClass(int initValue) {
          value = initValue;
          ref();
          //TEST_STREAM.print("Constructor, val=");
          //TEST_STREAM.println(value);
          //yield();
        }
        inline TestClass(const TestClass &source) {
          value = source.getValue();
          ref();
          //TEST_STREAM.print("Copy constructor, val=");
          //TEST_STREAM.println(value);
          //yield();
        }
        inline ~TestClass() {
          unref();
          //TEST_STREAM.print("Destructor, val=");
          //TEST_STREAM.println(value);
          //yield();
        }
        inline size_t getInstances(void) const {
          return (instances);
        }
        inline int getValue(void) const {
          return (value);
        }
        inline boolean operator==(const TestClass& rhs) {
          return (this->getValue() == rhs.getValue());
        }
      private:
        uint32_t value;
        static size_t instances;
        inline void ref(void) {
          instances++;
        }
        inline void unref(void) {
          if (instances) instances--;
        }
    };
    static const size_t checkClassConstructorDestructor_testBufferSize = 5;
    static void checkClassConstrDestr_bufferNotFull_expectInstancesEqualsToPushCount() {
      TEST_FUNC_START();
      //arrange
      TestClass * testBuffer = reinterpret_cast<TestClass*>(malloc(checkClassConstructorDestructor_testBufferSize * sizeof(TestClass)));
      util::arrays::RingBuffer<TestClass> testRingBuffer(testBuffer, checkClassConstructorDestructor_testBufferSize);
      TestClass t;
      static const int pushCount = 2;
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t instancesBefore = t.getInstances();
      for (int i = 0; i < pushCount; i++)
        testRingBuffer.push(t);
      size_t instancesAfter = t.getInstances();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(instancesBefore + pushCount == instancesAfter);
      //cleanup
      free(testBuffer);
      TEST_FUNC_END();
    }
    static void checkClassConstrDestr_pushPopSequence_expectInstancesAsDiffrenceBetweenPushAndPopCount() {
      TEST_FUNC_START();
      //arrange
      TestClass * testBuffer = reinterpret_cast<TestClass*>(malloc(checkClassConstructorDestructor_testBufferSize * sizeof(TestClass)));
      util::arrays::RingBuffer<TestClass> testRingBuffer(testBuffer, checkClassConstructorDestructor_testBufferSize);
      TestClass t;
      static const int pushCount = 4;
      static const int popCount = 3;
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t instancesBefore = t.getInstances();
      for (int i = 0; i < pushCount; i++)
        testRingBuffer.push(t);
      for (int i = 0; i < popCount; i++)
        testRingBuffer.pop();
      size_t instancesAfter = t.getInstances();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(instancesBefore + pushCount - popCount == instancesAfter);
      //cleanup
      free(testBuffer);
      TEST_FUNC_END();
    }
    static void checkClassConstrDestr_bufferFull_expectInstancesEqualsToBufferSize() {
      TEST_FUNC_START();
      //arrange
      TestClass * testBuffer = reinterpret_cast<TestClass*>(malloc(checkClassConstructorDestructor_testBufferSize * sizeof(TestClass)));
      util::arrays::RingBuffer<TestClass> testRingBuffer(testBuffer, checkClassConstructorDestructor_testBufferSize);
      TestClass t;
      static const int pushCount = 2;
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t instancesBefore = t.getInstances();
      for (size_t i = 0; i < checkClassConstructorDestructor_testBufferSize; i++)
        testRingBuffer.push(t);
      boolean bufferFull = testRingBuffer.full();
      for (int i = 0; i < pushCount; i++)
        testRingBuffer.push(t);
      size_t instancesAfter = t.getInstances();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(bufferFull);
      TEST_ASSERT(instancesBefore + checkClassConstructorDestructor_testBufferSize == instancesAfter);
      //cleanup
      free(testBuffer);
      TEST_FUNC_END();
    }
    static void checkClassConstrDestr_bufferDestructor_expectInstancesAsBefore() {
      TEST_FUNC_START();
      //arrange
      TestClass t(10);
      static const int pushCount = 4;
      boolean validationResult = false;
      //act
      TestClass * testBuffer = reinterpret_cast<TestClass*>(malloc(checkClassConstructorDestructor_testBufferSize * sizeof(TestClass)));
      size_t instancesBefore = t.getInstances();
      {
        util::arrays::RingBuffer<TestClass> testRingBuffer(testBuffer, checkClassConstructorDestructor_testBufferSize);
        validationResult = testRingBuffer.validate();
        for (int i = 0; i < pushCount; i++)
          testRingBuffer.push(t);
      }//testRingBuffer goes out of scope
      size_t instancesAfter = t.getInstances();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(instancesBefore == instancesAfter);
      //cleanup
      free(testBuffer);
      TEST_FUNC_END();
    }
    static void checkConstructorDestructorCalls(void) {
      checkClassConstrDestr_bufferNotFull_expectInstancesEqualsToPushCount();
      checkClassConstrDestr_pushPopSequence_expectInstancesAsDiffrenceBetweenPushAndPopCount();
      checkClassConstrDestr_bufferFull_expectInstancesEqualsToBufferSize();
      checkClassConstrDestr_bufferDestructor_expectInstancesAsBefore();
    }
    static void testClass(void) {
      //TEST_STREAM.println(sizeof(util::arrays::RingBuffer<TestClass>::data_t));
      //TEST_STREAM.println(sizeof(const TestClass &));
      //TEST_STREAM.println(sizeof(TestClass));
      //TEST_STREAM.println(sizeof(util::arrays::RingBuffer<char>::data_t));
      //TEST_STREAM.println(sizeof(const char &));
      //TEST_STREAM.println(sizeof(char));
      //TEST_STREAM.println(sizeof(util::arrays::RingBuffer<TestStruct>::data_t));
      //TEST_STREAM.println(sizeof(const TestStruct &));
      //TEST_STREAM.println(sizeof(TestStruct));
      TestClass t1 (2);
      TestClass t2 (3);
      TestClass t3 (4);
      TestClass t4 (5);
      TestClass t5 (6);
      TestClass t6 (7);
      RingBufferTester <TestClass, 4> ringBufferTester(t1, t2, t3, t4, t5, t6);
      ringBufferTester.runTests();
      checkConstructorDestructorCalls();
      RingBufferPerformanceTester<TestClass, 32, 4096>::runTests();
      RingBufferPerformanceTester<TestClass, 128, 4096>::runTests();
    }
  public:
    static void runTests(void) {
      testChar();
      testInt();
      testLong();
      testFloat();
      testStruct();
      testClass();
    }
};

size_t TestRingBuffer::TestClass::instances = 0;

class TestCstrRingBuffer {
  public:
    static const size_t testBufferSize = 256;

  public:
    static void validate_correctInit_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      //assert
      TEST_ASSERT(validationResult);
      TEST_FUNC_END();
    }
    static void validate_bufferNull_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::CStrRingBuffer testRingBuffer(NULL, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_FUNC_END();
    }
    static void validate_bufferSizeZero_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, 0);
      //act
      boolean validationResult = testRingBuffer.validate();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_FUNC_END();
    }
    static void test_validate(void) {
      validate_correctInit_expectTrue();
      validate_bufferNull_expectFalse();
      validate_bufferSizeZero_expectFalse();
    }
  public:
    static void push_get_multipleCStrings_expectCStringsStored(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      static const size_t testStrings = 4;
      static const size_t testStringSize = 33;
      const char testCStrings[testStrings][testStringSize] = {
        "TestCString1",
        "2",
        "Test  Test  Test  Test CString 3",
        "Test4"
      };
      static const size_t maxGetCStrings = 10;
      static const size_t getCStringSize = 33;
      char getResult[maxGetCStrings][getCStringSize] = {};
      size_t getReturnedSize[maxGetCStrings] = {};
      size_t countResult[maxGetCStrings] = {};
      boolean pushResult[maxGetCStrings] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      for (size_t i = 0; i < testStrings; i++) {
        pushResult[i] = testRingBuffer.push(testCStrings[i]);
        countResult[i] = testRingBuffer.count();
      }
      for (size_t i = 0; i < testStrings + 1; i++) {
        getReturnedSize[i] = testRingBuffer.get(i, getResult[i], getCStringSize);
      }
      //assert
      TEST_ASSERT(validationResult);
      for (size_t i = 0; i < testStrings; i++) {
        TEST_ASSERT(pushResult[i]);
        TEST_ASSERT(countResult[i] == i + 1);
        TEST_ASSERT(getReturnedSize[i] == strlen(testCStrings[i]));
        TEST_ASSERT(!strcmp(testCStrings[i], getResult[i]));
      }
      TEST_ASSERT(!getReturnedSize[testStrings + 1]);
      TEST_ASSERT(!strlen(getResult[testStrings + 1]));
      TEST_FUNC_END();
    }
    static void push_get_multipleCStringsBufferFull_expectOldestCStringOverwritten(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      const char testCString[] = "a longer test cstring to fill a ring buffer with";
      static const size_t maxGetCStrings = 10;
      static const size_t getCStringSize = 49;
      char getResult[maxGetCStrings][getCStringSize] = {};
      size_t getReturnedSize[maxGetCStrings] = {};
      boolean pushResult[maxGetCStrings] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t stringsFitIntoBuffer = 0;
      do {
        pushResult[stringsFitIntoBuffer] = testRingBuffer.push(testCString);
        stringsFitIntoBuffer++;
      } while (!testRingBuffer.full(testCString));
      size_t countResultBefore = testRingBuffer.count();
      pushResult[stringsFitIntoBuffer] = testRingBuffer.push(testCString);
      size_t countResultAfter = testRingBuffer.count();
      for (size_t i = 0; i < stringsFitIntoBuffer + 1; i++) {
        getReturnedSize[i] = testRingBuffer.get(i, getResult[i], getCStringSize);
      }
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countResultBefore == stringsFitIntoBuffer);
      TEST_ASSERT(countResultAfter == stringsFitIntoBuffer);
      for (size_t i = 0; i < stringsFitIntoBuffer; i++) {
        TEST_ASSERT(pushResult[i]);
        TEST_ASSERT(getReturnedSize[i] == strlen(testCString));
        TEST_ASSERT(!strcmp(testCString, getResult[i]));
      }
      TEST_ASSERT(!getReturnedSize[stringsFitIntoBuffer]);
      TEST_ASSERT(!strlen(getResult[stringsFitIntoBuffer]));
      TEST_FUNC_END();
    }
    static void push_get_cstringFitsExactlyIntoFreeSpace_expectCorrectCString(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      const char testCString1[] = "=a longer test cstring to fill a ring buffer with=";
      const char testCString2[] = "1";
      static const size_t maxGetCStrings = 10;
      static const size_t getCStringSize = 51;
      char getResult[maxGetCStrings][getCStringSize] = {};
      size_t getReturnedSize[maxGetCStrings] = {};
      boolean pushResult[maxGetCStrings] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t stringsFitIntoBuffer = 0;
      do {
        pushResult[stringsFitIntoBuffer] = testRingBuffer.push(testCString1);
        stringsFitIntoBuffer++;
      } while (!testRingBuffer.full(testCString1));
      size_t countResultBefore = testRingBuffer.count();
      pushResult[stringsFitIntoBuffer] = testRingBuffer.push(testCString2);
      size_t countResultAfter = testRingBuffer.count();
      for (size_t i = 0; i < stringsFitIntoBuffer + 1; i++) {
        getReturnedSize[i] = testRingBuffer.get(i, getResult[i], getCStringSize);
      }
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!((testBufferSize - 1) % (strlen(testCString1) + 1)));
      TEST_ASSERT(countResultBefore == stringsFitIntoBuffer);
      TEST_ASSERT(countResultAfter == stringsFitIntoBuffer);
      for (size_t i = 0; i < stringsFitIntoBuffer - 1; i++) {
        TEST_ASSERT(pushResult[i]);
        TEST_ASSERT(getReturnedSize[i] == strlen(testCString1));
        TEST_ASSERT(!strcmp(testCString1, getResult[i]));
      }
      TEST_ASSERT(getReturnedSize[stringsFitIntoBuffer - 1] == strlen(testCString2));
      TEST_ASSERT(!strcmp(testCString2, getResult[stringsFitIntoBuffer - 1]));
      TEST_FUNC_END();
    }
    static void push_cstringTooLarge_expectNoAction(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      static const size_t nullTerminatorSize = 1;
      char testCString[testBufferSize + 1 + nullTerminatorSize] = {};
      for (size_t i = 0; i < testBufferSize + 1; i++) {
        testCString[i] = 'a';
      }
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean pushResult = testRingBuffer.push(testCString);
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!pushResult);
      TEST_ASSERT(!countResult);
      TEST_FUNC_END();
    }
    static void push_notInitialised_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      const char testCString[] = "test";
      util::arrays::CStrRingBuffer testRingBuffer(NULL, 0);
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean pushResult = testRingBuffer.push(testCString);
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!pushResult);
      TEST_FUNC_END();
    }
    static void get_bufferSmallerThanStoredString_expectStringTruncated(void) {
      TEST_FUNC_START();
      //arrange
      const char testCString[] = "testСString";
      char testBuffer[testBufferSize] = {};
      static const size_t getResultSize = 4;
      char getResult[getResultSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean pushResult = testRingBuffer.push(testCString);
      size_t getReturnedSize = testRingBuffer.get(0, getResult, getResultSize);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(pushResult);
      TEST_ASSERT(getReturnedSize == getResultSize - 1);
      TEST_ASSERT(!strncmp(getResult, testCString, getResultSize - 1));
      TEST_FUNC_END();
    }
    static void get_bufferSmallerThanStoredFirstPart_expectStringTruncated(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      const char testCString[] = "a longer test cstring to fill a ring buffer with";
      static const size_t getBufferSize = 1;
      static const size_t nullCharSize = 1;
      char getResult[getBufferSize + nullCharSize] = {};
      size_t getReturnedSize = 0;
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t stringsInBuffer = 0;
      while (!testRingBuffer.full(testCString)) {
        testRingBuffer.push(testCString);
        stringsInBuffer++;
      }
      stringsInBuffer--;
      testRingBuffer.push(testCString);
      getReturnedSize = testRingBuffer.get(stringsInBuffer, getResult, getBufferSize + nullCharSize);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(getReturnedSize == getBufferSize);
      TEST_ASSERT(!strncmp(testCString, getResult, getBufferSize));
      TEST_FUNC_END();
    }
    static void get_bufferSmallerThanStoredSecondPart_expectStringTruncated(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      const char testCString[] = "a longer test cstring to fill a ring buffer with";
      const size_t getBufferSize = strlen(testCString) - 1;
      static const size_t nullCharSize = 1;
      char getResult[getBufferSize + nullCharSize] = {};
      size_t getReturnedSize = 0;
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t stringsInBuffer = 0;
      while (!testRingBuffer.full(testCString)) {
        testRingBuffer.push(testCString);
        stringsInBuffer++;
      }
      stringsInBuffer--;
      testRingBuffer.push(testCString);
      getReturnedSize = testRingBuffer.get(stringsInBuffer, getResult, getBufferSize + nullCharSize);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(getReturnedSize == getBufferSize);
      TEST_ASSERT(!strncmp(testCString, getResult, getBufferSize));
      TEST_FUNC_END();
    }
    static void get_emptybuffer_expectZeroLengthString(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      static const size_t getResultSize = 32;
      char getResult[getResultSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t getReturnedSize = testRingBuffer.get(0, getResult, getResultSize);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!getReturnedSize);
      TEST_ASSERT(!strlen(getResult));
      TEST_FUNC_END();
    }
    static void get_notInitialised_expectNoCrash(void) {
      TEST_FUNC_START();
      //arrange
      const char testCString[] = "test";
      util::arrays::CStrRingBuffer testRingBuffer(NULL, 0);
      static const size_t getResultSize = 32;
      char getResult[getResultSize] = {};
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t getReturnValue = testRingBuffer.push(testCString);
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!getReturnValue);
      TEST_ASSERT(!strlen(getResult));
      TEST_FUNC_END();
    }
    static void test_push_get(void) {
      push_get_multipleCStrings_expectCStringsStored();
      push_get_multipleCStringsBufferFull_expectOldestCStringOverwritten();
      push_get_cstringFitsExactlyIntoFreeSpace_expectCorrectCString();
      push_cstringTooLarge_expectNoAction();
      push_notInitialised_expectNoCrash();
      get_bufferSmallerThanStoredString_expectStringTruncated();
      get_bufferSmallerThanStoredFirstPart_expectStringTruncated();
      get_bufferSmallerThanStoredSecondPart_expectStringTruncated();
      get_emptybuffer_expectZeroLengthString();
      get_notInitialised_expectNoCrash();
    }
  public:
    static void pop_noRingBufferRollover_expectOldestCStringRemoved(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      const char testCStringPrefix[] = "test_string_";
      static const size_t maxGetCStrings = 32;
      static const size_t getCStringSize = 32;
      char testCStrings[maxGetCStrings][getCStringSize] = {};
      char getResult[maxGetCStrings][getCStringSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t stringsFitIntoBuffer = 0;
      do {
        strcpy(testCStrings[stringsFitIntoBuffer], testCStringPrefix);
        char numberString[getCStringSize] = {};
        static const int decimalRadix = 10;
        itoa(stringsFitIntoBuffer, numberString, decimalRadix);
        strcat(testCStrings[stringsFitIntoBuffer], numberString);
        testRingBuffer.push(testCStrings[stringsFitIntoBuffer]);
        stringsFitIntoBuffer++;
      } while (!testRingBuffer.full(testCStrings[stringsFitIntoBuffer - 1]));
      size_t countBeforePop = testRingBuffer.count();
      for (size_t i = 0; i < stringsFitIntoBuffer + 1; i++) {
        testRingBuffer.get(0, getResult[i], getCStringSize);
        testRingBuffer.pop();
      }
      size_t countAfterPop = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countBeforePop == stringsFitIntoBuffer);
      TEST_ASSERT(!countAfterPop);
      for (size_t i = 0; i < stringsFitIntoBuffer; i++) {
        TEST_ASSERT(!strcmp(testCStrings[i], getResult[i]));
      }
      TEST_FUNC_END();
    }
    static void pop_ringBufferRollover_expectRolloverCStringRemovedCorrectly(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      const char testCStringPrefix[] = "test_string_";
      const char testCStringFinal[] = "final_string";
      static const size_t maxGetCStrings = 32;
      static const size_t getCStringSize = 32;
      char testCStrings[maxGetCStrings][getCStringSize] = {};
      char getResult[maxGetCStrings][getCStringSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t stringsFitIntoBuffer = 0;
      do {
        strcpy(testCStrings[stringsFitIntoBuffer], testCStringPrefix);
        char numberString[getCStringSize] = {};
        static const int decimalRadix = 10;
        itoa(stringsFitIntoBuffer, numberString, decimalRadix);
        strcat(testCStrings[stringsFitIntoBuffer], numberString);
        testRingBuffer.push(testCStrings[stringsFitIntoBuffer]);
        stringsFitIntoBuffer++;
      } while (!testRingBuffer.full(testCStringFinal));
      strcpy(testCStrings[stringsFitIntoBuffer], testCStringFinal);
      testRingBuffer.push(testCStringFinal);
      size_t countBeforePop = testRingBuffer.count();
      for (size_t i = 0; i < stringsFitIntoBuffer; i++) {
        testRingBuffer.get(0, getResult[i], getCStringSize);
        testRingBuffer.pop();
      }
      size_t countAfterPop = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countBeforePop == stringsFitIntoBuffer);
      TEST_ASSERT(!countAfterPop);
      for (size_t i = 0; i < stringsFitIntoBuffer + 1; i++) {
        TEST_ASSERT(!strcmp(testCStrings[i + 1], getResult[i]));
      }
      TEST_FUNC_END();
    }
    static void pop_cstringFitsExactlyIntoFreeSpace_expectCStringRemovedCorrectly(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      const char testCString1[] = "=a longer test cstring to fill a ring buffer with=";
      const char testCString2[] = "2";
      static const size_t maxGetCStrings = 2;
      static const size_t getCStringSize = 51;
      char getResult[maxGetCStrings][getCStringSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t stringsFitIntoBuffer = 0;
      do {
        testRingBuffer.push(testCString1);
        stringsFitIntoBuffer++;
      } while (!testRingBuffer.full(testCString1));
      for (size_t i = 0; i < stringsFitIntoBuffer - 1 ; i++) {
        testRingBuffer.pop();
      }
      testRingBuffer.push(testCString2);
      size_t countResultBefore = testRingBuffer.count();
      testRingBuffer.get(0, getResult[0], getCStringSize);
      testRingBuffer.pop();
      size_t countResultAfter = testRingBuffer.count();
      testRingBuffer.get(0, getResult[1], getCStringSize);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!((testBufferSize - 1) % (strlen(testCString1) + 1)));
      TEST_ASSERT(countResultBefore == 2);
      TEST_ASSERT(countResultAfter == 1);
      TEST_ASSERT(!strcmp(testCString1, getResult[0]));
      TEST_ASSERT(!strcmp(testCString2, getResult[1]));
      TEST_FUNC_END();
    }
    static void pop_emptyRingBuffer_expectNoActionPerformed(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.pop();
      //assert
      TEST_ASSERT(validationResult);
      TEST_FUNC_END();
    }
    static void pop_notInitialised_expectNoActionPerformed(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::CStrRingBuffer testRingBuffer(NULL, 0);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.pop();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_FUNC_END();
    }
    static void test_pop(void) {
      pop_noRingBufferRollover_expectOldestCStringRemoved();
      pop_ringBufferRollover_expectRolloverCStringRemovedCorrectly();
      pop_cstringFitsExactlyIntoFreeSpace_expectCStringRemovedCorrectly();
      pop_emptyRingBuffer_expectNoActionPerformed();
      pop_notInitialised_expectNoActionPerformed();
    }
  public:
    static void count_emptyRingBuffer_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!countResult);
      TEST_FUNC_END();
    }
    static void count_singleCString_expectOne(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push("test");
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countResult == 1);
      TEST_FUNC_END();
    }
    static void count_multipleCStrings_expectCorrectNumber(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      char testCString[] = "a long test cstring to fill ring buffer with";
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t pushNumber = 0;
      while (!testRingBuffer.full(testCString)) {
        testRingBuffer.push(testCString);
        pushNumber++;
      }
      size_t countBefore = testRingBuffer.count();
      testRingBuffer.push(testCString);
      size_t countAfter = testRingBuffer.count();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(countBefore == pushNumber);
      TEST_ASSERT(countBefore == countAfter);
      TEST_FUNC_END();
    }
    static void count_notInitialised_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::CStrRingBuffer testRingBuffer(NULL, 0);
      //act
      boolean validationResult = testRingBuffer.validate();
      size_t countResult = testRingBuffer.count();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!countResult);
      TEST_FUNC_END();
    }
    static void test_count(void) {
      count_emptyRingBuffer_expectZero();
      count_singleCString_expectOne();
      count_multipleCStrings_expectCorrectNumber();
      count_notInitialised_expectZero();
    }
  public:
    static void full_fullRingBuffer_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      const char testCString[] = "TestCString";
      const size_t testCStringSize = strlen(testCString) + 1;
      //act
      boolean validationResult = testRingBuffer.validate();
      for (size_t i = 0; i < testBufferSize; i += testCStringSize)
        testRingBuffer.push(testCString);
      boolean fullResult = testRingBuffer.full(testCString);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(fullResult);
      TEST_FUNC_END();
    }
    static void full_nonFullRingBuffer_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      const char testCString[] = "TestCString";
      const size_t testCStringSize = strlen(testCString) + 1;
      //act
      boolean validationResult = testRingBuffer.validate();
      for (size_t i = 0; i < (testBufferSize - testCStringSize * 2); i += testCStringSize) {
        testRingBuffer.push(testCString);
      }
      boolean fullResult = testRingBuffer.full(testCString);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!fullResult);
      TEST_FUNC_END();
    }
    static void full_cstringFitsExactlyIntoFreeSpace_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      const char testCString[] = "0123456789ABCDEF";
      const size_t testCStringSize = strlen(testCString) + 1;
      //act
      boolean validationResult = testRingBuffer.validate();
      for (size_t i = 0; i < (testBufferSize - 1) / testCStringSize - 1; i++)
        testRingBuffer.push(testCString);
      boolean fullResult = testRingBuffer.full(testCString);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!((testBufferSize - 1) % testCStringSize));
      TEST_ASSERT(!fullResult);
      TEST_FUNC_END();
    }
    static void full_zeroLengthInputStringFullBuffer_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      const char testCString[] = "0123456789ABCDEF";
      const size_t testCStringSize = strlen(testCString) + 1;
      const char emptyCString[] = "";
      //act
      boolean validationResult = testRingBuffer.validate();
      while (!testRingBuffer.full(testCString))
        testRingBuffer.push(testCString);
      boolean fullResult = testRingBuffer.full(emptyCString);
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!((testBufferSize - 1) % testCStringSize));
      TEST_ASSERT(!fullResult);
      TEST_FUNC_END();
    }
    static void full_notInitialised_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::CStrRingBuffer testRingBuffer(NULL, 0);
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean fullResult = testRingBuffer.full(" ");
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(!fullResult);
      TEST_FUNC_END();
    }
    static void test_full(void) {
      full_fullRingBuffer_expectTrue();
      full_nonFullRingBuffer_expectFalse();
      full_cstringFitsExactlyIntoFreeSpace_expectFalse();
      full_zeroLengthInputStringFullBuffer_expectFalse();
      full_notInitialised_expectFalse();
    }
  public:
    static void empty_emptyRingBuffer_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean emptyResult = testRingBuffer.empty();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(emptyResult);
      TEST_FUNC_END();
    }
    static void empty_nonEmptyRingBuffer_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      char testBuffer[testBufferSize] = {};
      util::arrays::CStrRingBuffer testRingBuffer(testBuffer, testBufferSize);
      const char testCString[] = "a";
      //act
      boolean validationResult = testRingBuffer.validate();
      testRingBuffer.push(testCString);
      boolean emptyResult = testRingBuffer.empty();
      //assert
      TEST_ASSERT(validationResult);
      TEST_ASSERT(!emptyResult);
      TEST_FUNC_END();
    }
    static void empty_notInitialised_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      util::arrays::CStrRingBuffer testRingBuffer(NULL, 0);
      //act
      boolean validationResult = testRingBuffer.validate();
      boolean emptyResult = testRingBuffer.empty();
      //assert
      TEST_ASSERT(!validationResult);
      TEST_ASSERT(emptyResult);
      TEST_FUNC_END();
    }
    static void test_empty(void) {
      empty_emptyRingBuffer_expectTrue();
      empty_nonEmptyRingBuffer_expectFalse();
      empty_notInitialised_expectTrue();
    }
  public:
#pragma GCC push_options
#pragma GCC optimize ("O0")
    static const size_t perfBufferSize = 2048;
    static void test_performance_push_shortStrings(void) {
      PERF_TEST_FUNC_START();
      static char perfBuffer[perfBufferSize];
      util::arrays::CStrRingBuffer testRingBuffer(perfBuffer, perfBufferSize);
      static const uint32_t numberOfOperations = 4096;
      const char testCString[] = "short str";
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++) {
        testRingBuffer.push(testCString);
      }
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void test_performance_push_longStrings(void) {
      PERF_TEST_FUNC_START();
      static char perfBuffer[perfBufferSize];
      util::arrays::CStrRingBuffer testRingBuffer(perfBuffer, perfBufferSize);
      static const uint32_t numberOfOperations = 4096;
      const char testCString[] = "a particularily long string to effectively fill a ring buffer with in order to simulate realistic conditions during performance testing";
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++) {
        testRingBuffer.push(testCString);
      }
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void test_performance_get_shortStrings(void) {
      PERF_TEST_FUNC_START();
      static char perfBuffer[perfBufferSize];
      util::arrays::CStrRingBuffer testRingBuffer(perfBuffer, perfBufferSize);
      static const uint32_t numberOfOperations = 4096;
      const char testCString[] = "short str";
      do {
        testRingBuffer.push(testCString);
      } while (!testRingBuffer.full(testCString));
      testRingBuffer.push(testCString);
      testRingBuffer.push(testCString);
      static const size_t getBufferSize = 10;
      char getBuffer[getBufferSize] = {};
      const size_t index = testRingBuffer.count() - 2;
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++) {
        testRingBuffer.get(index, getBuffer, getBufferSize);
      }
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void test_performance_get_longStrings(void) {
      PERF_TEST_FUNC_START();
      static char perfBuffer[perfBufferSize];
      util::arrays::CStrRingBuffer testRingBuffer(perfBuffer, perfBufferSize);
      static const uint32_t numberOfOperations = 4096;
      const char testCString[] = "a particularily long string to effectively fill a ring buffer with in order to simulate realistic conditions during performance testing";
      do {
        testRingBuffer.push(testCString);
      } while (!testRingBuffer.full(testCString));
      testRingBuffer.push(testCString);
      static const size_t getBufferSize = 140;
      char getBuffer[getBufferSize] = {};
      const size_t index = testRingBuffer.count() - 2;
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++) {
        testRingBuffer.get(index, getBuffer, getBufferSize);
      }
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void test_performance(void) {
      test_performance_push_shortStrings();
      test_performance_push_longStrings();
      test_performance_get_shortStrings();
      test_performance_get_longStrings();
    }
#pragma GCC pop_options
  public:
    static void runTests(void) {
      test_validate();
      test_push_get();
      test_pop();
      test_count();
      test_full();
      test_empty();
      test_performance();
    }
};

class TestPrintToBuffer {
  public:
    static void writeSingle_writeSingleChar_expectCorrectCharInBuffer(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t bufferSize = 5;
      char buffer[bufferSize] = {};
      util::arrays::PrintToBuffer testPrint(buffer, bufferSize);
      static const char testChar1 = 'a';
      static const char testChar2 = 'b';
      static const char nullTerminator = '\0';
      size_t writeResult[bufferSize] = {};
      //act
      writeResult[0] = testPrint.write(testChar1);
      writeResult[1] = testPrint.write(testChar2);
      //assert
      TEST_ASSERT(buffer[0] == testChar1);
      TEST_ASSERT(buffer[1] == testChar2);
      TEST_ASSERT(buffer[2] == nullTerminator);
      TEST_ASSERT(writeResult[0] == 1);
      TEST_ASSERT(writeResult[1] == 1);
      TEST_FUNC_END();
    }
    static void writeSingle_bufferOverflow_expectExtraCharsIgnored(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t bufferSize = 5;
      char buffer[bufferSize] = {};
      util::arrays::PrintToBuffer testPrint(buffer, bufferSize);
      static const char testChar = 'a';
      static const char nullTerminator = '\0';
      static const size_t nullTerminatorSize = 1;
      size_t writeResult[bufferSize] = {};
      //act
      for (size_t i = 0; i < bufferSize; i++)
        writeResult[i] = testPrint.write(testChar);
      //assert
      for (size_t i = 0; i < bufferSize - nullTerminatorSize; i++) {
        TEST_ASSERT(buffer[i] == testChar);
        TEST_ASSERT(writeResult[i] == 1);
      }
      TEST_ASSERT(buffer[bufferSize - 1] == nullTerminator);
      TEST_ASSERT(!writeResult[bufferSize - 1]);
      TEST_FUNC_END();
    }
    static void writeSingle_writeSingleCharToNonEmptyBuffer_expectSingleCharInBuffer(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t bufferSize = 5;
      char buffer[bufferSize] = "1234";
      util::arrays::PrintToBuffer testPrint(buffer, bufferSize);
      static const char testChar = 'a';
      static const char nullTerminator = '\0';
      size_t writeResult[bufferSize] = {};
      //act
      writeResult[0] = testPrint.write(testChar);
      //assert
      TEST_ASSERT(buffer[0] == testChar);
      TEST_ASSERT(buffer[1] == nullTerminator);
      TEST_ASSERT(writeResult[0] == 1);
      TEST_FUNC_END();
    }
    static void test_writeSingle(void) {
      writeSingle_writeSingleChar_expectCorrectCharInBuffer();
      writeSingle_bufferOverflow_expectExtraCharsIgnored();
      writeSingle_writeSingleCharToNonEmptyBuffer_expectSingleCharInBuffer();
    }
  public:
    static void writeBuffer_writeCString_expectCorrectCStringWritten(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t bufferSize = 10;
      char buffer[bufferSize] = {};
      util::arrays::PrintToBuffer testPrint(buffer, bufferSize);
      const char testCString1[] = "abcde";
      const char testCString2[] = "1234";
      char referenceCString[bufferSize];
      strcpy(referenceCString, testCString1);
      strcat(referenceCString, testCString2);
      size_t writeResult[2] = {};
      //act
      writeResult[0] = testPrint.write(reinterpret_cast<const uint8_t*>(testCString1), strlen(testCString1));
      writeResult[1] = testPrint.write(reinterpret_cast<const uint8_t*>(testCString2), strlen(testCString2));
      //assert
      TEST_ASSERT(!strcmp(buffer, referenceCString));
      TEST_ASSERT(writeResult[0] == strlen(testCString1));
      TEST_ASSERT(writeResult[1] == strlen(testCString2));
      TEST_FUNC_END();
    }
    static void writeBuffer_bufferOverflow_expectCStringTruncated(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t bufferSize = 5;
      char buffer[bufferSize] = {};
      util::arrays::PrintToBuffer testPrint(buffer, bufferSize);
      const char testCString[] = "abcdefgh";
      static const size_t nullTerminatorSize = 1;
      //act
      size_t writeResult = testPrint.write(reinterpret_cast<const uint8_t*>(testCString), strlen(testCString));
      //assert
      TEST_ASSERT(!strncmp(buffer, testCString, bufferSize - nullTerminatorSize));
      TEST_ASSERT(writeResult == bufferSize - 1);
      TEST_FUNC_END();
    }
    static void test_writeBuffer(void) {
      writeBuffer_writeCString_expectCorrectCStringWritten();
      writeBuffer_bufferOverflow_expectCStringTruncated();
    }
  public:
    static void runTests(void) {
      test_writeSingle();
      test_writeBuffer();
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestRingBuffer::runTests();
  TestCstrRingBuffer::runTests();
  TestPrintToBuffer::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
