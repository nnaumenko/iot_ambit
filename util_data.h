/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file
 * @brief Custom data structures and data handling for general use
 * @detail The following functionality is grouped into the namespaces:
 *  * arrays: data structures storing multiple units of data
 *  * checksum: checksum calculation
 *  * filter: analog value filtering
 *  * analog: analog value acquiring and processing
 */

#ifndef UTIL_DATA_H
#define UTIL_DATA_H

#include <Arduino.h>

//new, placement new and delete are already defined in ESP8266 libraries
//void * operator new (size_t size) { return malloc (size); }
//void * operator new (size_t size, void * ptr) { return ptr; }
//void operator delete (void * ptr) { free (ptr); }

namespace util {

/// @brief Selects one of two type based on boolean condition.
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
/// See TypeSelect
template<typename T1, typename T2>
struct TypeSelect<false, T1, T2> {
  typedef T2 data_t;
};
/// See TypeSelect
template<typename T1, typename T2>
struct TypeSelect<true, T1, T2> {
  typedef T1 data_t;
};

namespace arrays {

//////////////////////////////////////////////////////////////////////
// RingBuffer
//////////////////////////////////////////////////////////////////////

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
    inline RingBuffer (size_t itemsInBuffer);
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
    boolean memoryAllocated = false;
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
  /// @brief Creates a ring buffer
  /// @param buffer Memory area to store buffer data
  /// @param itemsInBuffer How much items of type T can be stored in the buffer
  ringBuffer = reinterpret_cast<T*>(buffer);
  ringBufferSize = itemsInBuffer;
}

template <typename T>
RingBuffer<T>::RingBuffer (size_t itemsInBuffer) {
  /// @brief Creates a ring buffer and allocates memory to store the data
  /// @warning Not recommended for repeated use due to possible memory fragmentation
  /// @param itemsInBuffer How much items of type T can be stored in the buffer
  ringBuffer = reinterpret_cast<T*> (malloc(itemsInBuffer * sizeof(T)));
  if (ringBuffer) {
    new (ringBuffer) T[itemsInBuffer];
    ringBufferSize = itemsInBuffer;
    memoryAllocated = true;
  }
}
template <typename T>
RingBuffer<T>::~RingBuffer () {
  /// @brief Performs a ring buffer cleanup
  /// @details If no memory was allocated by this class, just calls pop() for every item in buffer
  /// @details If memory
  if (!validate()) return;
  if (memoryAllocated) {
    delete(ringBuffer);
  }
  else {
    const size_t tempCount = itemsCount; //itemsCount field will be modified by popUnsafe()
    for (size_t i = 0; i < tempCount; i++)
      popUnsafe();
  }
}

template <typename T>
boolean RingBuffer<T>::validate(void) const {
  /// @brief Checks if the ring buffer is initialised
  /// @return true if ring buffer is initialised, otherwise returns false
  return (ringBuffer && ringBufferSize);
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

//////////////////////////////////////////////////////////////////////
// CStrRingBuffer
//////////////////////////////////////////////////////////////////////

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

/// @brief A Print class which saves to buffer everything was printed with it
/// @details Text is saved to buffer until buffer is full. After that new
/// text is ignored
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

/// @brief Initialises PrintToBuffer
/// @param buffer Buffer to save printed text to
/// @param bufferSize Size of the buffer in chars
PrintToBuffer::PrintToBuffer(char * buffer, size_t bufferSize) {
  this->buffer = buffer;
  this->bufferSize = bufferSize;
}

}; //namespace arrays

namespace checksum {

//////////////////////////////////////////////////////////////////////
// crc16
//////////////////////////////////////////////////////////////////////

/// @brief Calculates checksum crc16 for a memory area
/// @details Implementation based on this great guide: http://www.ross.net/crc/download/crc_v3.txt
/// @param buffer A memory area to calculate crc16 for
/// @param bufferSize Memory area size in bytes
/// @param poly A polynomial to calculate CRC. High (17th) bit of the polynomial is implicitly set. Default is 0x8005.
/// @param reverseIn If true, bit order in every input byte is reversed before performing CRC calculations. True by default.
/// @param reverseOut If true, bit order in CRC value is reversed after calculations. True by default.
/// @return Calculated CRC16 value
uint16_t crc16(const void * buffer, size_t bufferSize, uint16_t poly = 0x8005, uint16_t init = 0x0000, boolean reverseIn = true, boolean reverseOut = true);

}; //namespace checksum

namespace analog {

//////////////////////////////////////////////////////////////////////
// FixedPoint
//////////////////////////////////////////////////////////////////////

/// @brief Fixed point number with integer and fraction parts
/// @tparam T An integer base type for fixed point number.
/// @tparam FractionBits Number of least significant bits storing fraction part
/// @tparam U A wider range data type, used as intermediary to avoid overflow during calculations
/// @tparam TRangeMin Minimum value possible for T
/// @tparam TRangeMax Maximum value possible for T
/// @details Basic arithmetic (+, -, *, /, +=, -=, *=, /=) and logic (<, <=, >, >=, !=, ==)
/// operations are possible.
/// @details If as a result of the arithmetic operation the Fixed Point value reaches
/// minimum or maximum possible values for integer part, it becames an overflow value. The
/// overflow values are capped at minimum and maximum of possible range. Any further arithmetic
/// operation with overflow value results in the same overflow value.
/// @details Overflow can be detected with overflow() method.
/// @details If both TRangeMin or TRangeMax are zero, no range check is performed and the
/// fixed-point value is not overflow-safe; overflow() method always returns false.
/// @details Minimum and maximum possible values for integer part are declared as constants min and max.
/// @details Conversion to the integer type T is possible and is performed by rounding (i.e. if fraction
/// part is greater or equal to 0.5 the return value is increased by 1).
template <typename T, size_t FractionBits, typename U = T, T TMinRange = static_cast<T>(0), T TMaxRange = static_cast<T>(0)>
class FixedPoint {
  public:
    /// @brief Default constructor, sets value to zero
    inline FixedPoint() {
      value.setT(tZero);
    }
    /// @brief Copy constructor, copies value from other FixedPoint object
    /// @param other Other FixedPoint object to copy value from
    inline FixedPoint(const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &other) {
      value = other.value;
    }
    /// @brief Creates a Fixed Point value from integer and decimal fraction part
    /// @details For example Pi can be defined as follows:
    ///     FixedPoint(3, 14159265);
    /// @param integer Integer part
    /// @param decFraction Decimal fraction part
    FixedPoint(T integer, T decFraction = tZero);
  public:
    /// @brief Convert integer part to integer type T
    /// @details Conversion is performed by rounding, e.g. conversion of value 1.5 will return 2
    /// @return Integer part of the value
    inline explicit operator T() const {
      if (TMinRange && TMaxRange) {
        if (value.get() == TMaxRange) return (TMaxRange);
        if (value.get() == TMinRange) return (TMinRange);
      }
      T absValue = value.get();
      boolean negativeValue = false;
      if (TMinRange && TMaxRange) {
        if (TMinRange < tZero) {
          if (absValue < tZero) {
            absValue = ~absValue + tOne;
            negativeValue = true;
          }
        }
      }
      absValue = (absValue >> FractionBits) + ((absValue >> (FractionBits - 1)) & tOne); //add 1 if most significant fraction bit is set (fraction part >= 0.5)
      return (negativeValue ? ~absValue + tOne : absValue);
    }
    /// @brief Assignment operator, copies value from other FixedPoint object
    /// @param other Other FixedPoint object to copy value from
    inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>& operator = (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &other) {
      value.setT(other.value.get());
      return (*this);
    }
  public:
    inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>& operator += (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      value.setU(static_cast<U>(value.get()) + static_cast<U>(rhs.value.get()));
      return (*this);
    }
    inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>& operator -= (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      value.setU(static_cast<U>(value.get()) - static_cast<U>(rhs.value.get()));
      return (*this);
    }
    inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>& operator *= (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      value.setU(static_cast<U>(value.get()) * static_cast<U>(rhs.value.get()) / static_cast<U>(fractionBitsPwr2));
      return (*this);
    }
    inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>& operator /= (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      value.setU(static_cast<U>(value.get()) * static_cast<U>(fractionBitsPwr2) /  static_cast<U>(rhs.value.get()));
      return (*this);
    }
    friend inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>
    operator + (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &lhs, const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> temp = lhs;
      temp += rhs;
      return (temp);
    }
    friend inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>
    operator - (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &lhs, const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> temp = lhs;
      temp -= rhs;
      return (temp);
    }
    friend inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>
    operator * (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &lhs, const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> temp = lhs;
      temp *= rhs;
      return (temp);
    }
    friend inline FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>
    operator / (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &lhs, const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) {
      FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> temp = lhs;
      temp /= rhs;
      return (temp);
    }
  public:
    inline boolean operator == (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) const {
      return (value.get() == rhs.value.get());
    }
    inline boolean operator != (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) const {
      return (!(*this == rhs));
    }
    inline boolean operator < (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) const {
      return (value.get() < rhs.value.get());
    }
    inline boolean operator > (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) const {
      return (rhs < *this);
    }
    inline boolean operator <= (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) const {
      return (!(*this > rhs));
    }
    inline boolean operator >= (const FixedPoint<T, FractionBits, U, TMinRange, TMaxRange> &rhs) const {
      return (!(*this < rhs));
    }
  public:
    static const T min = TMinRange ? ((TMinRange >> FractionBits) + 1) : TMinRange; ///< Minimum range for integer part of FixedPoint value
    static const T max = TMaxRange >> FractionBits; ///< Maximum range for integer part of FixedPoint value
    /// @brief Detect overflow
    /// @return true if the value of this object is overflow, false otherwise.
    /// If the minimum/maximum limits are not set, always returns false.
    boolean overflow(void) {
      if (!TMinRange || !TMaxRange) return (false);
      if ((value.get() == TMinRange) || (value.get() == TMaxRange)) return (true);
      return (false);
    }
  protected:
    /// @brief Internal value class, created to encapsulate the value and unify value assignment
    class {
      public:
        /// @brief Sets value from type T value
        /// @newValue New value of type T, consists of integer and fraction part at this point
        /// @return true if no overflow occured, false otherwise
        inline boolean setT(T newValue) {
          if (TMinRange && TMaxRange) {
            if (val == TMaxRange || val == TMinRange) return (false);
          }
          val = newValue;
          return (true);
        }
        /// @brief Sets value from type U value
        /// @newValue New value of type U, consists of integer and fraction part at this point
        /// @return true if no overflow occured, false otherwise
        inline boolean setU(U newValue) {
          if (TMinRange && TMaxRange) {
            if (val == TMaxRange || val == TMinRange) {
              return (false);
            }
          }
          if (TMaxRange && TMinRange) {
            if (newValue < static_cast<U>(TMinRange)) {
              val = TMinRange;
              return (false);
            }
            if (newValue > static_cast<U>(TMaxRange)) {
              val = TMaxRange;
              return (false);
            }
          }
          val = static_cast<T>(newValue);
          return (true);
        }
        /// @brief Returns value of type T (with integer and fraction parts)
        /// @returns Value of type T, consists of integer and fraction part
        inline T get(void) const {
          return (val);
        }
      private:
        T val = {}; ///< Storage for value
    } value;
  protected:
    static const T tZero = static_cast<T>(0); ///< 0 constant of type T
    static const T tOne = static_cast<T>(1);  ///< 1 constant of type T
    static const T fractionBitsPwr2 = tOne << FractionBits; ///< A number of type T, equals to 2 pow FractionBits
    //Make sure that FractionBits are less than total bit width of T
    static_assert(FractionBits < (sizeof(T) * 8), "Too many fraction bits"); 
    //Make sure that more than zero FractionBits are defined (if zero fraction bits required, simply use type T)
    static_assert(FractionBits > 0, "Too few fraction bits");
};

template <typename T, size_t FractionBits, typename U, T TMinRange, T TMaxRange>
FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>::FixedPoint(T integer, T decFraction) {
  static const T decimalRadix = static_cast<T>(10);
  //check integer part against range
  if (TMinRange && TMaxRange) {
    if (integer < min) {
      value.setT(TMinRange);
      return;
    }
    if (integer > max) {
      value.setT(TMaxRange);
      return;
    }
  }
  //fraction part
  T fractionPart = tZero;
  if (decFraction) {
    //negate fraction part if it is negative
    if (decFraction < tZero) decFraction = ~decFraction + tOne;
    //truncate decimal fractions to fit max limit (TODO: truncate to whole T range to improve precision)
    if (max) {
      while (decFraction > max) {
        decFraction /= decimalRadix;
      }
    }
    //calculate divider as power of decimalRadix
    T tempDecFraction = decFraction;
    int dividerPwr10 = 0;
    while (tempDecFraction) {
      dividerPwr10++;
      tempDecFraction /= decimalRadix;
    }
    //safely divide fraction by divider
    fractionPart = decFraction << FractionBits; //no overflow since decFraction was truncated to range
    for (int i = 0; i < dividerPwr10 ; i++) {
      fractionPart /= decimalRadix;
    }
  }
  if (integer < tZero) fractionPart = ~fractionPart + tOne;
  //calculate fixed point number as a sum of integer and fraction parts
  this->value.setT((integer << FractionBits) + fractionPart);
}

}; //namespace analog

}; //namespace util

#endif
