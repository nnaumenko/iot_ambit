#include <ESP8266WiFi.h>
#include "test.h"
#include "stringmap.h"

#ifndef ESP8266
#warning "Please select a ESP8266 board in Tools/Board"
#endif

const PROGMEM QuickStringMapItem testQuickStringMapItems[] {
  {0, "testval0"},
  {1, "testval1"},
  {2, "testval2"},
  { -1, "testval-1"},
  { 10, ""}
};

class TestQuickStringMap {
  public:
    static void getDefaultKey_ExpectCorrectDefaultKey (void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      StringMapKey defaultKey = testStringMap.getDefaultKey();
      //assert
      TEST_ASSERT (defaultKey == 10);
      TEST_FUNC_END();
    }
  public:
    static void test_getDefaultKey(void) {
      getDefaultKey_ExpectCorrectDefaultKey();
    }
  public:
    static void findChar_Found_ExpectValidKey(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      StringMapKey found = testStringMap.find("testval2");
      //assert
      TEST_ASSERT (found == 2);
      TEST_FUNC_END();
    }
    static void findChar_NotFound_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      StringMapKey notFound = testStringMap.find("testval3");
      //assert
      TEST_ASSERT (notFound == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
    static void testQuickStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      StringMapKey negativeKey = testStringMap.find("testval-1");
      //assert
      TEST_ASSERT (negativeKey == -1);
      TEST_FUNC_END();
    }
    static void findChar_CaseSensitive_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      StringMapKey notFoundBecauseFindIsCaseSensitive = testStringMap.find("Testval2");
      //assert
      TEST_ASSERT (notFoundBecauseFindIsCaseSensitive == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
    static void findChar_EmptyString_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      StringMapKey notFoundBecauseEmptyString = testStringMap.find("");
      //assert
      TEST_ASSERT (notFoundBecauseEmptyString == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
    static void findChar_NULLString_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      StringMapKey notFoundBecauseNULL = testStringMap.find((char*)NULL);
      //assert
      TEST_ASSERT (notFoundBecauseNULL == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
  public:
    static void test_findChar(void) {
      findChar_Found_ExpectValidKey();
      findChar_NotFound_ExpectDefaultKey();
      testQuickStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey();
      findChar_CaseSensitive_ExpectDefaultKey();
      findChar_EmptyString_ExpectDefaultKey();
      findChar_NULLString_ExpectDefaultKey();
    }
  public:
    static void isProgmem_Found_ExpectTrue(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean found = testStringMap.isProgmem(1);
      //assert
      TEST_ASSERT (found);
      TEST_FUNC_END();
    }
    static void isProgmem_NotFound_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFound = testStringMap.isProgmem(3);
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void isProgmem_DefaultKey_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFound = testStringMap.isProgmem(testStringMap.getDefaultKey());
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
  public:
    static void test_isProgmem(void) {
      isProgmem_Found_ExpectTrue();
      isProgmem_NotFound_ExpectFalse();
      isProgmem_DefaultKey_ExpectFalse();
    }
  public:
    static void findStringMapKey_Found_ExpectValidString(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      char * found = testStringMap.find(1);
      //assert
      TEST_ASSERT (!strcmp_P("testval1", found));
      TEST_FUNC_END();
    }
    static void findStringMapKey_NotFound_ExpectNULL(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      char * notFound = testStringMap.find(3);
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void findStringMapKey_FindDefault_ExpectNULL(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      char * notFoundBecauseDefaultKey = testStringMap.find(testStringMap.getDefaultKey());
      //assert
      TEST_ASSERT (!notFoundBecauseDefaultKey);
      TEST_FUNC_END();
    }
  public:
    static void test_findStringMapKey(void) {
      findStringMapKey_Found_ExpectValidString();
      findStringMapKey_NotFound_ExpectNULL();
      findStringMapKey_FindDefault_ExpectNULL();
    }
  public:
    static void constainsChar_Found_ExpectTrue(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean found = testStringMap.contains("testval2");
      //assert
      TEST_ASSERT (found);
      TEST_FUNC_END();
    }
    static void constainsChar_NotFound_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFound = testStringMap.contains("testval3");
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void constainsChar_CaseSensitive_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFoundBecauseCaseSensitive = testStringMap.contains("Testval2");
      //assert
      TEST_ASSERT (!notFoundBecauseCaseSensitive);
      TEST_FUNC_END();
    }
    static void constainsChar_EmptyString_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFoundBecauseEmptyString = testStringMap.contains("");
      //assert
      TEST_ASSERT (!notFoundBecauseEmptyString);
      TEST_FUNC_END();
    }
    static void constainsChar_NULLString_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFoundBecauseNULLString = testStringMap.contains((char*)NULL);
      //assert
      TEST_ASSERT (!notFoundBecauseNULLString);
      TEST_FUNC_END();
    }
  public:
    static void test_containsChar(void) {
      constainsChar_Found_ExpectTrue();
      constainsChar_NotFound_ExpectFalse();
      constainsChar_CaseSensitive_ExpectFalse();
      constainsChar_EmptyString_ExpectFalse();
      constainsChar_NULLString_ExpectFalse();
    }
  public:
    static void constainsStringMapKey_Found_ExpectTrue(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean found = testStringMap.contains(1);
      //assert
      TEST_ASSERT (found);
      TEST_FUNC_END();
    }
    static void constainsStringMapKey_NotFound_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFound = testStringMap.contains(3);
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void constainsStringMapKey_DefaultKey_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      //act
      boolean notFoundBecauseDefaultKey = testStringMap.contains(testStringMap.getDefaultKey());
      //assert
      TEST_ASSERT (!notFoundBecauseDefaultKey);
      TEST_FUNC_END();
    }
  public:
    static void test_containsStringMapKey(void) {
      constainsStringMapKey_Found_ExpectTrue();
      constainsStringMapKey_NotFound_ExpectFalse();
      constainsStringMapKey_DefaultKey_ExpectFalse();
    }
  public:
    static void runTests(void) {
      test_getDefaultKey();
      test_findChar();
      test_isProgmem();
      test_findStringMapKey();
      test_containsChar();
      test_containsStringMapKey();
    }
};

