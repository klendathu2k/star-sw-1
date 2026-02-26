/*!
 * \class StMtdHeader 
 */
/***************************************************************************
 *
 * $Id: StMtdHeader.h,v 2.3 2015/04/24 17:51:00 ullrich Exp $
 *
 * Author: Frank Geurts, April 25, 2011
 ***************************************************************************
 *
 * Description:
 *   Barrel MTD header data, contains the MTD data header
 *
 ***************************************************************************
 *
 * $Log: StMtdHeader.h,v $
 * Revision 2.3  2015/04/24 17:51:00  ullrich
 * Added data member mTpcSectorMask and mShouldHaveRejectEvent incl. access fcts.
 *
 * Revision 2.2  2013/04/06 12:18:35  ullrich
 * Increase MAXFIBER from 1 to 2.
 *
 * Revision 2.1  2011/04/25 21:24:02  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StMtdHeader_hh
#define StMtdHeader_hh

/// @file StMtdHeader.h
/// @brief Event-wise header data for the STAR Muon Telescope Detector (MTD).

#include <Stiostream.h>
#include "StObject.h"
#include "StEnumerations.h"

/// @brief Event-wise header data for the STAR Muon Telescope Detector (MTD).
class StMtdHeader : public StObject {
public:
    enum {MAXFIBER=2};
    
    /// @brief Default constructor.
    StMtdHeader();
    /// @brief Destructor.
    ~StMtdHeader();
    
    /// @brief Returns the fiber header word for the given fiber ID.
    short          fiberHeader(int fiberId) const;
    /// @brief Returns the fiber trigger word for the given fiber ID.
    unsigned int   fiberTriggerWord(int fiberId) const;
    /// @brief Returns the trigger time TDC count for the given fiber ID.
    unsigned int   triggerTime(int fiberId) const;
    /// @brief Returns the should-have-reject-event status flag.
    int            shouldHaveRejectEvent() const;
    /// @brief Returns the TPC sector tracking mask used in the first iteration.
    unsigned int   tpcSectorMask() const;
    
    /// @brief Sets the fiber header word for the given fiber ID.
    void         setFiberHeader(int fiberId, short val);
    /// @brief Sets the fiber trigger word for the given fiber ID.
    void         setFiberTriggerWord(int fiberId, unsigned int val);
    /// @brief Sets the trigger time TDC count for the given fiber ID.
    void         setTriggerTime(unsigned int tdc, int fiberId);
    /// @brief Sets the should-have-reject-event status flag.
    void         setShouldHaveRejectEvent(int reject);
    /// @brief Sets the TPC sector tracking mask.
    void         setTpcSectorMask(unsigned int mask);
    
protected:
    Short_t      mFiberHeader[MAXFIBER];       ///< Fiber header words, one per fiber
    UInt_t       mFiberTriggerWord[MAXFIBER];  ///< Fiber trigger words, one per fiber
    UInt_t       mTriggerTime[MAXFIBER];       ///< Trigger time TDC counts, one per fiber
    Int_t        mShouldHaveRejectEvent; // indication of event status in filtering
                                         // 0 - events not triggered di-muon
                                         // 1 - events should have been rejected 
                                         //     if only triggered by di-muon
                                         // 2 - events pass filtering cuts
    UInt_t       mTpcSectorMask;         // Mask of TPC sectors for tracking in the first iteration
    
    ClassDef(StMtdHeader,2)
};

#endif
