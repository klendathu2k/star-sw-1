/*!
 * \class StTofMCInfo 
 * \author Wei-Ming Zhang, April 2001 
 */
/***************************************************************************
 *
 * $Id: StTofMCInfo.h,v 2.3 2003/05/21 18:23:18 ullrich Exp $
 *
 * Author: Wei-Ming Zhang, April 2001 
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTofMCInfo.h,v $
 * Revision 2.3  2003/05/21 18:23:18  ullrich
 * Major Revision of ToF classes (F. Geurts)
 *
 * Revision 2.2  2002/02/22 22:56:51  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.1  2001/04/27 21:40:34  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTofMCInfo_hh
#define StTofMCInfo_hh

/// @file StTofMCInfo.h
/// @brief Monte Carlo truth information associated with a legacy TOF detector element.

#include "StObject.h"

/// @brief Monte Carlo truth record for a single hit in the legacy TOF detector.
class StTofMCInfo : public StObject {
public:
    /// @brief Default constructor.
    StTofMCInfo();
    /// @brief Constructs with all Monte Carlo truth fields.
    StTofMCInfo(int,   int,   int,   float, int,   float, 
		float, float, float, float, float, float,    
		float);

    /// @brief Equality comparison operator.
    int operator==(const StTofMCInfo& MCInfo) const;
    /// @brief Inequality comparison operator.
    int operator!=(const StTofMCInfo& MCInfo) const;

public:
    Int_t   mTrkId;    ///< Monte Carlo track ID
    Int_t   mGId;      ///< GEANT particle ID
    Int_t   mNHits;    ///< Number of Monte Carlo hits in the detector element
    Int_t   mNPhe;     ///< Number of photoelectrons produced
    Float_t mDe;       ///< Energy deposit [GeV]
    Float_t mPTot;     ///< Total momentum at the hit [GeV/c]
    Float_t mDs;       ///< Step length of Monte Carlo track [cm]
    Float_t mSLength;  ///< Track path length to the detector element [cm]
    Float_t mPmLength; ///< Path length within the material [cm]
    Float_t mTof;      ///< Monte Carlo time of flight [ns]
    Float_t mTime;     ///< Absolute Monte Carlo time [ns]
    Float_t mMTime;    ///< Mirror-reflected time [ns]
    Float_t mMTimeL;   ///< Mirror-reflected leading-edge time [ns]
    
    ClassDef(StTofMCInfo,2)
};

inline int StTofMCInfo::operator==(const StTofMCInfo& MCInfo) const
{
    return ((mTrkId == MCInfo.mTrkId) && (mGId  == MCInfo.mGId) &&
            (mNHits == MCInfo.mNHits) && (mNPhe == MCInfo.mNPhe)); 
}

inline int StTofMCInfo::operator!=(const StTofMCInfo& MCInfo) const
{
    return !(*this == MCInfo);  // use operator==()
}

ostream& operator<<(ostream& os, const StTofMCInfo& MCInfo);

#endif
