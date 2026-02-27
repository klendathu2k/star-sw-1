/***************************************************************************
 *
 * $Id: StMcSsdWaferHitCollection.hh,v 2.4 2012/03/22 00:48:45 perev Exp $
 *
 * Author: Manuel Calderon de la Barca Sanchez, Oct 1999
 ***************************************************************************
 *
 * Description: Monte Carlo Ssd Wafer Hit Collection class
 *
 ***************************************************************************
 *
 * $Log: StMcSsdWaferHitCollection.hh,v $
 * Revision 2.4  2012/03/22 00:48:45  perev
 * private => protected
 *
 * Revision 2.3  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.2  2009/07/24 19:08:08  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.1  2005/11/22 21:44:52  fisyak
 * Add compress Print for McEvent, add Ssd collections
 *
 * Revision 2.3  2005/01/27 23:40:48  calderon
 * Adding persistency to StMcEvent as a step for Virtual MonteCarlo.
 *
 * Revision 2.2  2000/03/06 18:05:23  calderon
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
/// @file StMcSsdWaferHitCollection.hh
/// @brief Monte Carlo hit collection for one SSD wafer.

#ifndef StMcSsdWaferHitCollection_hh
#define StMcSsdWaferHitCollection_hh
#include "StMcContainers.hh"
#include "StObject.h"

class StMcSsdHit;

/// @brief Collection of Monte Carlo SSD hits for a single wafer.
///
/// Stores a vector of StMcSsdHit pointers for one SSD wafer.
class StMcSsdWaferHitCollection : public StObject
{
public:
    /// @brief Default constructor.
    StMcSsdWaferHitCollection();
    // StMcSsdWaferHitCollection(const StMcSsdWaferHitCollection&); use default
    // const StMcSsdWaferHitCollection& operator=(const StMcSsdWaferHitCollection&); use default
    /// @brief Destructor.
    virtual ~StMcSsdWaferHitCollection();
    /// @brief Clears the hit vector.
    void Clear(const char* opt="");
    /// @brief Returns true to allow ROOT browser folding.
    bool IsFolder() const { return true;};
    /// @brief Populates the ROOT browser with hits.
    virtual void Browse(TBrowser *b); 
    
    /// @brief Returns a reference to the vector of hits.
    StSPtrVecMcSsdHit&       hits();
    /// @brief Returns a const reference to the vector of hits.
    const StSPtrVecMcSsdHit& hits() const;

protected:
    StSPtrVecMcSsdHit mHits; ///< Vector of Monte Carlo SSD hits.
    ClassDef(StMcSsdWaferHitCollection,1)
};
#endif
