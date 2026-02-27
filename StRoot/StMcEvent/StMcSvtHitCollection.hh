/***************************************************************************
 *
 * $Id: StMcSvtHitCollection.hh,v 2.6 2012/03/22 00:48:45 perev Exp $
 * $Log: StMcSvtHitCollection.hh,v $
 * Revision 2.6  2012/03/22 00:48:45  perev
 * private => protected
 *
 * Revision 2.5  2009/07/24 19:08:08  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.4  2005/01/27 23:40:48  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.3  2000/04/18 23:46:12  calderon
 * Fix bug in reurning barrel number
 * Enumerations for the Max barrels, ladders & wafers modified for
 * SSD inclusion in current scheme.
 *
 * Revision 2.2  2000/03/06 18:05:22  calderon
 * 1) Modified SVT Hits storage scheme from layer-ladder-wafer to
 * barrel-ladder-wafer.
 * 2) Added Rich Hit class and collection, and links to them in other
 * classes.
 *
 * Revision 2.1  1999/11/19 19:06:33  calderon
 * Recommit after redoing the files.
 *
 * Revision 2.0  1999/11/17 02:12:16  calderon
 * Completely revised for new StEvent
 *
 * Revision 1.2  1999/09/23 21:25:52  calderon
 * Added Log & Id
 * Modified includes according to Yuri
 *
 *
 **************************************************************************/
/// @file StMcSvtHitCollection.hh
/// @brief Top-level collection of Monte Carlo SVT/SSD hits, organised by barrel.

#ifndef StMcSvtHitCollection_hh
#define StMcSvtHitCollection_hh

#include "StMcSvtBarrelHitCollection.hh"
class StMcSvtHit;

/// @brief Top-level container for MC SVT/SSD hits arranged in a 4-level hierarchy:
///        collection → barrel → ladder → wafer.
///
/// The fourth barrel slot accommodates SSD hits alongside the three SVT barrels.
class StMcSvtHitCollection : public StObject {
public:
    StMcSvtHitCollection();
    virtual ~StMcSvtHitCollection();
    // StMcSvtHitCollection(const StMcSvtHitCollection&);            use default
    // StMcSvtHitCollection& operator=(const StMcSvtHitCollection&); use default
    
    /// @brief Add a hit to the appropriate barrel/ladder/wafer sub-collection.
    bool          addHit(StMcSvtHit*);
    /// @brief Total number of hits across all barrels.
    unsigned long numberOfHits() const;
    /// @brief Number of barrel sub-collections (3 SVT + 1 SSD = 4).
    unsigned int  numberOfBarrels() const;
    
    /// @brief Return the mutable barrel sub-collection at index @p i (0-based).
    StMcSvtBarrelHitCollection*       barrel(unsigned int);
    /// @brief Return the const barrel sub-collection at index @p i (0-based).
    const StMcSvtBarrelHitCollection* barrel(unsigned int) const;

protected:
    enum { mNumberOfBarrels = 4 };                          ///< 3 SVT barrels + 1 SSD barrel.
    StMcSvtBarrelHitCollection mBarrels[mNumberOfBarrels];  ///< Per-barrel hit sub-collections.
    ClassDef(StMcSvtHitCollection,1)
};

#endif