class TestStringMapIteratorWithQuickStringMap {
  public:
    static void first_expectFirstItem(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      StringMapIterator testIterator(testStringMap);
      //act
      testIterator.first();
      //assert
      TEST_ASSERT(testIterator.currentKey() == 0);
      TEST_ASSERT(testIterator.isCurrentStringProgmem());
      TEST_ASSERT(!strcmp_P("testval0", testIterator.currentString()));
      TEST_FUNC_END();

    }
    static void next_expectSecondItem(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      StringMapIterator testIterator(testStringMap);
      //act
      testIterator.first();
      testIterator.next();
      //assert
      TEST_ASSERT(testIterator.currentKey() == 1);
      TEST_ASSERT(testIterator.isCurrentStringProgmem());
      TEST_ASSERT(!strcmp_P("testval1", testIterator.currentString()));
      TEST_FUNC_END();
    }
    static void isDone_expectCorrectNumberOfIterations(void) {
      TEST_FUNC_START();
      //arrange
      QuickStringMap testStringMap (testQuickStringMapItems);
      StringMapIterator testIterator(testStringMap);
      //act
      testIterator.first();
      size_t i = 0;
      while (!testIterator.isDone()) {
        testIterator.next();
        i++;
      }
      //assert
      TEST_ASSERT(i == 4);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      first_expectFirstItem();
      next_expectSecondItem();
      isDone_expectCorrectNumberOfIterations();
    }
};

const char PROGMEM testProgmemStringMapString0[] = "testval0";
const char PROGMEM testProgmemStringMapString1[] = "testval1";
const char PROGMEM testProgmemStringMapString2[] = "testval2";
const char PROGMEM testProgmemStringMapString_1[] = "testval-1";

const PROGMEM ProgmemStringMapItem testProgmemStringMapItems[] {
  {0, testProgmemStringMapString0},
  {1, testProgmemStringMapString1},
  {2, testProgmemStringMapString2},
  { -1, testProgmemStringMapString_1},
  { 10, ""}
};

