/***************************************************************************
 *
 * $Id: StMcPxlSectorHitCollection.hh,v 2.1 2013/03/25 23:50:36 perev Exp $
 * $Log: StMcPxlSectorHitCollection.hh,v $
 * Revision 2.1  2013/03/25 23:50:36  perev
 * Mustafa.Pxl add
 *
 *
 **************************************************************************/
/// @file StMcPxlSectorHitCollection.hh
/// @brief Monte Carlo hit collection for one PXL sector.

#ifndef StMcPxlSectorHitCollection_hh
#define StMcPxlSectorHitCollection_hh

#include "StObject.h"
#include "StMcPxlLadderHitCollection.hh"

/// @brief Collection of Monte Carlo PXL hits for a single sector.
///
/// Holds kNumberOfLadders (4) StMcPxlLadderHitCollection objects,
/// one per ladder in the sector (ladders numbered CW 1-4 viewed from East,
/// with ladder 4 being the inner ladder).
class StMcPxlSectorHitCollection : public StObject
{
public:
    /// @brief Default constructor.
    StMcPxlSectorHitCollection();
    /// @brief Destructor.
    virtual ~StMcPxlSectorHitCollection();
    
    /// @brief Returns the total number of hits across all ladders in this sector.
    unsigned int numberOfHits() const;
    /// @brief Returns the number of ladders in this sector.
    unsigned int numberOfLadders() const;

    /// @brief Returns a pointer to the ladder sub-collection at the given index.
    StMcPxlLadderHitCollection*     ladder(unsigned int);
    /// @brief Returns a const pointer to the ladder sub-collection at the given index.
    const StMcPxlLadderHitCollection* ladder(unsigned int) const;

protected:
    enum { kNumberOfLadders = 4 }; ///< Number of ladders per PXL sector.
    StMcPxlLadderHitCollection mLadders[kNumberOfLadders]; ///< Array of ladder sub-collections.

    ClassDef(StMcPxlSectorHitCollection,1)
};

inline unsigned int StMcPxlSectorHitCollection::numberOfLadders() const {return kNumberOfLadders;}
#endif
