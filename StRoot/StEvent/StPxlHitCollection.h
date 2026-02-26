/*!
 * \class StPxlHitCollection 
 * \author X. Dong, Jan 2013
 */
/***************************************************************************
 *
 * $Id: StPxlHitCollection.h,v 2.1 2013/03/05 14:40:40 ullrich Exp $
 *
 * Author: X. Dong, Jan 2013
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StPxlHitCollection.h,v $
 * Revision 2.1  2013/03/05 14:40:40  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StPxlHitCollection_hh
#define StPxlHitCollection_hh
/// @file StPxlHitCollection.h
/// @brief Top-level hit collection for the STAR Pixel Detector (PXL).

#include "StObject.h"
#include "StPxlSectorHitCollection.h"

class StPxlHit;

/// @brief Top-level collection of PXL hits, organised by sector for the full detector.
class StPxlHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StPxlHitCollection();
    /// @brief Destructor.
    ~StPxlHitCollection();

    /// @brief Adds a PXL hit to the appropriate sector/ladder/sensor sub-collection.
    bool          addHit(StPxlHit*);
    /// @brief Returns the total number of hits across all sectors.
    unsigned int  numberOfHits() const;
    /// @brief Returns the number of sectors (10).
    unsigned int  numberOfSectors() const;

    /// @brief Returns a pointer to the hit collection for the given sector (0-indexed).
    StPxlSectorHitCollection*       sector(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given sector (0-indexed).
    const StPxlSectorHitCollection* sector(unsigned int) const;

private:
    enum { mNumberOfSectors = 10 };
    StPxlSectorHitCollection mSectors[mNumberOfSectors]; ///< Per-sector hit collections.
    
    ClassDef(StPxlHitCollection,1)
};

inline unsigned int StPxlHitCollection::numberOfSectors() const { return mNumberOfSectors; }

#endif
