/***************************************************************************
 *
 * $Id: StMcEmcModuleHitCollection.hh,v 2.7 2012/03/22 00:35:22 perev Exp $
 *
 * Author: Aleksei Pavlinov, May 2000
 ***************************************************************************
 *
 * Description: Monte Carlo Emc Module Hit Collection class
 *
 ***************************************************************************
 *
 * $Log: StMcEmcModuleHitCollection.hh,v $
 * Revision 2.7  2012/03/22 00:35:22  perev
 * private => protected
 *
 * Revision 2.6  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.5  2007/10/05 00:01:21  calderon
 * Changes to include a EMC hit collection that does not care about
 * parent tracks, so that now there are two collections.  This
 * new collection will be useful to compare all the deposited energy in a hit tower
 * in a given event. The information about the track parentage is still
 * kept in the original collection unchanged.
 *
 * Revision 2.4  2005/01/27 23:40:47  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.3  2001/05/31 02:45:55  perev
 * const(ing)
 *
 * Revision 2.2  2000/08/30 14:52:03  calderon
 * New changes made by Aleksei.
 *
 * Revision 2.1  2000/06/06 23:01:09  calderon
 * Inital revision
 *
 *
 **************************************************************************/
/// @file StMcEmcModuleHitCollection.hh
/// @brief Per-module sub-collection of Monte Carlo EMC hits.
#ifndef StMcEmcModuleHitCollection_hh
#define StMcEmcModuleHitCollection_hh
#include "StMcContainers.hh"
#include "TDataSet.h"

class StMcCalorimeterHit;

/// @brief Per-module sub-collection of Monte Carlo EMC hits.
///
/// Stores two vectors of StMcCalorimeterHit: track-associated hits and
/// detector hits (energy summed per cell, parent track not preserved).
class StMcEmcModuleHitCollection : public TDataSet {
public:
    /// @brief Default constructor.
    StMcEmcModuleHitCollection();
    /// @brief Constructor specifying the module index.
    StMcEmcModuleHitCollection(const unsigned int m);
    /// @brief Destructor.
    virtual ~StMcEmcModuleHitCollection();
    /// @brief Clears all hits from both collections.
    void Clear(const char* opt="");
    /// @brief Returns true; this collection is a folder in the ROOT browser.
virtual bool IsFolder() const { return true;};
    /// @brief Populates the ROOT browser with hit entries.
virtual void Browse(TBrowser *b); 

    /// @brief Initialises the module index.
    void init(const unsigned int m);
    /// @brief Returns the number of track-associated hits in this module.
    unsigned long numberOfHits() const;
    /// @brief Returns the number of detector hits (energy-summed per cell) in this module.
    unsigned long numberOfDetectorHits() const;
    /// @brief Returns the total energy deposit across all hits in this module (GeV).
    float sum() const;

    /// @brief Returns the vector of track-associated calorimeter hits.
    StSPtrVecMcCalorimeterHit&       hits();
    /// @brief Returns the const vector of track-associated calorimeter hits.
    const StSPtrVecMcCalorimeterHit& hits() const;
    
    // detector hits are like hits, but there is at most one detector hit per
    // element (tower, preshower, or strip).  If multiple hits occur in a single
    // element their energy depositions are summed, regardless of the parent track.
    // Also, detector hits do not preserve information about the parent track of 
    // the hit, since there may be more than one.  APK - 09/07
    /// @brief Returns the vector of detector hits (energy summed per cell, no track info).
    StSPtrVecMcCalorimeterHit&       detectorHits();
    /// @brief Returns the const vector of detector hits.
    const StSPtrVecMcCalorimeterHit& detectorHits() const;


    /// @brief Reinitialises the module index (functor interface).
    void operator()(const unsigned int m) { init(m); } 

protected:
    StSPtrVecMcCalorimeterHit mHits;         ///< Track-associated calorimeter hits.
    StSPtrVecMcCalorimeterHit mDetectorHits; ///< Energy-summed detector hits (no track info).
    ClassDef(StMcEmcModuleHitCollection,1)
};
#endif
