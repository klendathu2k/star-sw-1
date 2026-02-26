/**
 * \class StEventInfo
 * \brief Per-event metadata stored inside StEvent.
 * \author Thomas Ullrich, Jun 2000
 *
 * \details StEventInfo stores the basic identifiers and timing information
 * for a single collision event: event number, run number, Unix timestamp,
 * L0 trigger mask, RHIC bunch-crossing numbers, and raw event size.  One
 * instance is attached to each StEvent and accessed via StEvent::info().
 *
 * The trigger mask and event type string are the primary fields used in
 * event selection.
 *
 * \sa StEvent, StRunInfo
 */
/***************************************************************************
 *
 * $Id: StEventInfo.h,v 2.6 2009/11/23 16:34:06 fisyak Exp $
 *
 * Author: Thomas Ullrich, Jun 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEventInfo.h,v $
 * Revision 2.6  2009/11/23 16:34:06  fisyak
 * Cleanup, remove dependence on dst tables, clean up software monitors
 *
 * Revision 2.5  2002/02/22 22:56:47  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.4  2001/09/19 04:48:08  ullrich
 * Added event size.
 *
 * Revision 2.3  2001/04/05 04:00:36  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2000/09/06 22:34:24  ullrich
 * Changed mBunchCrossingNumber from scalar to array to hold all 64 bits.
 *
 * Revision 2.1  2000/06/19 01:32:16  perev
 *  Thomas StEvent branches added
 *
 **************************************************************************/
#ifndef StEventInfo_hh
#define StEventInfo_hh
/// @file StEventInfo.h
/// @brief Per-event metadata (event number, timestamp, trigger mask, bunch-crossing) stored inside StEvent.

#include "StObject.h"
#include "TString.h"
class event_header_st;

class StEventInfo : public StObject {
public:
    StEventInfo();
    virtual ~StEventInfo();

    /// \name Event identification
    /// @{
    const TString& type() const;                              ///< Event type string (e.g. "physics", "laser").
    int            id() const;                                ///< Event number within the run.
    int            runId() const;                             ///< Run number.
    int            time() const;                              ///< Unix timestamp of the event trigger.
    unsigned int   triggerMask() const;                       ///< Bitmask of fired L0 triggers.
    /// RHIC bunch-crossing number; \a index selects the 32-bit word (0 or 1).
    unsigned int   bunchCrossingNumber(unsigned int) const;
    unsigned int   eventSize() const;                         ///< Raw event size in bytes.
    /// @}
     
    void setType(const char*);
    void setRunId(int);
    void setId(int);
    void setTime(int);
    void setTriggerMask(unsigned int);
    void setBunchCrossingNumber(unsigned int, unsigned int);
    void setEventSize(unsigned int);
    
protected:
    TString  mType;                    ///< Event type string (e.g. "physics", "laser").
    Int_t    mRunId;                   ///< Run number.
    Int_t    mId;                      ///< Event number within the run.
    Int_t    mTime;                    ///< Unix timestamp of the event trigger.
    UInt_t   mTriggerMask;             ///< Bitmask of fired L0 triggers.
    UInt_t   mBunchCrossingNumber[2];  ///< RHIC bunch-crossing number words; [0]=lower 32 bits, [1]=upper 32 bits.
    UInt_t   mEventSize;               ///< Raw event size in bytes.

    ClassDef(StEventInfo,2)
};
#endif
