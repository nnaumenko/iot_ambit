#include <ESP8266WiFi.h>

#include "test.h"
#include "fakestream.h"

#include "util_data.h"

template<typename T>
class TestRef {
  public:
    static void constructorAccessor_initRamPointer_expectSameRamPointer(void) {
      TEST_FUNC_START();
      //arrange
      T testValue {};
      const T * testPointer = &testValue;
      const boolean testPgm = false;
      //act
      util::Ref<T> testRef(testPointer, testPgm);
      const T * resultPointer = static_cast<const T*>(testRef);
      const boolean resultPgm = testRef.pgm();
      //assert
      TEST_ASSERT(resultPointer == testPointer);
      TEST_ASSERT(resultPgm == testPgm);
      TEST_FUNC_END();
    }
    static void constructorAccessor_initProgmemPointer_expectSameProgmemPointer(void) {
      TEST_FUNC_START();
      //arrange
      T testValue {};
      const T * testPointer = &testValue;
      const boolean testPgm = true;
      //act
      util::Ref<T> testRef(testPointer, testPgm);
      const T * resultPointer = static_cast<const T*>(testRef);
      const boolean resultPgm = testRef.pgm();
      //assert
      TEST_ASSERT(resultPointer == testPointer);
      TEST_ASSERT(resultPgm == testPgm);
      TEST_FUNC_END();
    }
    static void constructorAccessor_initNullptrPointer_expectNullptrPointer(void) {
      TEST_FUNC_START();
      //arrange
      const T * testPointer = nullptr;
      const boolean testPgm = false;
      //act
      util::Ref<T> testRef(testPointer, testPgm);
      const T * resultPointer = static_cast<const T*>(testRef);
      const boolean resultPgm = testRef.pgm();
      //assert
      TEST_ASSERT(resultPointer == testPointer);
      TEST_ASSERT(resultPgm == testPgm);
      TEST_FUNC_END();
    }
    static void testConstructorAndAccessors(void) {
      constructorAccessor_initRamPointer_expectSameRamPointer();
      constructorAccessor_initProgmemPointer_expectSameProgmemPointer();
      constructorAccessor_initNullptrPointer_expectNullptrPointer();
    }
  public:
    static void operatorBool_nullptrRam_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      const T * testPointer = nullptr;
      const boolean testPgm = false;
      util::Ref<T> testRef(testPointer, testPgm);
      //act
      const boolean result = testRef;
      //assert
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    static void operatorBool_nullptrProgmem_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      const T * testPointer = nullptr;
      const boolean testPgm = true;
      util::Ref<T> testRef(testPointer, testPgm);
      //act
      const boolean result = testRef;
      //assert
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    static void operatorBool_nonNullptrRam_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      T testValue {};
      const T * testPointer = &testValue;
      const boolean testPgm = false;
      util::Ref<T> testRef(testPointer, testPgm);
      //act
      const boolean result = testRef;
      //assert
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    static void operatorBool_nonNullptrProgmem_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      T testValue {};
      const T * testPointer = &testValue;
      const boolean testPgm = true;
      util::Ref<T> testRef(testPointer, testPgm);
      //act
      const boolean result = testRef;
      //assert
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    static void testOperatorBool(void) {
      operatorBool_nullptrRam_expectFalse();
      operatorBool_nullptrProgmem_expectFalse();
      operatorBool_nonNullptrRam_expectTrue();
      operatorBool_nonNullptrProgmem_expectTrue();
    }
  public:
    static void assignment_initRamPointer_expectSameRamPointer(void) {
      TEST_FUNC_START();
      //arrange
      T testValueSrc {};
      T testValueDst {};
      const T * testPointerSrc = &testValueSrc;
      const T * testPointerDst = &testValueDst;
      const boolean testPgmSrc = false;
      const boolean testPgmDst = true;
      util::Ref<T> testSrc(testPointerSrc, testPgmSrc);
      util::Ref<T> testDst(testPointerDst, testPgmDst);
      //act
      testDst = testSrc;
      const T * resultPointer = static_cast<const T*>(testDst);
      const boolean resultPgm = testDst.pgm();
      //assert
      TEST_ASSERT(resultPointer == testPointerSrc);
      TEST_ASSERT(resultPgm == testPgmSrc);
      TEST_FUNC_END();
    }
    static void assignment_initProgmemPointer_expectSameProgmemPointer(void) {
      TEST_FUNC_START();
      //arrange
      T testValueSrc {};
      T testValueDst {};
      const T * testPointerSrc = &testValueSrc;
      const T * testPointerDst = &testValueDst;
      const boolean testPgmSrc = true;
      const boolean testPgmDst = false;
      util::Ref<T> testSrc(testPointerSrc, testPgmSrc);
      util::Ref<T> testDst(testPointerDst, testPgmDst);
      //act
      testDst = testSrc;
      const T * resultPointer = static_cast<const T*>(testDst);
      const boolean resultPgm = testDst.pgm();
      //assert
      TEST_ASSERT(resultPointer == testPointerSrc);
      TEST_ASSERT(resultPgm == testPgmSrc);
      TEST_FUNC_END();
    }
    static void assignment_initNullptrPointer_expectNullptrPointer(void) {
      TEST_FUNC_START();
      //arrange
      T testValueDst {};
      const T * testPointerSrc = nullptr;
      const T * testPointerDst = &testValueDst;
      const boolean testPgmSrc = false;
      const boolean testPgmDst = true;
      util::Ref<T> testSrc(testPointerSrc, testPgmSrc);
      util::Ref<T> testDst(testPointerDst, testPgmDst);
      //act
      testDst = testSrc;
      const T * resultPointer = static_cast<const T*>(testDst);
      const boolean resultPgm = testDst.pgm();
      //assert
      TEST_ASSERT(resultPointer == testPointerSrc);
      TEST_ASSERT(resultPgm == testPgmSrc);
      TEST_FUNC_END();
    }
    static void testAssignment(void) {
      assignment_initRamPointer_expectSameRamPointer();
      assignment_initProgmemPointer_expectSameProgmemPointer();
      assignment_initNullptrPointer_expectNullptrPointer();
    }
  public:
    static void equalOperator_compareSamePointerSameMemoryAreas_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      T testValue {};
      const T * testPointer1 = &testValue;
      const T * testPointer2 = &testValue;
      const boolean testPgm1 = true;
      const boolean testPgm2 = true;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    static void equalOperator_compareSamePointerDifferentMemoryAreas_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      T testValue {};
      const T * testPointer1 = &testValue;
      const T * testPointer2 = &testValue;
      const boolean testPgm1 = true;
      const boolean testPgm2 = false;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    static void equalOperator_compareDifferentPointerSameMemoryAreas_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      T testValue1 {};
      T testValue2 {};
      const T * testPointer1 = &testValue1;
      const T * testPointer2 = &testValue2;
      const boolean testPgm1 = false;
      const boolean testPgm2 = false;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    static void equalOperator_compareDifferentPointerDifferentMemoryAreas_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      T testValue1 {};
      T testValue2 {};
      const T * testPointer1 = &testValue1;
      const T * testPointer2 = &testValue2;
      const boolean testPgm1 = true;
      const boolean testPgm2 = false;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    static void equalOperator_compareNullptrsInRam_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      const T * testPointer1 = nullptr;
      const T * testPointer2 = nullptr;
      const boolean testPgm1 = false;
      const boolean testPgm2 = false;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    static void equalOperator_compareNullptrsInProgmem_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      const T * testPointer1 = nullptr;
      const T * testPointer2 = nullptr;
      const boolean testPgm1 = true;
      const boolean testPgm2 = true;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    static void equalOperator_compareNullptrsInDifferentMemoryAreas_expectTrue(void) {
      TEST_FUNC_START();
      //arrange
      const T * testPointer1 = nullptr;
      const T * testPointer2 = nullptr;
      const boolean testPgm1 = false;
      const boolean testPgm2 = true;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    static void equalOperator_compareNullptrToPointer_expectFalse(void) {
      TEST_FUNC_START();
      //arrange
      T testValue {};
      const T * testPointer1 = &testValue;
      const T * testPointer2 = nullptr;
      const boolean testPgm1 = false;
      const boolean testPgm2 = false;
      util::Ref<T> testRef1(testPointer1, testPgm1);
      util::Ref<T> testRef2(testPointer2, testPgm2);
      //act
      boolean result = (testRef1 == testRef2);
      //assert
      TEST_ASSERT(!result);
      TEST_FUNC_END();
    }
    static void testEqualOperator(void) {
      equalOperator_compareSamePointerSameMemoryAreas_expectTrue();
      equalOperator_compareSamePointerDifferentMemoryAreas_expectFalse();
      equalOperator_compareDifferentPointerSameMemoryAreas_expectFalse();
      equalOperator_compareDifferentPointerDifferentMemoryAreas_expectFalse();
      equalOperator_compareNullptrsInRam_expectTrue();
      equalOperator_compareNullptrsInProgmem_expectTrue();
      equalOperator_compareNullptrsInDifferentMemoryAreas_expectTrue();
      equalOperator_compareNullptrToPointer_expectFalse();
    }
  public:
    static void runTests(void) {
      testConstructorAndAccessors();
      testOperatorBool();
      testAssignment();
      testEqualOperator();
    }
};

class TestStrRef : public TestRef<char> {
  public:
    static void strRef_constructor_initWithConstCharPointer_expectReferenceToCStringInRam(void) {
      TEST_FUNC_START();
      //arrange
      const char testStr[] = "testStr";
      //act
      util::StrRef testStrRef(testStr);
      const char * resultPointer = static_cast<const char *>(testStrRef);
      const boolean resultPgm = testStrRef.pgm();
      //assert
      TEST_ASSERT(resultPointer == testStr);
      TEST_ASSERT(!resultPgm);
      TEST_FUNC_END();
    }
    static void strRef_constructor_initWithFlashStringHelper_expectReferenceToCStringInProgmem(void) {
      TEST_FUNC_START();
      //arrange
      static const char PROGMEM testStr[] = "testStr";
      const __FlashStringHelper * testPtr = FPSTR(testStr);
      //act
      util::StrRef testStrRef(testPtr);
      const char * resultPointer = static_cast<const char *>(testStrRef);
      const boolean resultPgm = testStrRef.pgm();
      //assert
      TEST_ASSERT(resultPointer == testStr);
      TEST_ASSERT(resultPgm);
      TEST_FUNC_END();
    }
    static void strRef_testConstructors(void) {
      strRef_constructor_initWithConstCharPointer_expectReferenceToCStringInRam();
      strRef_constructor_initWithFlashStringHelper_expectReferenceToCStringInProgmem();
    }
  public:
    static void strRef_get_CStringInRam_expectCStringCopied(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 8;
      const char testStr[testBufferSize] = "testStr";
      util::StrRef testStrRef(testStr);
      char buffer[testBufferSize] = "buffer";
      //act
      testStrRef.get(buffer, testBufferSize);
      //assert
      TEST_ASSERT(!strcmp(buffer, testStr));
      TEST_FUNC_END();
    }
    static void strRef_get_CStringInProgmem_expectCStringCopied(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 8;
      static const char PROGMEM testStr[testBufferSize] = "testStr";
      const __FlashStringHelper * testPtr = FPSTR(testStr);
      util::StrRef testStrRef(testPtr);
      char buffer[testBufferSize] = "buffer";
      //act
      testStrRef.get(buffer, testBufferSize);
      //assert
      TEST_ASSERT(!strcmp_P(buffer, testStr));
      TEST_FUNC_END();
    }
    static void strRef_get_tooLongCStringInRam_expectCStringTruncated(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 5;
      const char testStr[] = "testStr";
      util::StrRef testStrRef(testStr);
      char buffer[testBufferSize] = "buff";
      static const size_t nullTerminatorSize = 1;
      //act
      testStrRef.get(buffer, testBufferSize);
      //assert
      TEST_ASSERT(strlen(testStr) >= testBufferSize);
      TEST_ASSERT(!strncmp(buffer, testStr, testBufferSize - nullTerminatorSize));
      TEST_ASSERT(strlen(buffer) == (testBufferSize - nullTerminatorSize));
      TEST_FUNC_END();
    }
    static void strRef_get_tooLongCStringInProgmem_expectCStringTruncated(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 5;
      static const char PROGMEM testStr[] = "testStr";
      const __FlashStringHelper * testPtr = FPSTR(testStr);
      util::StrRef testStrRef(testPtr);
      char buffer[testBufferSize] = "buff";
      static const size_t nullTerminatorSize = 1;
      //act
      testStrRef.get(buffer, testBufferSize);
      //assert
      TEST_ASSERT(strlen_P(testStr) >= testBufferSize);
      TEST_ASSERT(!strncmp_P(buffer, testStr, testBufferSize - nullTerminatorSize));
      TEST_ASSERT(strlen(buffer) == (testBufferSize - nullTerminatorSize));
      TEST_FUNC_END();
    }
    static void strRef_get_nullptrInRam_expectEmptyBuffer(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 8;
      const char * testStr = nullptr;
      util::StrRef testStrRef(testStr);
      char buffer[testBufferSize] = "test";
      //act
      testStrRef.get(buffer, testBufferSize);
      //assert
      TEST_ASSERT(!strlen(buffer));
      TEST_FUNC_END();
    }
    static void strRef_get_nullptrInProgmem_expectEmptyBuffer(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 8;
      const char * testStr = nullptr;
      const __FlashStringHelper * testPtr = FPSTR(testStr);
      util::StrRef testStrRef(testPtr);
      char buffer[testBufferSize] = "test";
      //act
      testStrRef.get(buffer, testBufferSize);
      //assert
      TEST_ASSERT(!strlen(buffer));
      TEST_FUNC_END();
    }
    static void strRef_get_emptyString_expectEmptyBuffer(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 8;
      const char testStr[testBufferSize] = "";
      util::StrRef testStrRef(testStr);
      char buffer[testBufferSize] = "buffer";
      //act
      testStrRef.get(buffer, testBufferSize);
      //assert
      TEST_ASSERT(!strlen(buffer));
      TEST_FUNC_END();
    }
    static void strRef_get_bufferSizeZero_expectBufferNotModified(void) {
      TEST_FUNC_START();
      //arrange
      static const size_t testBufferSize = 8;
      const char testStr[testBufferSize] = "testStr";
      util::StrRef testStrRef(testStr);
      const char refStr[testBufferSize] = "buffer";
      char buffer[testBufferSize];
      strcpy(buffer, refStr);
      //act
      testStrRef.get(buffer, 0);
      //assert
      TEST_ASSERT(strcmp(refStr, testStr));
      TEST_ASSERT(!strcmp(buffer, refStr));
      TEST_FUNC_END();
    }
    static void strRef_testGet(void) {
      strRef_get_CStringInRam_expectCStringCopied();
      strRef_get_CStringInProgmem_expectCStringCopied();
      strRef_get_tooLongCStringInRam_expectCStringTruncated();
      strRef_get_tooLongCStringInProgmem_expectCStringTruncated();
      strRef_get_nullptrInRam_expectEmptyBuffer();
      strRef_get_nullptrInProgmem_expectEmptyBuffer();
      strRef_get_emptyString_expectEmptyBuffer();
      strRef_get_bufferSizeZero_expectBufferNotModified();
    }
  public:
    static void strRef_print_printCStringInRam_expectCStringPrintedCorrectly(void) {
      TEST_FUNC_START();
      //arrange
      const char testStr[] = "testStr";
      util::StrRef testStrRef(testStr);
      FakeStreamOut outputStreamMock;
      outputStreamMock.begin();
      //act
      testStrRef.print(outputStreamMock);
      //assert
      TEST_ASSERT(!strcmp(outputStreamMock.getOutBufferContent(), testStr));
      TEST_FUNC_END();
    }
    static void strRef_print_printCStringInProgmem_expectCStringPrintedCorrectly(void) {
      TEST_FUNC_START();
      //arrange
      static const char PROGMEM testStr[] = "testStr";
      const __FlashStringHelper * testPtr = FPSTR(testStr);
      util::StrRef testStrRef(testPtr);
      FakeStreamOut outputStreamMock;
      outputStreamMock.begin();
      //act
      testStrRef.print(outputStreamMock);
      //assert
      TEST_ASSERT(!strcmp_P(outputStreamMock.getOutBufferContent(), testStr));
      TEST_FUNC_END();
    }
    static void strRef_print_printNullptr_expectNothingPrinted(void) {
      TEST_FUNC_START();
      //arrange
      const char * testStr = nullptr;
      util::StrRef testStrRef(testStr);
      FakeStreamOut outputStreamMock;
      outputStreamMock.begin();
      //act
      testStrRef.print(outputStreamMock);
      //assert
      TEST_ASSERT(!strlen(outputStreamMock.getOutBufferContent()));
      TEST_FUNC_END();
    }
    static void strRef_testPrint(void) {
      strRef_print_printCStringInRam_expectCStringPrintedCorrectly();
      strRef_print_printCStringInProgmem_expectCStringPrintedCorrectly();
      strRef_print_printNullptr_expectNothingPrinted();
    }
  public:
    static void runStrRefTests(void) {
      strRef_testConstructors();
      strRef_testGet();
      strRef_testPrint();
      TEST_PRINTLN_DATA(sizeof(util::StrRef));
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestRef<int>::runTests();
  TestStrRef::runTests();
  TestStrRef::runStrRefTests();
  TEST_END();
}

void loop() {
  delay(100);
}
