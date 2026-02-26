/***************************************************************************
 *
 * $Id: StHltTriggerReason.h,v 2.1 2011/02/01 19:45:48 ullrich Exp $
 *
 * Author: Liang Xue, Aihong Tang, Jan 2011
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StHltTriggerReason.h,v $
 * Revision 2.1  2011/02/01 19:45:48  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StHltTriggerReason_hh
#define StHltTriggerReason_hh

/// @file StHltTriggerReason.h
/// @brief HLT trigger reason record linking a trigger type to the physics object that fired it.

#include <Stiostream.h>
#include "StObject.h"
#include "StArray.h"

class StHltTriggerReasonCapable;

/// @brief Enumeration of HLT trigger reason types identifying which physics signature fired the trigger.
enum StHltTriggerReasonType {
	kHighPt         = 0x10000,  ///< High transverse-momentum track trigger.
	kDiElectron     = 0x20000,  ///< Di-electron pair trigger.
	kHeavyFragment  = 0x40000,  ///< Heavy nuclear fragment trigger.
	kAllEvents      = 0x80000,  ///< Accept-all events trigger.
	kRandomEvents   = 0x100000, ///< Random event sampling trigger.
	kBESGoodEvents  = 0x200000, ///< Beam energy scan good-event trigger.
};  

/// @brief Associates an HLT trigger reason bit with the physics object that caused the trigger.
class StHltTriggerReason : public StObject {
public:
    StHltTriggerReason();
    ~StHltTriggerReason();

    /// @brief Trigger reason bit identifying the physics signature that fired.
    StHltTriggerReasonType reasonBit() const;
    /// @brief Pointer to the physics object that fired the trigger (StHltHighPt, StHltHeavyFragment, or StHltDiElectron).
    StHltTriggerReasonCapable* reason();
    const StHltTriggerReasonCapable* reason() const;

    /// @brief Set the trigger reason bit.
    void setReasonBit(StHltTriggerReasonType);
    /// @brief Set the pointer to the triggering physics object.
    void setReason(StHltTriggerReasonCapable*);
    
    
private:
    StHltTriggerReasonType mReasonBit; ///< Trigger reason bit identifying the physics signature.
#ifdef __CINT__
    StObjLink mReason;
#else
    StLink<StHltTriggerReasonCapable> mReason; ///< Pointer to the triggering physics object (StHltHighPt, StHltHeavyFragment, or StHltDiElectron).
#endif //__CINT__
    
    ClassDef(StHltTriggerReason,1)
};

inline StHltTriggerReasonType StHltTriggerReason::reasonBit() const {return mReasonBit;}
inline StHltTriggerReasonCapable* StHltTriggerReason::reason() { return mReason; }
inline const StHltTriggerReasonCapable* StHltTriggerReason::reason() const { return mReason; }

ostream& operator<<(ostream&, const StHltTriggerReason&); ///< print operator

#endif
