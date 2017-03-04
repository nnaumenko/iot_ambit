/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
* @file
* @brief Legacy diagnostic log kept for compatibility
* 
* Obsolete, use DiagLog instead.
*/

#ifndef DIAG_LEGACY_H
#define DIAG_LEGACY_H

#include <Arduino.h>

//@todo remove after migrating to template-based architecture
#define DiagLogLegacy Serial

#endif
