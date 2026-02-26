/***************************************************************************
 *
 * $Id: StTriggerId.h,v 2.8 2011/02/02 20:26:30 ullrich Exp $
 *
 * Author: Thomas Ullrich, January 2003
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTriggerId.h,v $
 * Revision 2.8  2011/02/02 20:26:30  ullrich
 * Switched data member type of mask from uint64_t to ULong64_t
 *
 * Revision 2.7  2011/02/02 20:20:10  ullrich
 * Extend to 64 bit (Jamie)
 *
 * Revision 2.6  2006/05/04 19:07:02  ullrich
 * Extended mMaxTriggerIds to 42.
 *
 * Revision 2.5  2004/10/11 23:00:20  ullrich
 * Add copy constructor and assign. op., implement ostream op., define to enum.
 *
 * Revision 2.4  2003/09/02 17:58:06  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.3  2003/02/18 22:19:58  jeromel
 * Skip mIdx
 *
 * Revision 2.2  2003/02/18 21:34:46  jeromel
 * Changed vector to arrays
 *
 * Revision 2.1  2003/01/30 18:14:15  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTriggerId_hh
#define StTriggerId_hh

/// @file StTriggerId.h
/// @brief Container for STAR trigger IDs with versioning information for a single event.

#include "StObject.h"
#include <Stiostream.h>
#include <vector>
#include <stdint.h>

/// @brief Stores the set of trigger IDs that fired for an event, together with their
///        version numbers (name, threshold, prescale).  Up to mMaxTriggerIds triggers
///        per event are supported.
class StTriggerId : public StObject {
public:
    StTriggerId();
    StTriggerId(const StTriggerId &id);
    const StTriggerId &operator=(const StTriggerId &id);
    ~StTriggerId();
	
    /// @brief Return the 64-bit trigger mask (one bit per fired trigger).
    uint64_t mask() const;
    
    /// @brief Return the maximum number of trigger IDs that can be stored.
    unsigned int maxTriggerIds() const;
    /// @brief Return true if trigger with numerical @p id fired in this event.
    bool         isTrigger(unsigned int id) const;
    
    /// @brief Return the combined version for trigger @p id.
    unsigned int version(unsigned int id) const;
    /// @brief Return the name version for trigger @p id.
    unsigned int nameVersion(unsigned int id) const;
    /// @brief Return the threshold version for trigger @p id.
    unsigned int thresholdVersion(unsigned int id) const;
    /// @brief Return the prescale version for trigger @p id.
    unsigned int prescaleVersion(unsigned int id) const;    

    /// @brief Return the trigger ID stored at internal index @p idx.
    unsigned int triggerId(const int idx) const;    
    /// @brief Return a vector of all trigger IDs that fired.
    vector<unsigned int> triggerIds() const;  


    /// @brief Set the 64-bit trigger mask.
    void setMask(uint64_t);
    /// @brief Add a trigger with the given ID and version information.
    void addTrigger(unsigned int, unsigned int,
	          unsigned int, unsigned int, unsigned int);

    friend ostream& operator<<(ostream&, const StTriggerId&);
    
private:
    unsigned int index(unsigned int) const;
    
protected:
    enum {mMaxTriggerIds = 64};

private:
    UInt_t       mIdx;                           //!
    ULong64_t    mMask;                          ///< 64-bit bitmask of fired trigger IDs.
    UInt_t       mId[mMaxTriggerIds];            ///< Trigger ID for each stored entry.
    UInt_t       mVersion[mMaxTriggerIds];       ///< Combined version for each trigger.
    UInt_t       mNameVersion[mMaxTriggerIds];   ///< Name version for each trigger.
    UInt_t       mThresholdVersion[mMaxTriggerIds]; ///< Threshold version for each trigger.
    UInt_t       mPrescaleVersion[mMaxTriggerIds];  ///< Prescale version for each trigger.
    
    ClassDef(StTriggerId,5)
};

inline unsigned int
StTriggerId::maxTriggerIds() const {return  mMaxTriggerIds;}

inline unsigned int
StTriggerId::triggerId(const int idx) const
{
    if (idx>=0 && idx<mMaxTriggerIds)
        return mId[idx];
    else
        return 0;
}

#endif
