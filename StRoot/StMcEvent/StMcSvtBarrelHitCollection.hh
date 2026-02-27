/***************************************************************************
 *
 * $Id: StMcSvtBarrelHitCollection.hh,v 2.5 2012/03/22 00:48:45 perev Exp $
 *
 * Author: Manuel Calderon de la Barca Sanchez, March 2000
 ***************************************************************************
 *
 * Description: Monte Carlo Svt Barrel Hit Collection class
 *
 ***************************************************************************
 *
 * $Log: StMcSvtBarrelHitCollection.hh,v $
 * Revision 2.5  2012/03/22 00:48:45  perev
 * private => protected
 *
 * Revision 2.4  2009/07/24 19:08:08  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.3  2005/01/27 23:40:48  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.2  2000/04/18 23:46:12  calderon
 * Fix bug in reurning barrel number
 * Enumerations for the Max barrels, ladders & wafers modified for
 * SSD inclusion in current scheme.
 *
 * Revision 2.1  2000/03/06 18:05:22  calderon
 * 1) Modified SVT Hits storage scheme from layer-ladder-wafer to
 * barrel-ladder-wafer.
 * 2) Added Rich Hit class and collection, and links to them in other
 * classes.
 *
 *
 **************************************************************************/
/// @file StMcSvtBarrelHitCollection.hh
/// @brief Monte Carlo SVT barrel hit collection — second level of the SVT hierarchy.

#ifndef StMcSvtBarrelHitCollection_hh
#define StMcSvtBarrelHitCollection_hh

#include "StMcSvtLadderHitCollection.hh"

/// @brief Container for MC SVT/SSD hits in one barrel, organised by ladder.
///
/// Up to 20 ladders are supported to accommodate the SSD barrel which has
/// more ladders than each SVT barrel.
class StMcSvtBarrelHitCollection : public StObject
{    
public:
    StMcSvtBarrelHitCollection();
    virtual ~StMcSvtBarrelHitCollection();
    // StMcSvtBarrelHitCollection(const StMcSvtBarrelHitCollection&); use default
    // const StMcSvtBarrelHitCollection&
    // operator=(const StMcSvtBarrelHitCollection&);               use default
    
    /// @brief Total number of hits across all ladders in this barrel.
    unsigned long numberOfHits() const;
    /// @brief Number of ladder sub-collections available in this barrel.
    unsigned int  numberOfLadders() const;
    
    /// @brief Return the mutable ladder sub-collection at index @p i (0-based).
    StMcSvtLadderHitCollection*       ladder(unsigned int);
    /// @brief Return the const ladder sub-collection at index @p i (0-based).
    const StMcSvtLadderHitCollection* ladder(unsigned int) const;

    /// @brief Set the barrel number and propagate it to all ladder sub-collections.
    void setBarrelNumber(int);
    
protected:
    enum { mMaxNumberOfLadders = 20 };                      ///< Maximum ladders per barrel (SSD has up to 20).
    int                    mBarrelNumber;                   ///< Index of this barrel within the SVT/SSD system.
    StMcSvtLadderHitCollection mLadders[mMaxNumberOfLadders]; ///< Per-ladder hit sub-collections.
    ClassDef(StMcSvtBarrelHitCollection,1)
};
#endif
