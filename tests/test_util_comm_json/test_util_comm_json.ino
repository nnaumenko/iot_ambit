#include <ESP8266WiFi.h>
#include "test.h"

#include "fakestream.h"
#include "util_comm.h"

#ifndef ESP8266
#warning "Please select a ESP8266 board in Tools/Board"
#endif

class TestJSONOutput {
  public:
    static void value_addToObject_expectCorrectValuesAdded(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut testOutput;
      util::json::JSONOutput testJSON(testOutput);
      static const char * PROGMEM testProgmemString = "PROGMEM";
      char referenceJSON[] = "{\"RAMString\":\"RAM\",\"PROGMEMString\":\"PROGMEM\",\"Long\":123,\"BooleanTrue\":true,\"BooleanFalse\":false,\"null_value\":null}";
      //act
      testJSON.value("RAMString", "RAM");
      testJSON.value("PROGMEMString", FPSTR(testProgmemString));
      testJSON.value("Long", 123L);
      testJSON.value("BooleanTrue", (boolean)true);
      testJSON.value("BooleanFalse", (boolean)false);
      testJSON.value("null_value");
      testJSON.~JSONOutput();
      //assert
      TEST_ASSERT(!testOutput.isOutBufferOverflow());
      TEST_ASSERT(!strcmp(testOutput.getOutBufferContent(), referenceJSON));
      TEST_FUNC_END();
    }
    static void value_addToArray_expectCorrectValuesAddedNamesIgnored(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut testOutput;
      util::json::JSONOutput testJSON(testOutput);
      static const char * PROGMEM testProgmemString = "PROGMEM";
      char referenceJSON[] = "{\"array\":[\"RAM\",\"PROGMEM\",123,true,false,null]}";
      //act
      testJSON.beginArray("array");
      testJSON.value("RAMString", "RAM");
      testJSON.value("PROGMEMString", FPSTR(testProgmemString));
      testJSON.value("Long", 123L);
      testJSON.value("BooleanTrue", (boolean)true);
      testJSON.value("BooleanFalse", (boolean)false);
      testJSON.value("null_value");
      testJSON.~JSONOutput();
      //assert
      TEST_ASSERT(!testOutput.isOutBufferOverflow());
      TEST_ASSERT(!strcmp(testOutput.getOutBufferContent(), referenceJSON));
      TEST_FUNC_END();
    }
    static void test_value(void) {
      value_addToObject_expectCorrectValuesAdded();
      value_addToArray_expectCorrectValuesAddedNamesIgnored();
    }
  public:
    static void structure_nesting_expectCorrectJSON(void) {
      TEST_FUNC_START();
      //arrange
      FakeStreamOut testOutput;
      util::json::JSONOutput testJSON(testOutput);
      char referenceJSON[] = "{\"v1\":1,\"a1\":[2,3,{\"v4\":4,\"v5\":5},{\"v6\":6,\"v7\":7},[8,9],[10,11],12]}";
      //act
      testJSON.value("v1", 1L);
      testJSON.beginArray("a1");
      testJSON.value("v2", 2L);
      testJSON.value("v3", 3L);
      testJSON.beginObject("o1");
      testJSON.value("v4", 4L);
      testJSON.value("v5", 5L);
      testJSON.finish();
      testJSON.beginObject("o2");
      testJSON.value("v6", 6L);
      testJSON.value("v7", 7L);
      testJSON.finish();
      testJSON.beginArray("a2");
      testJSON.value("v8", 8L);
      testJSON.value("v9", 9L);
      testJSON.finish();
      testJSON.beginArray("a3");
      testJSON.value("v10", 10L);
      testJSON.value("v11", 11L);
      testJSON.finish();
      testJSON.value("v12", 12L);
      testJSON.~JSONOutput();
      //assert
      TEST_ASSERT(!testOutput.isOutBufferOverflow());
      TEST_ASSERT(!strcmp(testOutput.getOutBufferContent(), referenceJSON));
      TEST_FUNC_END();
    }
    static void test_structure(void) {
      structure_nesting_expectCorrectJSON();
    }
  public:
    static void runTests(void) {
      test_value();
      test_structure();
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestFakeStream::runTests();
  TestJSONOutput::runTests();
  TEST_END();
}

void loop() {
  delay(100);
}
