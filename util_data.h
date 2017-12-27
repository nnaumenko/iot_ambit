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
 *  * dsp: digital signal processing
 *  * quantity: physical quantities
 */

#ifndef UTIL_DATA_H
#define UTIL_DATA_H

#include <Arduino.h>

//new, placement new and delete are already defined in ESP8266 libraries
//void * operator new (size_t size) { return malloc (size); }
//void * operator new (size_t size, void * ptr) { return ptr; }
//void operator delete (void * ptr) { free (ptr); }

namespace util {

//////////////////////////////////////////////////////////////////////
// TypeSelect
//////////////////////////////////////////////////////////////////////

/// @brief Selects one of two type based on boolean condition.
/// @tparam conditon Boolean expression based on which the type
/// will be selected.
/// @tparam T1 Type to be used if condition is true
/// @tparam T2 Type to be used if condition is false
/// @details TypeSelect::data_t is defined as <typename T1> if
/// <boolean condition> is true.
/// @par TypeSelect::data_t is defined as <typename T2> if
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

//////////////////////////////////////////////////////////////////////
// Ref
//////////////////////////////////////////////////////////////////////

/// @brief A pointer which also stores memory area (RAM of PROGMEM)
/// @tparam T Pointer's type (e.g. Ref<int> uses pointer type
/// const int *)
/// @warning If pointer is located in RAM, this class does not
/// guarantee pointer's validity and does not safely handle situations
/// such as "use-after-delete"; it merely stores a pointer.
template <typename T>
class Ref {
  public:
    inline Ref(const T * pointer, boolean progmem) {
      /// @param pointer Pointer itself
      /// @param progmem True if memory area referenced by pointer is
      /// located in PROGMEM, false if memory area is located in RAM
      this->pointer = pointer;
      this->progmem = progmem;
    }
  public:
    inline boolean pgm(void) const {
      /// @return true if memory area referenced by pointer is located in
      /// PROGMEM, false if memory area is located in RAM
      return (this->progmem);
    }
  public:
    inline explicit operator const T* () const {
      /// @return Pointer itself; the pointer contains no information
      /// whether it refers to RAM or PROGMEM; this information is
      /// obtained via pgm() method
      return (pointer);
    }
    inline operator bool() {
      /// @return true if pointer is nullptr, false otherwise
      return (pointer);
    }
  public:
    inline boolean operator == (const Ref<T> &rhs) const {
      return ((pointer == rhs.pointer) && (progmem == rhs.progmem));
    }
  private:
    const T * pointer = nullptr;  ///< Pointer itself
    boolean progmem = false;      ///< Is reference located in progmem
} __attribute__((packed));

//////////////////////////////////////////////////////////////////////
// StrRef
//////////////////////////////////////////////////////////////////////

/// @brief Pointer to c-string (const char *) which also stores
/// c-string's memory area (RAM or PROGMEM)
class StrRef : public Ref<char> {
  public:
    /// @brief Initialise default pointer
    StrRef() : Ref <char>(nullptr, false) {}
    /// @brief Initialise from c-string in RAM
    /// @param Pointer to c-string in RAM
    StrRef (const char * ramPointer) :
      Ref<char>(ramPointer, false) {}
    /// @brief Initialise from c-string in PROGMEM
    /// @param Pointer to c-string in PROGMEM
    StrRef (const __FlashStringHelper * progmemPointer) :
      Ref<char>(reinterpret_cast<const char *>(progmemPointer), false) {}
  public:
    void get(char * buffer, size_t bufferSize) {
      /// @brief Copy cstring referenced by this pointer to a
      /// specified buffer
      /// @param buffer Buffer to copy cstring to
      /// @param bufferSize Size of the buffer in chars
      if (!static_cast<boolean>(*this)) return;
      static const size_t nullTerminator = '\0';
      buffer[bufferSize - sizeof(nullTerminator)] = nullTerminator;
      if (pgm()) {
        strncpy_P(buffer, (const char *)*this, bufferSize - sizeof(nullTerminator));
        return;
      }
      strncpy(buffer, (const char *)*this, bufferSize - sizeof(nullTerminator));
    }
} __attribute__((packed));

//////////////////////////////////////////////////////////////////////
// Introspection & basic reflection
//////////////////////////////////////////////////////////////////////

#define INTROSPECTED_CLASS_TYPE_AUTO() __COUNTER__

#define TYPE_ID uint16_t

#define INTROSPECTED_SET_CLASS_TYPE(CLASS_TYPE) \
  public: \
  static const TYPE_ID classType = CLASS_TYPE

#define INTROSPECT_THIS_CLASS() classType

#define INTROSPECTED_BASE(CLASS_TYPE) \
  public: \
  INTROSPECTED_SET_CLASS_TYPE (CLASS_TYPE); \
  inline TYPE_ID getObjectType(void) const {return (objectType);} \
  protected: \
  TYPE_ID objectType = INTROSPECT_THIS_CLASS()

#define INTROSPECTED_SET_OBJECT_TYPE() objectType = INTROSPECT_THIS_CLASS()

#define INTROSPECT_CLASS(T) T::INTROSPECT_THIS_CLASS()

#define INTROSPECT_OBJECT() getObjectType()

#define VALIDATE_OBJECT_TYPE(OBJECT) ((OBJECT).INTROSPECT_THIS_CLASS() == (OBJECT).INTROSPECT_OBJECT())

#define REFLECT_OBJECT(TYPE,OBJREF) (reinterpret_cast<TYPE *>(OBJREF))

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
  /// @par Non-initialised circular buffer behaves as empty
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
  /// @par If memory was allocated by this class, it is released using delete()
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
  /// @par Non-initialised circular buffer behaves as empty
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

namespace dsp {

//////////////////////////////////////////////////////////////////////
// FixedPoint
//////////////////////////////////////////////////////////////////////

/// @brief Fixed point number with integer and fraction parts
/// @tparam T An integer base type for fixed point number.
/// @tparam FractionBits Number of least significant bits storing fraction part
/// @tparam U A wider range data type, used as intermediary to avoid overflow during calculations.
/// Ideally should be twice as wide as T. If U is not specified, T is also used as intermediary (
/// not recommended since it may lead to overflow during multiplication/division and initialisation
/// from integer with decimal point shift).
/// @tparam TRangeMin Minimum value possible for T
/// @tparam TRangeMax Maximum value possible for T
/// @details Basic arithmetic (+, -, *, /, +=, -=, *=, /=) and logic (<, <=, >, >=, !=, ==)
/// operations are possible.
/// @par If as a result of the arithmetic operation the Fixed Point value reaches
/// minimum or maximum possible values for integer part, it becames an overflow value. The
/// overflow values are capped at minimum and maximum of possible range. Any further arithmetic
/// operation with overflow value results in the same overflow value.
/// @par Overflow can be detected with overflow() function.
/// @par If both TRangeMin or TRangeMax are zero, no range check is performed and the
/// fixed-point value is not overflow-safe; overflow() always returns false.
/// @par Minimum and maximum possible values for integer part are declared as constants min and max.
/// @par Conversion to the integer type T is possible and is performed by rounding (i.e. if fraction
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
    FixedPoint(T integer, size_t decimalsPrecision = 0);
    FixedPoint(const char * str);
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
    static const T min = TMinRange  ? ((TMinRange >> FractionBits) + 1) : TMinRange; ///< Minimum range for integer part of FixedPoint value
    static const T max = TMaxRange >> FractionBits; ///< Maximum range for integer part of FixedPoint value
  public:
    /// @brief Detect overflow
    /// @par value Value to check for overflow
    /// @return true if the value of this object is overflow, false otherwise.
    /// If the minimum/maximum limits are not set, always returns false.
    inline boolean overflow(void) const {
      if (!TMinRange && !TMaxRange) return (false);
      if ((value.get() == TMinRange) || (value.get() == TMaxRange)) return (true);
      return (false);
    }
  public:
    T getValue(size_t decimalPrecision, boolean *status = nullptr) const;
  protected:
    /// @brief Internal value class, created to encapsulate the value and unify value assignment
    class Value {
      public:
        /// @brief Sets value from type T value
        /// @newValue New value of type T, consists of integer and fraction part at this point
        /// @return true if no overflow occured, false otherwise
        inline boolean setT(T newValue) {
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
    } __attribute__((packed));
    Value value;
  protected:
    static const T tZero = static_cast<T>(0); ///< 0 constant of type T
    static const T tOne = static_cast<T>(1);  ///< 1 constant of type T
    static const T fractionBitsPwr2 = tOne << FractionBits; ///< A number of type T, equals to 2 pow FractionBits
    //Make sure that FractionBits are less than total bit width of T
    static_assert(FractionBits < (sizeof(T) * 8), "Too many fraction bits");
    //Make sure that more than zero FractionBits are defined (if zero fraction bits required, simply use type T)
    static_assert(FractionBits > 0, "Too few fraction bits");
} __attribute__((packed));

template <typename T, size_t FractionBits, typename U, T TMinRange, T TMaxRange>
FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>::FixedPoint(T value, size_t decimalsPrecision) {
  /// @brief Creates a Fixed Point value from integer value with known decimal precision
  /// @details For example Pi can be defined as follows (provided that large number fits into type T):
  ///     FixedPoint(314159265, 8);
  /// @param value Integer part
  /// @param decimalsPrecision How many least significant decimal digits are fractions
  /// (= for how many digits to left-shift decimal point)
  if (!value) {
    this->value.setT(tZero);
    return;
  }
  if (decimalsPrecision) {
    static const T decimalRadix = static_cast<T>(10);
    T divider = tOne;
    for (size_t i = 0; i < decimalsPrecision; i++) {
      if (TMinRange && TMaxRange) {
        if (divider < (TMaxRange / decimalRadix)) {
          divider *= decimalRadix;
        }//if (divider < (TMaxRange / decimalRadix))
        else {
          value /= decimalRadix;
          if (!value) {
            this->value.setT(tZero);
            return;
          }
        }
      }
      else {//if (TMinRange && TMaxRange)
        divider *= decimalRadix;
      }
    }
    this->value.setU(static_cast<U>(value) * static_cast<U>(fractionBitsPwr2) / static_cast<U>(divider));
    return;
  }
  if (TMinRange && TMaxRange) {
    if (value < min) {
      this->value.setT(TMinRange);
      return;
    }
    if (value > max) {
      this->value.setT(TMaxRange);
      return;
    }
  }
  this->value.setT(value << FractionBits);
}

template <typename T, size_t FractionBits, typename U, T TMinRange, T TMaxRange>
FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>::FixedPoint(const char * str) {
  const char * currStrChar = str;
  const char decimalPointChar = '.';
  const char minusSignChar = '-';
  const char plusSignChar = '+';
  const T decimalRadixT = static_cast<T>(10);
  value.setT(tZero);
  //make sure str is not NULL and not empty
  if ((!str) || (!(*currStrChar))) return;
  //check plus & minus sign
  boolean negative = false;
  if ((*currStrChar) == minusSignChar) {
    currStrChar++;
    negative = true;
    if (TMinRange && TMaxRange) {
      if (TMinRange > tZero) return;
    }
  } else {
    if ((*currStrChar) == plusSignChar) currStrChar++;
  }
  //process integer part
  T integerPart = static_cast<U>(0);
  while ((*currStrChar) && ((*currStrChar) != decimalPointChar)) {
    if (((*currStrChar) < '0') || ((*currStrChar) > '9')) {
      value.setT(tZero);
      return;
    }
    T currentDigit = static_cast<T>((*currStrChar) - '0');
    if (negative) currentDigit = ~currentDigit + tOne;
    integerPart = integerPart * decimalRadixT + currentDigit;
    if (TMinRange && TMaxRange) {
      if (integerPart < (min + 1)) {
        value.setT(TMinRange);
        return;
      }
      if (integerPart > (max - 1)) {
        value.setT(TMaxRange);
        return;
      }
    }//if (TMinRange && TMaxRange)
    currStrChar++;
  }//while
  integerPart = integerPart << FractionBits;
  if (!(*currStrChar)) {
    value.setT(integerPart);
    return;
  }
  //set fraction part pointer to the last digit of the str
  const char * decimalPointStrChar = currStrChar;
  while (*(currStrChar + 1)) {
    currStrChar++;
  }
  //process fraction part
  T fractionPart = tZero;
  while (currStrChar != decimalPointStrChar) {
    if (((*currStrChar) < '0') || ((*currStrChar) > '9')) {
      value.setT(tZero);
      return;
    }
    T currentDigit = static_cast<T>((*currStrChar) - '0');
    fractionPart = (fractionPart + (currentDigit << FractionBits)) / decimalRadixT;
    currStrChar--;
  }
  if (negative) fractionPart = ~fractionPart + tOne;
  value.setT(integerPart + fractionPart);
}


template <typename T, size_t FractionBits, typename U, T TMinRange, T TMaxRange>
T FixedPoint<T, FractionBits, U, TMinRange, TMaxRange>::getValue(size_t decimalPrecision, boolean *status) const {
  /// @brief Converts a Fixed Point value to the base type with a specified decimal precision
  /// @details For example, result of conversion of value of 10.573 can be 10
  /// (for decimalPrecision 0), 106 (for decimalPrecision 1), 1057 (for decimalPrecision 2),
  /// 10573 (for decimalPrecision 3), 105730 (for decimalPrecision 4), etc.
  /// @param decimalPrecision How many least significant decimal digits of the returned value
  /// are fractions (= for how many digits to left-shift decimal point)
  /// @param status Pointer to boolean variable which will be set to true if conversion was
  /// successful and to false otherwise. If not needed, this parameter can be skipped or
  /// set equal to nullptr
  /// @return Conversion result or TMinRange/TMaxRange if result is out of range of type T
  /// (TMinRange...TMaxRange)
  if (status) {
    (*status) = false;
  }
  U tempValue = static_cast<U>(this->value.get());
  static const U decimalRadix = static_cast<U>(10);
  for (size_t i = 0; i < decimalPrecision; i++) {
    tempValue *= decimalRadix;
    if (TMinRange && TMaxRange) {
      if (tempValue < static_cast<U>(TMinRange)) {
        return (TMinRange);
      }
      if (tempValue > static_cast<U>(TMaxRange)) {
        return (TMaxRange);
      }
    }//if (TMinRange && TMaxRange)
  }//for
  if (status) {
    (*status) = true;
  }
  tempValue /= static_cast<U>(fractionBitsPwr2);
  return (static_cast<T>(tempValue));
}

}; //namespace dsp

//////////////////////////////////////////////////////////////////////
// Value
//////////////////////////////////////////////////////////////////////

typedef int32_t ValueBase;
static const ValueBase ValueBaseMin = INT32_MIN;
static const ValueBase ValueBaseMax = INT32_MAX;
static const size_t ValueFractionBits = 10;
typedef int64_t IntermediaryValue;

using Value = dsp::FixedPoint<ValueBase, ValueFractionBits, IntermediaryValue, ValueBaseMin, ValueBaseMax>;

static const Value ValuePi = Value(314159265, 8);

/// @brief Checks whether the valve is overflown (out of range)
/// @details This function is for compatibility only
/// @par Reserved for the case when Value is a Plain-Old-Data type (e.g. float) and does not have methods
/// @param value Value to check
/// @return True if value is overflown, false if value is a regular number
inline boolean overflow(const Value & value) {
  return (value.overflow());
}

/// @brief Returns an integer value with known decimal precision
/// @par For example for value 10.7 and precision 1 (1 digit after decimal point) will return 107
/// @par This function is for compatibility only
/// @par Reserved for the case when Value is a Plain-Old-Data type (e.g. float) and does not have methods
/// @param value Value
/// @return Converted value or zero if conversion failed
inline ValueBase getValue(const Value & value, size_t decimals, boolean *status = nullptr) {
  return (value.getValue(decimals, status));
}

//////////////////////////////////////////////////////////////////////
// Timestamp
//////////////////////////////////////////////////////////////////////

typedef uint32_t Timestamp;

inline Timestamp getTimestamp(void) {
  return (millis());
}

const Value timestampPerSecond(1000); // 1000 milliseconds per second

namespace dsp {

//////////////////////////////////////////////////////////////////////
// Filter
//////////////////////////////////////////////////////////////////////

template <typename T, typename Timestamp>
class TemplateFilter {
  public:
    enum class Status {
      NONE,                               ///< No status returned
      OK,                                 ///< Filter functions normally
      ERROR_INIT_DATA_INCORRECT,          ///< Error: incorrect data provided for filter initialisation
      ERROR_INIT_NOT_ENOUGH_MEMORY,       ///< Error: incorrect data provided for filter initialisation
      ERROR_INIT_FAILED,                  ///< Error: filter initialisation failed
      WARNING_INIT_DATA_INCORRECT,        ///< Warning: incorrect data provided for filter initialisation; filter may produce unexpected results
      WARNING_TOO_MANY_INPUTS,            ///< Warning: too many inputs provided for the filter; some inputs were ignored
      ERROR_TOO_FEW_INPUTS,               ///< Error: too few inputs provided for the filter
    };
  public:
    virtual ~TemplateFilter() {};
    inline const T& filter(Timestamp timestamp = 0);
    template <typename... Inputs> const T& filter(Timestamp timestamp, const Inputs... inputs);
    Status getStatus(void);
  protected:
    virtual Status filterProcess(const T *inputs, size_t inputsNumber, T &output, Timestamp timestamp) = 0;
  protected:
    Timestamp getDeltaTime(Timestamp currentTime);
  protected:
    void setInitStatus(Status initStatus);
    void setInputsNumber(size_t min, size_t max);
  private:
    Status status = Status::NONE;
  private:
    T outputValue = static_cast<T>(0);
    Timestamp lastTime = static_cast<Timestamp>(0);
  private:
    size_t minInputs = 0;
    size_t maxInputs = 0;
};

template <typename T, typename Timestamp>
const T& TemplateFilter<T, Timestamp>::filter(Timestamp timestamp) {
  outputValue = static_cast<T>(0);
  if ((status == Status::ERROR_INIT_DATA_INCORRECT) ||
      (status == Status::ERROR_INIT_NOT_ENOUGH_MEMORY) ||
      (status == Status::ERROR_INIT_FAILED)) {
    lastTime = timestamp;
    return (outputValue);
  }
  if (minInputs) {
    status = Status::ERROR_TOO_FEW_INPUTS;
    lastTime = timestamp;
    return (outputValue);
  }
  status = filterProcess(nullptr, 0, outputValue, timestamp);
  lastTime = timestamp;
  return (outputValue);
}

template <typename T, typename Timestamp>
template <typename... Inputs> const T& TemplateFilter<T, Timestamp>::filter(Timestamp timestamp, const Inputs... inputs) {
  T inputsArray[] = { T(inputs)... };
  outputValue = static_cast<T>(0);
  if ((status == Status::ERROR_INIT_DATA_INCORRECT) ||
      (status == Status::ERROR_INIT_NOT_ENOUGH_MEMORY) ||
      (status == Status::ERROR_INIT_FAILED)) {
    lastTime = timestamp;
    return (outputValue);
  }
  if (minInputs && maxInputs && (sizeof...(inputs) < minInputs)) {
    status = Status::ERROR_TOO_FEW_INPUTS;
    lastTime = timestamp;
    return (outputValue);
  }
  status = filterProcess(inputsArray, sizeof...(inputs), outputValue, timestamp);
  if ((status == Status::OK) && minInputs && maxInputs && (sizeof...(inputs) > maxInputs)) {
    status = Status::WARNING_TOO_MANY_INPUTS;
  }
  lastTime = timestamp;
  return (outputValue);
}

template <typename T, typename Timestamp>
Timestamp TemplateFilter<T, Timestamp>::getDeltaTime(Timestamp currentTime) {
  if (!lastTime) return (static_cast<Timestamp>(0));
  return (currentTime - lastTime);
}

template <typename T, typename Timestamp>
typename TemplateFilter<T, Timestamp>::Status TemplateFilter<T, Timestamp>::getStatus(void) {
  return (status);
}

template <typename T, typename Timestamp>
void TemplateFilter<T, Timestamp>::setInitStatus(Status initStatus) {
  status = initStatus;
}

template <typename T, typename Timestamp>
void TemplateFilter<T, Timestamp>::setInputsNumber(size_t min, size_t max) {
  if (min > max) return;
  minInputs = min;
  maxInputs = max;
}


//////////////////////////////////////////////////////////////////////
// MovingAverage
//////////////////////////////////////////////////////////////////////

template <typename T, typename Timestamp>
class MovingAverage : public TemplateFilter<T, Timestamp> {
  public:
    MovingAverage(size_t numValues);
    virtual typename TemplateFilter<T, Timestamp>::Status filterProcess(const T *inputs, size_t inputsNumber, T &output, Timestamp timestamp);
    virtual ~MovingAverage() {}
  private:
    arrays::RingBuffer<T> ringBuffer;
    T lastValue = static_cast<T>(0);
};

template <typename T, typename Timestamp>
MovingAverage<T, Timestamp>::MovingAverage(size_t numValues) : ringBuffer(numValues) {
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  if (!ringBuffer.validate()) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_NOT_ENOUGH_MEMORY);
    return;
  }
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
typename TemplateFilter<T, Timestamp>::Status MovingAverage<T, Timestamp>::filterProcess(const T * inputs, size_t inputsNumber, T & output, Timestamp timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (TemplateFilter<T, Timestamp>::Status::ERROR_TOO_FEW_INPUTS);
  ringBuffer.push(inputs[0]);
  T total = static_cast<T>(0);
  T subtotal = static_cast<T>(0);
  for (int i = 0; i < ringBuffer.count(); i++) {
    T backupSubtotal = subtotal;
    subtotal += ringBuffer[i];
    if (overflow(subtotal)) {
      //When overflow occurs we divide part of the sum (subtotal) by amount of items and add it to total (precision is lost but overflow avoided)
      total += (backupSubtotal / static_cast<T>(ringBuffer.count()));
      subtotal = ringBuffer[i];
    }
  }
  if (ringBuffer.count()) {
    total += (subtotal / static_cast<T>(ringBuffer.count())); //if there was no overflow, then total is still zero at this point
  }
  output = total;
  return (TemplateFilter<T, Timestamp>::Status::OK);
};

//////////////////////////////////////////////////////////////////////
// LowPass
//////////////////////////////////////////////////////////////////////

template <typename T, typename Timestamp>
class LowPass : public TemplateFilter<T, Timestamp> {
  public:
    LowPass(const T &fc, const T &pi, const T & fcDivider = static_cast<T>(1));
    virtual typename TemplateFilter<T, Timestamp>::Status filterProcess(const T *inputs, size_t inputsNumber, T &output, Timestamp timestamp);
    virtual ~LowPass() {}
  private:
    T fc = static_cast<T>(0);
    T fcDivider = static_cast<T>(1);
    T fcPi2 = static_cast<T>(0);
    T lastOutput = static_cast<T>(0);
};

template <typename T, typename Timestamp>
LowPass<T, Timestamp>::LowPass(const T & fc, const T & pi, const T & fcDivider) {
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  if ((fc <= static_cast<T>(0)) || (fcDivider <= static_cast<T>(0)) || overflow (fc / fcDivider)) {
    this->fc = static_cast<T>(0);
    this->fcDivider = static_cast<T>(1);
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  this->fc = fc;
  this->fcDivider = fcDivider;
  fcPi2 = fc * pi * static_cast<T>(2);
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
typename TemplateFilter<T, Timestamp>::Status LowPass<T, Timestamp>::filterProcess(const T * inputs, size_t inputsNumber, T & output, Timestamp timestamp) {

  Timestamp deltaTime (this->getDeltaTime(timestamp));
  if (!inputsNumber) {
    return (TemplateFilter<T, Timestamp>::Status::ERROR_TOO_FEW_INPUTS);
  }
  if (!deltaTime) {
    output = static_cast<T>(inputs[0]);
    lastOutput = static_cast<T>(inputs[0]);
  }
  else {
    T dt = static_cast<T>(deltaTime);
    output = lastOutput + (dt * fcPi2 * (inputs[0] - lastOutput)) / (dt * fcPi2 / fcDivider + static_cast<T>(1)) / fcDivider;
    lastOutput = output;
  }
  return (TemplateFilter<T, Timestamp>::Status::OK);
}

//////////////////////////////////////////////////////////////////////
// LinearScale
//////////////////////////////////////////////////////////////////////

template <typename T, typename Timestamp>
class LinearScale : public TemplateFilter<T, Timestamp> {
  public:
    LinearScale(const T &x1, const T &y1, const T &x2, const T &y2);
    LinearScale(const T &a, const T &b);
    LinearScale(const T &b);
    virtual typename TemplateFilter<T, Timestamp>::Status filterProcess(const T *inputs, size_t inputsNumber, T &output, Timestamp timestamp);
  private:
    T a = static_cast<T>(1);
    T b = static_cast<T>(0);
};

template <typename T, typename Timestamp>
LinearScale<T, Timestamp>::LinearScale(const T & x1, const T & y1, const T & x2, const T & y2) {
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  if (x1 == x2) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  a = (y1 - y2) / (x1 - x2);
  b = y1 - a * x1;
  if (overflow(a) || overflow(b) || (a == static_cast<T>(0))) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_FAILED);
    return;
  }
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
LinearScale<T, Timestamp>::LinearScale(const T & a, const T & b) {
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  this->a = a;
  this->b = b;
  if (overflow(a) || overflow(b)) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
LinearScale<T, Timestamp>::LinearScale(const T & b) {
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  this->a = static_cast<T>(1);
  this->b = b;
  if (overflow(b)) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
typename TemplateFilter<T, Timestamp>::Status LinearScale<T, Timestamp>::filterProcess(const T * inputs, size_t inputsNumber, T & output, Timestamp timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (TemplateFilter<T, Timestamp>::Status::ERROR_TOO_FEW_INPUTS);
  output = a * inputs[0] + b;
  return (TemplateFilter<T, Timestamp>::Status::OK);
}

//////////////////////////////////////////////////////////////////////
// SquareScale
//////////////////////////////////////////////////////////////////////

template <typename T, typename Timestamp>
class SquareScale : public TemplateFilter<T, Timestamp> {
  public:
    SquareScale(const T &x1, const T &y1,
                const T &x2, const T &y2,
                const T &x3, const T &y3);
    SquareScale(const T &a, const T &b, const T&c);
    virtual typename TemplateFilter<T, Timestamp>::Status filterProcess(const T *inputs, size_t inputsNumber, T &output, Timestamp timestamp);
  private:
    T a = static_cast<T>(0);
    T b = static_cast<T>(1);
    T c = static_cast<T>(0);
    T h = static_cast<T>(0);
};

template <typename T, typename Timestamp>
SquareScale<T, Timestamp>::SquareScale(
  const T & x1, const T & y1,
  const T & x2, const T & y2,
  const T & x3, const T & y3) {
  //Set input number
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  //Calculate square function a, b, c coefficients
  const T denominator1 = (x1 - x2);
  const T denominator2 = (x1 - x3);
  const T denominator3 = (x2 - x3);
  if ((denominator1 == static_cast<T>(0)) ||
      (denominator2 == static_cast<T>(0)) ||
      (denominator3 == static_cast<T>(0))) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_DATA_INCORRECT);
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
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_FAILED);
    return;
  }
  //Check parabola vertex position
  if (a == static_cast<T>(0)) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
    return;
  }
  h = b / a / static_cast<T>(-2);
  //k = c - h * h;
  if (overflow(h)) return;
  T xmin = x1;
  if (x2 < xmin) xmin = x2;
  if (x3 < xmin) xmin = x3;
  T xmax = x3;
  if (x2 > xmax) xmax = x2;
  if (x1 > xmax) xmax = x1;
  if ((h > xmin) && (h < xmax)) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::WARNING_INIT_DATA_INCORRECT);
    return;
  }
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
SquareScale<T, Timestamp>::SquareScale(const T & a, const T & b, const T & c) {
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  this->a = a;
  this->b = b;
  this->c = c;
  if (overflow(a) || overflow(b) || overflow (c)) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_DATA_INCORRECT);
    return;
  }
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
typename TemplateFilter<T, Timestamp>::Status SquareScale<T, Timestamp>::filterProcess(const T * inputs, size_t inputsNumber, T & output, Timestamp timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (TemplateFilter<T, Timestamp>::Status::ERROR_TOO_FEW_INPUTS);
  output = a * inputs[0] * inputs[0] + b * inputs[0] + c;
  return (TemplateFilter<T, Timestamp>::Status::OK);
}

