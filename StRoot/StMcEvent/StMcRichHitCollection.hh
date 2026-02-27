/***************************************************************************
 *
 * $Id: StMcRichHitCollection.hh,v 2.5 2012/03/22 00:47:08 perev Exp $
 *
 * Author: Manuel Calderon de la Barca Sanchez, March 2000
 ***************************************************************************
 *
 * Description: Container for StMcRichHit
 *
 ***************************************************************************
 *
 * $Log: StMcRichHitCollection.hh,v $
 * Revision 2.5  2012/03/22 00:47:08  perev
 * private => protected
 *
 * Revision 2.4  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.3  2009/07/24 19:08:08  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.2  2005/01/27 23:40:47  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.1  2000/03/06 18:05:22  calderon
 * 1) Modified SVT Hits storage scheme from layer-ladder-wafer to
 * barrel-ladder-wafer.
 * 2) Added Rich Hit class and collection, and links to them in other
 * classes.
 *
 *
 **************************************************************************/
/// @file StMcRichHitCollection.hh
/// @brief Container for Monte Carlo RICH detector hits.
#ifndef StMcRichHitCollection_hh
#define StMcRichHitCollection_hh
#include "StMcContainers.hh"
#include "StObject.h"

class StMcRichHit;

/// @brief Container for Monte Carlo RICH (Ring Imaging Cherenkov) detector hits.
class StMcRichHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcRichHitCollection();
    /// @brief Destructor.
    virtual ~StMcRichHitCollection();
    // StMcRichHitCollection(const StMcRichHitCollection&);            use default
    // StMcRichHitCollection& operator=(const StMcRichHitCollection&); use default
    /// @brief Clears all hits from the collection.
    void Clear(const char* opt="");
    /// @brief Returns true; this collection is a folder in the ROOT browser.
    bool IsFolder() const { return true;};
    /// @brief Populates the ROOT browser with hit entries.
virtual void Browse(TBrowser *b); 
    
    /// @brief Adds a RICH hit to the collection; returns true on success.
    bool          addHit(StMcRichHit*);
    /// @brief Returns the number of hits in the collection.
    unsigned long numberOfHits() const;

    /// @brief Returns the vector of RICH hits.
    StSPtrVecMcRichHit&       hits();
    /// @brief Returns the const vector of RICH hits.
    const StSPtrVecMcRichHit& hits() const;

protected:
    StSPtrVecMcRichHit mHits; ///< Collection of Monte Carlo RICH hits.
    ClassDef(StMcRichHitCollection,1)
};
#endif
