/*!
 * \class StSstLadderHitCollection 
 * \author Jonathan Bouchet, Thomas Ullrich, May 2015
 */
/***************************************************************************
 *
 * $Id: StSstLadderHitCollection.h,v 2.1 2015/05/13 16:50:59 ullrich Exp $
 *
 * Author: Jonathan Bouchet, Thomas Ullrich, May 2015
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSstLadderHitCollection.h,v $
 * Revision 2.1  2015/05/13 16:50:59  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StSstLadderHitCollection_hh
#define StSstLadderHitCollection_hh

/// @file StSstLadderHitCollection.h
/// @brief Defines the StSstLadderHitCollection class grouping SST hits by ladder.

#include "StObject.h"
#include "StSstWaferHitCollection.h"

/// @brief Collection of SST hits belonging to a single ladder, organized by wafer.
class StSstLadderHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSstLadderHitCollection();
    /// @brief Destructor.
    ~StSstLadderHitCollection();
    // StSstLadderHitCollection(const StSstLadderHitCollection&); use default
    // const StSstLadderHitCollection&
    // operator=(const StSstLadderHitCollection&);                use default
    
    /// @brief Returns the total number of hits across all wafers on this ladder.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of wafer sub-collections on this ladder.
    unsigned int  numberOfWafers() const;
    
    /// @brief Returns a pointer to the hit collection for the given wafer index.
    StSstWaferHitCollection*       wafer(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given wafer index.
    const StSstWaferHitCollection* wafer(unsigned int) const;

private:
    enum { mMaxNumberOfWafers = 16 };
    StSstWaferHitCollection  mWafers[mMaxNumberOfWafers];   ///<  Array of wafer hit collections [0-15].
    
    ClassDef(StSstLadderHitCollection,1)
};
#endif
