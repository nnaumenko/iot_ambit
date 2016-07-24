/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "stringmap.h"

//////////////////////////////////////////////////////////////////////
// StringMap
//////////////////////////////////////////////////////////////////////

char * StringMap::find(StringMapKey toFind) const {
  for (int i = 0; i < this->count(); i++)
    if (this->getKey(i) == toFind) return (this->getString(i));
  return (NULL);
}

StringMapKey StringMap::find(const char * toFind) const {
  for (int i = 0; i < this->count(); i++)
    if (!this->compare(i, toFind)) return (this->getKey(i));
  return (this->getDefaultKey());
}

boolean StringMap::contains(StringMapKey toFind) const {
  for (int i = 0; i < this->count(); i++)
    if (this->getKey(i) == toFind) return (true);
  return (false);
}

boolean StringMap::contains(const char * toFind) const {
  for (int i = 0; i < this->count(); i++)
    if (!this->compare(i, toFind)) return (true);
  return (false);
}

boolean StringMap::isProgmem (StringMapKey toFind) const {
  for (int i = 0; i < this->count(); i++)
    if (this->getKey(i) == toFind) return (this->isStringProgmem(i));
  return (false);
}

int StringMap::compare (size_t index, const char * toCompare) const {
  if (index >= this->count()) return (1); //arbitrary non-zero number to make sure comparison result is not match
  if (isStringProgmem(index))
    return (strcmp_P(toCompare, getString(index)));
  else
    return (strcmp(toCompare, getString(index)));
}

void StringMap::print(Print &client, StringMapKey key) const {
  if (!this->contains(key)) return;
  char * s = this->find(key);
  if (s == NULL) return;
  if (isProgmem(key))
    client.print((__FlashStringHelper *)s);
  else
    client.print(s);
}

StringMapKey StringMap::getDefaultKey(void) const {
  return (this->defaultKey);
}

void StringMap::setDefaultKey(StringMapKey defaultKey) {
  this->defaultKey = defaultKey;
}


//////////////////////////////////////////////////////////////////////
// ConstStringMap
//////////////////////////////////////////////////////////////////////

size_t ConstStringMap::count(void) const {
  return (this->itemsCount);
}

boolean ConstStringMap::isStringProgmem(size_t index) const {
  if (index >= this->count()) return (false);
  return (true);
}

void ConstStringMap::initItems(void) {
  size_t i = 0;
  this->itemsCount = 65535;//since actual item count at this point is not known yet, temporarily set itemsCount to max possible value, so that range check in getString/getKey never fails
  while ((this->getString(i) != NULL) &&
         (this->isStringProgmem(i) ? strlen_P(this->getString(i)) : strlen(this->getString(i))))
  {
    i++;
  }
  this->setDefaultKey(this->getKey(i));
  this->itemsCount = i;
}

//////////////////////////////////////////////////////////////////////
// QuickStringMap
//////////////////////////////////////////////////////////////////////

QuickStringMap::QuickStringMap(const QuickStringMapItem source[]) {
  this->source = source;
  this->initItems();
}

char * QuickStringMap::getString(size_t index) const {
  if (index >= this->count()) return (NULL);
  return ((char *)source[index].string);
}

StringMapKey QuickStringMap::getKey(size_t index) const {
  if (index >= this->count()) return (this->getDefaultKey());
  return ((StringMapKey)pgm_read_dword(&this->source[index].key)); //assumed StringMapKey size is dword
}

static_assert (sizeof(StringMapKey) == 4, "StringMapKey is not dword, QuickStringMap class might work incorrectly.");

//////////////////////////////////////////////////////////////////////
// ProgmemStringMap
//////////////////////////////////////////////////////////////////////

ProgmemStringMap::ProgmemStringMap(const ProgmemStringMapItem source[]) {
  this->source = source;
  this->initItems();
}

char * ProgmemStringMap::getString(size_t index) const {
  if (index >= this->count()) return (NULL);
  return ((char *)source[index].string);
}

StringMapKey ProgmemStringMap::getKey(size_t index) const {
  if (index >= this->count()) return (this->getDefaultKey());
  return ((StringMapKey)pgm_read_dword(&this->source[index].key)); //assumed StringMapKey size is dword
}
static_assert (sizeof(StringMapKey) == 4, "StringMapKey is not dword, ProgmemStringMap class might work incorrectly.");

//////////////////////////////////////////////////////////////////////
// StringMapIterator
//////////////////////////////////////////////////////////////////////

StringMapIterator::StringMapIterator (const StringMap &source) {
  this->iteratorIndex = 0;
  this->source = &source;
}

void StringMapIterator::first(void) {
  this->iteratorIndex = 0;
}

void StringMapIterator::next(void) {
  if (this->source == NULL) return;
  if (this->iteratorIndex < this->source->count()) this->iteratorIndex++;
}

boolean StringMapIterator::isDone(void) {
  if (this->source == NULL) return (true);
  return (this->iteratorIndex >= this->source->count());
}

StringMapKey StringMapIterator::currentKey(void) {
  if (this->source == NULL) return (this->source->getDefaultKey());
  return (this->source->getKey(this->iteratorIndex));
}

boolean StringMapIterator::isCurrentStringProgmem(void) {
  if (this->source == NULL) return (false);
  return (this->source->isStringProgmem(this->iteratorIndex));
}

char * StringMapIterator::currentString(void) {
  if (this->source == NULL) return (NULL);
  return (this->source->getString(this->iteratorIndex));
}
