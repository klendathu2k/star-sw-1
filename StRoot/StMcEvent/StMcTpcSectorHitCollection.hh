/***************************************************************************
 *
 * $Id: StMcTpcSectorHitCollection.hh,v 2.5 2012/05/16 21:36:15 fisyak Exp $
 *
 * Author: Manuel Calderon de la Barca Sanchez, Oct 1999
 ***************************************************************************
 *
 * Description: Monte Carlo Tpc Sector Hit Collection class
 *
 ***************************************************************************
 *
 * $Log: StMcTpcSectorHitCollection.hh,v $
 * Revision 2.5  2012/05/16 21:36:15  fisyak
 * Incresa no. possible row to 100
 *
 * Revision 2.4  2012/03/22 01:02:31  perev
 * private => protected
 *
 * Revision 2.3  2009/07/24 19:08:09  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.2  2005/01/27 23:40:49  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.1  2000/03/06 18:05:23  calderon
 * 1) Modified SVT Hits storage scheme from layer-ladder-wafer to
 * barrel-ladder-wafer.
 * 2) Added Rich Hit class and collection, and links to them in other
 * classes.
 *
 * Revision 2.0  1999/11/17 02:01:00  calderon
 * Completely revised for new StEvent
 *
 *
 **************************************************************************/
/// @file StMcTpcSectorHitCollection.hh
/// @brief Monte Carlo TPC sector hit collection — second level of the TPC hierarchy.

#ifndef StMcTpcSectorHitCollection_hh
#define StMcTpcSectorHitCollection_hh

#include "StMcTpcPadrowHitCollection.hh"

/// @brief Container for MC TPC hits belonging to one TPC sector, organised by padrow.
class StMcTpcSectorHitCollection : public StObject
{    
public:
    StMcTpcSectorHitCollection();
    virtual ~StMcTpcSectorHitCollection();
    // StMcTpcSectorHitCollection(const StMcTpcSectorHitCollection&);            use default
    // StMcTpcSectorHitCollection& operator=(const StMcTpcSectorHitCollection&); use default
    
    /// @brief Total number of hits across all padrows in this sector.
    unsigned long numberOfHits() const;
    /// @brief Number of padrow sub-collections (up to 100).
    unsigned int  numberOfPadrows() const;
    
    /// @brief Return the mutable padrow sub-collection at index @p i (0-based).
    StMcTpcPadrowHitCollection*       padrow(unsigned int);
    /// @brief Return the const padrow sub-collection at index @p i (0-based).
    const StMcTpcPadrowHitCollection* padrow(unsigned int) const;

protected:
    enum { mNumberOfPadrows = 100 };                        ///< Maximum number of TPC padrows per sector.
    StMcTpcPadrowHitCollection mPadrows[mNumberOfPadrows];  ///< Per-padrow hit sub-collections.
    ClassDef(StMcTpcSectorHitCollection,1)
};
#endif
