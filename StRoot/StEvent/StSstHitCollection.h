/*!
 * \class StSstHitCollection 
 * \author Jonathan Bouchet, Thomas Ullrich, May 2015
 */
/***************************************************************************
 *
 * $Id: StSstHitCollection.h,v 2.1 2015/05/13 16:50:59 ullrich Exp $
 *
 * Author: Jonathan Bouchet, Thomas Ullrich, May 2015
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSstHitCollection.h,v $
 * Revision 2.1  2015/05/13 16:50:59  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StSstHitCollection_hh
#define StSstHitCollection_hh

/// @file StSstHitCollection.h
/// @brief Defines the StSstHitCollection class, the top-level SST hit container.

#include "StObject.h"
#include "StSstLadderHitCollection.h"
class StSstHit;

/// @brief Top-level collection of all reconstructed SST hits, organized by ladder.
class StSstHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StSstHitCollection();
    /// @brief Destructor.
    ~StSstHitCollection();
    // StSstHitCollection(const StSstHitCollection&);            use default
    // StSstHitCollection& operator=(const StSstHitCollection&); use default
    
    /// @brief Adds a hit to the appropriate ladder/wafer sub-collection.
    bool          addHit(StSstHit*);
    /// @brief Returns the total number of hits across all ladders.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of ladder sub-collections.
    unsigned int  numberOfLadders() const;
    
    /// @brief Returns a pointer to the hit collection for the given ladder index.
    StSstLadderHitCollection*       ladder(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given ladder index.
    const StSstLadderHitCollection* ladder(unsigned int) const;

private:
    enum { mNumberOfLadders = 20 };
    StSstLadderHitCollection mLadders[mNumberOfLadders];   ///<  Array of ladder hit collections [0-19].
    
    ClassDef(StSstHitCollection,1)
};
#endif