//////////////////////////////////////////////////////////////////////
// SplineScale
//////////////////////////////////////////////////////////////////////

template <typename T, typename Timestamp>
class SplineScale : public TemplateFilter<T, Timestamp> {
  public:
    SplineScale(const T &x1, const T &y1,
                const T &x2, const T &y2,
                const T &x3, const T &y3);
    virtual typename TemplateFilter<T, Timestamp>::Status filterProcess(const T *inputs, size_t inputsNumber, T &output, Timestamp timestamp);
  private:
    T a1 = static_cast<T>(0);
    T b1 = static_cast<T>(1);
    T a2 = static_cast<T>(0);
    T b2 = static_cast<T>(1);
    T x1 = static_cast<T>(0);
    T y1 = static_cast<T>(0);
    T x2 = static_cast<T>(0);
    T y2 = static_cast<T>(0);
    T x3 = static_cast<T>(0);
    T y3 = static_cast<T>(0);
};

template <typename T, typename Timestamp>
SplineScale<T, Timestamp>::SplineScale(
  const T & xMin, const T & yMin,
  const T & xMid, const T & yMid,
  const T & xMax, const T & yMax) {
  //Set input number
  static const size_t inputNumber = 1;
  TemplateFilter<T, Timestamp>::setInputsNumber(inputNumber, inputNumber);
  //Initialise points
  x1 = xMin;
  y1 = yMin;
  x2 = xMid;
  y2 = yMid;
  x3 = xMax;
  y3 = yMax;
  //Sort points by x in accending order
  struct TSwapHelper {
    inline static void swap(T &a, T &b) {
      T temp = a;
      a = b;
      b = temp;
    }
  };
  if (x1 > x2) {
    TSwapHelper::swap(x1, x2);
    TSwapHelper::swap(y1, y2);
  }
  if (x2 > x3) {
    TSwapHelper::swap(x2, x3);
    TSwapHelper::swap(y2, y3);
  }
  if (x1 > x2) {
    TSwapHelper::swap(x1, x2);
    TSwapHelper::swap(y1, y2);
  }

  //Calculate cubic spline coefficients
  const T denominator1 = 2 * (x1 - x2);
  const T denominator2 = x1 - x3;
  const T denominator3 = x2 - x3;
  if ((denominator1 == static_cast<T>(0)) ||
      (denominator2 == static_cast<T>(0)) ||
      (denominator3 == static_cast<T>(0))) {
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_DATA_INCORRECT);
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
    TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::ERROR_INIT_FAILED);
    return;
  }
  TemplateFilter<T, Timestamp>::setInitStatus(TemplateFilter<T, Timestamp>::Status::OK);
}

