/***************************************************************************
 *
 * $Id: StHltTriggerReasonCapable.h,v 2.1 2011/02/01 19:45:47 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltTriggerReasonCapable.h,v $
 * Revision 2.1  2011/02/01 19:45:47  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltTriggerReasonCapable_hh
#define StHltTriggerReasonCapable_hh

/// @file StHltTriggerReasonCapable.h
/// @brief Base class for HLT physics objects that can serve as a trigger reason.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"

/// @brief Abstract base class for HLT physics objects that can be the reason for an online trigger decision.
class StHltTriggerReasonCapable : public StObject {
public:
    StHltTriggerReasonCapable();
    ~StHltTriggerReasonCapable();
    
    ClassDef(StHltTriggerReasonCapable,1)
};

ostream& operator<<(ostream&, const StHltTriggerReasonCapable&); ///< print operator

#endif
