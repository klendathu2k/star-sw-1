/*!
 * \class StSstWaferHitCollection 
 * \author Jonathan Bouchet, Thomas Ullrich, May 2015
 */
/***************************************************************************
 *
 * $Id: StSstWaferHitCollection.h,v 2.1 2015/05/13 16:50:59 ullrich Exp $
 *
 * Author: Jonathan Bouchet, Thomas Ullrich, May 2015
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSstWaferHitCollection.h,v $
 * Revision 2.1  2015/05/13 16:50:59  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StSstWaferHitCollection_hh
#define StSstWaferHitCollection_hh

/// @file StSstWaferHitCollection.h
/// @brief Defines the StSstWaferHitCollection class holding SST hits for a single wafer.

#include "StObject.h"
#include "StContainers.h"

class StSstHit;

/// @brief Collection of SST hits on a single silicon wafer.
class StSstWaferHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSstWaferHitCollection();
    // StSstWaferHitCollection(const StSstWaferHitCollection&); use default
    // const StSstWaferHitCollection&
    // operator=(const StSstWaferHitCollection&);               use default
    /// @brief Destructor.
    ~StSstWaferHitCollection();
    
    /// @brief Returns a reference to the vector of hits on this wafer.
    StSPtrVecSstHit&       hits();
    /// @brief Returns a const reference to the vector of hits on this wafer.
    const StSPtrVecSstHit& hits() const;

private:
    StSPtrVecSstHit mHits;   ///<  Hits reconstructed on this wafer.
    
    ClassDef(StSstWaferHitCollection,1)
};
#endif
