/***************************************************************************
 *
 * $Id: StDetectorName.h,v 2.2 2015/05/13 17:06:13 ullrich Exp $
 *
 * Author: unknown
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StDetectorName.h,v $
 * Revision 2.2  2015/05/13 17:06:13  ullrich
 * Added hooks and interfaces to Sst detector (part of HFT).
 *
 *
 **************************************************************************/
/// @file StDetectorName.h
/// @brief Provides a lookup function mapping detector integer IDs to human-readable names.
#ifndef StDetectorName_h
#define StDetectorName_h
#include "StEnumerations.h"

/// Returns the human-readable name string for the given detector numeric ID.
R__EXTERN const char* DetectorName(int id);

#endif