template <typename T, typename Timestamp>
typename TemplateFilter<T, Timestamp>::Status SplineScale<T, Timestamp>::filterProcess(const T * inputs, size_t inputsNumber, T & output, Timestamp timestamp) {
  (void)timestamp;
  if (!inputsNumber) return (TemplateFilter<T, Timestamp>::Status::ERROR_TOO_FEW_INPUTS);
  output = (inputs[0] < x2) ?
           (a1 * (inputs[0] - x1) * (inputs[0] - x1) * (inputs[0] - x1) + b1 * (inputs[0] - x1) + y1) :
           (a2 * (inputs[0] - x3) * (inputs[0] - x3) * (inputs[0] - x3) + b2 * (inputs[0] - x3) + y3);
  return (TemplateFilter<T, Timestamp>::Status::OK);
}

//////////////////////////////////////////////////////////////////////
// Filters
//////////////////////////////////////////////////////////////////////

using Filter = TemplateFilter<Value, Timestamp>;
using FilterMovingAverage = MovingAverage<Value, Timestamp>;
using FilterLowPass = LowPass<Value, Timestamp>;
using FilterLinearScale = LinearScale<Value, Timestamp>;
using FilterSquareScale = SquareScale<Value, Timestamp>;
using FilterSplineScale = SplineScale<Value, Timestamp>;