class TestProgmemStringMap {
  public:
    static void getDefaultKey_ExpectCorrectDefaultKey (void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      StringMapKey defaultKey = testStringMap.getDefaultKey();
      //assert
      TEST_ASSERT (defaultKey == 10);
      TEST_FUNC_END();
    }
  public:
    static void test_getDefaultKey(void) {
      getDefaultKey_ExpectCorrectDefaultKey();
    }
  public:
    static void findChar_Found_ExpectValidKey(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      StringMapKey found = testStringMap.find("testval2");
      //assert
      TEST_ASSERT (found == 2);
      TEST_FUNC_END();
    }
    static void findChar_NotFound_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      StringMapKey notFound = testStringMap.find("testval3");
      //assert
      TEST_ASSERT (notFound == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
    static void testProgmemStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      StringMapKey negativeKey = testStringMap.find("testval-1");
      //assert
      TEST_ASSERT (negativeKey == -1);
      TEST_FUNC_END();
    }
    static void findChar_CaseSensitive_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      StringMapKey notFoundBecauseFindIsCaseSensitive = testStringMap.find("Testval2");
      //assert
      TEST_ASSERT (notFoundBecauseFindIsCaseSensitive == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
    static void findChar_EmptyString_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      StringMapKey notFoundBecauseEmptyString = testStringMap.find("");
      //assert
      TEST_ASSERT (notFoundBecauseEmptyString == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
    static void findChar_NULLString_ExpectDefaultKey(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      StringMapKey notFoundBecauseNULL = testStringMap.find((char*)NULL);
      //assert
      TEST_ASSERT (notFoundBecauseNULL == testStringMap.getDefaultKey());
      TEST_FUNC_END();
    }
  public:
    static void test_findChar(void) {
      findChar_Found_ExpectValidKey();
      findChar_NotFound_ExpectDefaultKey();
      testProgmemStringMap_FindChar_NegativeKey_ExpectCorrectNegativeKey();
      findChar_CaseSensitive_ExpectDefaultKey();
      findChar_EmptyString_ExpectDefaultKey();
      findChar_NULLString_ExpectDefaultKey();
    }
  public:
    static void isProgmem_Found_ExpectTrue(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean found = testStringMap.isProgmem(1);
      //assert
      TEST_ASSERT (found);
      TEST_FUNC_END();
    }
    static void isProgmem_NotFound_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFound = testStringMap.isProgmem(3);
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void isProgmem_DefaultKey_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFound = testStringMap.isProgmem(testStringMap.getDefaultKey());
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
  public:
    static void test_isProgmem(void) {
      isProgmem_Found_ExpectTrue();
      isProgmem_NotFound_ExpectFalse();
      isProgmem_DefaultKey_ExpectFalse();
    }
  public:
    static void findStringMapKey_Found_ExpectValidString(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      char * found = testStringMap.find(1);
      //assert
      TEST_ASSERT (!strcmp_P("testval1", found));
      TEST_FUNC_END();
    }
    static void findStringMapKey_NotFound_ExpectNULL(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      char * notFound = testStringMap.find(3);
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void findStringMapKey_FindDefault_ExpectNULL(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      char * notFoundBecauseDefaultKey = testStringMap.find(testStringMap.getDefaultKey());
      //assert
      TEST_ASSERT (!notFoundBecauseDefaultKey);
      TEST_FUNC_END();
    }
  public:
    static void test_findStringMapKey(void) {
      findStringMapKey_Found_ExpectValidString();
      findStringMapKey_NotFound_ExpectNULL();
      findStringMapKey_FindDefault_ExpectNULL();
    }
  public:
    static void constainsChar_Found_ExpectTrue(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean found = testStringMap.contains("testval2");
      //assert
      TEST_ASSERT (found);
      TEST_FUNC_END();
    }
    static void constainsChar_NotFound_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFound = testStringMap.contains("testval3");
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void constainsChar_CaseSensitive_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFoundBecauseCaseSensitive = testStringMap.contains("Testval2");
      //assert
      TEST_ASSERT (!notFoundBecauseCaseSensitive);
      TEST_FUNC_END();
    }
    static void constainsChar_EmptyString_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFoundBecauseEmptyString = testStringMap.contains("");
      //assert
      TEST_ASSERT (!notFoundBecauseEmptyString);
      TEST_FUNC_END();
    }
    static void constainsChar_NULLString_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFoundBecauseNULLString = testStringMap.contains((char*)NULL);
      //assert
      TEST_ASSERT (!notFoundBecauseNULLString);
      TEST_FUNC_END();
    }
  public:
    static void test_containsChar(void) {
      constainsChar_Found_ExpectTrue();
      constainsChar_NotFound_ExpectFalse();
      constainsChar_CaseSensitive_ExpectFalse();
      constainsChar_EmptyString_ExpectFalse();
      constainsChar_NULLString_ExpectFalse();
    }
  public:
    static void constainsStringMapKey_Found_ExpectTrue(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean found = testStringMap.contains(1);
      //assert
      TEST_ASSERT (found);
      TEST_FUNC_END();
    }
    static void constainsStringMapKey_NotFound_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFound = testStringMap.contains(3);
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void constainsStringMapKey_DefaultKey_ExpectFalse(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      //act
      boolean notFoundBecauseDefaultKey = testStringMap.contains(testStringMap.getDefaultKey());
      //assert
      TEST_ASSERT (!notFoundBecauseDefaultKey);
      TEST_FUNC_END();
    }
  public:
    static void test_containsStringMapKey(void) {
      constainsStringMapKey_Found_ExpectTrue();
      constainsStringMapKey_NotFound_ExpectFalse();
      constainsStringMapKey_DefaultKey_ExpectFalse();
    }
  public:
    static void runTests(void) {
      test_getDefaultKey();
      test_findChar();
      test_isProgmem();
      test_findStringMapKey();
      test_containsChar();
      test_containsStringMapKey();
    }
};

