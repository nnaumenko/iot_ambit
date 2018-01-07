/*
 * Copyright (C) 2016-2018 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "util_data.h"

namespace util {

namespace arrays {

//////////////////////////////////////////////////////////////////////
// CStrRingBuffer
//////////////////////////////////////////////////////////////////////

boolean CStrRingBuffer::push(const char * item) {
  /// @brief Adds c-string to the ring buffer
  /// @details Adds c-string to the ring buffer. If there is no free
  /// space available in the ring buffer to store the c-string, an
  /// oldest stored c-string is removed from the buffer
  /// @param item C-string to be added to the ring buffer
  if (!validate()) return (false);
  const size_t itemLength = strlen(item) + nullCharSize;
  if (itemLength >= ringBufferSize) return (false);
  while (this->fullUnsafe(itemLength)) {
    pop();
  }
  const size_t nextIndex = getNextIndex();
  if ((nextIndex + itemLength) <= ringBufferSize) { // The entire new string fits into free area at the end of ring buffer
    strcpy(&ringBuffer[nextIndex], item); //Copy entire string to buffer
    ringBuffer[nextIndex + itemLength - nullCharSize] = nullChar;
  }
  else { //Only part of string fits into free area at the end of the ring buffer
    const size_t firstPartLen = ringBufferSize - nextIndex; //How many characters fit into the end of the buffer
    const size_t secondPartLen = itemLength - firstPartLen;
    strncpy(&ringBuffer[nextIndex], item, firstPartLen); //Copy first part of the string to the end of the buffer
    //no need to set trailing null-terminator as we reserved the last character of the buffer and set it to zero
    strncpy(ringBuffer, &item[firstPartLen], secondPartLen);//Copy second part of the string to the beginnning of the buffer
    ringBuffer[secondPartLen] = nullChar;
  }
  totalCharCount += itemLength;
  cstrCount++;
  return (true);
}

void CStrRingBuffer::pop(void) {
  /// @brief Removes an oldest stored c-string from the ring buffer
  if (!validate()) return;
  if ((!cstrCount) || (!totalCharCount)) return;
  size_t lengthOldestItem = strlen(&ringBuffer[indexOldestItem]) + nullCharSize;
  //if the string being removed reaches the end of the ring buffer and there is no trailing zero at the end
  //then this string is stored as two parts, first part at the end and the second part at the beginning of the ring buffer
  if ((indexOldestItem + lengthOldestItem) <= ringBufferSize) { //the string being removed from buffer is a single-part string
    indexOldestItem += lengthOldestItem;
  }
  else { //the string being removed from buffer has its first part at the end of the buffer and second part at the beginning of the buffer
    size_t length2ndPart = strlen(ringBuffer) + nullCharSize;
    indexOldestItem = length2ndPart;
    lengthOldestItem += length2ndPart;
  }
  totalCharCount -= lengthOldestItem;
  cstrCount--;
}

size_t CStrRingBuffer::count(void) const {
  /// @brief Checks number of c-strings stored in the ring buffer
  /// @return Number of c-string currently stored in the ring buffer
  if (!validate()) return (0);
  return (cstrCount);
}

boolean CStrRingBuffer::full(const char * item) const {
  /// @brief Checks whether the supplied c-string can be stored in the
  /// ring buffer without removing previously stored c-string(s)
  /// @param item C-string checked by buffer
  /// @return true if the supplied c-string cannot be pushed to the ring
  /// buffer without removing an oldest stored c-string(s) from the
  /// buffer, otherwise returns false
  if (!validate()) return (false);
  return (fullUnsafe(strlen(item)));
}

boolean CStrRingBuffer::full(size_t length) const {
  /// @brief Checks whether the c-string with known length can potentially
  /// be stored in the ring buffer without removing previously stored c-string(s)
  /// @param length c-string length
  /// @return true if the c-string with provided length cannot be pushed to the
  /// ring buffer without removing an oldest stored c-string(s) from the
  /// buffer, otherwise returns false
  if (!validate()) return (false);
  return (fullUnsafe(length));
}

boolean CStrRingBuffer::fullUnsafe(const char * item) const {
  /// @brief Performs the full() operation without validity check
  /// @warning Should not be called if the ring buffer is not
  /// initialised. It is a caller's responsibility to make sure
  /// these conditions are met
  return (fullUnsafe(strlen(item)));
}

boolean CStrRingBuffer::fullUnsafe(size_t length) const {
  /// @brief Performs the full() operation without validity check
  /// @warning Should not be called if the ring buffer is not
  /// initialised. It is a caller's responsibility to make sure
  /// these conditions are met
  return ((ringBufferSize - totalCharCount) < length);
}

boolean CStrRingBuffer::empty(void) const {
  /// @brief Checks whether c-string ring buffer is empty
  /// @return true if there are no c-strings stored in the ring
  /// buffer, otherwise returns false
  if (!validate()) return (true);
  return (totalCharCount == 0);
}

size_t CStrRingBuffer::get(size_t index, char *dst, size_t dstSize) {
  /// @brief Get a c-string from the ring buffer
  /// @param index Index of c-string in the ring buffer (0 = oldest
  /// stored c-string in ring buffer)
  /// @param dst Destination to copy a c-string from ring buffer
  /// @param dstSize Size of buffer at dst, chars
  /// @return Number of chars actually copied (not including
  /// null-terminator)
  if (!dst || !dstSize) return (0);
  if ((!validate()) || (index >= cstrCount)) {
    if (dst && dstSize) dst[0] = nullChar;
    return (0);
  }
  const size_t startChar = getCstrStartChar(index);
  const char * cstr = &(ringBuffer[startChar]);
  const size_t length = strlen(cstr);
  if ((startChar + length + nullCharSize) <= ringBufferSize) { //this cstring is stored as a single-part
    const size_t lengthToCopy = ((dstSize - nullCharSize) < length) ? (dstSize - nullCharSize) : length;
    strncpy (dst, cstr, lengthToCopy);
    dst[lengthToCopy] = nullChar;
    return (lengthToCopy);
  }
  else { //the first part of this cstring is stored at the end of the buffer and the second part at the beginning
    const size_t firstPartLen = (dstSize - nullCharSize) < length ? (dstSize - nullCharSize) : length;
    size_t secondPartLen = strlen(ringBuffer);
    if ((dstSize - firstPartLen - nullCharSize) < secondPartLen) secondPartLen = dstSize - firstPartLen - nullCharSize;
    strncpy (dst, cstr, firstPartLen);
    if (secondPartLen) strncpy(&dst[firstPartLen], ringBuffer, secondPartLen);
    dst[firstPartLen + secondPartLen] = nullChar;
    return (firstPartLen + secondPartLen);
  }
}

size_t CStrRingBuffer::getCstrStartChar(size_t index) {
  /// @brief Get the cstring start position by cstring's index in ring buffer
  /// @param index Index in the ring buffer
  /// @return Cstring start position in the ring buffer
  /// @warning This method is unsafe: index must not exceed cstrCount. It is a
  /// caller's responsibility to ensure this condition is met.
  size_t currIndex = indexOldestItem;
  for (size_t i = 0; i < index; i++) {
    currIndex += (strlen (&ringBuffer[currIndex]) + nullCharSize);
    if (currIndex > ringBufferSize) currIndex = strlen(ringBuffer) + nullCharSize; //cstring is stored in two parts, first part at the end and second part at the beginning of the ring buffer
  }
  return (currIndex);
}

//////////////////////////////////////////////////////////////////////
// PrintToBuffer
//////////////////////////////////////////////////////////////////////

///@brief Saves single char to buffer
///@param character Char to save
size_t PrintToBuffer::write (uint8_t character) {
  if (bufferPosition >= (bufferSize - nullCharSize)) return (0);
  buffer[bufferPosition++] = static_cast<char>(character);
  buffer[bufferPosition] = nullChar;
  return (1);
}

///@brief Saves multiple chars to buffer
///@param buffer Location to copy chars from
///@param size Size of the buffer in chars
size_t PrintToBuffer::write(const uint8_t *buffer, size_t size) {
  const size_t maxCharsToCopy = bufferSize - bufferPosition - nullCharSize;
  if (maxCharsToCopy <= nullCharSize) return (0);
  const size_t charsToCopy = size > maxCharsToCopy ? maxCharsToCopy : size;
  strncpy(&this->buffer[bufferPosition], reinterpret_cast<const char*>(buffer), charsToCopy);
  this->buffer[bufferPosition + charsToCopy] = nullChar;
  bufferPosition += charsToCopy;
  return (charsToCopy);
}

}; //namespace arrays

namespace checksum {

//////////////////////////////////////////////////////////////////////
// crc functions
//////////////////////////////////////////////////////////////////////

/// @brief Reverse bit order in a 8-bit value
/// @param x Input value
/// @return Value with bits reversed
static uint8_t reverse8(uint8_t x) {
  x = ((x & 0xF0) >> 4) | ((x & 0x0F) << 4);
  x = ((x & 0xCC) >> 2) | ((x & 0x33) << 2);
  x = ((x & 0xAA) >> 1) | ((x & 0x55) << 1);
  return x;
}

/// @brief Reverse bit order in a 16-bit value
/// @param x Input value
/// @return Value with bits reversed
static uint16_t reverse16(uint16_t x) {
  x = (((x & 0xAAAA)) >> 1) | ((x & 0x5555) << 1);
  x = (((x & 0xCCCC)) >> 2) | ((x & 0x3333) << 2);
  x = (((x & 0xF0F0)) >> 4) | ((x & 0x0F0F) << 4);
  x = (((x & 0xFF00)) >> 8) | ((x & 0x00FF) << 8);
  return (x);
}

/*
/// @brief Reverse bit order in a 32-bit value
/// @param x Input value
/// @return Value with bits reversed
static uint32_t reverse32(uint32_t x) {
  x = (((x & 0xAAAAAAAA) >> 1) | ((x & 0x55555555) << 1));
  x = (((x & 0xCCCCCCCC) >> 2) | ((x & 0x33333333) << 2));
  x = (((x & 0xF0F0F0F0) >> 4) | ((x & 0x0F0F0F0F) << 4));
  x = (((x & 0xFF00FF00) >> 8) | ((x & 0x00FF00FF) << 8));
  return ((x >> 16) | (x << 16));
}
*/

