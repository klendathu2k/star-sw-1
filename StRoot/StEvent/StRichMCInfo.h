/*!
 * \class StRichMCInfo 
 * \author Brian Lasiuk, May 2000
 *
 *    Contains the simulator info that is NOT stored in the
 *    g2t tables --> most importantly a tag of the process type
 *
 */
/***************************************************************************
 *
 * $Id: StRichMCInfo.h,v 2.3 2002/02/22 22:56:49 jeromel Exp $
 *
 * Author: Brian Lasiuk, May 2000
 ***************************************************************************
 *
 * Description:
 *   Contains the simulator info that is NOT stored in the
 *   g2t tables --> most importantly a tag of the process type
 ***************************************************************************
 *
 * $Log: StRichMCInfo.h,v $
 * Revision 2.3  2002/02/22 22:56:49  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  2001/04/05 04:00:40  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/05/22 21:44:38  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRichMCInfo_hh
#define StRichMCInfo_hh

/// @file StRichMCInfo.h
/// @brief Monte Carlo truth record for a RICH pixel or hit, holding generator-level particle information.

#include "StObject.h"
#include "StEnumerations.h"

/// @brief Generator-level truth record attached to a RICH MC pixel or hit.
///
/// Stores simulator and GEANT identifiers, the parent track index, particle charge,
/// and the GEANT process code that produced the signal — information not available in
/// the standard g2t tables.
class StRichMCInfo : public StObject {
public:
    /// @brief Default constructor.
    StRichMCInfo();
    /// @brief Constructor with particle ID, GEANT ID, track index, charge, and process code.
    StRichMCInfo(int id, int gid,  int trk,
                 float q, int process);
    // StRichMCInfo(const StRichMCInfo&);            use default
    // StRichMCInfo& operator=(const StRichMCInfo&); use default
    ~StRichMCInfo();

    int operator==(const StRichMCInfo&) const;
    int operator!=(const StRichMCInfo&) const;

    /// @brief Returns the simulator particle identifier.
    int   id()      const;
    /// @brief Returns the GEANT particle identifier.
    int   gid()     const;
    /// @brief Returns the index of the parent Monte Carlo track.
    int   trackp()  const;
    /// @brief Returns the electric charge of the particle.
    float charge()  const;
    /// @brief Returns the GEANT process code that produced this hit.
    int   process() const;

protected:
    Int_t     mId;       ///< Simulator particle identifier
    Int_t     mGid;      ///< GEANT particle identifier
    Int_t     mTrackp;   ///< Index of the parent Monte Carlo track
    Float_t   mCharge;   ///< Electric charge of the particle
    Int_t     mProcess;  ///< GEANT process code that produced the hit

    ClassDef(StRichMCInfo,1)
};

inline int StRichMCInfo::id()  const { return ( mId ); }
inline int StRichMCInfo::gid()  const { return ( mGid );}
inline int StRichMCInfo::trackp()  const {return ( mTrackp );}
inline float StRichMCInfo::charge()  const {return ( mCharge );}
inline int StRichMCInfo::process()  const { return ( mProcess );}

#endif
