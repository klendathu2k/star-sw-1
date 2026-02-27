/***************************************************************************
 *
 * $Id: StMcPxlHitCollection.hh,v 2.1 2013/03/25 23:50:36 perev Exp $
 * $Log: StMcPxlHitCollection.hh,v $
 * Revision 2.1  2013/03/25 23:50:36  perev
 * Mustafa.Pxl add
 *
 *
 **************************************************************************/
/// @file StMcPxlHitCollection.hh
/// @brief Top-level Monte Carlo hit collection for the STAR PXL detector.

#ifndef StMcPxlHitCollection_hh
#define StMcPxlHitCollection_hh

#include "StMcPxlSectorHitCollection.hh"
class StMcPxlHit;

/// @brief Top-level collection of Monte Carlo PXL hits, organised by sector.
///
/// Contains kNumberOfSectors (10) StMcPxlSectorHitCollection objects,
/// one per PXL sector.
class StMcPxlHitCollection : public StObject {
public:

    /// @brief Default constructor.
    StMcPxlHitCollection();
    /// @brief Destructor.
    virtual ~StMcPxlHitCollection();
    
    /// @brief Adds a hit to the appropriate sector/ladder/sensor sub-collection.
    /// @return true if the hit was added successfully.
    bool addHit(StMcPxlHit*);
    /// @brief Returns the total number of hits across all sectors.
    unsigned int numberOfHits() const;
    /// @brief Returns the number of sectors in this collection.
    unsigned int  numberOfSectors() const;
    
    /// @brief Returns a pointer to the sector sub-collection at the given index.
    StMcPxlSectorHitCollection*       sector(unsigned int);
    /// @brief Returns a const pointer to the sector sub-collection at the given index.
    const StMcPxlSectorHitCollection* sector(unsigned int) const;
protected:
    enum { kNumberOfSectors = 10 }; ///< Number of PXL sectors.
    StMcPxlSectorHitCollection mSectors[kNumberOfSectors]; ///< Array of sector sub-collections.
    ClassDef(StMcPxlHitCollection,1)
};

inline unsigned int StMcPxlHitCollection::numberOfSectors() const {return kNumberOfSectors;}
#endif
