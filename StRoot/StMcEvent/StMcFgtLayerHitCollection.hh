/***************************************************************************
 *
 * $Id: StMcFgtLayerHitCollection.hh,v 2.4 2012/03/22 00:40:41 perev Exp $
 *
 * Author: Fabrice Retiere/Kai Schweda, Aug 2003
 ***************************************************************************
 *
 * Description: Monte Carlo Fgt Layer Hit Collection class from Kai
 *
 * The Fgt detector hits are stored here.
 *
 ***************************************************************************
 *
 * $Log: StMcFgtLayerHitCollection.hh,v $
 * Revision 2.4  2012/03/22 00:40:41  perev
 * private => protected
 *
 * Revision 2.3  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.2  2009/07/24 19:08:07  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.1  2005/04/18 20:11:33  calderon
 * Addition of Fgt and Fst files.  Modified other files to accomodate changes.
 *
 *
 *
 **************************************************************************/
/// @file StMcFgtLayerHitCollection.hh
/// @brief Per-layer sub-collection of Monte Carlo FGT hits.
#ifndef StMcFgtLayerHitCollection_hh
#define StMcFgtLayerHitCollection_hh

#include "StMcContainers.hh"
#include "StObject.h"

class StMcFgtHit;

/// @brief Per-layer sub-collection of Monte Carlo FGT hits.
///
/// Stores all StMcFgtHit objects for a single FGT disc (layer).
class StMcFgtLayerHitCollection : public StObject
{
public:
    /// @brief Default constructor.
    StMcFgtLayerHitCollection();
    /// @brief Destructor.
    virtual ~StMcFgtLayerHitCollection();
    /// @brief Clears all hits from the collection.
    void Clear(const char* opt="");
    /// @brief Returns true; this collection is a folder in the ROOT browser.
    bool IsFolder() const { return true;};
    /// @brief Populates the ROOT browser with hit entries.
virtual void Browse(TBrowser *b); 
    
    /// @brief Returns the number of hits in this layer.
    unsigned long numberOfHits() const;

    /// @brief Returns the vector of FGT hits for this layer.
    StSPtrVecMcFgtHit&       hits();
    /// @brief Returns the const vector of FGT hits for this layer.
    const StSPtrVecMcFgtHit& hits() const; 

protected:
    StSPtrVecMcFgtHit mHits; ///< Collection of Monte Carlo FGT hits for this layer.
    ClassDef(StMcFgtLayerHitCollection,1)
};
#endif