enum class FilterType {
  MOVING_AVERAGE,
  LOW_PASS,
  LINEAR_SCALE,
  SQUARE_SCALE
};

}; //namespace dsp

//////////////////////////////////////////////////////////////////////
// Physical Quantities (temperature, humidity, pressure, luminance, etc)
//////////////////////////////////////////////////////////////////////

namespace quantity {

//////////////////////////////////////////////////////////////////////
// Quantity
//////////////////////////////////////////////////////////////////////

/// @brief A templated/configurable class representing physical quantity, such as temperature,
/// humidity, pressure, luminance, etc.
/// @details Every Quantity object has the following properties:
///
/// * WHAT does the quantity represent?
///
/// * WHEN the quantity was measured?
///
/// * HOW MUCH is the numerical measurement result?
///
/// * IN WHICH UNITS the measurement result is represented?
///
/// * IS the measurement result valid?
///
/// * IN WHICH RANGE can measurement results possibly be?
///
/// @par For example consider the following physical quantity:
/// @par     "outdoors temperature (id = 12) is 10.25 degrees Celsius as measured at 15381
/// milliseconds from startup, measured in range -50 to 125 degrees Celsius"
/// @par It will be represented as follows:
/// @par     "outdoors temperature (identifier 12, group 3)":
/// @par     id = 12
/// @par     idGroup = 3
/// @par     "measured at 15381 milliseconds from startup"
/// @par     timestamp = 15381
/// @par     "the measurement result was 10.25"
/// @par     value = 10.25
/// @par     "the result is in degrees Celsius"
/// @par     unitText = "C"
/// @par     "the measurement result is valid (no error occured while getting data
/// from sensor)"
/// @par     valid = true
/// @par     "measured in range -50 to 125 degrees Celsius"
/// @par     minRange = -50
/// @par     maxRange = 125
/// Acceptable range for the value is optional and can be omitted. If the range is specified,
/// the values outside of this range will be clamped to high or low limit.
/// @par This is the basic class, intended to be inherited by concrete physical quantity
/// classes. It was designed not to make use of virtual methods. The descendant classes
/// are able to set some of the fields (via setters).
/// @par The quantity can be printed or passed to external systems even without knowing its
/// concrete type, by acquiring the field values via getters.
/// @par The object of this class is intended to be initialised only by constructor
/// (protected and reserved for use by descendant classes) or by copy constructor from another
/// quantity. Once initialised, the quantity's information cannot be modified. The numeric
/// value cannot be modified as well but it can be converted to a different measurement unit.
/// The conversion is handled by descendant classes.
/// @par An identifier and identifier group define the meaning of the quantity (location, purpose,
/// device, etc...). Identifier and identifier group are only stored in this class and are not
/// processed or checked by its methods. The identifying scheme, uniquenes and other rules are
/// defined and enforced by the object's creator.
/// @par A validate() method can be used to evaluate whether the numeric value can be considered
/// as a valid one.
/// @par The value and measurment unit used during the initialisation is stored in the object
/// (initValue and initUnit). When the conversion to other measurment unit is performed, the
/// setValue and setUnit fields are modified via setter methods. This modification is to be
/// handled in descendant classes. Obtaining value and unit of the quantity via getter methods
/// will return setValue and setUnit.
/// @par The class has introspection mechanism allowing to identify concrete type of object,
/// even if the object is represented as a pointer to base class type.

class Quantity {
  public:
    typedef Value value_t;
    typedef Timestamp timestamp_t;
    typedef uint8_t unit_t;
    typedef uint16_t id_t;
    typedef StrRef text_t;
  protected:
    inline Quantity(id_t id,
                    id_t idGroup,
                    value_t value,
                    unit_t unit,
                    text_t unitText,
                    timestamp_t timestamp = {},
                    boolean valid = true,
                    value_t minRange = value_t(0),
                    value_t maxRange = value_t(0));
  public:
    inline boolean validate(void) const {
      /// @brief Checks whether the object was initialised
      /// @return true if the value is valid, false otherwise
      return (valid);
    }
  public:
    inline id_t getId(void) const {
      /// @brief Returns physical quantity identifier (location, sensor ID, etc...)
      /// @returns Physical quantity identifier in numeric form
      return (id);
    }
    inline id_t getIdGroup(void) const {
      /// @brief Returns physical quantity identifier group
      /// @returns Physical quantity group identifier in numeric form
      return (idGroup);
    }
    inline timestamp_t getTimestamp(void) const {
      /// @brief Returns timestamp representing the moment of measurement of
      /// the physical quantity
      /// @returns Timestamp for measurement moment
      return (timestamp);
    }
    inline value_t getValue(void) const {
      /// @return Physical quantity numeric value
      return (setUnitValue);
    }
    inline text_t PROGMEM getUnitText(void) const {
      /// @return C-string (in PROGMEM) with physical quantity measurement unit in
      /// human-readable form or nullptr if human-readable form was not provided
      return (setUnitText);
    }
    inline value_t getMinRange(void) {
      /// @brief Returns minimum range for this quantity (if set)
      /// @return Minimum range or zero if not set
      return (setUnitMinRange);
    }
    inline value_t getMaxRange(void) {
      /// @brief Returns minimum range for this quantity (if set)
      /// @return Minimum range or zero if not set
      return (setUnitMaxRange);
    }
  protected:
    boolean setConvertedValue(value_t value,
                              unit_t units,
                              text_t unitText = {},
                              value_t minRange = value_t(0),
                              value_t maxRange = value_t(0));
  protected:
    inline value_t getInitValue(void) const {
      /// @return Value used to initialise the object
      return (initValue);
    }
    inline unit_t getInitUnit(void) const {
      /// @return Measurement unit in numeric form used along with
      /// initValue to initialise the object
      return (initUnit);
    }
    inline value_t getInitMinRange(void) const {
      /// @return Minimum range at initialisation (or 0 if not defined)
      return (initMinRange);
    }
    inline value_t getInitMaxRange(void) const {
      /// @return Maximum range at initialisation (or 0 if not defined)
      return (initMaxRange);
    }
  private:
    //Essential components of the quantity
    //As an example the following physical quantity will be used:
    //"outdoors temperature (id=12) is 10.25 degrees Celsius as measured at 15381 milliseconds from startup"
    //WHAT does the quantity represent, e.g. "outdoors temperature, id=12, group=3"
    id_t id {};                         ///< Identifier
    id_t idGroup {};                    ///< Identifier group
    //WHEN the quantity was measured, e.g. "at 15381 milliseconds from startup"
    timestamp_t timestamp {};           ///< Timestamp at the moment of measurement
    //HOW MUCH is the numerical measurement result, e.g. "10.25"
    value_t initValue {};               ///< Value when initialised, expressed in initUnits
    value_t setUnitValue {};            ///< initValue converted from initUnits to setUnits
    //In WHICH UNITS the measurement result is represented, e.g. in degrees Celsius
    unit_t initUnit {};                 ///< measurment unit when initialised
    unit_t setUnit {};                  ///< measurment unit in which setUnitValue is expressed
    text_t setUnitText;                 ///< setUnit in human-readable form
    //IS the numerical result valid (true/false)
    boolean valid = false;              ///< if false, the value of setUnitValue must be disregarded
    //IN WHICH RANGE can measurement results possibly be
    value_t initMinRange {};            ///< minimum range in initUnit units
    value_t initMaxRange {};            ///< maximum range in initUnit units
    value_t setUnitMinRange {};        ///< minimum range in setUnit units
    value_t setUnitMaxRange {};        ///< maximum range in setUnit units
  private:
    //Introspection
    INTROSPECTED_BASE(INTROSPECTED_CLASS_TYPE_AUTO());
} __attribute__((packed));

Quantity::Quantity(id_t id,
                   id_t idGroup,
                   value_t value,
                   unit_t unit,
                   text_t unitText,
                   timestamp_t timestamp,
                   boolean valid,
                   value_t minRange,
                   value_t maxRange) {
  /// @brief Initialises object by setting its data
  /// @param id Identifier (designation) of the physical quantity (location, sensor ID, etc...)
  /// @param idGroup Group to which id belongs
  /// @param value A numerical value of the physical quantity
  /// @param unit Identifier of the measurement unit in which value is represented
  /// @param unitText Human-readable form of unit parameter
  /// @param timestamp Represents the moment of time when the physical quantity was measured (if
  /// available)
  /// @param valid False if value is invalid (e.g. error occured during measurement), true otherwise
  /// @param minRange Minimum possible value
  /// @param minRange Maximum possible value
  /// @details This constructor is reserved for use by descendant classes
  INTROSPECTED_SET_OBJECT_TYPE();
  this->id = id;
  this->idGroup = idGroup;
  this->timestamp = timestamp;
  this->initValue = value;
  this->initUnit = unit;
  this->setUnit = unit;
  this->setUnitText = unitText;
  this->valid = valid;
  this->initMinRange = minRange;
  this->initMaxRange = maxRange;
  this->setUnitMinRange = this->initMinRange;
  this->setUnitMaxRange = this->initMaxRange;
  if ((this->initMinRange != value_t(0)) || (this->initMaxRange != value_t(0))) {
    if (this->initValue < this->initMinRange) this->initValue = minRange;
    if (this->initValue > this->initMaxRange) this->initValue = maxRange;
  }
  this->setUnitValue = initValue;
}

//////////////////////////////////////////////////////////////////////
// Generic Quantity
//////////////////////////////////////////////////////////////////////

/// @brief Generic physical quantity
/// @details Used for physical quantities for which no concrete class exists or
/// when the nature of quantity is defined by user and cannot be known at
/// compile time.
/// @par No conversion is possible.
/// @par Measurement unit is defined in human-readable form only.
/// @par Generic physical quantity value and measurment unit are always returned
/// as they were set in constructor
class Generic : public Quantity {
  public:
    ///@brief Measurement unit: only GENERIC is avaiable
    enum class Unit : unit_t {
      GENERIC,
    };
    INTROSPECTED_SET_CLASS_TYPE(INTROSPECTED_CLASS_TYPE_AUTO());
  public:
    inline Generic (id_t id,
                    id_t idGroup,
                    value_t value,
                    text_t genericUnitText,
                    timestamp_t timestamp = {},
                    boolean valid = true,
                    value_t minRange = {},
                    value_t maxRange = {})
      : Quantity(id,
                 idGroup,
                 value,
                 static_cast<unit_t>(Unit::GENERIC),
                 genericUnitText,
                 timestamp,
                 valid,
                 minRange,
                 maxRange)
    {
      INTROSPECTED_SET_OBJECT_TYPE();
    }
};

//////////////////////////////////////////////////////////////////////
// Dimensionless Quantity
//////////////////////////////////////////////////////////////////////

/// @brief Dimensionless physical quantities
/// @details Used for dimensionless quantities such as ratios, factors,
/// percentage, etc.
/// @par The avaialble measurment units are NONE and PERCENT
/// @par Unit NONE used for truly dimensionless qunatity such as factor
/// @par Unit PERCENT used for percentages
/// @par Conversion from NONE to PERCENT multiplies the value by 100%
class Dimensionless : public Quantity {
  public:
    enum class Unit : unit_t {
      NONE,     ///< Used for truly dimensionless qunatity such as factor
      PERCENT,  ///< Used for percentages
    };
  public:
    INTROSPECTED_SET_CLASS_TYPE(INTROSPECTED_CLASS_TYPE_AUTO());
  public:
    inline Dimensionless (id_t id,
                          id_t idGroup,
                          value_t value,
                          Unit unit,
                          timestamp_t timestamp = {},
                          boolean valid = true,
                          value_t minRange = {},
                          value_t maxRange = {})
      : Quantity(id,
                 idGroup,
                 value,
                 static_cast<unit_t>(unit),
                 getUnitTextByUnit(unit),
                 timestamp,
                 valid,
                 minRange,
                 maxRange)
    {
      INTROSPECTED_SET_OBJECT_TYPE();
    }
  public:
    boolean convertToUnit(Dimensionless::Unit unit);
    static text_t getUnitTextByUnit(Dimensionless::Unit unit);
};


//////////////////////////////////////////////////////////////////////
// Temperature
//////////////////////////////////////////////////////////////////////

class Temperature : public Quantity {
    /// @brief Temperature as physical quantity
    /// @details The avaialble measurment units are CELSIUS and FAHRENHEIT,
    /// representing degrees Celsius and degrees Fahrenheit respectively
  public:
    enum class Unit : unit_t {
      CELSIUS,      ///< Degrees Celsius
      FAHRENHEIT,   ///< Degrees Fahrenheit
    };
  public:
    INTROSPECTED_SET_CLASS_TYPE(INTROSPECTED_CLASS_TYPE_AUTO());
  public:
    inline Temperature (id_t id,
                        id_t idGroup,
                        value_t value,
                        Unit unit,
                        timestamp_t timestamp = {},
                        boolean valid = true,
                        value_t minRange = {},
                        value_t maxRange = {})
      : Quantity(id,
                 idGroup,
                 value,
                 static_cast<unit_t>(unit),
                 getUnitTextByUnit(unit),
                 timestamp,
                 valid,
                 minRange,
                 maxRange)
    {
      INTROSPECTED_SET_OBJECT_TYPE();
    }
  public:
    boolean convertToUnit(Temperature::Unit unit);
    static text_t getUnitTextByUnit(Temperature::Unit unit);
  private:
    static value_t celsiusToFahrenheit(value_t celsiusValue);
    static value_t fahrenheitToCelsius(value_t fahrenheitValue);
};

}; //namespace quantity

}; //namespace util

#endif
