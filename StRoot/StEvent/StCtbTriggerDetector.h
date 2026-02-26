/*!
 * \class StCtbTriggerDetector
 * \author Thomas Ullrich, Sep 1999
 * \brief CTB (Central Trigger Barrel) trigger detector MIP data for one event.
 */
/***************************************************************************
 *
 * $Id: StCtbTriggerDetector.h,v 2.7 2007/07/11 23:06:45 perev Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StCtbTriggerDetector.h,v $
 * Revision 2.7  2007/07/11 23:06:45  perev
 * Cleanup+fix StXXXTriggerDetector
 *
 * Revision 2.6  2004/02/11 01:42:09  ullrich
 * Added new constructor to load data from StTriggerData.
 *
 * Revision 2.5  2002/11/19 20:21:00  ullrich
 * Added method to sum all mips.
 *
 * Revision 2.4  2002/02/22 22:56:46  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:34  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2000/05/09 10:22:21  ullrich
 * Updated to cope with modified dst_TrgDet.idl
 *
 * Revision 2.1  1999/10/13 19:42:56  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StCtbTriggerDetector_hh
#define StCtbTriggerDetector_hh

/// @file StCtbTriggerDetector.h
/// @brief CTB (Central Trigger Barrel) trigger detector MIP signal data.

#include "StObject.h"

class dst_TrgDet_st;
class StTriggerData;

/// @brief Stores CTB (Central Trigger Barrel) MIP signal data per tray and slat
///        for the triggered crossing and optional pre/post-crossing samples.
///        The CTB is a scintillator barrel around the TPC used for charged-particle multiplicity triggering.
class StCtbTriggerDetector : public StObject {
public:
    StCtbTriggerDetector();
    /// @brief Construct from legacy DST trigger detector table.
    StCtbTriggerDetector(const dst_TrgDet_st&);
    /// @brief Construct from decoded StTriggerData.
    StCtbTriggerDetector(const StTriggerData&);
    virtual ~StCtbTriggerDetector();
    // StCtbTriggerDetector(const StCtbTriggerDetector&);            use default
    // StCtbTriggerDetector& operator=(const StCtbTriggerDetector&); use default
    
    /// @brief Return the number of CTB trays.
    unsigned int   numberOfTrays() const;
    /// @brief Return the number of slats per tray.
    unsigned int   numberOfSlats() const;
    /// @brief Return the number of pre-crossing samples stored.
    unsigned int   numberOfPreSamples() const;
    /// @brief Return the number of post-crossing samples stored.
    unsigned int   numberOfPostSamples() const;
    /// @brief Return the number of auxiliary words per crossing.
    unsigned int   numberOfAuxWords() const;
    /// @brief Return the MIP count for @p tray, slat @p slot, at crossing sample @p evt.
    float          mips(unsigned int tray, unsigned int slot, unsigned int evt = 0) const;
    /// @brief Return the timing value for @p tray, slat @p slot, at crossing sample @p evt.
    char           time(unsigned int tray, unsigned int slot, unsigned int evt = 0) const;
    /// @brief Return auxiliary word @p i at crossing sample @p evt.
    float          aux(unsigned int, unsigned int evt = 0) const;

    /// @brief Return the summed MIP count over all trays and slats at crossing sample @p evt.
    double         mips(unsigned int evt = 0) const;     // sum over all trays, slot

    /// @brief Set the MIP count for tray, slot, and crossing sample.
    void setMips(unsigned int, unsigned int, unsigned int, float);
    /// @brief Set the timing value for tray, slot, and crossing sample.
    void setTime(unsigned int, unsigned int, unsigned int, char);
    /// @brief Set auxiliary word at the given crossing sample.
    void setAux(unsigned int, unsigned int, float);
    /// @brief Set the number of pre-crossing samples.
    void setNumberOfPreSamples(unsigned int);
    /// @brief Set the number of post-crossing samples.
    void setNumberOfPostSamples(unsigned int);
    
protected:
    enum {mMaxTrays = 120,
          mMaxSlats = 2,
          mMaxEventSamples = 11,
          mMaxAux = 16};
    char mBeg[1];//!
    Float_t  mMips[mMaxTrays][mMaxSlats][mMaxEventSamples]; ///< MIP counts [tray][slat][crossing].
    Char_t   mTime[mMaxTrays][mMaxSlats][mMaxEventSamples]; ///< Timing values [tray][slat][crossing].
    Float_t  mAux[mMaxAux][mMaxEventSamples];               ///< Auxiliary words [word][crossing].
    Int_t    mNumberOfPreSamples;                           ///< Number of stored pre-crossing samples.
    Int_t    mNumberOfPostSamples;                          ///< Number of stored post-crossing samples.
    char mEnd[1];//!
    
    ClassDef(StCtbTriggerDetector,2)
};
#endif
