/***************************************************************************
 *
 * $Id: StMcIstHitCollection.hh,v 2.8 2015/03/13 18:44:58 perev Exp $
 * $Log: StMcIstHitCollection.hh,v $
 * Revision 2.8  2015/03/13 18:44:58  perev
 * Roll back
 *
 * Revision 2.6  2012/12/18 21:02:06  perev
 * Ist development (Jonathan)
 *
 * Revision 2.5  2012/03/22 00:43:52  perev
 * private => protected
 *
 * Revision 2.4  2009/07/24 19:08:07  perev
 * Cleanup + Btof added (Geurts)
 *
 * Revision 2.3  2009/02/06 15:38:12  fisyak
 * Jonathan: decoding for upgr15 geometry
 *
 * Revision 2.2  2005/05/11 20:54:29  calderon
 * Added persistency: ClassImp, ClassDef and inheritance from StObject.
 *
 * Revision 2.1  2004/09/14 05:00:29  calderon
 * Added support for Ist, Ssd and changes to Pixel, from "El Kai".
 *
 * Revision 2.1  2003/08/20 18:50:21  calderon
 * Addition of Tof classes and Pixel classes.  Modified track, event, and
 * container code to reflect this.
 * Fix bug in StMcVertex and in clearing of some hit collections.
 *
 *
 **************************************************************************/
/// @file StMcIstHitCollection.hh
/// @brief Top-level Monte Carlo hit collection for the STAR IST detector.

#ifndef StMcIstHitCollection_hh
#define StMcIstHitCollection_hh

#include "StMcIstLayerHitCollection.hh"
class StMcIstHit;

/// @brief Top-level collection of Monte Carlo IST hits, organised by layer/ladder.
///
/// Contains StMcIstLayerHitCollection objects indexed by ladder number
/// (mNumberOfLayers entries, corresponding to the 24 IST ladders).
class StMcIstHitCollection : public StObject {
public:

    /// @brief Default constructor.
    StMcIstHitCollection();
    /// @brief Destructor.
    virtual ~StMcIstHitCollection();
    
    /// @brief Adds a hit to the appropriate layer sub-collection.
    /// @return true if the hit was added successfully.
    bool addHit(StMcIstHit*);
    /// @brief Returns the total number of hits across all layers.
    unsigned long numberOfHits() const;
    /// @brief Returns the number of layer sub-collections in this container.
    unsigned int  numberOfLayers() const;
    
    /// @brief Returns a pointer to the layer sub-collection at the given index.
    StMcIstLayerHitCollection*       layer(unsigned int);
    /// @brief Returns a const pointer to the layer sub-collection at the given index.
    const StMcIstLayerHitCollection* layer(unsigned int) const;
protected:
    enum { mNumberOfLayers = 24 }; ///< Number of IST ladders (index dimension).
    StMcIstLayerHitCollection mLayers[mNumberOfLayers]; ///< Array of layer sub-collections.
    ClassDef(StMcIstHitCollection,1)
};
#endif