uint16_t crc16(const void * buffer, size_t bufferSize, uint16_t poly, uint16_t init, boolean reverseIn, boolean reverseOut) {
  static const uint32_t polyHighBit = 0x01000000;
  static const uint32_t poly32 = (poly << 8) | polyHighBit;
  static const size_t crcBytes = 2;
  const uint8_t * data = reinterpret_cast<const uint8_t*>(buffer);
  uint32_t crc = (static_cast<uint32_t>(init) << 8);
  if (!buffer) bufferSize = 0;
  for (size_t i = 0; i < (bufferSize + crcBytes); i++) {
    if (i < bufferSize) crc |= reverseIn ? static_cast<uint32_t>(reverse8(*data++)) : static_cast<uint32_t>(*data++);
    for (int j = 0; j < 8; j++) {
      crc = crc << 1;
      if (crc & polyHighBit) crc ^= poly32;
    }
  }
  return (reverseOut ? reverse16(static_cast<uint16_t>(crc >> 8)) : static_cast<uint16_t>(crc >> 8));
}

}; //namespace checksum

namespace dsp {

//////////////////////////////////////////////////////////////////////
// Filter
//////////////////////////////////////////////////////////////////////

Filter::timestamp_t Filter::getDeltaTime(Filter::timestamp_t currentTime) {
  if (!lastTime) return (static_cast<timestamp_t>(0));
  return (currentTime - lastTime);
}

//////////////////////////////////////////////////////////////////////
// MovingAverage
//////////////////////////////////////////////////////////////////////

MovingAverage::MovingAverage(size_t numValues) : ringBuffer(numValues) {
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  if (!ringBuffer.validate()) {
    setInitStatus(Filter::Status::ERROR_INIT_NOT_ENOUGH_MEMORY);
    return;
  }
  setInitStatus(Filter::Status::OK);
}

Filter::Status MovingAverage::filterProcess(const value_t * inputs, size_t inputsNumber, value_t & output, timestamp_t timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (Filter::Status::ERROR_TOO_FEW_INPUTS);
  ringBuffer.push(inputs[0]);
  value_t total = value_t(0);
  value_t subtotal = value_t(0);
  for (size_t i = 0; i < ringBuffer.count(); i++) {
    value_t backupSubtotal = subtotal;
    subtotal += ringBuffer[i];
    if (overflow(subtotal)) {
      //When overflow occurs we divide part of the sum (subtotal) by amount of items and add it to total (precision is lost but overflow avoided)
      total += (backupSubtotal / value_t(ringBuffer.count()));
      subtotal = ringBuffer[i];
    }
  }
  if (ringBuffer.count()) {
    total += (subtotal / value_t(ringBuffer.count())); //if there was no overflow, then total is still zero at this point
  }
  output = total;
  return (Filter::Status::OK);
};

//////////////////////////////////////////////////////////////////////
// LowPass
//////////////////////////////////////////////////////////////////////

LowPass::LowPass(const value_t & fc, const value_t & pi, const value_t & fcDivider) {
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  if ((fc <= value_t(0)) || (fcDivider <= value_t(0)) || overflow (fc / fcDivider)) {
    this->fc = value_t(0);
    this->fcDivider = value_t(1);
    setInitStatus(Filter::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  this->fc = fc;
  this->fcDivider = fcDivider;
  fcPi2 = fc * pi * value_t(2);
  setInitStatus(Filter::Status::OK);
}

Filter::Status LowPass::filterProcess(const value_t * inputs, size_t inputsNumber, value_t & output, timestamp_t timestamp) {
  timestamp_t deltaTime (this->getDeltaTime(timestamp));
  if (!inputsNumber) {
    return (Filter::Status::ERROR_TOO_FEW_INPUTS);
  }
  if (!deltaTime) {
    output = value_t(inputs[0]);
    lastOutput = value_t(inputs[0]);
  }
  else {
    const value_t dt = value_t(deltaTime);
    output = lastOutput + (dt * fcPi2 * (inputs[0] - lastOutput)) / (dt * fcPi2 / fcDivider + value_t(1)) / fcDivider;
    lastOutput = output;
  }
  return (Filter::Status::OK);
}

//////////////////////////////////////////////////////////////////////
// LinearScale
//////////////////////////////////////////////////////////////////////

LinearScale::LinearScale(const value_t & x1, const value_t & y1, const value_t & x2, const value_t & y2) {
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  if (x1 == x2) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  a = (y1 - y2) / (x1 - x2);
  b = y1 - a * x1;
  if (overflow(a) || overflow(b) || (a == value_t(0))) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_FAILED);
    return;
  }
  setInitStatus(Filter::Status::OK);
}

LinearScale::LinearScale(const value_t & a, const value_t & b) {
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  this->a = a;
  this->b = b;
  if (overflow(a) || overflow(b)) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  Filter::setInitStatus(Filter::Status::OK);
}

LinearScale::LinearScale(const value_t & b) {
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  this->a = value_t(1);
  this->b = b;
  if (overflow(b)) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  Filter::setInitStatus(Filter::Status::OK);
}

Filter::Status LinearScale::filterProcess(const value_t * inputs, size_t inputsNumber, value_t & output, timestamp_t timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (Filter::Status::ERROR_TOO_FEW_INPUTS);
  output = a * inputs[0] + b;
  return (Filter::Status::OK);
}

//////////////////////////////////////////////////////////////////////
// SquareScale
//////////////////////////////////////////////////////////////////////

SquareScale::SquareScale(
  const value_t & x1, const value_t & y1,
  const value_t & x2, const value_t & y2,
  const value_t & x3, const value_t & y3) {
  //Set input number
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  //Calculate square function a, b, c coefficients
  const value_t denominator1 = (x1 - x2);
  const value_t denominator2 = (x1 - x3);
  const value_t denominator3 = (x2 - x3);
  if ((denominator1 == value_t(0)) ||
      (denominator2 == value_t(0)) ||
      (denominator3 == value_t(0))) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  a = x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2);
  a /= denominator1;
  a /= denominator2;
  a /= denominator3;
  b = (y1 - y2) * x3 * x3 + (y3 - y1) * x2 * x2 + (y2 - y3) * x1 * x1;
  b /= denominator1;
  b /= denominator2;
  b /= denominator3;
  c = y1 * (x2 - x3) * x2 * x3 + y2 * (x3 - x1) * x3 * x1 + y3 * (x1 - x2) * x1 * x2;
  c /= denominator1;
  c /= denominator2;
  c /= denominator3;
  if (overflow(a) || overflow(b) || overflow(c)) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_FAILED);
    return;
  }
  //Check parabola vertex position
  if (a == value_t(0)) {
    Filter::setInitStatus(Filter::Status::OK);
    return;
  }
  h = b / a / value_t(-2);
  //k = c - h * h;
  if (overflow(h)) return;
  value_t xmin = x1;
  if (x2 < xmin) xmin = x2;
  if (x3 < xmin) xmin = x3;
  value_t xmax = x3;
  if (x2 > xmax) xmax = x2;
  if (x1 > xmax) xmax = x1;
  if ((h > xmin) && (h < xmax)) {
    Filter::setInitStatus(Filter::Status::WARNING_INIT_DATA_INCORRECT);
    return;
  }
  Filter::setInitStatus(Filter::Status::OK);
}

