#include "stringmap.h"

//////////////////////////////////////////////////////////////////////
// StringMap
//////////////////////////////////////////////////////////////////////

StringMapKey StringMap::find(const char * toFind) {
  for (int i = 0; i < this->count(); i++)
    if (!this->compare(i, toFind)) return (this->getKey(i));
  return (STRINGMAP_ITEM_DEFAULT);
}

char * StringMap::find(StringMapKey toFind) {
  for (int i = 0; i < this->count(); i++)
    if (this->getKey(i) == toFind) return (this->getString(i));
  return (NULL);
}

boolean StringMap::isProgmem (StringMapKey toFind) {
  for (int i = 0; i < this->count(); i++)
    if (this->getKey(i) == toFind) return (this->isStringProgmem(i));
  return (false);
}

int StringMap::compare (size_t index, const char * toCompare) {
  if (index >= this->count()) return (1); //arbitrary non-zero number to make sure comparison result is not match
  if (isStringProgmem(index))
    return (strcmp_P(toCompare, getString(index)));
  else
    return (strcmp(toCompare, getString(index)));
}

void StringMap::print(Print &client, StringMapKey key) {
  if (key == STRINGMAP_ITEM_DEFAULT) return;
  char * s = this->find(key);
  if (s == NULL) return;
  if (isProgmem(key))
    client.print((__FlashStringHelper *)s);
  else
    client.print(s);
}


//////////////////////////////////////////////////////////////////////
// QuickStringMap
//////////////////////////////////////////////////////////////////////

QuickStringMap::QuickStringMap(const QuickStringMapItem source[]) {
  this->source = source;

  int i = 0;
  this->itemsCount = 65535;
  //actual item count at this point is not known yet
  //itemsCount is temporarily set to maximum possible value, so that range check in getKey never fails
  while (this->getKey(i) != STRINGMAP_ITEM_DEFAULT) i++;
  this->itemsCount = i;
}

size_t QuickStringMap::count(void) {
  return (this->itemsCount);
}

char * QuickStringMap::getString(size_t index) {
  if (index >= this->count()) return (NULL);
  return ((char *)source[index].string);
}

boolean QuickStringMap::isStringProgmem(size_t index) {
  if (index >= this->count()) return (false);
  return (true);
}

StringMapKey QuickStringMap::getKey(size_t index) {
  if (index >= this->count()) return (STRINGMAP_ITEM_DEFAULT);
  return (pgm_read_word(&source[index].key)); //assumed that StringMapKey corresponds to word
}

