/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef STRINGMAP_H
#define STRINGMAP_H

#include <Arduino.h>

typedef int StringMapKey;

#define STRINGMAP_ITEM_DEFAULT 0

class StringMap {
  protected:
    StringMap() {}
  public:
    StringMapKey find(const char * toFind);
    char * find(StringMapKey toFind);
    boolean isProgmem (StringMapKey toFind);
    int compare (size_t index, const char * string);
    void print(Print &client, StringMapKey key);
  protected:
    virtual size_t count(void) = 0;
    virtual char * getString(size_t index) = 0;
    virtual boolean isStringProgmem(size_t index) = 0;
    virtual StringMapKey getKey(size_t index) = 0;
};

#define QUICK_STRINGMAP_STRING_SIZE 30

struct QuickStringMapItem {
  StringMapKey key;
  char string[QUICK_STRINGMAP_STRING_SIZE];
};

class QuickStringMap : public StringMap {
  public:
    QuickStringMap(const QuickStringMapItem source[]);
  protected:
    virtual size_t count(void);
    virtual char * getString(size_t index);
    virtual boolean isStringProgmem(size_t index);
    virtual StringMapKey getKey(size_t index);
  private:
    const QuickStringMapItem * source = NULL;
    size_t itemsCount = 0;
};

#endif