class TestStringMapIteratorWithProgmemStringMap {
  public:
    static void first_expectFirstItem(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      StringMapIterator testIterator(testStringMap);
      //act
      testIterator.first();
      //assert
      TEST_ASSERT(testIterator.currentKey() == 0);
      TEST_ASSERT(testIterator.isCurrentStringProgmem());
      TEST_ASSERT(!strcmp_P("testval0", testIterator.currentString()));
      TEST_FUNC_END();

    }
    static void next_expectSecondItem(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      StringMapIterator testIterator(testStringMap);
      //act
      testIterator.first();
      testIterator.next();
      //assert
      TEST_ASSERT(testIterator.currentKey() == 1);
      TEST_ASSERT(testIterator.isCurrentStringProgmem());
      TEST_ASSERT(!strcmp_P("testval1", testIterator.currentString()));
      TEST_FUNC_END();
    }
    static void isDone_expectCorrectNumberOfIterations(void) {
      TEST_FUNC_START();
      //arrange
      ProgmemStringMap testStringMap (testProgmemStringMapItems);
      StringMapIterator testIterator(testStringMap);
      //act
      testIterator.first();
      size_t i = 0;
      while (!testIterator.isDone()) {
        testIterator.next();
        i++;
      }
      //assert
      TEST_ASSERT(i == 4);
      TEST_FUNC_END();
    }
  public:
    static void runTests(void) {
      first_expectFirstItem();
      next_expectSecondItem();
      isDone_expectCorrectNumberOfIterations();
    }
};

const char PROGMEM testStringMapLiteStrings[] =
  "testval0" "\0"
  "testval1" "\0"
  "testval2" "\0";

const char PROGMEM testStringMapLiteEmpty [] = "\0";

