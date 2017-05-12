/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
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

}; //namespace util
