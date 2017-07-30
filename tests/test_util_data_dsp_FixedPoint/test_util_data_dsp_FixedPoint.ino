#include <ESP8266WiFi.h>

#include "test.h"

#include "util_data.h"

//Comment this #define to perform secondary tests (unsigned values, values without range, etc.)
#define PRIMARY_TEST

template <typename T, size_t FractionBits, typename U, T TMaxRange, T TMinRange>
class FixedPointTester {
  public:
    static const T fixedPointMinRange = (TMinRange >> FractionBits) + 1;
    static const T fixedPointMaxRange = TMaxRange >> FractionBits;
  public:
    static void defaultConstructor_noParameters_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint;
      T testReferenceValue = {};
      //act
      boolean result = (static_cast<T>(testFixedPoint) == testReferenceValue);
      //assert
      TEST_ASSERT(result);
      TEST_FUNC_END();
    }
    static void constructor_initFromT_expectSameValue(void) {
      TEST_FUNC_START();
      //arrange
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange);
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange);
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOverflowMax(static_cast<T>(fixedPointMaxRange + static_cast<T>(1)));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOverflowMin(static_cast<T>(fixedPointMinRange - static_cast<T>(1)));
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointOne) == static_cast<T>(1));
      TEST_ASSERT(static_cast<T>(testFixedPointMax) == fixedPointMaxRange);
      TEST_ASSERT(static_cast<T>(testFixedPointMin) == fixedPointMinRange);
      if (TMaxRange && TMinRange) {
        TEST_ASSERT(static_cast<T>(testFixedPointOverflowMax) == TMaxRange);
        TEST_ASSERT(static_cast<T>(testFixedPointOverflowMin) == TMinRange);
      }
      else {
        TEST_PRINTLN(Range check is skipped because range not is set);
      }
      TEST_FUNC_END();
    }
    static void constructor_initFromIntegerAndFraction2bits_expectCorrectValue(void) {
      TEST_FUNC_START();
      if ((FractionBits >= 2) && ((sizeof(T) * 8 - FractionBits) >= 8)) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint2(static_cast<T>(2));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint3(static_cast<T>(3));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint4(static_cast<T>(4));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint5(static_cast<T>(5));
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1point5(static_cast<T>(15), 1);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1point25(static_cast<T>(125), 2);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult3 = testFixedPoint1point5 * testFixedPoint2;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult5 = testFixedPoint1point25 * testFixedPoint4;
        //assert
        TEST_ASSERT(testFixedPointResult3 == testFixedPoint3);
        TEST_ASSERT(testFixedPointResult5 == testFixedPoint5);
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 2 of more fraction bits and 8 or more integer bits);
      }
      TEST_FUNC_END();
    }
    static void constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue(void) {
      TEST_FUNC_START();
      if ((FractionBits >= 8) && ((sizeof(T) * 8 - FractionBits) >= 14)) {
        //arrange
        //if there are less than 8 fraction bits, test values are set to 1 (in this case this test will not be executed anyway) to avoid overflow warning
        static const T tenThousands = ((FractionBits >= 8) && ((sizeof(T) * 8 - FractionBits) >= 14)) ? static_cast<T>(10000) : static_cast<T>(1);
        static const T t4375 = ((FractionBits >= 8) && ((sizeof(T) * 8 - FractionBits) >= 14)) ? static_cast<T>(4375) : static_cast<T>(1);
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint4375(t4375);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint10000(tenThousands);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint0point4375(t4375, 4); //0,4375 = 112/256
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult4375 = testFixedPoint0point4375 * testFixedPoint10000;
        //assert
        TEST_ASSERT(testFixedPointResult4375 == testFixedPoint4375);
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 8 of more fraction bits and 14 or more integer bits);
      }
      TEST_FUNC_END();
    }
    static void copyConstructor_copyFromFixedPoint_expectSameValue(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointSrc(static_cast<T>(1));
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointDst(testFixedPointSrc);
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointDst) == static_cast<T>(testFixedPointDst));
      TEST_FUNC_END();
    }
    static void assignment_copyFromFixedPoint_expectSameValue(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1(TMinRange);
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint2;
      //act
      testFixedPoint2 = testFixedPoint1;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPoint1) == static_cast<T>(testFixedPoint2));
      TEST_FUNC_END();
    }
    static void conversionToBasicType_rounding_expectRoundingRulesFollowed(void) {
      TEST_FUNC_START();
      if (FractionBits >= 2) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(static_cast<T>(4));
        T minusOne = (TMinRange < 0) ? static_cast<T>(-1) : static_cast<T>(0);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusOne(minusOne);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusThree(minusOne + minusOne + minusOne);
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf = testFixedPointOne / testFixedPointTwo;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThreeQuarters = testFixedPointThree / testFixedPointFour;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusHalf = testFixedPointMinusOne / testFixedPointTwo;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusQuarter = testFixedPointMinusOne / testFixedPointFour;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusThreeQuarters = testFixedPointMinusThree / testFixedPointFour;
        //assert
        TEST_ASSERT(static_cast<T>(testFixedPointQuarter) == static_cast<T>(0));
        TEST_ASSERT(static_cast<T>(testFixedPointHalf) == static_cast<T>(1));
        TEST_ASSERT(static_cast<T>(testFixedPointThreeQuarters) == static_cast<T>(1));
        if (TMinRange < 0) {
          TEST_ASSERT(static_cast<T>(testFixedPointMinusQuarter) == static_cast<T>(0));
          TEST_ASSERT(static_cast<T>(testFixedPointMinusHalf) == minusOne);
          TEST_ASSERT(static_cast<T>(testFixedPointMinusThreeQuarters) == minusOne);
        }
        else {
          TEST_PRINTLN(Negative value testing skipped because T is unsigned type or range not set);
        }
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 2 of more fraction bits);
      }
      TEST_FUNC_END();
    }
    static void constructorFromString_provideValidNumberStrings_expectCorrectValues(void) {
      TEST_FUNC_START();
      if ((FractionBits >= 4) && ((sizeof(T) * 8 - FractionBits) >= 20) && (TMinRange < static_cast<T>(0))) {
        //arrange
        //if conditions for executing this function (see above) are not met, init all numbers with zeros (this code will not be executed anyway)
        const char testNumberStr1[] = "+124.75";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> referenceNumber1((sizeof(T) * 8 - FractionBits) >= 20 ? static_cast<T>(12475) : static_cast<T>(0), 2);
        const char testNumberStr2[] = "060.125";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> referenceNumber2((sizeof(T) * 8 - FractionBits) >= 20 ? static_cast<T>(60125) : static_cast<T>(0), 3);
        const char testNumberStr3[] = "83.0625";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> referenceNumber3((sizeof(T) * 8 - FractionBits) >= 20 ? static_cast<T>(830625) : static_cast<T>(0), 4);
        const char testNumberStr4[] = "-97.25";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> referenceNumber4((sizeof(T) * 8 - FractionBits) >= 20 ? static_cast<T>(-9725) : static_cast<T>(0), 2);
        const char testNumberStr5[] = "-.75";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> referenceNumber5((sizeof(T) * 8 - FractionBits) >= 20 && (TMinRange < static_cast<T>(0)) ?
            static_cast<T>(-75) : 0, 2);
        const char testNumberStr6[] = "3.";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> referenceNumber6((sizeof(T) * 8 - FractionBits) >= 20 ? static_cast<T>(3) : static_cast<T>(0), 0);
        const char testNumberStr7[] = "221";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> referenceNumber7((sizeof(T) * 8 - FractionBits) >= 20 ? static_cast<T>(221) : static_cast<T>(0), 0);
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber1(testNumberStr1);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber2(testNumberStr2);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber3(testNumberStr3);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber4(testNumberStr4);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber5(testNumberStr5);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber6(testNumberStr6);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber7(testNumberStr7);
        //assert
        TEST_ASSERT(testNumber1 == referenceNumber1);
        TEST_ASSERT(testNumber2 == referenceNumber2);
        TEST_ASSERT(testNumber3 == referenceNumber3);
        TEST_ASSERT(testNumber4 == referenceNumber4);
        TEST_ASSERT(testNumber5 == referenceNumber5);
        TEST_ASSERT(testNumber6 == referenceNumber6);
        TEST_ASSERT(testNumber7 == referenceNumber7);
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers);
      }
      TEST_FUNC_END();
    }
    static void constructorFromString_provideInvalidNumberStrings_expectZero(void) {
      TEST_FUNC_START();
      if ((FractionBits >= 4) && ((sizeof(T) * 8 - FractionBits) >= 20) && (TMinRange < static_cast<T>(0))) {
        //arrange
        //if conditions for executing this function (see above) are not met, init all numbers with zeros (this code will not be executed anyway)
        const char testNumberStr1[] = "";       //empty string
        const char testNumberStr2[] = "3.4.5";  //more than one decimal point
        const char testNumberStr3[] = "0x0625"; //unacceptable character in integer number
        const char testNumberStr4[] = "-97h.25";//unacceptable character in integer part
        const char testNumberStr5[] = "+97.25h";//unacceptable character in fraction part
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber1(testNumberStr1);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber2(testNumberStr2);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber3(testNumberStr3);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber4(testNumberStr4);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testNumber5(testNumberStr5);
        //assert
        TEST_ASSERT(testNumber1 == static_cast<T>(0));
        TEST_ASSERT(testNumber2 == static_cast<T>(0));
        TEST_ASSERT(testNumber3 == static_cast<T>(0));
        TEST_ASSERT(testNumber4 == static_cast<T>(0));
        TEST_ASSERT(testNumber4 == static_cast<T>(0));
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 4 of more fraction bits and 20 or more integer bits; TMinRange must be below zero to accept negative numbers);
      }
      TEST_FUNC_END();
    }
    static void constructorFromString_null_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      char * testPtr = nullptr;
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue(testPtr);
      //assert
      TEST_ASSERT(testValue == static_cast<T>(0));
      TEST_FUNC_END();
    }
    static void getValue_fixedPointValues_expectCorrectConvertedValues(void) {
      TEST_FUNC_START();
      if ((FractionBits >= 2) && ((sizeof(T) * 8 - FractionBits) >= 9)) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue1(75, 1);//7.5
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue2(50, 1);//5.0
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue3(50, 2);//0.5
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue4(25, 2);//0.25
        boolean status1 = false;
        boolean status2 = false;
        boolean status3 = false;
        boolean status4 = false;
        boolean status5 = false;
        boolean status6 = false;
        //act
        T result1 = testValue1.getValue(0, &status1); //8
        T result2 = testValue1.getValue(1, &status2); //75
        T result3 = testValue2.getValue(1, &status3); //50
        T result4 = testValue3.getValue(2, &status4); //50
        T result5 = testValue3.getValue(1, &status5); //5
        T result6 = testValue4.getValue(3, &status6); //250
        //assert
        TEST_ASSERT(status1);
        TEST_ASSERT(status2);
        TEST_ASSERT(status3);
        TEST_ASSERT(status4);
        TEST_ASSERT(status5);
        TEST_ASSERT(status6);
        TEST_ASSERT(result1 = static_cast<T>(8));
        TEST_ASSERT(result2 = static_cast<T>(75));
        TEST_ASSERT(result3 = static_cast<T>(50));
        TEST_ASSERT(result4 = static_cast<T>(50));
        TEST_ASSERT(result5 = static_cast<T>(5));
        TEST_ASSERT(result6 = static_cast<T>(250));
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 2 of more fraction bits and 9 or more integer bits);
      }
      TEST_FUNC_END();
    }
    static void testAssignmentAndConversion(void) {
      defaultConstructor_noParameters_expectZero();
      constructor_initFromT_expectSameValue();
      constructor_initFromIntegerAndFraction2bits_expectCorrectValue();
      constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue();
      copyConstructor_copyFromFixedPoint_expectSameValue();
      assignment_copyFromFixedPoint_expectSameValue();
      conversionToBasicType_rounding_expectRoundingRulesFollowed();
      constructorFromString_provideValidNumberStrings_expectCorrectValues();
      constructorFromString_provideInvalidNumberStrings_expectZero();
      constructorFromString_null_expectZero();
      getValue_fixedPointValues_expectCorrectConvertedValues();
    }
  public:
    static void addition_addTwoNumbers_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointOne + testFixedPointThree;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointOne) == static_cast<T>(1));
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == static_cast<T>(3));
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == static_cast<T>(4));
      TEST_FUNC_END();
    }
    static void subtraction_subtractOneNumberFromAnother_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointThree - testFixedPointOne;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointOne) == static_cast<T>(1));
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == static_cast<T>(3));
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == static_cast<T>(2));
      TEST_FUNC_END();
    }
    static void multiplication_multiplyTwoNumbers_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointThree * testFixedPointTwo;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointTwo) == static_cast<T>(2));
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == static_cast<T>(3));
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == static_cast<T>(6));
      TEST_FUNC_END();
    }
    static void division_divideOneNumberByAnother_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointSix(static_cast<T>(6));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointSix / testFixedPointThree;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointSix) == static_cast<T>(6));
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == static_cast<T>(3));
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == static_cast<T>(2));
      TEST_FUNC_END();
    }
    static void fractions_divideMultiply_expectNoFractionsLoss(void) {
      TEST_FUNC_START();
      //arrange
      T twentyFive = static_cast<T>(25);
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(static_cast<T>(4));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTen(static_cast<T>(10));
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult1 = testFixedPointQuarter * testFixedPointTen;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult2 = testFixedPointResult1 * testFixedPointTen;
      //assert
      TEST_ASSERT(static_cast<T>(FractionBits) >= 2);
      TEST_ASSERT(static_cast<T>(testFixedPointResult2) == twentyFive);
      TEST_FUNC_END();
    }
    static void fractions_divideAddMultiply_expectNoFractionsLoss(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(static_cast<T>(4));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTen(static_cast<T>(10));
      //act
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = (testFixedPointQuarter + testFixedPointQuarter) * testFixedPointTen;
      //assert
      TEST_ASSERT(static_cast<T>(FractionBits) >= 2);
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == static_cast<T>(5));
      TEST_FUNC_END();
    }
    static void multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange);
        //act
        testFixedPointMax *= testFixedPointOne;
        testFixedPointMin *= testFixedPointOne;
        //assert
        TEST_ASSERT(static_cast<T>(testFixedPointMax) == fixedPointMaxRange);
        TEST_ASSERT(static_cast<T>(testFixedPointMin) == fixedPointMinRange);
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void testArithmeticalOperators(void) {
      addition_addTwoNumbers_expectCorrectResult();
      subtraction_subtractOneNumberFromAnother_expectCorrectResult();
      multiplication_multiplyTwoNumbers_expectCorrectResult();
      division_divideOneNumberByAnother_expectCorrectResult();
      fractions_divideMultiply_expectNoFractionsLoss();
      fractions_divideAddMultiply_expectNoFractionsLoss();
      multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits();
    }
  public:
    static void equalAndNotEqual_noFractions_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo1(static_cast<T>(2));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo2(static_cast<T>(2));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
      //act
      boolean equal = (testFixedPointTwo1 == testFixedPointTwo2);
      boolean notEqual = !(testFixedPointTwo1 == testFixedPointThree);
      //assert
      TEST_ASSERT(static_cast<T>(FractionBits) >= 2);
      TEST_ASSERT(equal);
      TEST_ASSERT(notEqual);
      TEST_FUNC_END();
    }
    static void greaterLesserOrEqual_noFractions_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
      //act
      boolean lesser = (testFixedPointTwo < testFixedPointThree);
      boolean notLesser = !(testFixedPointThree < testFixedPointTwo);
      boolean greater = (testFixedPointThree > testFixedPointTwo);
      boolean notGreater = !(testFixedPointTwo > testFixedPointThree);
      boolean lesserOrEqualLesser = (testFixedPointTwo <= testFixedPointThree);
      boolean lesserOrEqualEqual = (testFixedPointTwo <= testFixedPointTwo);
      boolean lesserOrEqualGreater = !(testFixedPointThree <= testFixedPointTwo);
      boolean greaterOrEqualLesser = !(testFixedPointTwo >= testFixedPointThree);
      boolean greaterOrEqualEqual = (testFixedPointTwo >= testFixedPointTwo);
      boolean greaterOrEqualGreater = (testFixedPointThree >= testFixedPointTwo);
      //assert
      TEST_ASSERT(static_cast<T>(FractionBits) >= 2);
      TEST_ASSERT(lesser);
      TEST_ASSERT(notLesser);
      TEST_ASSERT(greater);
      TEST_ASSERT(notGreater);
      TEST_ASSERT(lesserOrEqualLesser);
      TEST_ASSERT(lesserOrEqualEqual);
      TEST_ASSERT(lesserOrEqualGreater);
      TEST_ASSERT(greaterOrEqualLesser);
      TEST_ASSERT(greaterOrEqualEqual);
      TEST_ASSERT(greaterOrEqualGreater);
      TEST_FUNC_END();
    }
    static void equalAndNotEqual_fractions_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(static_cast<T>(4));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf1 = testFixedPointOne / testFixedPointTwo;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf2 = testFixedPointOne / testFixedPointTwo;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
      //act
      boolean equal = (testFixedPointHalf1 == testFixedPointHalf2);
      boolean notEqual = !(testFixedPointHalf1 == testFixedPointQuarter);
      //assert
      TEST_ASSERT(equal);
      TEST_ASSERT(notEqual);
      TEST_FUNC_END();
    }
    static void greaterLesserOrEqual_fractions_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(static_cast<T>(4));
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf = testFixedPointOne / testFixedPointTwo;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
      //act
      boolean lesser = (testFixedPointQuarter < testFixedPointHalf);
      boolean notLesser = !(testFixedPointHalf < testFixedPointQuarter);
      boolean greater = (testFixedPointHalf > testFixedPointQuarter);
      boolean notGreater = !(testFixedPointQuarter > testFixedPointHalf);
      boolean lesserOrEqualLesser = (testFixedPointQuarter <= testFixedPointHalf);
      boolean lesserOrEqualEqual = (testFixedPointQuarter <= testFixedPointQuarter);
      boolean lesserOrEqualGreater = !(testFixedPointHalf <= testFixedPointQuarter);
      boolean greaterOrEqualLesser = !(testFixedPointQuarter >= testFixedPointHalf);
      boolean greaterOrEqualEqual = (testFixedPointQuarter >= testFixedPointQuarter);
      boolean greaterOrEqualGreater = (testFixedPointHalf >= testFixedPointQuarter);
      //assert
      TEST_ASSERT(lesser);
      TEST_ASSERT(notLesser);
      TEST_ASSERT(greater);
      TEST_ASSERT(notGreater);
      TEST_ASSERT(lesserOrEqualLesser);
      TEST_ASSERT(lesserOrEqualEqual);
      TEST_ASSERT(lesserOrEqualGreater);
      TEST_ASSERT(greaterOrEqualLesser);
      TEST_ASSERT(greaterOrEqualEqual);
      TEST_ASSERT(greaterOrEqualGreater);
      TEST_FUNC_END();
    }
    static void testLogicalOperators(void) {
      equalAndNotEqual_noFractions_expectCorrectResult();
      greaterLesserOrEqual_noFractions_expectCorrectResult();
      equalAndNotEqual_fractions_expectCorrectResult();
      greaterLesserOrEqual_fractions_expectCorrectResult();
    }
  public:
    static void arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(static_cast<T>(1));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult1 = testFixedPointMin - testFixedPointOne;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult2 = testFixedPointMax + testFixedPointOne;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMultiplicationResult1 = testFixedPointMin * testFixedPointTwo;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMultiplicationResult2 = testFixedPointMax * testFixedPointTwo;
        //assert
        TEST_ASSERT(static_cast<T>(testFixedPointAdditionResult1) == TMinRange);
        TEST_ASSERT(static_cast<T>(testFixedPointAdditionResult2) == TMaxRange);
        TEST_ASSERT(static_cast<T>(testFixedPointMultiplicationResult1) == TMinRange);
        TEST_ASSERT(static_cast<T>(testFixedPointMultiplicationResult2) == TMaxRange);
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void arithmeticOperators_overflowValues_expectValuesNotModified(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange - static_cast<T>(2));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange + static_cast<T>(1));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult1 = testFixedPointMin + testFixedPointTwo;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult2 = testFixedPointMax - testFixedPointTwo;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointDivisionResult1 = testFixedPointMin / testFixedPointTwo;
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointDivisionResult2 = testFixedPointMax / testFixedPointTwo;
        //assert
        TEST_ASSERT(static_cast<T>(testFixedPointAdditionResult1) == TMinRange);
        TEST_ASSERT(static_cast<T>(testFixedPointAdditionResult2) == TMaxRange);
        TEST_ASSERT(static_cast<T>(testFixedPointDivisionResult1) == TMinRange);
        TEST_ASSERT(static_cast<T>(testFixedPointDivisionResult2) == TMaxRange);
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void assignment_assignToOverflownValue_exepectValueAssigned(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(static_cast<T>(fixedPointMinRange - static_cast<T>(1)));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(static_cast<T>(fixedPointMaxRange + static_cast<T>(1)));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(static_cast<T>(3));
        //act
        T negativeOverflowResult = static_cast<T>(testFixedPointMin);
        T positiveOverflowResult = static_cast<T>(testFixedPointMax);
        testFixedPointMin = testFixedPointTwo;
        testFixedPointMax = testFixedPointThree;
        //assert
        TEST_ASSERT(negativeOverflowResult == TMinRange);
        TEST_ASSERT(positiveOverflowResult == TMaxRange);
        TEST_ASSERT(static_cast<T>(testFixedPointMin) == static_cast<T>(2));
        TEST_ASSERT(static_cast<T>(testFixedPointMax) == static_cast<T>(3));
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    };
    static void overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange - static_cast<T>(2));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange + static_cast<T>(1));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(static_cast<T>(2));
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointZero(static_cast<T>(0));
        //act
        boolean resultMin = testFixedPointMin.overflow();
        boolean resultMax = testFixedPointMax.overflow();
        boolean resultTwo = testFixedPointTwo.overflow();
        boolean resultZero = testFixedPointZero.overflow();
        //assert
        TEST_ASSERT(resultMin);
        TEST_ASSERT(resultMax);
        TEST_ASSERT(!resultTwo);
        TEST_ASSERT(!resultZero);
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void constructorFromString_tooLongInteger_expectOverflow(void) {//TODO
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        char testStr1[] = "99999999999999999999";
        char testStr2[] = "-99999999999999999999";
        char testStr3[] = "99999999999999999999.9";
        char testStr4[] = "-99999999999999999999.9";
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue1(testStr1);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue2(testStr2);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue3(testStr3);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue4(testStr4);
        //assert
        TEST_ASSERT((testValue1 == util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(TMaxRange)));
        TEST_ASSERT((testValue2 == util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(TMinRange)));
        TEST_ASSERT((testValue3 == util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(TMaxRange)));
        TEST_ASSERT((testValue4 == util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(TMinRange)));
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void constructorFromString_tooLongFraction_expectNoOverflow(void) {//TODO
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        char testStr1[] = "0.99999999999999999999";
        char testStr2[] = "-0.99999999999999999999";
        char testStr3[] = "1.99999999999999999999";
        char testStr4[] = "-1.99999999999999999999";
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue1RangeMin(static_cast<T>(9),1); //0.9
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue1RangeMax(static_cast<T>(1),0); //1.0
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue2RangeMin(static_cast<T>(-1),0); //-1.0
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue2RangeMax(static_cast<T>(-9),1); //-0.9
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue3RangeMin(static_cast<T>(19),1); //1.9
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue3RangeMax(static_cast<T>(2),0); //2.0
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue4RangeMin(static_cast<T>(-2),0); //-2.0
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> refValue4RangeMax(static_cast<T>(-19),1); //-1.9
        //act
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue1(testStr1);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue2(testStr2);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue3(testStr3);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testValue4(testStr4);
        //assert
        TEST_ASSERT(testValue1 >= refValue1RangeMin);
        TEST_ASSERT(testValue1 <= refValue1RangeMax);
        TEST_ASSERT(testValue2 >= refValue2RangeMin);
        TEST_ASSERT(testValue2 <= refValue2RangeMax);
        TEST_ASSERT(testValue3 >= refValue1RangeMin);
        TEST_ASSERT(testValue3 <= refValue3RangeMax);
        TEST_ASSERT(testValue4 >= refValue4RangeMin);
        TEST_ASSERT(testValue4 <= refValue4RangeMax);
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();    }
    static void getValue_edgesOfAcceptableRange_expectNoOverflow(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        T lowRange = fixedPointMinRange + static_cast<T>(1);
        T highRange = fixedPointMaxRange - static_cast<T>(1);
        T lowRangeDividedByTen = fixedPointMinRange / static_cast<T>(10) + static_cast<T>(1);
        T highRangeDividedByTen = fixedPointMaxRange / static_cast<T>(10) - static_cast<T>(1);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointLowRange(lowRange);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHighRange(highRange);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointLowRangeDividedByTen(lowRangeDividedByTen);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHighRangeDividedByTen(highRangeDividedByTen);
        boolean statusLowRangeZeroDecimals = false;
        boolean statusHighRangeZeroDecimals = false;
        boolean statusLowRangeOneDecimal = false;
        boolean statusHighRangeOneDecimal = false;
        //act
        T resultLowRange = testFixedPointLowRange.getValue(0, &statusLowRangeZeroDecimals);
        T resultHighRange = testFixedPointHighRange.getValue(0, &statusHighRangeZeroDecimals);
        T resultLowRangeDividedByTen = testFixedPointLowRangeDividedByTen.getValue(1, &statusLowRangeOneDecimal);
        T resultHighRangeDividedByTen = testFixedPointHighRangeDividedByTen.getValue(1, &statusHighRangeOneDecimal);
        //assert
        TEST_ASSERT(statusLowRangeZeroDecimals);
        TEST_ASSERT(statusHighRangeZeroDecimals);
        TEST_ASSERT(statusLowRangeOneDecimal);
        TEST_ASSERT(statusHighRangeOneDecimal);
        TEST_ASSERT(resultLowRange = lowRange);
        TEST_ASSERT(resultHighRange = highRange);
        TEST_ASSERT(resultLowRangeDividedByTen = lowRangeDividedByTen * static_cast<T>(10));
        TEST_ASSERT(resultHighRangeDividedByTen = lowRangeDividedByTen * static_cast<T>(10));
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void getValue_tooManyDecimals_expectOverflow(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinRange(fixedPointMinRange);
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMaxRange(fixedPointMaxRange);
        boolean statusMinRange = false;
        boolean statusMaxRange = false;
        //act
        T resultMinRange = testFixedPointMinRange.getValue(8, &statusMinRange);
        T resultMaxRange = testFixedPointMaxRange.getValue(8, &statusMaxRange);
        //assert
        TEST_ASSERT(!testFixedPointMinRange.overflow());
        TEST_ASSERT(!testFixedPointMaxRange.overflow());
        TEST_ASSERT(!statusMinRange);
        TEST_ASSERT(!statusMaxRange);
        TEST_ASSERT(resultMinRange == TMinRange);
        TEST_ASSERT(resultMaxRange == TMaxRange);
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void rangeCheckAndOverflow(void) {
      arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit();
      arithmeticOperators_overflowValues_expectValuesNotModified();
      assignment_assignToOverflownValue_exepectValueAssigned();
      overflow_OverflownAndNonoverflownValues_expectTrueForOverflownValuesOnly();
      constructorFromString_tooLongInteger_expectOverflow();
      constructorFromString_tooLongFraction_expectNoOverflow();
      getValue_edgesOfAcceptableRange_expectNoOverflow();
      getValue_tooManyDecimals_expectOverflow();
    }
  public:
#pragma GCC push_options
#pragma GCC optimize ("O0")
    static const uint32_t performanceTestNumberOfOperations = 1000000;
    static void performanceTestAddition(void) {
      PERF_TEST_FUNC_START();
      static const uint32_t numberOfOperations = performanceTestNumberOfOperations;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointBase(fixedPointMinRange);
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointIncrement =
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(static_cast<T>(1)) /
        util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(static_cast<T>(2));
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++)
        testFixedPointBase += testFixedPointIncrement;
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void performanceTestMultiplication(void) {
      PERF_TEST_FUNC_START();
      static const uint32_t numberOfOperations = performanceTestNumberOfOperations;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointBase(fixedPointMinRange);
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMultiplier(static_cast<T>(1));
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++)
        testFixedPointBase *= testFixedPointMultiplier;
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void performanceTestAssignment(void) {
      PERF_TEST_FUNC_START();
      static const uint32_t numberOfOperations = performanceTestNumberOfOperations;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointToAssign;
      util::dsp::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointToAssignFrom(static_cast<T>(1));
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++)
        testFixedPointToAssign = testFixedPointToAssignFrom;
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
#pragma GCC pop_options
    static void testPerformance(void) {
      performanceTestAddition();
      performanceTestMultiplication();
      performanceTestAssignment();
    }
  public:
    static void runTests(void) {
      testAssignmentAndConversion();
      testArithmeticalOperators();
      testLogicalOperators();
      rangeCheckAndOverflow();
      testPerformance();
    }
};

class TestFixedPoint {
  public:
    static void runTests(void) {
      #ifdef PRIMARY_TEST
      FixedPointTester<int32_t, 16, int64_t, INT32_MAX, INT32_MIN>::runTests();
      FixedPointTester<int32_t, 8, float, INT32_MAX, INT32_MIN>::runTests();
      FixedPointTester<int16_t, 4, int32_t, INT16_MAX, INT16_MIN>::runTests();
      FixedPointTester<int8_t, 2, int16_t, INT8_MAX, INT8_MIN>::runTests();
      #else
      FixedPointTester<uint32_t, 8, uint64_t, UINT32_MAX, 0>::runTests();
      FixedPointTester<uint16_t, 8, uint32_t, UINT16_MAX, 0>::runTests();
      FixedPointTester<uint32_t, 8, uint64_t, 0, 0>::runTests();
      FixedPointTester<uint16_t, 8, uint32_t, 0, 0>::runTests();
      #endif
    }
};

TEST_GLOBALS();

void setup() {
  TEST_SETUP();
  TEST_BEGIN();
  TestFixedPoint::runTests();

  TEST_END();
}

void loop() {
  delay(100);
}
