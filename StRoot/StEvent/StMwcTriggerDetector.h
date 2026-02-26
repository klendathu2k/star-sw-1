/*!
 * \class StMwcTriggerDetector
 * \author Thomas Ullrich, Sep 1999
 * \brief MWC (Multi-Wire Chamber) trigger detector MIP data for one event.
 */
/***************************************************************************
 *
 * $Id: StMwcTriggerDetector.h,v 2.5 2007/07/11 23:06:45 perev Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StMwcTriggerDetector.h,v $
 * Revision 2.5  2007/07/11 23:06:45  perev
 * Cleanup+fix StXXXTriggerDetector
 *
 * Revision 2.4  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.3  2001/04/05 04:00:39  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2000/05/09 10:22:28  ullrich
 * Updated to cope with modified dst_TrgDet.idl
 *
 * Revision 2.1  1999/10/13 19:43:29  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StMwcTriggerDetector_hh
#define StMwcTriggerDetector_hh

/// @file StMwcTriggerDetector.h
/// @brief MWC (Multi-Wire Chamber) trigger detector MIP data per TPC sector.

#include "StObject.h"

class dst_TrgDet_st;

/// @brief Stores MWC (Multi-Wire Chamber) trigger detector data: MIP signals per TPC sector
///        and sub-sector for the triggered crossing and optional pre/post samples.
///        The MWC provides charged-particle multiplicity information used in the L0 trigger topology.
class StMwcTriggerDetector : public StObject {
public:
    StMwcTriggerDetector();
    /// @brief Construct from legacy DST trigger detector table.
    StMwcTriggerDetector(const dst_TrgDet_st&);
    // StMwcTriggerDetector(const StMwcTriggerDetector&);            use default
    // StMwcTriggerDetector& operator=(const StMwcTriggerDetector&); use default
    virtual ~StMwcTriggerDetector();

    /// @brief Return the number of TPC sectors instrumented.
    unsigned int   numberOfSectors() const;
    /// @brief Return the number of sub-sectors per sector.
    unsigned int   numberOfSubSectors() const;
    /// @brief Return the number of pre-crossing samples stored.
    unsigned int   numberOfPreSamples() const;
    /// @brief Return the number of post-crossing samples stored.
    unsigned int   numberOfPostSamples() const;
    /// @brief Return the number of auxiliary words per crossing.
    unsigned int   numberOfAuxWords() const;

    /// @brief Return the MIP count for @p sec, sub-sector @p subsec, at crossing @p evt.
    float  mips(unsigned int sec, unsigned int subsec, unsigned int evt = 0) const;
    /// @brief Return auxiliary word @p i at crossing sample @p evt.
    float  aux(unsigned int, unsigned int evt = 0) const;

    /// @brief Set the MIP count for sector, sub-sector, and crossing sample.
    void setMips(unsigned int, unsigned int, unsigned int, float);
    /// @brief Set auxiliary word at the given crossing sample.
    void setAux(unsigned int, unsigned int, float);
    /// @brief Set the number of pre-crossing samples.
    void setNumberOfPreSamples(unsigned int);
    /// @brief Set the number of post-crossing samples.
    void setNumberOfPostSamples(unsigned int);
    
protected:
    enum {mMaxSectors = 24,
          mMaxSubSectors = 4,
          mMaxEventSamples = 11,
          mMaxAux = 32};
    char mBeg[1];//!
    Float_t  mMips[mMaxSectors][mMaxSubSectors][mMaxEventSamples]; ///< MIP counts [sector][sub-sector][crossing].
    Float_t  mAux[mMaxAux][mMaxEventSamples];                      ///< Auxiliary words [word][crossing].
    Int_t    mNumberOfPreSamples;                                   ///< Number of stored pre-crossing samples.
    Int_t    mNumberOfPostSamples;                                  ///< Number of stored post-crossing samples.
    char mEnd[1];//!
    
    ClassDef(StMwcTriggerDetector,2)
};
#endif
