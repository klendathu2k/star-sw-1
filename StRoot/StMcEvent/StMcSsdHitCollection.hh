/***************************************************************************
 *
 * $Id: StMcSsdHitCollection.hh,v 2.4 2012/03/22 01:08:54 perev Exp $
 * $Log: StMcSsdHitCollection.hh,v $
 * Revision 2.4  2012/03/22 01:08:54  perev
 * private => protected
 *
 * Revision 2.3  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.2  2005/01/27 23:40:48  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.1  2004/09/14 05:00:30  calderon
 * Added support for Ist, Ssd and changes to Pixel, from "El Kai".
 *
 * Revision 2.1  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Ssd classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 *
 **************************************************************************/
/// @file StMcSsdHitCollection.hh
/// @brief Top-level Monte Carlo hit collection for the STAR SSD detector.

#ifndef StMcSsdHitCollection_hh
#define StMcSsdHitCollection_hh

#include "StMcSsdLadderHitCollection.hh"

class StMcSsdHit;

/// @brief Top-level collection of Monte Carlo SSD hits, organised by ladder.
///
/// Contains StMcSsdLadderHitCollection objects for each of the 20 SSD ladders.
class StMcSsdHitCollection : public StObject {
public:

    /// @brief Default constructor.
    StMcSsdHitCollection();
    /// @brief Destructor.
    virtual ~StMcSsdHitCollection();
    
    /// @brief Adds a hit to the appropriate ladder/wafer sub-collection.
    /// @return true if the hit was added successfully.
    bool addHit(StMcSsdHit*);
    /// @brief Returns the total number of hits across all ladders.
    unsigned long numberOfHits() const;
    /// @brief Returns the number of ladders in this collection.
    unsigned int  numberOfLadders() const;
    
    /// @brief Returns a pointer to the ladder sub-collection at the given index.
    StMcSsdLadderHitCollection*       ladder(unsigned int);
    /// @brief Returns a const pointer to the ladder sub-collection at the given index.
    const StMcSsdLadderHitCollection* ladder(unsigned int) const;
protected:
    enum { mNumberOfLadders = 20 }; ///< Number of SSD ladders.
    StMcSsdLadderHitCollection mLadders[mNumberOfLadders]; ///< Array of ladder sub-collections.
    ClassDef(StMcSsdHitCollection,1)
};
#endif
