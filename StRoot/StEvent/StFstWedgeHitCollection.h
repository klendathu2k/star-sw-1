/***************************************************************************
* $Id: StFstWedgeHitCollection.h$
*
* Author: Shenghui Zhang, Oct. 2021
****************************************************************************
* Description:
* Data collection for FST hits, and one instance corresponds to one wedge.
***************************************************************************/

#ifndef StFstWedgeHitCollection_hh
#define StFstWedgeHitCollection_hh

/// @file StFstWedgeHitCollection.h
/// @brief Collection of FST reconstructed hits for a single wedge.

#include "StObject.h"
#include "StFstSensorHitCollection.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StFstConsts.h"


/// @brief Container of StFstSensorHitCollection objects for one FST wedge (kFstNumSensorsPerWedge sensors).
class StFstWedgeHitCollection : public StObject
{
public:
   StFstWedgeHitCollection();

   /// @brief Return the total number of hits summed over all sensors in this wedge.
   unsigned int  numberOfHits() const;

   /// @brief Return a pointer to the hit collection for sensor @p i (0-based).
   StFstSensorHitCollection       *sensor(unsigned int);
   /// @brief Return a const pointer to the hit collection for sensor @p i (0-based).
   const StFstSensorHitCollection *sensor(unsigned int) const;

private:
   StFstSensorHitCollection  mSensors[kFstNumSensorsPerWedge]; ///< Hit sub-collections, one per FST sensor in this wedge.

   ClassDef(StFstWedgeHitCollection, 1)
};
#endif


/***************************************************************************
* StFstWedgeHitCollection.h,v 1.0
* Revision 1.0 2013/10/04 Shenghui Zhang
* Initial version
****************************************************************************/
