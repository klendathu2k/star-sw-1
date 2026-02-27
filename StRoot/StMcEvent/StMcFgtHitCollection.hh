/***************************************************************************
 *
 * $Id: StMcFgtHitCollection.hh,v 2.4 2012/03/22 00:39:27 perev Exp $
 *
 * $Log: StMcFgtHitCollection.hh,v $
 * Revision 2.4  2012/03/22 00:39:27  perev
 * private => protected
 *
 * Revision 2.3  2009/10/13 19:14:27  perev
 * Wei-Ming update
 *
 * Revision 2.1  2005/04/18 20:11:33  calderon
 * Addition of Fgt and Fst files.  Modified other files to accomodate changes.
 *
 *
 *
 **************************************************************************/
/// @file StMcFgtHitCollection.hh
/// @brief Top-level container for Monte Carlo Forward GEM Tracker (FGT) hits.
#ifndef StMcFgtHitCollection_hh
#define StMcFgtHitCollection_hh

#include "StMcFgtLayerHitCollection.hh"
class StMcFgtHit;

/// @brief Top-level container for Monte Carlo Forward GEM Tracker (FGT) hits.
///
/// Organises StMcFgtHit objects into per-layer (per-disc) sub-collections.
class StMcFgtHitCollection : public StObject {
public:
    
    /// @brief Default constructor.
    StMcFgtHitCollection();
    /// @brief Destructor.
    ~StMcFgtHitCollection();
    
    /// @brief Adds an FGT hit to the appropriate layer sub-collection; returns true on success.
    bool addHit(StMcFgtHit*);
    /// @brief Returns the total number of hits across all layers.
    unsigned long numberOfHits() const;
    /// @brief Returns the number of layers (discs) in the FGT.
    unsigned int  numberOfLayers() const;
    
    /// @brief Returns a pointer to the sub-collection for the given layer index.
    StMcFgtLayerHitCollection*       layer(unsigned int);
    /// @brief Returns a const pointer to the sub-collection for the given layer index.
    const StMcFgtLayerHitCollection* layer(unsigned int) const;
protected:
    enum { mNumberOfLayers = 9 }; ///< Number of FGT layers (discs). @see StFgtGeom.
    StMcFgtLayerHitCollection mLayers[mNumberOfLayers]; ///< Per-layer hit sub-collections.
    ClassDef(StMcFgtHitCollection,1)
};
#endif
