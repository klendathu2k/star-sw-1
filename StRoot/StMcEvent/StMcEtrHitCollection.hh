/***************************************************************************
 *
 ***************************************************************************/
/// @file StMcEtrHitCollection.hh
/// @brief Container for Monte Carlo End-cap Tagger Ring (ETR) hits.
#ifndef StMcEtrHitCollection_hh
#define StMcEtrHitCollection_hh

#include "StMcContainers.hh"
#include "StObject.h"

class StMcEtrHit;

/// @brief Container for Monte Carlo End-cap Tagger Ring (ETR) hits.
///
/// Organises hits across mNumberOfLayers layers and mNumberOfSectors sectors.
class StMcEtrHitCollection : public StObject {
public:
    
    /// @brief Default constructor.
    StMcEtrHitCollection();
    /// @brief Destructor.
    ~StMcEtrHitCollection();
    
    /// @brief Adds an ETR hit to the collection; returns true on success.
    bool addHit(StMcEtrHit*);
    /// @brief Returns the total number of hits in the collection.
    unsigned long numberOfHits() const;

    /// @brief Returns the vector of ETR hits.
    StSPtrVecMcEtrHit&       hits();
    /// @brief Returns the const vector of ETR hits.
    const StSPtrVecMcEtrHit& hits() const;

    /// @brief Returns the number of ETR layers.
    unsigned int  numberOfLayers() const;
    /// @brief Returns the number of ETR sectors.
    unsigned int  numberOfSectors() const;
    
protected:
    StSPtrVecMcEtrHit mHits; ///< Collection of Monte Carlo ETR hits.

    enum { mNumberOfLayers = 3 };   ///< Number of ETR layers.
    enum { mNumberOfSectors = 12 }; ///< Number of ETR sectors.
    ClassDef(StMcEtrHitCollection,1)
};
#endif