SquareScale::SquareScale(const value_t & a, const value_t & b, const value_t & c) {
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  this->a = a;
  this->b = b;
  this->c = c;
  if (overflow(a) || overflow(b) || overflow (c)) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  Filter::setInitStatus(Filter::Status::OK);
}

Filter::Status SquareScale::filterProcess(const value_t * inputs, size_t inputsNumber, value_t & output, timestamp_t timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (Filter::Status::ERROR_TOO_FEW_INPUTS);
  output = a * inputs[0] * inputs[0] + b * inputs[0] + c;
  return (Filter::Status::OK);
}

//////////////////////////////////////////////////////////////////////
// SplineScale
//////////////////////////////////////////////////////////////////////

SplineScale::SplineScale(
  const value_t & xMin, const value_t & yMin,
  const value_t & xMid, const value_t & yMid,
  const value_t & xMax, const value_t & yMax) {
  //Set input number
  static const size_t inputNumber = 1;
  Filter::setInputsNumber(inputNumber, inputNumber);
  //Initialise points
  x1 = xMin;
  y1 = yMin;
  x2 = xMid;
  y2 = yMid;
  x3 = xMax;
  y3 = yMax;
  //Sort points by x in accending order
  struct ValueTSwapHelper {
    inline static void swap(value_t &a, value_t &b) {
      value_t temp = a;
      a = b;
      b = temp;
    }
  };
  if (x1 > x2) {
    ValueTSwapHelper::swap(x1, x2);
    ValueTSwapHelper::swap(y1, y2);
  }
  if (x2 > x3) {
    ValueTSwapHelper::swap(x2, x3);
    ValueTSwapHelper::swap(y2, y3);
  }
  if (x1 > x2) {
    ValueTSwapHelper::swap(x1, x2);
    ValueTSwapHelper::swap(y1, y2);
  }

  //Calculate cubic spline coefficients
  const value_t denominator1 = 2 * (x1 - x2);
  const value_t denominator2 = x1 - x3;
  const value_t denominator3 = x2 - x3;
  if ((denominator1 == value_t(0)) ||
      (denominator2 == value_t(0)) ||
      (denominator3 == value_t(0))) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  a1 = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
  a1 /= denominator1;
  a1 /= denominator2;
  a1 /= denominator3;
  a1 /= (x1 - x2);
  a2 = a1 * (x2 - x1);
  a2 /= (x2 - x3);
  b1 = x1 * x1 * (y3 - y2) - x1 * (x2 * (-3 * y1 + y2 + 2 * y3) + 3 * x3 * (y1 - y2)) + x2 * x2 * (y3 - y1) + x2 * x3 * (y2 - y1) + 2 * x3 * x3 * (y1 - y2);
  b1 /= denominator1;
  b1 /= denominator2;
  b1 /= denominator3;
  b2 = 2 * x1 * x1 * (y2 - y3) + x2 * (x1 * (y3 - y2) + x3 * (2 * y1 + y2 - 3 * y3)) + 3 * x1 * x3 * (y3 - y2) + x2 * x2 * (y3 - y1) + x3 * x3 * (y2 - y1);
  b2 /= denominator1;
  b2 /= denominator2;
  b2 /= denominator3;
  //Check validity
  if (overflow(a1) || overflow(a2) || overflow(b1) || overflow(b2)) {
    Filter::setInitStatus(Filter::Status::ERROR_INIT_FAILED);
    return;
  }
  Filter::setInitStatus(Filter::Status::OK);
}

