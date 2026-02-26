/*!
 * \class StL1Trigger
 * \author Thomas Ullrich, Nov 2001
 * \brief Level-1 software trigger data, extending L0 with a second trigger word.
 */
/***************************************************************************
 *
 * $Id: StL1Trigger.h,v 2.2 2002/02/22 22:56:48 jeromel Exp $
 *
 * Author: Thomas Ullrich, Nov 2001
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StL1Trigger.h,v $
 * Revision 2.2  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.1  2001/11/07 21:18:17  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StL1Trigger_hh
#define StL1Trigger_hh

/// @file StL1Trigger.h
/// @brief Level-1 trigger data, extending the L0 trigger word with a second (prime) trigger word.

#include "StTrigger.h"

class dst_L0_Trigger_st;
class dst_L1_Trigger_st;

/// @brief Level-1 (L1) trigger object: stores the primary trigger word (inherited from StTrigger)
///        plus the secondary "prime" trigger word from the L1 processor.
class StL1Trigger : public StTrigger {
public:
    StL1Trigger();
    /// @brief Construct from legacy DST L0 and L1 trigger tables.
    StL1Trigger(const dst_L0_Trigger_st&, const dst_L1_Trigger_st&);
    // StL1Trigger(const StL1Trigger&);            use default
    // StL1Trigger& operator=(const StL1Trigger&); use default
    ~StL1Trigger();

    /// @brief Return the L1 prime trigger word (additional trigger condition bits).
    unsigned int triggerWordPrime() const;
    /// @brief Set the L1 prime trigger word.
    void setTriggerWordPrime(unsigned int);
    
protected:
    UInt_t mTriggerWordPrime; ///< Secondary (prime) L1 trigger word.
    ClassDef(StL1Trigger,1)
};
#endif
