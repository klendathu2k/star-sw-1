/*!
 * \class StFtpcSectorHitCollection 
 * \author Thomas Ullrich, July 1999
 */
/***************************************************************************
 *
 * $Id: StFtpcSectorHitCollection.h,v 2.3 2002/02/22 22:56:48 jeromel Exp $
 *
 * Author: Thomas Ullrich, July 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StFtpcSectorHitCollection.h,v $
 * Revision 2.3  2002/02/22 22:56:48  jeromel
 * Doxygen basic documentation in all header files. None of this is required
 * for QM production.
 *
 * Revision 2.2  1999/10/28 22:25:24  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:43:11  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StFtpcSectorHitCollection_hh
#define StFtpcSectorHitCollection_hh

/// @file StFtpcSectorHitCollection.h
/// @brief Defines the StFtpcSectorHitCollection class holding FTPC hits for a single sector.

#include "StObject.h"
#include "StContainers.h"

class StFtpcHit;

/// @brief Collection of FTPC hits within a single azimuthal sector of a readout plane.
class StFtpcSectorHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StFtpcSectorHitCollection();
    // StFtpcSectorHitCollection(const StFtpcSectorHitCollection&);            use default
    // StFtpcSectorHitCollection& operator=(const StFtpcSectorHitCollection&); use default
    /// @brief Destructor.
    ~StFtpcSectorHitCollection();
    
    /// @brief Returns a reference to the vector of hits in this sector.
    StSPtrVecFtpcHit&       hits();
    /// @brief Returns a const reference to the vector of hits in this sector.
    const StSPtrVecFtpcHit& hits() const;

private:
    StSPtrVecFtpcHit mHits;   ///<  Hits reconstructed in this sector.
    
    ClassDef(StFtpcSectorHitCollection,1)
};
#endif