Filter::Status SplineScale::filterProcess(const value_t * inputs, size_t inputsNumber, value_t & output, timestamp_t timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (Filter::Status::ERROR_TOO_FEW_INPUTS);
  output = (inputs[0] < x2) ?
           (a1 * (inputs[0] - x1) * (inputs[0] - x1) * (inputs[0] - x1) + b1 * (inputs[0] - x1) + y1) :
           (a2 * (inputs[0] - x3) * (inputs[0] - x3) * (inputs[0] - x3) + b2 * (inputs[0] - x3) + y3);
  return (Filter::Status::OK);
}

}; //namespace dsp

namespace quantity {

//////////////////////////////////////////////////////////////////////
// Quantity
//////////////////////////////////////////////////////////////////////

boolean Quantity::setConvertedValue(
  value_t value,
  unit_t units,
  text_t unitText,
  value_t minRange,
  value_t maxRange)
{
  /// @brief Sets converted value and measurement units (accessible via public getter methods)
  /// @param value Value converted into units
  /// @param units Measurement units in numeric form
  /// @param unitText Measurement units in human-readable form
  /// @return True if the value if valid and can be converted
  if (!validate()) return (false);
  setUnitValue = value;
  setUnit = units;
  setUnitText = unitText;
  if ((initMinRange != Value(0)) || (initMaxRange != Value(0))) {
    setUnitMinRange = minRange;
    setUnitMaxRange = maxRange;
    if (this->setUnitValue < this->setUnitMinRange) this->setUnitValue = minRange;
    if (this->setUnitValue > this->setUnitMaxRange) this->setUnitValue = maxRange;
  }
  return (true);
}

//////////////////////////////////////////////////////////////////////
// Dimensionless
//////////////////////////////////////////////////////////////////////

boolean Dimensionless::convertToUnit(Dimensionless::Unit unit) {
  /// @brief Performs conversion to a measurement unit
  /// @param unit Measurement unit to convert to
  /// @return true if conversion was successful,
  /// false if error occured during conversion
  const Unit initDimensionlessUnit = static_cast<Unit>(getInitUnit());
  static const value_t percentFactor(100);
  //If not initialised, fail
  if (!validate()) return (false);
  //If init and target units are the same, revert to init values
  if (unit == initDimensionlessUnit) {
    setConvertedValue(getInitValue(),
                      getInitUnit(),
                      getUnitTextByUnit(initDimensionlessUnit),
                      getInitMinRange(),
                      getInitMaxRange());
    return (true);
  }
  //Convert from no-unit to percent (multiply by 100%)
  if (initDimensionlessUnit == Unit::NONE && unit == Unit::PERCENT) {
    setConvertedValue(getInitValue() * percentFactor,
                      static_cast <unit_t>(Unit::PERCENT),
                      getUnitTextByUnit(Unit::PERCENT),
                      getInitMinRange() * percentFactor,
                      getInitMaxRange() * percentFactor);
    return (true);
  }
  //Convert from percent to no-unit (divide by 100%)
  if (initDimensionlessUnit == Unit::PERCENT && unit == Unit::NONE) {
    setConvertedValue(getInitValue() / percentFactor,
                      static_cast <unit_t>(Unit::NONE),
                      getUnitTextByUnit (Unit::NONE),
                      getInitMinRange() / percentFactor,
                      getInitMaxRange() / percentFactor);
    return (true);
  }
  //Unit not found or conversion not defined, fail
  return (false);
}

Dimensionless::text_t Dimensionless::getUnitTextByUnit(Dimensionless::Unit unit) {
  /// @brief Returns human-readable form of a measurement unit
  /// @param Measurement unit
  /// @return A cstring in RAM with human-readable form of unit
  //  static const char unitTextNone[] = "";
  //static const char unitTextPercent[] = "%";
  switch (unit) {
    case Unit::NONE:
      //  return (unitTextNone);
      return (StrRef(F("")));
    case Unit::PERCENT:
      //      return (unitTextPercent);
      return (StrRef(F("%")));
  }
  return (StrRef());
}

//////////////////////////////////////////////////////////////////////
// Temperature
//////////////////////////////////////////////////////////////////////

boolean Temperature::convertToUnit(Temperature::Unit unit) {
  /// @brief Performs conversion to a measurement unit
  /// @param unit Measurement unit to convert to
  /// @return true if conversion was successful,
  /// false if error occured during conversion
  const Unit initTempUnit = static_cast<Unit>(getInitUnit());
  //If not initialised, fail
  if (!validate()) return (false);
  //If init and target units are the same, revert to init values
  if (unit == initTempUnit) {
    setConvertedValue(getInitValue(),
                      getInitUnit(),
                      getUnitTextByUnit(initTempUnit),
                      getInitMinRange(),
                      getInitMaxRange());
    return (true);
  }
  //Convert Celsius to Fahrenheit
  if (initTempUnit == Unit::CELSIUS && unit == Unit::FAHRENHEIT) {
    setConvertedValue(celsiusToFahrenheit(getInitValue()),
                      static_cast <unit_t>(Unit::FAHRENHEIT),
                      getUnitTextByUnit(Unit::FAHRENHEIT),
                      celsiusToFahrenheit(getInitMinRange()),
                      celsiusToFahrenheit(getInitMaxRange()));
    return (true);
  }
  //Convert Fahrenheit to Celsius
  if (initTempUnit == Unit::FAHRENHEIT && unit == Unit::CELSIUS) {
    setConvertedValue(fahrenheitToCelsius(getInitValue()),
                      static_cast <unit_t>(Unit::CELSIUS),
                      getUnitTextByUnit (Unit::CELSIUS),
                      fahrenheitToCelsius(getInitMinRange()),
                      fahrenheitToCelsius(getInitMaxRange()));
    return (true);
  }
  //Unit not found or conversion not defined, fail
  return (false);
}

Temperature::text_t Temperature::getUnitTextByUnit(Temperature::Unit unit) {
  /// @brief Returns human-readable form of a measurement unit
  /// @param Measurement unit
  /// @return A cstring in RAM with human-readable form of unit
  //static const char unitTextCelsius[] = "C";
  //static const char unitTextFahrenheit[] = "F";
  switch (unit) {
    case Unit::CELSIUS:
      //      return (unitTextCelsius);
      return (text_t(F("C")));
    case Unit::FAHRENHEIT:
      //      return (unitTextFahrenheit);
      return (text_t(F("F")));
  }
  return (StrRef());
  //return (nullptr);
}

Temperature::value_t Temperature::celsiusToFahrenheit(value_t celsiusValue) {
  ///Convert temperature from degrees Celsius to degrees Fahrenheit
  static const value_t celsiusFahrenheitA1(9);
  static const value_t celsiusFahrenheitA2(5);
  static const value_t celsiusFahrenheitB(32);
  return ((celsiusValue * celsiusFahrenheitA1 / celsiusFahrenheitA2) + celsiusFahrenheitB);
}

Temperature::value_t Temperature::fahrenheitToCelsius(value_t fahrenheitValue) {
  ///Convert temperature from degrees Fahrenheit to degrees Celsius
  static const value_t fahrenheitCelsiusA1(5);
  static const value_t fahrenheitCelsiusA2(9);
  static const value_t fahrenheitCelsiusB(32);
  return ((fahrenheitValue - fahrenheitCelsiusB) * fahrenheitCelsiusA1 / fahrenheitCelsiusA2);
}

}; //namespace quantity

}; //namespace util