class TestStringMapLite {
  public:
    static void count_Expect3 (void) {
      TEST_FUNC_START();
      //arrange
      //act
      int count = StringMapLite::count(testStringMapLiteStrings);
      //assert
      TEST_ASSERT (count == 3);
      TEST_FUNC_END();
    }
    static void count_EmptyStrings_Expect0 (void) {
      TEST_FUNC_START();
      //arrange
      //act
      int count = StringMapLite::count(testStringMapLiteEmpty);
      //assert
      TEST_ASSERT (!count);
      TEST_FUNC_END();
    }
    static void count_NULLStrings_Expect0 (void) {
      TEST_FUNC_START();
      //arrange
      //act
      int count = StringMapLite::count(NULL);
      //assert
      TEST_ASSERT (!count);
      TEST_FUNC_END();
    }
  public:
    static void testCount(void) {
      count_Expect3();
    }
  public:
    static void findChar_Found_ExpectValidKey(void) {
      TEST_FUNC_START();
      //arrange
      int findResult[3];
      //act
      findResult[0] = StringMapLite::find(testStringMapLiteStrings, "testval0");
      findResult[1] = StringMapLite::find(testStringMapLiteStrings, "testval1");
      findResult[2] = StringMapLite::find(testStringMapLiteStrings, "testval2");
      //assert
      TEST_ASSERT (findResult[0] == 0);
      TEST_ASSERT (findResult[1] == 1);
      TEST_ASSERT (findResult[2] == 2);
      TEST_FUNC_END();
    }
    static void findChar_NotFound_ExpectNotFound(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int notFound = StringMapLite::find(testStringMapLiteStrings, "testval3");
      //assert
      TEST_ASSERT (notFound == StringMapLite::NOT_FOUND);
      TEST_FUNC_END();
    }
    static void findChar_CaseSensitive_ExpectNotFound(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int notFoundBecauseFindIsCaseSensitive = StringMapLite::find(testStringMapLiteStrings, "Testval1");
      //assert
      TEST_ASSERT (notFoundBecauseFindIsCaseSensitive == StringMapLite::NOT_FOUND);
      TEST_FUNC_END();
    }
    static void findChar_EmptyStrings_ExpectNotFound(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int notFoundBecauseEmptyString = StringMapLite::find(testStringMapLiteStrings, "");
      int notFoundBecauseEmptyStringMap = StringMapLite::find(testStringMapLiteEmpty, "test1");
      //assert
      TEST_ASSERT (notFoundBecauseEmptyString == StringMapLite::NOT_FOUND);
      TEST_ASSERT (notFoundBecauseEmptyStringMap == StringMapLite::NOT_FOUND);
      TEST_FUNC_END();
    }
    static void findChar_NULLStrings_ExpectNotFound(void) {
      TEST_FUNC_START();
      //arrange
      //act
      int notFoundBecauseNULL = StringMapLite::find(testStringMapLiteStrings, (char *)NULL);
      int notFoundBecauseNULLStringMap = StringMapLite::find(NULL, "test1");
      //assert
      TEST_ASSERT (notFoundBecauseNULL == StringMapLite::NOT_FOUND);
      TEST_ASSERT (notFoundBecauseNULLStringMap == StringMapLite::NOT_FOUND);
      TEST_FUNC_END();
    }
  public:
    static void testFindChar(void) {
      findChar_Found_ExpectValidKey();
      findChar_NotFound_ExpectNotFound();
      findChar_CaseSensitive_ExpectNotFound();
      findChar_EmptyStrings_ExpectNotFound();
      findChar_NULLStrings_ExpectNotFound();
    }
  public:
    static void findInt_Found_ExpectCorrespondingString(void) {
      TEST_FUNC_START();
      //arrange
      const char * findResult[3];
      //act
      findResult[0] = StringMapLite::find(testStringMapLiteStrings, 0);
      findResult[1] = StringMapLite::find(testStringMapLiteStrings, 1);
      findResult[2] = StringMapLite::find(testStringMapLiteStrings, 2);
      //assert
      TEST_ASSERT (findResult[0]);
      TEST_ASSERT (findResult[1]);
      TEST_ASSERT (findResult[2]);
      TEST_ASSERT (!strcmp_P("testval0", findResult[0]));
      TEST_ASSERT (!strcmp_P("testval1", findResult[1]));
      TEST_ASSERT (!strcmp_P("testval2", findResult[2]));
      TEST_FUNC_END();
    }
    static void findInt_NotFound_ExpectNULL(void) {
      TEST_FUNC_START();
      //arrange
      //act
      const char * notFound = StringMapLite::find(testStringMapLiteStrings, 3);
      //assert
      TEST_ASSERT (!notFound);
      TEST_FUNC_END();
    }
    static void findInt_EmptyStrings_ExpectNULL(void) {
      TEST_FUNC_START();
      //arrange
      //act
      const char * notFoundBecauseEmptyStringMap = StringMapLite::find(testStringMapLiteEmpty, 0);
      //assert
      TEST_ASSERT (!notFoundBecauseEmptyStringMap);
      TEST_FUNC_END();
    }
    static void findInt_NULLStrings_ExpectNULL(void) {
      TEST_FUNC_START();
      //arrange
      //act
      const char * notFoundBecauseNULLStringMap = StringMapLite::find(NULL, 0);
      //assert
      TEST_ASSERT (!notFoundBecauseNULLStringMap);
      TEST_FUNC_END();
    }

  public:
    static void testFindInt(void) {
      findInt_Found_ExpectCorrespondingString();
      findInt_NotFound_ExpectNULL();
      findInt_EmptyStrings_ExpectNULL();
      findInt_NULLStrings_ExpectNULL();
    }
  public:
    static void runTests(void) {
      testCount();
      testFindChar();
      testFindInt();
    }
};

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestQuickStringMap::runTests();
  TestStringMapIteratorWithQuickStringMap::runTests();
  TestProgmemStringMap::runTests();
  TestStringMapIteratorWithProgmemStringMap::runTests();
  TestStringMapLite::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
