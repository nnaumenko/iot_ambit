/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef STRINGMAP_H
#define STRINGMAP_H

#include <Arduino.h>

//the example of the strings parameter is as follows:
//const char PROGMEM exampleStrings[] = "someString\0" "someOtherString\0"

class StringMapLite {
  public:
    static int find (const char * strings, const char * toFind);
    static const char * find (const char * strings, int index);
    static size_t count(const char * strings);
  public:
    static const int NOT_FOUND = -1;
};

typedef long StringMapKey;

class StringMap {
    friend class StringMapIterator;
  protected:
    StringMap() {}
  public:
    StringMapKey find(const char * toFind) const;
    char * find(StringMapKey toFind) const;
    boolean contains(StringMapKey toFind) const;
    boolean contains(const char * toFind) const;
    boolean isProgmem(StringMapKey toFind) const;
    void print(Print &client, StringMapKey key) const;
  public:
    StringMapKey getDefaultKey(void) const;
  protected:
    void setDefaultKey(StringMapKey defaultKey);
  private:
    StringMapKey defaultKey = 0; //returned when key or string is not found in the StringMap
  protected:
    int compare (size_t index, const char * string) const;
    virtual size_t count(void) const = 0;
    virtual char * getString(size_t index) const = 0;
    virtual boolean isStringProgmem(size_t index) const = 0;
    virtual StringMapKey getKey(size_t index) const = 0;
};

class ConstStringMap : public StringMap {
  public:
    virtual size_t count(void) const;
  private:
    size_t itemsCount = 0;
  protected:
    virtual boolean isStringProgmem(size_t index) const;
  protected:
    void initItems(void);
};

#define QUICK_STRINGMAP_STRING_SIZE 30

struct QuickStringMapItem {
  StringMapKey key;
  char string[QUICK_STRINGMAP_STRING_SIZE];
};

class QuickStringMap : public ConstStringMap {
  public:
    QuickStringMap(const QuickStringMapItem source[]);
  protected:
    virtual char * getString(size_t index) const;
    virtual StringMapKey getKey(size_t index) const;
  private:
    const QuickStringMapItem * source = NULL;
};

struct ProgmemStringMapItem {
  StringMapKey key;
  const char *string;
};

class ProgmemStringMap : public ConstStringMap {
  public:
    ProgmemStringMap(const ProgmemStringMapItem source[]);
  protected:
    virtual char * getString(size_t index) const;
    virtual StringMapKey getKey(size_t index) const;
  private:
    const ProgmemStringMapItem * source = NULL;
};

class StringMapIterator {
  public:
    StringMapIterator (const StringMap &source);
  private:
    const StringMap * source;
  public:
    void first(void);
    void next(void);
    boolean isDone(void);
    StringMapKey currentKey(void);
    boolean isCurrentStringProgmem(void);
    char * currentString(void);
  private:
    size_t iteratorIndex = 0;
};

#endif
