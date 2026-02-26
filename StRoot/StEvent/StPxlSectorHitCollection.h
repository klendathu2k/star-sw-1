/*!
 * \class StPxlSectorHitCollection 
 * \author X. Dong, Jan 2013
 */
/***************************************************************************
 *
 * $Id: StPxlSectorHitCollection.h,v 2.1 2013/03/05 14:40:41 ullrich Exp $
 *
 * Author: X. Dong, Jan 2013
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StPxlSectorHitCollection.h,v $
 * Revision 2.1  2013/03/05 14:40:41  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StPxlSectorHitCollection_hh
#define StPxlSectorHitCollection_hh
/// @file StPxlSectorHitCollection.h
/// @brief PXL hit collection for one sector of the STAR Pixel Detector.

#include "StObject.h"
#include "StPxlLadderHitCollection.h"

/// @brief Collection of PXL hits for a single sector, containing per-ladder sub-collections.
class StPxlSectorHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StPxlSectorHitCollection();
    /// @brief Destructor.
    ~StPxlSectorHitCollection();

    /// @brief Returns the total number of hits across all ladders in this sector.
    unsigned int numberOfHits() const;
    /// @brief Returns the number of ladders per sector (4).
    unsigned int numberOfLadders() const;

    /// @brief Returns a pointer to the hit collection for the given ladder (0-indexed).
    StPxlLadderHitCollection*       ladder(unsigned int);
    /// @brief Returns a const pointer to the hit collection for the given ladder (0-indexed).
    const StPxlLadderHitCollection* ladder(unsigned int) const;

private:
    enum { mNumberOfLadders = 4 };
    StPxlLadderHitCollection mLadders[mNumberOfLadders]; ///< Per-ladder hit collections.
    
    ClassDef(StPxlSectorHitCollection,1)
};

inline unsigned int StPxlSectorHitCollection::numberOfLadders() const { return mNumberOfLadders; }

#endif
