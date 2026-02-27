/***************************************************************************
 * $Id: StMcPxlSensorHitCollection.hh,v 2.1 2013/03/25 23:50:36 perev Exp $
 * $Log: StMcPxlSensorHitCollection.hh,v $
 * Revision 2.1  2013/03/25 23:50:36  perev
 * Mustafa.Pxl add
 *
 *
 *
 **************************************************************************/
/// @file StMcPxlSensorHitCollection.hh
/// @brief Monte Carlo hit collection for one PXL sensor.

#ifndef StMcPxlSensorHitCollection_hh
#define StMcPxlSensorHitCollection_hh

#include "StObject.h"
#include "StMcContainers.hh"

class StMcPxlHit;

/// @brief Collection of Monte Carlo PXL hits for a single sensor.
///
/// Stores a vector of StMcPxlHit pointers for one PXL sensor.
/// Represents the leaf level of the PXL hit hierarchy.
class StMcPxlSensorHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StMcPxlSensorHitCollection();
    /// @brief Destructor.
    ~StMcPxlSensorHitCollection();

    /// @brief Returns a reference to the vector of hits.
    StSPtrVecMcPxlHit&       hits();
    /// @brief Returns a const reference to the vector of hits.
    const StSPtrVecMcPxlHit& hits() const;
    
private:
    StSPtrVecMcPxlHit mHits; ///< Vector of Monte Carlo PXL hits.
    
    ClassDef(StMcPxlSensorHitCollection,1)
};
#endif
