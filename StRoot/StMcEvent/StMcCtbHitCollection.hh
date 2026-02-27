/***************************************************************************
 *
 * $Id: StMcCtbHitCollection.hh,v 2.4 2012/03/22 00:33:29 perev Exp $
 * $Log: StMcCtbHitCollection.hh,v $
 * Revision 2.4  2012/03/22 00:33:29  perev
 * private => protected
 *
 * Revision 2.3  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.2  2005/01/27 23:40:47  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.1  2003/02/19 03:29:42  calderon
 * Introduction of CTB classes to repository.
 *
 * Revision 1.0  2003/03/18 00:00:00  gans
 * Introduction of Ctb classes.  Modified several classes
 * accordingly.
 */
/// @file StMcCtbHitCollection.hh
/// @brief Container for Monte Carlo Central Trigger Barrel (CTB) hits.
#ifndef StMcCtbHitCollection_hh
#define StMcCtbHitCollection_hh
#include "StMcContainers.hh"
#include "StObject.h"

class StMcCtbHit;

/// @brief Container for Monte Carlo Central Trigger Barrel (CTB) hits.
class StMcCtbHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcCtbHitCollection();
    /// @brief Destructor.
    virtual ~StMcCtbHitCollection();
    // StMcCtbHitCollection(const StMcCtbHitCollection&);            use default
    // StMcCtbHitCollection& operator=(const StMcCtbHitCollection&); use default
    /// @brief Clears all hits from the collection.
    void Clear(const char* opt="");
    /// @brief Returns true; this collection is a folder in the ROOT browser.
    bool IsFolder() const { return true;};
    /// @brief Populates the ROOT browser with hit entries.
virtual void Browse(TBrowser *b); 
    
    /// @brief Adds a CTB hit to the collection; returns true on success.
    bool          addHit(StMcCtbHit*);
    /// @brief Returns the number of hits in the collection.
    unsigned long numberOfHits() const;

    /// @brief Returns the vector of CTB hits.
    StSPtrVecMcCtbHit&       hits();
    /// @brief Returns the const vector of CTB hits.
    const StSPtrVecMcCtbHit& hits() const;

protected:
    StSPtrVecMcCtbHit mHits; ///< Collection of Monte Carlo CTB hits.
    ClassDef(StMcCtbHitCollection,1)
};
#endif
