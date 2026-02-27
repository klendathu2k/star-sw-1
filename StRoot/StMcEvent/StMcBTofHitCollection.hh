/***************************************************************************
 *
 * $Id: StMcBTofHitCollection.hh,v 2.3 2012/03/22 00:27:04 perev Exp $
 * $Log: StMcBTofHitCollection.hh,v $
 * Revision 2.3  2012/03/22 00:27:04  perev
 * private => protected
 *
 * Revision 2.2  2012/03/01 16:48:29  perev
 * method Browse() added
 *
 * Revision 2.1  2009/07/24 19:08:06  perev
 * Cleanup + Btof added (Geurts)
 *
 *
 */
/// @file StMcBTofHitCollection.hh
/// @brief Top-level container for Barrel Time-of-Flight MC hits.
#ifndef StMcBTofHitCollection_hh
#define StMcBTofHitCollection_hh
#include "StMcContainers.hh"
#include "StObject.h"

class StMcBTofHit;

/// @brief Top-level container for Barrel Time-of-Flight (BTof) MC hits.
///
/// Owns a collection of StMcBTofHit objects and provides access by reference.
class StMcBTofHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcBTofHitCollection();
    /// @brief Destructor — clears and frees all owned hits.
    virtual ~StMcBTofHitCollection();
    /// @brief Clears the hit collection.
    void Clear(const char* opt="");
    /// @brief Returns true; marks this object as a browsable folder in ROOT.
    bool IsFolder() const { return true;};
    /// @brief Supports browsing in the ROOT TBrowser.
virtual void Browse(TBrowser *b); 
    
    /// @brief Adds a hit to the collection; returns false if the pointer is null.
    bool          addHit(StMcBTofHit*);
    /// @brief Returns the number of hits in the collection.
    unsigned long numberOfHits() const;

    /// @brief Returns a mutable reference to the hit vector.
    StSPtrVecMcBTofHit&       hits();
    /// @brief Returns a const reference to the hit vector.
    const StSPtrVecMcBTofHit& hits() const;

protected:
    StSPtrVecMcBTofHit mHits; ///< Internal storage for BTof MC hits.
    ClassDef(StMcBTofHitCollection,1)
};
#endif
