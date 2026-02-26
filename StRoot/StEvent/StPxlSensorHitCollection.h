/*!
 * \class StPxlSensorHitCollection 
 * \author X. Dong, Jan 2013
 */
/***************************************************************************
 *
 * $Id: StPxlSensorHitCollection.h,v 2.1 2013/03/05 14:40:41 ullrich Exp $
 *
 * Author: X. Dong, Jan 2013
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StPxlSensorHitCollection.h,v $
 * Revision 2.1  2013/03/05 14:40:41  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StPxlSensorHitCollection_hh
#define StPxlSensorHitCollection_hh
/// @file StPxlSensorHitCollection.h
/// @brief PXL hit collection for one sensor of the STAR Pixel Detector.

#include "StObject.h"
#include "StContainers.h"

class StPxlHit;

/// @brief Collection of PXL hits for a single sensor (pixel chip).
class StPxlSensorHitCollection : public StObject {
public:
    /// @brief Default constructor.
    StPxlSensorHitCollection();
    /// @brief Destructor.
    ~StPxlSensorHitCollection();

    /// @brief Returns a reference to the vector of hits on this sensor.
    StSPtrVecPxlHit&       hits();
    /// @brief Returns a const reference to the vector of hits on this sensor.
    const StSPtrVecPxlHit& hits() const;

private:
    StSPtrVecPxlHit mHits; ///< Owned collection of hits on this sensor.
    
    ClassDef(StPxlSensorHitCollection,1)
};
#endif
