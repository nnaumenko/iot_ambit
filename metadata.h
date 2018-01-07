/*
 * Copyright (C) 2016-2018 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
* @file
* @brief Module and quantity metadata registry
*
* Introduces numerical identifiers used to pinpoint modules and quantities
*/

#ifndef METADATA_H
#define METADATA_H

// Module identifiers

typedef uint16_t ModuleId;

static const ModuleId ModuleIdUndefined = ModuleId(0);

static const ModuleId ModuleIdDiag = ModuleId(1);
static const ModuleId ModuleIdWebCC = ModuleId(2);
static const ModuleId ModuleIdWebConfig = ModuleId(3);
static const ModuleId ModuleIdWiFi = ModuleId(4);

static const ModuleId ModuleIdDHT = ModuleId(64);

#endif
