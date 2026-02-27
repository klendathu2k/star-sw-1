/***************************************************************************
 *
 * $Id: StMcMtdHitCollection.hh,v 2.3 2012/03/22 00:45:30 perev Exp $
 * $Log: StMcMtdHitCollection.hh,v $
 * Revision 2.3  2012/03/22 00:45:30  perev
 * private => protected
 *
 * Revision 2.2  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.1  2011/10/11 16:22:39  perev
 * Add Mtd
 *
 *
 */
/// @file StMcMtdHitCollection.hh
/// @brief Container for Muon Telescope Detector (MTD) MC hits.
#ifndef StMcMtdHitCollection_hh
#define StMcMtdHitCollection_hh
#include "StMcContainers.hh"
#include "StObject.h"

class TBrowser;
class StMcMtdHit;

/// @brief Container for Muon Telescope Detector (MTD) MC hits.
///
/// Owns a collection of StMcMtdHit objects and provides access by reference.
class StMcMtdHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcMtdHitCollection();
    /// @brief Destructor — clears and frees all owned hits.
    virtual ~StMcMtdHitCollection();
    /// @brief Clears the hit collection.
    void Clear(const char* opt="");
    /// @brief Returns true; marks this object as a browsable folder in ROOT.
    bool IsFolder() const { return true;};
    /// @brief Supports browsing in the ROOT TBrowser.
virtual void Browse(TBrowser *b); 
    /// @brief Adds a hit to the collection; returns false if the pointer is null.
    bool addHit(StMcMtdHit*);
    /// @brief Returns the number of hits in the collection.
    unsigned long numberOfHits() const;

    /// @brief Returns a mutable reference to the hit vector.
    StSPtrVecMcMtdHit&       hits();
    /// @brief Returns a const reference to the hit vector.
    const StSPtrVecMcMtdHit& hits() const;

protected:
    StSPtrVecMcMtdHit mHits; ///< Internal storage for MTD MC hits.
    ClassDef(StMcMtdHitCollection,1)
};
#endif
