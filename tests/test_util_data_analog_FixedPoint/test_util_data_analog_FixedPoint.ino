#include <ESP8266WiFi.h>

#include "test.h"

#include "util_data.h"

template <typename T, size_t FractionBits, typename U, T TMaxRange, T TMinRange>
class FixedPointTester {
  public:
    static const T fixedPointMinRange = (TMinRange >> FractionBits) + 1;
    static const T fixedPointMaxRange = TMaxRange >> FractionBits;
    static const T zero = static_cast<T>(0);
    static const T one = static_cast<T>(1);
    static const T two = static_cast<T>(2);
    static const T three = static_cast<T>(3);
    static const T four = static_cast<T>(4);
    static const T five = static_cast<T>(5);
    static const T six = static_cast<T>(6);
    static const T ten = static_cast<T>(10);
  public:
    static void defaultConstructor_noParameters_expectZero(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint;
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
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOverflowMax(fixedPointMaxRange + one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOverflowMin(fixedPointMinRange - one);
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointOne) == one);
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
    static void constructor_initFromIntegerAndFraction2bits_expectCorrectValue(void) { //TODO: implement test
      TEST_FUNC_START();
      if (FractionBits >= 2) {
        //arrange
        static const T twentyFive = five + ten + ten;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint2(two);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint3(three);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint4(four);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint5(five);
        //act
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1point5(one, five);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1point25(one, twentyFive);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult3 = testFixedPoint1point5 * testFixedPoint2;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult5 = testFixedPoint1point25 * testFixedPoint4;
        //assert
        TEST_ASSERT(testFixedPointResult3 == testFixedPoint3);
        TEST_ASSERT(testFixedPointResult5 == testFixedPoint5);
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 2 of more fraction bits);
      }
      TEST_FUNC_END();
    }
    static void constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue(void) {
      TEST_FUNC_START();
      if ((FractionBits >= 8)&&((sizeof(T)*8 - FractionBits)>=13)) {
        //arrange
        //if there are less than 8 fraction bits, test values are set to 1 (in this case this test will not be executed anyway) to avoid overflow warning
        static const T hundred = (FractionBits >= 8) ? ten * ten : one;
        static const T thousand = (FractionBits >= 8) ? ten * ten * ten : one;
        static const T tenThousands = (FractionBits >= 8) ? ten * ten * ten * ten : one;
        static const T t4375 = (FractionBits >= 8) ? thousand * four + hundred * three + ten * (five + two) + five : one;
        //act
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint4375(t4375);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint10000(tenThousands);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint0point4375(zero, t4375); //0,4375 = 112/256
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult4375 = testFixedPoint0point4375 * testFixedPoint10000;
        //assert
        TEST_ASSERT(testFixedPointResult4375 == testFixedPoint4375);
      }
      else {
        TEST_PRINTLN(Testing function skipped because it was designed for 8 of more fraction bits and 13 or more integer bits);
      }
      TEST_FUNC_END();
    }
    static void constructor_initFromIntegerAndNegativeFraction_expectFractionSignDiscarded(void) {
      TEST_FUNC_START();
      if (TMinRange < 0) {
        //arrange
        //in case TMinRange is not set or non-negative (unsigned type used) value is set to 1 instead of -1 to avoid warning (in this case this code will not be executed anyway)
        T minusOne = (TMinRange < 0) ? zero - one : one;
        T minusFive = minusOne + minusOne + minusOne + minusOne + minusOne;
        //act
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1point5(one, five);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1pointMinus5(one, minusFive);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinus1point5(minusOne, five);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinus1pointMinus5(minusOne, minusFive);
        //assert
        TEST_ASSERT(testFixedPoint1point5 == testFixedPoint1pointMinus5);
        TEST_ASSERT(testFixedPointMinus1point5 == testFixedPointMinus1pointMinus5);
      }
      else {
        TEST_PRINTLN(Negative value testing skipped because T is unsigned type or range not set);
      }
      TEST_FUNC_END();
    }
    static void constructor_initFromIntegerAndTooLongFraction_expectFractionTruncated(void) {
      TEST_FUNC_START();
      if (TMaxRange) {
        //arrange
        T largeFractionValue = five;
        while (largeFractionValue < fixedPointMaxRange){
          largeFractionValue *= ten;
        }
        largeFractionValue++;
          //act
          util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1point5(one, five);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint(one, largeFractionValue);
        //assert
        TEST_ASSERT(testFixedPoint1point5 == testFixedPoint);
      }
      else {
        TEST_PRINTLN(Test function skipped because range not set);
      }
      TEST_FUNC_END();
    }
    static void copyConstructor_copyFromFixedPoint_expectSameValue(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointSrc(one);
      //act
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointDst(testFixedPointSrc);
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointDst) == static_cast<T>(testFixedPointDst));
      TEST_FUNC_END();
    }
    static void assignment_copyFromFixedPoint_expectSameValue(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint1(TMinRange);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPoint2;
      //act
      testFixedPoint2 = testFixedPoint1;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPoint1) == static_cast<T>(testFixedPoint2));
      TEST_FUNC_END();
    }
    static void testAssignmentAndConversion(void) {
      defaultConstructor_noParameters_expectZero();
      constructor_initFromT_expectSameValue();
      constructor_initFromIntegerAndFraction2bits_expectCorrectValue();
      constructor_initFromIntegerAndFraction8x13bits_expectCorrectValue();
      copyConstructor_copyFromFixedPoint_expectSameValue();
      assignment_copyFromFixedPoint_expectSameValue();
    }
  public:
    static void addition_addTwoNumbers_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(three);
      //act
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointOne + testFixedPointThree;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointOne) == one);
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == three);
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == four);
      TEST_FUNC_END();
    }
    static void subtraction_subtractOneNumberFromAnother_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(three);
      //act
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointThree - testFixedPointOne;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointOne) == one);
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == three);
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == two);
      TEST_FUNC_END();
    }
    static void multiplication_multiplyTwoNumbers_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(two);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(three);
      //act
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointThree * testFixedPointTwo;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointTwo) == two);
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == three);
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == six);
      TEST_FUNC_END();
    }
    static void division_divideOneNumberByAnother_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointSix(six);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(three);
      //act
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = testFixedPointSix / testFixedPointThree;
      //assert
      TEST_ASSERT(static_cast<T>(testFixedPointSix) == six);
      TEST_ASSERT(static_cast<T>(testFixedPointThree) == three);
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == two);
      TEST_FUNC_END();
    }
    static void fractions_divideMultiply_expectNoFractionsLoss(void) {
      TEST_FUNC_START();
      //arrange
      T twentyFive = two * ten + five;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(four);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTen(ten);
      //act
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult1 = testFixedPointQuarter * testFixedPointTen;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult2 = testFixedPointResult1 * testFixedPointTen;
      //assert
      TEST_ASSERT(static_cast<T>(FractionBits) >= 2);
      TEST_ASSERT(static_cast<T>(testFixedPointResult2) == twentyFive);
      TEST_FUNC_END();
    }
    static void fractions_divideAddMultiply_expectNoFractionsLoss(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(four);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTen(ten);
      //act
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointResult = (testFixedPointQuarter + testFixedPointQuarter) * testFixedPointTen;
      //assert
      TEST_ASSERT(static_cast<T>(FractionBits) >= 2);
      TEST_ASSERT(static_cast<T>(testFixedPointResult) == five);
      TEST_FUNC_END();
    }
    static void multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits(void) {
      TEST_FUNC_START();
      if (TMinRange && TMaxRange) {
        //arrange
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange);
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
    static void conversionToBasicType_rounding_expectRoundingRulesFollowed(void) {
      TEST_FUNC_START();
      if (FractionBits >= 2) {
        //arrange
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(two);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(three);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(four);
        T minusOne = (TMinRange < 0) ? zero - one : one;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusOne(minusOne);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusThree(minusOne + minusOne + minusOne);
        //act
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf = testFixedPointOne / testFixedPointTwo;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThreeQuarters = testFixedPointThree / testFixedPointFour;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusHalf = testFixedPointMinusOne / testFixedPointTwo;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusQuarter = testFixedPointMinusOne / testFixedPointFour;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMinusThreeQuarters = testFixedPointMinusThree / testFixedPointFour;
        //assert
        TEST_ASSERT(static_cast<T>(testFixedPointQuarter) == zero);
        TEST_ASSERT(static_cast<T>(testFixedPointHalf) == one);
        TEST_ASSERT(static_cast<T>(testFixedPointThreeQuarters) == one);
        if (TMinRange < 0) {
          TEST_ASSERT(static_cast<T>(testFixedPointMinusQuarter) == zero);
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
    static void testArithmeticalOperators(void) {
      addition_addTwoNumbers_expectCorrectResult();
      subtraction_subtractOneNumberFromAnother_expectCorrectResult();
      multiplication_multiplyTwoNumbers_expectCorrectResult();
      division_divideOneNumberByAnother_expectCorrectResult();
      fractions_divideMultiply_expectNoFractionsLoss();
      fractions_divideAddMultiply_expectNoFractionsLoss();
      multiplicationOverflow_multiplyRangeLimitByOne_expectRangeLimits();
      conversionToBasicType_rounding_expectRoundingRulesFollowed();
    }
  public:
    static void equalAndNotEqual_noFractions_expectCorrectResult(void) {
      TEST_FUNC_START();
      //arrange
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo1(two);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo2(two);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(three);
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
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(two);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointThree(three);
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
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(two);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(four);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf1 = testFixedPointOne / testFixedPointTwo;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf2 = testFixedPointOne / testFixedPointTwo;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
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
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(two);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointFour(four);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointHalf = testFixedPointOne / testFixedPointTwo;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointQuarter = testFixedPointOne / testFixedPointFour;
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
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointOne(one);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(two);
        //act
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult1 = testFixedPointMin - testFixedPointOne;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult2 = testFixedPointMax + testFixedPointOne;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMultiplicationResult1 = testFixedPointMin * testFixedPointTwo;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMultiplicationResult2 = testFixedPointMax * testFixedPointTwo;
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
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMin(fixedPointMinRange - one - one);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMax(fixedPointMaxRange + one);
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointTwo(two);
        //act
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult1 = testFixedPointMin + testFixedPointTwo;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointAdditionResult2 = testFixedPointMax - testFixedPointTwo;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointDivisionResult1 = testFixedPointMin / testFixedPointTwo;
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointDivisionResult2 = testFixedPointMax / testFixedPointTwo;
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
    static void rangeCheckAndOverflow(void) {
      arithmeticOperators_resultOutOfRange_expectResultCappedAtRangeLimit();
      arithmeticOperators_overflowValues_expectValuesNotModified();
    }
  public:
#pragma GCC push_options
#pragma GCC optimize ("O0")
    static const uint32_t performanceTestNumberOfOperations = 1000000;
    static void performanceTestAddition(void) {
      PERF_TEST_FUNC_START();
      static const uint32_t numberOfOperations = performanceTestNumberOfOperations;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointBase(fixedPointMinRange);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointIncrement =
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(one) /
        util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>(two);
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++)
        testFixedPointBase += testFixedPointIncrement;
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
    static void performanceTestMultiplication(void) {
      PERF_TEST_FUNC_START();
      static const uint32_t numberOfOperations = performanceTestNumberOfOperations;
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointBase(fixedPointMinRange);
      util::analog::FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> testFixedPointMultiplier(one);
      PERF_TEST_START();
      for (uint32_t i = 0; i < numberOfOperations; i++)
        testFixedPointBase *= testFixedPointMultiplier;
      PERF_TEST_END();
      PERF_TEST_FUNC_END();
    }
#pragma GCC pop_options
    static void testPerformance(void) {
      performanceTestAddition();
      performanceTestMultiplication();
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
      FixedPointTester<int32_t, 16, int64_t, INT32_MAX, INT32_MIN>::runTests();
      FixedPointTester<int32_t, 8, float, INT32_MAX, INT32_MIN>::runTests();
      FixedPointTester<int16_t, 4, int32_t, INT16_MAX, INT16_MIN>::runTests();
      FixedPointTester<int8_t, 2, int16_t, INT8_MAX, INT8_MIN>::runTests();
      FixedPointTester<uint32_t, 8, uint64_t, UINT32_MAX, 0>::runTests();
      FixedPointTester<uint16_t, 8, uint32_t, UINT16_MAX, 0>::runTests();
      FixedPointTester<uint32_t, 8, uint64_t, 0, 0>::runTests();
      FixedPointTester<uint16_t, 8, uint32_t, 0, 0>::runTests();
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
