/***************************************************************************
 *
 * $Id: StMcSvtLadderHitCollection.hh,v 2.6 2012/03/22 00:49:53 perev Exp $
 *
 * Author: Manuel Calderon de la Barca Sanchez, Oct 1999
 ***************************************************************************
 *
 * Description: Monte Carlo Svt Ladder Hit Collection class
 *
 ***************************************************************************
 *
 * $Log: StMcSvtLadderHitCollection.hh,v $
 * Revision 2.6  2012/03/22 00:49:53  perev
 * private => protected
 *
 * Revision 2.5  2009/07/24 19:08:08  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.4  2005/01/27 23:40:48  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.3  2000/04/18 23:46:13  calderon
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
 * Revision 2.0  1999/11/17 02:01:00  calderon
 * Completely revised for new StEvent
 *
 *
 **************************************************************************/
/// @file StMcSvtLadderHitCollection.hh
/// @brief Monte Carlo SVT ladder hit collection — third level of the SVT hierarchy.

#ifndef StMcSvtLadderHitCollection_hh
#define StMcSvtLadderHitCollection_hh

#include "StMcSvtWaferHitCollection.hh"

/// @brief Container for MC SVT/SSD hits on one ladder, organised by wafer.
class StMcSvtLadderHitCollection : public StObject
{
public:
    StMcSvtLadderHitCollection();
    virtual ~StMcSvtLadderHitCollection();
    // StMcSvtLadderHitCollection(const StMcSvtLadderHitCollection&); use default
    // const StMcSvtLadderHitCollection& operator=(const StMcSvtLadderHitCollection&);                use default
    
    /// @brief Total number of hits across all wafers on this ladder.
    unsigned long numberOfHits() const;
    /// @brief Number of wafer sub-collections available on this ladder.
    unsigned int  numberOfWafers() const;
    
    /// @brief Return the mutable wafer sub-collection at index @p i (0-based).
    StMcSvtWaferHitCollection*       wafer(unsigned int);
    /// @brief Return the const wafer sub-collection at index @p i (0-based).
    const StMcSvtWaferHitCollection* wafer(unsigned int) const;

    /// @brief Set the barrel number this ladder belongs to (needed by wafer routing).
    void setBarrelNumber(int);
    
protected:
    enum { mMaxNumberOfWafers = 16 };                     ///< Maximum wafers per ladder (SSD requires up to 16).
    int                        mBarrelNumber;              ///< Barrel index to which this ladder belongs.
    StMcSvtWaferHitCollection  mWafers[mMaxNumberOfWafers]; ///< Per-wafer hit sub-collections.
    ClassDef(StMcSvtLadderHitCollection,1)
};
#endif
