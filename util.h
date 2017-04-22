/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file
 * @brief Utility functions and classes.
 */

#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>

//new, placement new and delete are already defined in ESP8266 libraries
//void * operator new (size_t size) { return malloc (size); }
//void * operator new (size_t size, void * ptr) { return ptr; }
//void operator delete (void * ptr) { free (ptr); }

namespace util {

/// @brief Selects one of two type based on boolean condition.
///
/// @tparam conditon Boolean expression based on which the type
/// will be selected.
/// @tparam T1 Type to be used if condition is true
/// @tparam T2 Type to be used if condition is false
/// @details TypeSelect::data_t is defined as <typename T1> if
/// <boolean condition> is true.
/// @details TypeSelect::data_t is defined as <typename T2> if
/// <boolean condition> is false.
template<boolean condition, typename T1, typename T2>
struct TypeSelect;
template<typename T1, typename T2>
struct TypeSelect<false, T1, T2> {
  typedef T2 data_t;
};
template<typename T1, typename T2>
struct TypeSelect<true, T1, T2> {
  typedef T1 data_t;
};

namespace arrays {

/// @brief Circular buffer
/// @tparam T Type of the data to be stored in buffer. Must have a trivial
/// constructor and copy constructor to compile (or to be a PODtype)
/// @warning If <T> has a constructor and its sizeof() is smaller than pointer's
/// sizeof(), <T> will be passed by value which results in extra copy constructor
/// calls). To avoid this, make sure that sizeof(T) is greater or equal than
/// sizeof(T*).
/// @details Implements a circular buffer. Data are sent to the circular buffer with
/// push() method until buffer is full and oldest data is overwritten. Oldest data
/// can be removed from the circular buffer with pop() method. Any element in circular
/// buffer is accessible via subscript operator (0 is oldest stored element).

template <typename T>
class RingBuffer {
  public:
    /// Defines the way to pass <typename T>: via value (T) or via reference(const T&),
    /// depending on which is smaller
    ///
    /// @warning If <T> has a constructor and its sizeof() is smaller than pointer's
    /// sizeof(), <T> will be passed by value which results in extra copy constructor
    /// calls). If calling constructor is costly or otherwise undesirable, make sure
    /// that sizeof(T) is greater or equal than sizeof(T*).
    typedef typename TypeSelect < (sizeof(T*) < sizeof(T)), const T &, T >::data_t data_t;
    // typedef typename TypeSelect < __is_pod(T), T, const T &>::data_t data_t;
  public:
    inline RingBuffer ();
    inline RingBuffer (void * buffer, size_t itemsInBuffer);
    inline ~RingBuffer ();
    inline boolean validate(void) const;
    void push(data_t item);
    inline void pop(void);
    inline void pop(size_t number);
    inline size_t count(void) const;
    inline boolean full(void) const;
    inline boolean empty(void) const;
    data_t operator [] (size_t index) const;
  private:
    inline void popUnsafe(void);
  private:
    T * ringBuffer = NULL;
    size_t ringBufferSize = 0;
    size_t indexOldestItem = 0;
    size_t itemsCount = 0;
  private:
    const T defaultItem = T();
};

template <typename T>
RingBuffer<T>::RingBuffer () {
  /// @brief Creates non-initialised ring buffer
  /// @details Only needed for compatibility
  /// @details Non-initialised circular buffer behaves as empty
  /// ring buffer with the following differences:
  /// * validate() always returns false
  /// * push() and pop() perform no action
  /// * count() always returns 0
}

template <typename T>
RingBuffer<T>::RingBuffer (void * buffer, size_t itemsInBuffer) {
  /// @brief Creates a ring buffer.
  /// @param buffer Memory area to store buffer data
  /// @param itemsInBuffer How much items of type T can be stored in the buffer
  ringBuffer = reinterpret_cast<T*>(buffer);
  ringBufferSize = itemsInBuffer;
}

template <typename T>
RingBuffer<T>::~RingBuffer () {
  /// @brief Performs a ring buffer cleanup by calling pop() for every items in buffer
  if (!validate()) return;
  const size_t tempCount = itemsCount; //itemsCount field will be modified by popUnsafe()
  for (size_t i = 0; i < tempCount; i++)
    popUnsafe();
}

template <typename T>
boolean RingBuffer<T>::validate(void) const {
  /// @brief Checks if the ring buffer is initialised
  /// @return true if ring buffer is initialised, otherwise returns false
  return ((ringBuffer != NULL) && (ringBufferSize != 0));
}

template <typename T>
void RingBuffer<T>::push(RingBuffer<T>::data_t item) {
  /// @brief Adds item to the ring buffer
  /// @details Adds item to the ring buffer. If the ring buffer is full,
  /// an oldest item is removed from the ring buffer
  /// @param item Item to be added to the ring buffer
  if (!validate()) return;
  if (itemsCount == ringBufferSize) popUnsafe();
  size_t nextIndex = indexOldestItem + itemsCount;
  if (nextIndex >= ringBufferSize) nextIndex -= (ringBufferSize);
  new (&ringBuffer[nextIndex]) T(item);
  itemsCount++;
}

template <typename T>
void RingBuffer<T>::popUnsafe(void) {
  /// @brief Removes an oldest item from the ring buffer without performing any validity check
  /// @warning Should not be called if the ring buffer is empty or non-initialised. It is
  /// a caller's responsibility to make sure these conditions are met
  ringBuffer[indexOldestItem].~T();
  indexOldestItem++;
  if (indexOldestItem >= ringBufferSize) indexOldestItem = 0;
  itemsCount--;
}

template <typename T>
void RingBuffer<T>::pop(void) {
  /// @brief Removes an oldest item from the ring buffer
  if (!validate()) return;
  if (!itemsCount) return;
  popUnsafe();
}

template <typename T>
void RingBuffer<T>::pop(size_t number) {
  /// @brief Removes a certain amount of oldest items from the ring buffer
  /// @param number Number of items to remove
  if (!validate()) return;
  if (number > itemsCount) number = itemsCount;
  for (size_t i = 0; i < number; i++)
    popUnsafe();
}

template <typename T>
size_t RingBuffer<T>::count(void) const {
  /// @brief Checks number of items in the ring buffer
  /// @return Number of items currently stored in the ring buffer
  if (!validate()) return (0);
  return (itemsCount);
}

template <typename T>
boolean RingBuffer<T>::full(void) const {
  /// @brief Checks whether the ring buffer is full
  /// @return true if the item cannot be pushed to the ring buffer without
  /// removing an oldest item from the buffer, otherwise returns false
  if (!validate()) return (false);
  return (itemsCount == ringBufferSize);
}

template <typename T>
boolean RingBuffer<T>::empty(void) const {
  /// @brief Checks whether the ring buffer is empty
  /// @return true if there are no items in the ring buffer, otherwise
  /// returns false
  if (!validate()) return (true);
  return (!itemsCount);
}

template <typename T>
typename RingBuffer<T>::data_t RingBuffer<T>::operator [] (size_t index) const {
  /// @brief Get an item from the ring buffer
  /// @param index Index of item in the ring buffer (0 = oldest item in ring buffer)
  /// @return Item from the ring buffer or defaultItem if error occured during operation
  if (!validate()) return (defaultItem);
  if (index >= itemsCount) return (defaultItem);
  size_t ringBufferIndex = indexOldestItem + index;
  if (ringBufferIndex >= ringBufferSize) ringBufferIndex -= ringBufferSize;
  return (ringBuffer[ringBufferIndex]);
}

/// @brief Circular buffer for storing c-strings
/// @details Implements a c-string circular buffer. C-strings are sent to the circular
/// buffer with push() method until buffer is full, then oldest c-strings are
/// overwritten. Oldest c-strings can also be manually removed from the circular
/// buffer with pop() method. Circular buffer contents are accessible with get()
/// method.
class CStrRingBuffer {
  public:
    inline CStrRingBuffer ();
    inline CStrRingBuffer (char * buffer, size_t charsInBuffer);
    inline boolean validate(void) const;
    boolean push(const char * item);
    void pop(void);
    size_t count(void) const;
    boolean full(const char * item) const;
    boolean full(size_t length) const;
    boolean empty(void) const;
    size_t get(size_t index, char *dst, size_t dstSize);
  private:
    inline boolean fullUnsafe(const char * item) const;
    inline boolean fullUnsafe(size_t length) const;
    inline size_t getNextIndex(void) const;
  private:
    size_t getCstrStartChar(size_t index);
  private:
    char * ringBuffer = NULL;
    size_t ringBufferSize = 0;
    size_t indexOldestItem = 0;
    size_t totalCharCount = 0;
    size_t cstrCount = 0;
    static const char nullChar = '\0';
    static const size_t nullCharSize = sizeof (nullChar);
};

CStrRingBuffer::CStrRingBuffer() {
  /// @brief Creates non-initialised c-string circular buffer
  /// @details Only needed for compatibility
  /// @details Non-initialised circular buffer behaves as empty
  /// ring buffer with the following differences:
  /// * validate() always returns false
  /// * push() and pop() perform no action
  /// * count() always returns 0
}

CStrRingBuffer::CStrRingBuffer(char * buffer, size_t charsInBuffer) {
  /// @brief Creates a ring buffer
  /// @param buffer Memory area to store buffer data
  /// @param charsInBuffer Buffer size in chars
  if (charsInBuffer < (2 * nullCharSize)) return;
  if (!buffer) return;
  //Minimum buffer length 2 chars, 1 char is reserved for '\0' at the end
  //of the buffer to be able to print cstrings directly from the ring buffer
  ringBuffer = buffer;
  ringBufferSize = charsInBuffer - nullCharSize;
  ringBuffer[charsInBuffer - 1] = '\0';
}

boolean CStrRingBuffer::validate(void) const {
  /// @brief Checks if the c-string ring buffer is initialised
  /// @return true if ring buffer is initialised, otherwise returns false
  return ((ringBuffer != NULL) && (ringBufferSize != 0));
}

size_t CStrRingBuffer::getNextIndex(void) const {
  /// @brief Finds next position to store a c-string in buffer
  /// @return Position in the buffer to store next c-string
  const size_t nextIndex = indexOldestItem + totalCharCount;
  if (nextIndex >= ringBufferSize) return (nextIndex - ringBufferSize);
  return (nextIndex);
}

class PrintToBuffer : public Print {
  public:
    inline PrintToBuffer(char * buffer, size_t bufferSize);
    virtual size_t write(uint8_t character);
    virtual size_t write(const uint8_t *buffer, size_t size);//TODO
  private:
    char * buffer = NULL;
    size_t bufferSize = 0;
    size_t bufferPosition = 0;
    static const char nullChar = '\0';
    static const size_t nullCharSize = sizeof (nullChar);
};

PrintToBuffer::PrintToBuffer(char * buffer, size_t bufferSize) {
  this->buffer = buffer;
  this->bufferSize = bufferSize;
}

}; //namespace arrays;

}; //namespace util;

#endif
