/***************************************************************************
 *
 * $Id: StMcIstLayerHitCollection.hh,v 2.5 2012/03/22 00:44:33 perev Exp $
 *
 * Author: Fabrice Retiere/Kai Schweda, Aug 2003
 ***************************************************************************
 *
 * Description: Monte Carlo Ist Layer Hit Collection class from Kai
 *
 * The pixel detector hits are stored here.
 *
 ***************************************************************************
 *
 * $Log: StMcIstLayerHitCollection.hh,v $
 * Revision 2.5  2012/03/22 00:44:33  perev
 * private => protected
 *
 * Revision 2.4  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.3  2009/07/24 19:08:07  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.2  2005/05/11 20:54:29  calderon
 * Added persistency: ClassImp, ClassDef and inheritance from StObject.
 *
 * Revision 2.1  2004/09/14 05:00:30  calderon
 * Added support for Ist, Ssd and changes to Pixel, from "El Kai".
 *
 * Revision 2.1  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Ist classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 *
 *
 **************************************************************************/
/// @file StMcIstLayerHitCollection.hh
/// @brief Monte Carlo hit collection for one IST layer (ladder slot).

#ifndef StMcIstLayerHitCollection_hh
#define StMcIstLayerHitCollection_hh

#include "StMcContainers.hh"
#include "StObject.h"

class StMcIstHit;

/// @brief Collection of Monte Carlo IST hits for a single layer entry.
///
/// Stores a flat vector of StMcIstHit pointers for one IST layer/ladder slot.
/// Used as the second level of the IST hit hierarchy in StMcIstHitCollection.
class StMcIstLayerHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcIstLayerHitCollection();
    /// @brief Destructor.
    virtual ~StMcIstLayerHitCollection();
    /// @brief Clears the hit vector.
    void Clear(const char* opt="");
    /// @brief Returns true to allow ROOT browser folding.
    bool IsFolder() const { return true;};
    /// @brief Populates the ROOT browser with hits.
    virtual void Browse(TBrowser *b); 
    
    /// @brief Returns the number of hits in this layer.
    unsigned long numberOfHits() const;

    /// @brief Returns a reference to the vector of hits.
    StSPtrVecMcIstHit&       hits();
    /// @brief Returns a const reference to the vector of hits.
    const StSPtrVecMcIstHit& hits() const; 

protected:
    StSPtrVecMcIstHit mHits; ///< Vector of Monte Carlo IST hits.
    ClassDef(StMcIstLayerHitCollection,1)
};
#endif
