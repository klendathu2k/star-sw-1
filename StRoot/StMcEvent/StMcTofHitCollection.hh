/***************************************************************************
 *
 * $Id: StMcTofHitCollection.hh,v 2.5 2012/03/22 00:50:46 perev Exp $
 * $Log: StMcTofHitCollection.hh,v $
 * Revision 2.5  2012/03/22 00:50:46  perev
 * private => protected
 *
 * Revision 2.4  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.3  2009/07/24 19:08:09  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.2  2005/01/27 23:40:48  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.1  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Pixel classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 */
/// @file StMcTofHitCollection.hh
/// @brief Container for legacy pVPD/TOFr MC hits.
#ifndef StMcTofHitCollection_hh
#define StMcTofHitCollection_hh
#include "StMcContainers.hh"
#include "StObject.h"

class StMcTofHit;

/// @brief Container for legacy pVPD/TOFr MC hits (pre-Run 8 era).
///
/// Owns a collection of StMcTofHit objects and provides access by reference.
class StMcTofHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcTofHitCollection();
    /// @brief Destructor — clears and frees all owned hits.
    virtual ~StMcTofHitCollection();
    /// @brief Clears the hit collection.
    void Clear(const char* opt="");
    /// @brief Returns true; marks this object as a browsable folder in ROOT.
    bool IsFolder() const { return true;};
    /// @brief Supports browsing in the ROOT TBrowser.
virtual void Browse(TBrowser *b); 
    
    /// @brief Adds a hit to the collection; returns false if the pointer is null.
    bool          addHit(StMcTofHit*);
    /// @brief Returns the number of hits in the collection.
    unsigned long numberOfHits() const;

    /// @brief Returns a mutable reference to the hit vector.
    StSPtrVecMcTofHit&       hits();
    /// @brief Returns a const reference to the hit vector.
    const StSPtrVecMcTofHit& hits() const;

protected:
    StSPtrVecMcTofHit mHits; ///< Internal storage for legacy TOF MC hits.
    ClassDef(StMcTofHitCollection,1)
};
#endif
